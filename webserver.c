#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>




#define DEBUG


#include "lib/line.h"
#include "lib/lines.h"
#include "connections/connections.h"
#include "server/server.h"
#include "server/serverconfig.h"
#include "lib/error.h"
#include "lib/general.h"


#include "lib/debug.h"


/**
 * Some useful functions
 */

struct Main { };


/** \memberof Main
 *
 * Prints a usage message.
 *
 */
void printHelp()
{
	printf( "Webserver\n\n" );
	printf( "usage:\t webserver [-p port]\n" );
	//printf( "[options] [hostname] [port]\noptions:\n" );
	printf( "options:\n" );
	printf( "\t-p port\t\t port number to listen" );

	printf( "\n\n" );
}



/** \memberof Main
 *
 * Prints debug information for a Line object.
 *
 */
void printLn( Line *ln )
{
	printf( "display of Line %p\n", ln );

	if ( ln-> str ) {
		printf( "length: %d\n", strlen( ln->str ) );
		printf( "string: '%s'\n", ln->str );
	}
}


/** \memberof Main
 *
 * Prints debug information for a Lines object.
 *
 */
/*
void printLns( Lines *lns )
{
	printf( "display of Lines %p\n", lns );
	printf( "number of lines: %d\n", lnsCount( lns ) );
	Line *x = lns->head;
	int s = 0;

	while ( x ) {
		printLn( x );
		s += x->str ? strlen( x->str ) : 0;
		printf( "total length so far: %d\n", s );
		x = x->next;
	}
}*/


/** \memberof Main
 *
 * Main loop.
 *
 */
int main( int argc, char *argv[] )
{
	Server *srv = srvInit();

	srvSetup( srv, argc, argv );
	
	srvStart( srv );
	
/*	{
		int in = consAdd( srv->cons );
		consConnectionInit( srv->cons, in );
		struct sockaddr_in s;
		
		consConnectionSetup( srv->cons, in , 0, s );

		int out = consAdd( srv->cons );
		
		consConnectionInit( srv->cons, out );
		
		consConnectionSetup( srv->cons, out , 1, s );
		
		consConnectionForward( srv->cons, in, out );
	}*/
// 	{
// 		int con = consAdd( srv->cons );
// 		consConnectionInit( srv->cons, con );
// 		struct sockaddr_in s;
// 		
// 		consConnectionSetup( srv->cons, con , -1, s );
// 		dsCpyString( srv->cons->clients[con].forward, "Test\r\n...\r\n");
// 	}

	LOG( "Server running on port %d\n", srv->cfg->port );

	signal( SIGPIPE, SIG_IGN );

	while ( 1 ) {
		int pr = poll( srv->cons->poll, srv->cons->size, -1 );

		if ( pr < 0 )
			handle_error( "poll" );

		if ( pr ) {
			srvPollCycle( srv );
		}
	}
}
