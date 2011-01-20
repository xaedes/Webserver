

#define DEBUGOFF

#include "server/server.h"
#include "server/connections.h"
#include "server/responsefactory.h"
#include "communication/response.h"
#include "lib/general.h"
#include "lib/debug.h"
#include "lib/error.h"

#include <poll.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/types.h>
#include <arpa/inet.h>


Server *srvInit( )
{
	Server *srv = malloc( sizeof( Server ) );

	if ( !srv )
		handle_error( "srvInit" );

	return srv;
}

Server* srvSetup(Server* srv, int argc, char* argv[])
{
	srv->cfg = srvCfgInit();
	srvCfgLoad( srv->cfg, argc, argv );
	srv->cons = consInit();
	srv->rfactory = rfInit();
	srv->rfactory->cons = srv->cons;
	srv->rfactory->cfg = srv->cfg;
	
	return srv;
}

void srvFree( Server* srv )
{
	if ( !srv )
		return;

	if ( !srv->readBuffer ) {
		free( srv->readBuffer );
	}
	
	srvCfgFree( srv->cfg );
	consFree( srv->cons );
	rfFree( srv->rfactory );

	free( srv );
}


Server *srvStart( Server *srv )
{
	srv->socket = socket( AF_INET, SOCK_STREAM, 0 );

	if ( srv->socket < 0 ) 	{
		handle_error( "socket" );
	}

	int socketflags = fcntl( srv->socket, F_GETFL, 0 );

	fcntl( srv->socket, F_SETFL, socketflags | O_NONBLOCK );

	socklen_t sockOpt = 1;
	setsockopt( srv->socket, SOL_SOCKET, SO_REUSEADDR, &sockOpt, sizeof( sockOpt ) );

	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons( srv->cfg->port );

	addr.sin_addr.s_addr = INADDR_ANY;	//listen to any


	if ( bind( srv->socket, ( struct sockaddr* )( &addr ), sizeof( addr ) ) < 0 ) {
		handle_error( "bind" );
	}

	if ( listen( srv->socket, 256 ) < 0 ) {
		handle_error( "listen" );
	}

	srv->readBuffer = malloc( srv->cfg->readBufferSize );

	if ( !srv->readBuffer )
		handle_error( "srvStart" );


	
	int con = consAdd( srv->cons );
	consConnectionInit( srv->cons, con );
	consConnectionSetup( srv->cons, con , srv->socket, &addr );
	consConnectionSetIOStatus( srv->cons, con, ios_receive_httpheaders );	//for accepting clients via poll()


	return srv;
}

void srvAcceptClient( Server *srv ) 
{
	socklen_t addr_size=sizeof( struct sockaddr_in );
	struct sockaddr_in addr;
	
	int fd;
	
	do {
		fd = accept( srv->socket, ( struct sockaddr* ) ( &addr ), &addr_size );
		
		
		
		if (fd < 0)	{
			if ( ( errno != EAGAIN ) && ( errno != EWOULDBLOCK ) ) {
				handle_error( "srvAcceptClient" );
			}
			
		} else {
			int con = consAdd( srv->cons );
			if ( !srv->cons->clients[con].initialized )
				consConnectionInit( srv->cons, con );
			consConnectionSetup( srv->cons, con , fd, &addr );
			
			
			consConnectionSetIOStatus( srv->cons, con, ios_receive_httpheaders );
			
			//consConnectionForward( srv->cons, 1, con );
		
			
			
			LOG( "Client just connected - Clients connected total: %d\n", srv->cons->size );
		}
	} while ( fd > 0 );
}

void srvForwardSenderCycle( Server *srv, int i ) 
{
}

void srvForwardReceiverCycle( Server *srv, int i )
{
}

void srvPollCycle( Server* srv, int i )
{
	struct pollfd poll = srv->cons->poll[i];
	short int revents = poll.revents;
	
	if ( revents & POLLOUT ) {
		if ( !srvResponseCycle( srv, i ) )	
			return;
	}
	
	if ( revents & POLLIN ) {
		if ( srv->cons->poll[i].fd == srv->socket ) {
			srvAcceptClient( srv );
		} else 
		if ( !srvRequestCycle( srv, i ) )
			return;
		
	} else if ( revents & POLLHUP ) {
		//printLns( srv->cons->clients[i].lnsRead );
/*		if ( ( srv->cons->clients[i].ios == ios_forward_receiver ) ||
		     ( srv->cons->clients[i].ios == ios_forward_sender ) ) {
			if ( srv->cons->clients[i].forwardHangUp )
				srv->cons->clients[i].forwardHangUp( i );
		}*/
		consDel( srv->cons, i-- );
		LOG( "Client just hung up - Clients connected total: %d\n", srv->cons->size );
		return;
	}
	
	if ( revents & POLLERR ) {
		//printLns( srv->cons->clients[i].lnsRead );
		consDel( srv->cons, i-- );
		LOG( "Client error - Clients connected total: %d\n", srv->cons->size );
		return;
	}
}


