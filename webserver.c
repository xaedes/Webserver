#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>

#define DEBUG


#include "lib/line.h"
#include "lib/lines.h"
#include "server/connections.h"
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
	char bu[200];
	getcwd( bu, 200 );
	fprintf( stderr, "%s\n", bu );

	Server *srv = srvInit();

	srv->cfg = srvCfgInit();
	srvCfgLoad( srv->cfg, argc, argv );
	srv->cons = consInit();

	srvStart( srv );

	LOG( "Server running on port %d\n", srv->cfg->port );

	signal( SIGPIPE, SIG_IGN );

	while ( 1 ) {
		int pr = poll( srv->cons->poll, srv->cons->size, -1 );

		if ( pr < 0 )
			handle_error( "poll" );

		if ( pr ) {
			int i = 0;

			for ( i = 0 ; ( i < srv->cons->size ) && ( 0 <= i ) ; ++i ) {
				srvPollCycle( srv, i );
			}
		}
	}
}
