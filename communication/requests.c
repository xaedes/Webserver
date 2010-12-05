#include "communication/requests.h"
#include "lib/error.h"
#include "lib/general.h"
#include <stdlib.h>
#include <string.h>

Request *rqstInit( Lines *lns )
{
	Request *rqst = malloc( sizeof( Request ) );

	if ( !rqst )
		handle_error( "rqstInit" );

	memset( rqst, 0,  sizeof( Request ) );

	rqst->lns = lns;

	return rqst;
}

//char *parseToken( char *from, char *until

int parseWhileNoSP( char *from, char *end, char **dest, char **newpos )
{
	int pos = 0;
	int max = imax( 0, ( int )( end - from ) );

	if ( !dest )
		handle_fail( "parseWhileNoSP: destination pointer is null pointer" );

	*dest = realloc( *dest, max + 1 );

	if ( !( *dest ) )
		handle_error( "parseWhileNoSP" );

	while (( pos < max ) &&
	        ( from[pos] != ' ' ) ) {
		( *dest )[pos] = from[pos];
		++pos;
	}

	( *dest )[pos] = 0; //termination

	*newpos = from + pos + 1;
	cstring_memtrim( dest );

	return pos;
}

Request *rqstParseRequestLine( Request *rqst )
{
	char *ln = rqst->lns->head->str;
	char *pos = ln;
	int len = strlen( ln );
	char *end = pos + len;
	int max;

	parseWhileNoSP( pos, end, &rqst->method, &pos );
	parseWhileNoSP( pos, end, &rqst->uri, &pos );
	parseWhileNoSP( pos, end, &rqst->httpversion, &pos );


	return rqst;
}

Request *rqstParse( Request *rqst )
{
	rqstParseRequestLine( rqst );
	//TODO : parse headers
}

void rqstFree( Request *rqst )
{
	if ( !rqst )
		return;

	if ( rqst->method )
		free( rqst->method );

	if ( rqst->uri )
		free( rqst->uri );

	if ( rqst->httpversion )
		free( rqst->httpversion );

	free( rqst );
}

/*

Requests *rqstsInit()
{
	Requests *rqsts = malloc( sizeof( Requests ) );
	if ( !rqsts )
		handle_error( "rqstsInit" );
	memset( rqsts, 0,  sizeof( Requests ) );
	rqsts->vc = vcInit();

	return rqsts;
}

Requests *rqstsAdd( Requests *rqsts, Request *rqst )
{
	int i = vcAdd( rqsts->vc );
	rqsts->vc->items[i] = rqst;
	return rqsts;
}
*/