int srvResponseCycle( Server* srv, int i )
{
	Response *rsp = srv->cons->clients[i].rsp;
	int *sent;
	int *size;
	char *buffer;
	if ( srv->cons->clients[i].ios == ios_send_respond ) {
		if ( rsp->sendState == 0 ) {	//sende dss
			sent = &rsp-> message->sent;
			size = &rsp->message->data->size;
			buffer = rsp->message->data->buffer;
		} else if ( rsp->sendState == 1 ) { //sende appendix
			sent = &rsp->appendix->sent;
			size = &rsp->appendix->bufFill;
			buffer = rsp->appendix->buffer;
		}
	} /*else if ( srv->cons->clients[i].ios == ios_forward_receiver ){	
		sent = &srv->cons->clients[i].forwardSent;
		size = &srv->cons->clients[srv->cons->clients[i].iForwardSource].forward->size;
		buffer = srv->cons->clients[srv->cons->clients[i].iForwardSource].forward->buffer;
	}*/

	int len = imin( srv->cfg->writeBufferSize,
					( *size ) - ( *sent ) );
	
	if ( len == 0 ) {
		if ( srv->cons->clients[i].ios == ios_send_respond ) {
			if ( ( rsp->sendState == 0 ) && rsp->sendAppdx ) { //appendix muss noch gesendet werden
				rsp->sendState = 1;
			} else 
			if ( ( rsp->sendState == 1 ) && !rsp->appendix->eof ) { //appendix noch nicht zu ende gesendet
				rspApdxLoad( rsp->appendix );
			} else {
				if ( strcasecmp( rsp->rqst->httpversion, "HTTP/1.1" ) == 0 )
					consConnectionSetIOStatus( srv->cons, i, ios_receive_httpheaders );
				else if ( strcasecmp( rsp->rqst->httpversion, "HTTP/1.0" ) == 0 )
				{
					consDel( srv->cons, i-- );
					LOG( "srvResponse : Connection to client closed (HTTP/1.0) - Clients connected total: %d\n", srv->cons->size );
					return 0; //poll cycle beenden
				}
			}
		} /*else if ( srv->cons->clients[i].ios == ios_forward_receiver ){
			//todo: deaktiviere POLLOUT für die connection ; aktiviere POLLIN für forward sender
			
			return 1;
		}*/
	} else {

		len = write( srv->cons->poll[i].fd, buffer, len );

		if ( len < 0 ) {
			if ( ( errno == EPIPE ) || ( errno == ECONNRESET ) ) {
				//client hat vorher disconnected
				consDel( srv->cons, i-- );
				LOG( "srvResponse : Client just hung up - Clients connected total: %d\n", srv->cons->size );
				return 0;	//poll cycle beenden
			} else {
				handle_error( "srvResponse" );
			}
		} else if ( len == 0 ) {
			//keine ahnung (todo?)
		}
/*						fprintf( stderr, "wrote %d bytes\n", len );
		exit(0);*/
		( *sent ) += len;
	}
	return 1;	//weitermachen
}


int srvRequestCycle( Server* srv, int i )
{
	Client *client = &srv->cons->clients[i];
	
// 	if ( client->ios == ios_forward_sender ) {
// 		if ( srv->cons->clients[client->iForwardDest].forwardSent < client->forward->size ) {
// 			//todo: deaktiviere POLLIN für die connection ; aktiviere POLLOUT für forward receiver
// 			return 1;
// 		}
// 	} 

	int len = read( srv->cons->poll[i].fd, srv->readBuffer, srv->cfg->readBufferSize );
	
	
	if ( len < 0 ) {
		if ( ( errno == EPIPE ) || ( errno == ECONNRESET ) ) {
			//client hat vorher disconnected
			consDel( srv->cons, i-- );
			LOG( "read : Client just hung up - Clients connected total: %d\n", srv->cons->size );
			return 0; //poll cycle beenden
		} else {
			handle_error( "srvRequest" );
		}
	}
	if ( len ) {
// 		if ( client->ios == ios_receive ) {
			
		int msgSize = sizeUntilDoubleNewline( &client->rqst->lastNCR, &client->rqst->lastNLF, srv->readBuffer, len );

		dsCatChars( client->rqst->buffer, srv->readBuffer, msgSize );
		dsZeroTerminate( client->rqst->buffer );
		
		memset( srv->readBuffer, 0, len);

		if ( msgSize < len ) {
			//Message Ende erreicht - Message Body fängt an
			
			lnsParse( client->lnsRead, client->rqst->buffer->buffer, client->rqst->buffer->size );
			
			
			rqstParse( client->rqst );
			LOG( "Client request complete:\n", srv->cons->size);
			LOG( "method: '%s' | uri: '%s' | http-version: '%s'\n", client->rqst->method, client->rqst->uri, client->rqst->httpversion );

			rfProduce( srv->rfactory, i );
			//client->rsp = rspInit();
			//client->rsp->rqst = client->rqst;
			//LOG( "Response built: \nLength: %d\n%s\n", srv->cons->clients[i].rsp->dss->size, srv->cons->clients[i].rsp->dss->buffer );
		} else if ( msgSize == len ) {
			//kein CRLFCRLF enthalten
		}
/*		} else if ( client->ios == ios_forward_sender ) {
			dsCpyChars( client->forward, srv->readBuffer, len );
			srv->cons->clients[client->iForwardDest].forwardSent = 0;
		}*/
		
	} else { // len == 0
		if ( client->ios == ios_receive_httpheaders ) {
			//printLns( srv->cons->clients[i].lnsRead );
			consDel( srv->cons, i-- );
			LOG( "Client just hung up - Clients connected total: %d\n", srv->cons->size );
			return 0;  //poll cycle beenden
		} /*else if ( client->ios == ios_forward_sender ) {
			//wird erst später gelöscht
			client->forwardEOF = 1;
		}*/
	}
	return 1;
}











