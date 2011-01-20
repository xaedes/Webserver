
#include "connection.h"
#include "connections.h"
#include "responsefactory.h"
#include "server/server.h"
#include "lib/general.h"
#include "lib/error.h"
#include "lib/debug.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


Connection* conInit ( Connection* con )
{
	memset ( &con->begin, 0, &con->end - &con->begin );
	memset ( con->poll, 0, sizeof ( struct pollfd ) );

	con->lnsRead = lnsInit();
	con->rqst = rqstInit();
	con->rqst->lns = con->lnsRead;
	con->rsp = rspInit();
	con->rsp->rqst = con->rqst;
	con->initialized = 1;

	return con;
}

Connection *conFree ( Connection* con )
{

	if ( con->sfd )
		close ( con->sfd );
	if ( con->lnsRead )
		lnsFree ( con->lnsRead );
	if ( con->rsp )
		rspFree ( con->rsp );
	if ( con->rqst )
		rqstFree ( con->rqst );

	memset ( con, 0, sizeof ( Connection ) );

	return con;

}

Connection* conReset ( Connection* con )
{

	//struct pollfd *poll;

	memset ( con->poll, 0, sizeof ( struct pollfd ) );

	//int sfd;

	if ( con->sfd )
		close ( con->sfd );
	con->sfd = 0;

	//struct sockaddr_in addr;

	memset ( &con->addr, 0, sizeof ( struct sockaddr_in ) );

	//Lines *lnsRead;

	if ( con->lnsRead )
		lnsClear ( con->lnsRead );
	else
		con->lnsRead = lnsInit();


	//Request *rqst;

	if ( con->rqst )
		rqstReset ( con->rqst );
	else
		con->rqst = rqstInit();

	con->rqst->lns = con->lnsRead;

	//Response *rsp;

	if ( con->rsp )
		rspReset ( con->rsp );
	else
		con->rsp = rspInit();
	con->rsp->rqst = con->rqst;


	//Con_State state;

	con->state = cs_none;

	//int initialized;

	con->initialized = 1;

	return con;
}

Connection* conSetup ( Connection* con, int fd, struct sockaddr_in* addr )
{
	con->sfd = fd;
	if ( addr )
		con->addr = *addr;

	con->poll->fd = fd;

	return con;

}

Connection* conChangeState ( Connection* con, Con_State state )
{
	conExitState ( con, con->state );
	conEnterState ( con, state );
}

Connection* conExitState ( Connection* con, Con_State state )
{
	switch ( state ) {
	case cs_send_httpheaders:
	case cs_send_httpmsgbody:
	case cs_send_complete:
	case cs_handle_requestheader:
	case cs_receive_httpheaders:
		/* FALLTHROUGH */
	case cs_none:
		break;

	default:
		handle_fail ( "conExitState: unimplemented Con_State" );
	}

	return con;
}

Connection* conEnterState ( Connection* con, Con_State state )
{
	switch ( state ) {
	case cs_accept_clients:
		con->poll->events = POLLIN;
		
		con->state = state;
		
		break;
	case cs_receive_httpheaders:
		con->poll->events = POLLIN;

		rqstReset ( con->rqst );
		lnsClear ( con->lnsRead );
		con->rqst->lns = con->lnsRead;

		con->state = state;
		break;
	case cs_handle_requestheader:

		con->poll->events = 0;

		rspReset ( con->rsp );
		con->rsp->rqst = con->rqst;

		con->state = state;

		//die rf sollte alles regeln
		rfProduce ( con->cons->responseFactory, con );

		break;
	case cs_handle_requestwithbody:
		//con->poll->events = POLLIN

		con->poll->events = 0;
		rspReset ( con->rsp );

		con->state = state;

		//die rf sollte alles regeln
		rfProduce ( con->cons->responseFactory, con );

		break;
	case cs_send_httpheaders:
		con->poll->events = POLLOUT;



		con->state = state;
		break;
	case cs_send_httpmsgbody:
		con->poll->events = POLLOUT;

		//msgbody should be already set

		con->state = state;
		break;
	case cs_wait_forbroadcast:
		con->poll->events = 0;

		rspReset ( con->rsp );
		con->rsp->rqst = con->rqst;

		con->state = state;
		break;
	case cs_receive_httpmsgbody:
		//con->poll->events = POLLIN

		con->state = state;
		break;
	case cs_lost:
		con->poll->events = 0;
		
		con->state = state;
		break;
	case cs_send_complete: {
		Request *rqst = con->rqst;
		
		if ( strcasecmp( rqst->httpversion,  "HTTP/1.1" ) == 0 )
			conChangeState( con, cs_receive_httpheaders );
		else if ( strcasecmp( rqst->httpversion, "HTTP/1.0" ) == 0 )
			conChangeState( con, cs_lost );
		
	}	break;
	default:
		handle_fail ( "conEnterState: unimplemented Con_State" );
	}

	return con;
}


