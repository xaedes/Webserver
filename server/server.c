

#define DEBUGOFF

#include "server/server.h"
#include "connections/connections.h"
#include "connections/responsefactory.h"
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
	srv->cons->srv = srv;
	srv->rfactory = rfInit();
	srv->rfactory->cons = srv->cons;
	srv->rfactory->cfg = srv->cfg;
	srv->cons->responseFactory = srv->rfactory;
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


	
	Connection *con = &srv->cons->cons[ consAdd( srv->cons ) ];
	conInit( con );
	conSetup( con, srv->socket, &addr );
	conChangeState( con, cs_accept_clients );


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
			Connection *con = &srv->cons->cons[consAdd( srv->cons )];
			if ( !con->initialized )
				conInit( con );
			
			conSetup( con, fd, &addr );
			
			conChangeState( con, cs_receive_httpheaders );
			
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

void srvPollCycle( Server* srv )
{
	int i;
	for ( i = 0 ; ( i < srv->cons->size ) && ( 0 <= i ) ; ++i ) {
		Connection *con = &srv->cons->cons[i];
		struct pollfd poll = srv->cons->poll[i];
		short int revents = poll.revents;
		
		if ( revents & POLLOUT ) {
			conOnPollOut( con );
		}
		
		if ( revents & POLLIN ) {
			if ( srv->cons->poll[i].fd == srv->socket ) {
				srvAcceptClient( srv );
			} else 
				conOnPollIn( con );
			
		} else if ( revents & POLLHUP ) {
			conOnPollHup( con );
		}
		
		if ( revents & POLLERR ) {
			conOnPollErr( con );
		}
		
		if ( con->state == cs_lost ) {
			consDel( srv->cons, i-- );
			LOG( "Connection lost - Clients connected total: %d\n", srv->cons->size );
		}
	}
}