Connection *conOnPollIn ( Connection *con )
{
	Server *srv = con->cons->srv;
	switch ( con->state ) {
	case cs_none:
		break;
	case cs_receive_httpheaders: {
		
		int len = read( con->poll->fd, srv->readBuffer, srv->cfg->readBufferSize );
		
		
		if ( len < 0 ) {
			if ( ( errno == EPIPE ) || ( errno == ECONNRESET ) ) {
				//client hat vorher disconnected
				conChangeState( con, cs_lost );
			} else {
				handle_error( "conOnPollIn" );
			}
		}
		if ( len ) {
			int msgSize = sizeUntilDoubleNewline( &con->rqst->lastNCR, &con->rqst->lastNLF, srv->readBuffer, len );

			dsCatChars( con->rqst->buffer, srv->readBuffer, msgSize );
			dsZeroTerminate( con->rqst->buffer );
			
			memset( srv->readBuffer, 0, len);

			if ( msgSize < len ) {
				//Message Ende erreicht - Message Body fängt an
				
				//bereits empfangenen msg body speichern
				int bdyBegin = msgSize + strspn( srv->readBuffer + msgSize, "\r\n" );
				if ( bdyBegin < len ) {
					dsCpyChars( con->rqst->buffer2, srv->readBuffer + bdyBegin, len - bdyBegin );
				}
				
				lnsParse( con->lnsRead, con->rqst->buffer->buffer, con->rqst->buffer->size );
				
				
				rqstParse( con->rqst );
				LOG( "Client request complete:\n", srv->cons->size);
				LOG( "method: '%s' | uri: '%s' | http-version: '%s'\n", con->rqst->method, con->rqst->uri, con->rqst->httpversion );

				conChangeState( con, cs_handle_requestheader );
			} else if ( msgSize == len ) {
				//kein CRLFCRLF enthalten
			}
			
		} else { // len == 0
			conChangeState( con, cs_lost );
		}
	}	break;
	default:
		handle_fail ( "conOnPollIn: unimplemented Con_State" );
	}

	return con;
}

void conSendBuffer( Connection *con, char *buffer, int size, int *sent )
{
	Server *srv = con->cons->srv;
	int len = imin( srv->cfg->writeBufferSize,
					( size ) - ( *sent ) );
	
	if ( len == 0 ) {
		//offenbar nichts mehr zu senden
		return;
	} else {

		len = write( con->sfd, buffer, len );

		if ( len < 0 ) {
			if ( ( errno == EPIPE ) || ( errno == ECONNRESET ) ) {
				//client hat vorher disconnected
				conChangeState( con, cs_lost );
				return;
			} else {
				handle_error( "conSendBuffer" );
			}
		} else if ( len == 0 ) {
			//keine ahnung (todo?)
		}
		( *sent ) += len;
	}
}

Connection* conOnPollOut ( Connection* con )
{
	Server *srv = con->cons->srv;
	switch ( con->state ) {
	case cs_none:
		break;
	case cs_send_httpheaders: {
		Response *rsp = con->rsp;
		
		conSendBuffer( con, rsp->message->data->buffer, 
							rsp->message->data->size,
							&rsp->message->sent );
		
		if ( rsp->message->sent == rsp->message->data->size ) {
			if ( rsp->sendAppdx ) {
				conChangeState( con, cs_send_httpmsgbody );
			} else {
				conChangeState( con, cs_send_complete );
			}
		}
		
	}	break;
	case cs_send_httpmsgbody: {
		Response *rsp = con->rsp;
		
		conSendBuffer( con, rsp->appendix->buffer, 
							rsp->appendix->bufFill,
							&rsp->appendix->sent );
		
		if ( rsp->appendix->sent == rsp->appendix->bufFill ) {
			if ( !rsp->appendix->eof ) {
				rspApdxLoad( rsp->appendix );
			} else {
				conChangeState( con, cs_send_complete );
			}
		}
	} break;
	
	default:
		handle_fail ( "conOnPollOut: unimplemented Con_State" );
	}

	return con;
}

Connection* conOnPollHup(Connection* con)
{
	switch ( con->state ) {
	case cs_receive_httpheaders:
		conChangeState( con, cs_lost );
		break;

	default:
		handle_fail ( "conOnPollHup: unimplemented Con_State" );
	}

	return con;
	
}


Connection* conOnPollErr(Connection* con)
{
	switch ( con->state ) {
	case cs_receive_httpheaders:
		conChangeState( con, cs_lost );
		break;
		
	case cs_lost:
		/* FALLTHROUGH */
	default:
		handle_fail ( "conOnPollErr: unimplemented Con_State" );
	}

	return con;

}













