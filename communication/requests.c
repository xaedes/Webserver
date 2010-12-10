#include "communication/requests.h"
#include "lib/error.h"
#include "lib/general.h"
#include <stdlib.h>
#include <string.h>

int sizeUntilDoubleNewline( int *nCR, int *nLF, const char *string, int size )
{
	int i = 0;
	int lastNonCRLD = -1;
	int ncr = 0;
	int nlf = 0;
	while ( i < size ) {
		switch( string[i] ) {
		case '\r':
			++ncr;
			if ( ncr >= 2 ) {
				return lastNonCRLD + 1;
			}
			break;
		case '\n':
			++nlf;
			if ( nlf >= 2 ) {
				return lastNonCRLD + 1;
			}
			break;
		default:
			ncr = nlf = 0;
			lastNonCRLD = i;
			break;
		}
		++i;
	}
	*nCR = ncr;
	*nLF = nlf;
	return size;
}

Request *rqstInit( )
{
	Request *rqst = malloc( sizeof( Request ) );

	if ( !rqst )
		handle_error( "rqstInit" );

	memset( rqst, 0,  sizeof( Request ) );

	/**< \todo make the initial buffer size configurable */
	rqst->buffer = dsInit( 512 );
	rqst->http = hmInit();
	rqst->http->type = HTTP_MESSAGE_REQUEST;
	rqst->strings = vcInit();
	rqst->dstrings = vcInit();

	return rqst;
}

//char *parseToken( char *from, char *until


int parseWhileNo( char *from, char *end, const char endChar, char **dest, char **newpos )
{
	int pos = 0;
	int max = imax( 0, ( int )( end - from ) );

	if ( !dest )
		handle_fail( "parseWhileNo: destination pointer is null pointer" );

	*dest = realloc( *dest, max + 1 );

	if ( !( *dest ) )
		handle_error( "parseWhileNo" );

	while (( pos < max ) &&
	        ( from[pos] != endChar ) ) {
		( *dest )[pos] = from[pos];
		++pos;
	}

	( *dest )[pos] = 0; //termination

	*newpos = from + pos + 1;

	return pos;
}

int parseWhileNoSP( char *from, char *end, char **dest, char **newpos )
{
	return parseWhileNo( from, end, ' ', dest, newpos );
}


Request *rqstParseRequestLine( Request *rqst )
{
	char *ln = rqst->lns->items[0];
	char *pos = ln;
	int len = strlen( ln );
	char *end = pos + len;
	
	
	parseWhileNoSP( pos, end, &rqst->method, &pos );
	parseWhileNoSP( pos, end, &rqst->uri, &pos );
	parseWhileNoSP( pos, end, &rqst->httpversion, &pos );
	
	
	return rqst;
}

Request *rqstParseHeader( Request *rqst, int firstLine, int lastLine )
{
	Lines *lns = rqst->lns;
	char *pos = lns->items[firstLine];
	
	
	
	char *name = pos;

	pos = index( pos, ':' );

	*pos = '\0';
	
	pos++;
	
	DString *value = dsInit( 128 );
	int s = strspn( pos, " \t" );
	dsCpyString( value, pos + s );
	
	int i = 0;
	for ( i = firstLine + 1; i <= lastLine; ++i ) {
		int s = strspn( lns->items[i], " \t" );

		dsCatChar( value, ' ' );
		dsCatString( value, lns->items[i] + s );
	}
	
	vcPush( rqst->dstrings, value );
	
	hmAddHeader(rqst->http, name, value->buffer );
	
	
	return rqst;
}

Request *rqstParseHeaders( Request *rqst )
{
	int i = 1;
	Lines *lns = rqst->lns;
	while( i < lns->size ) {
		int j = i + 1;
		while( ( j < lns->size ) && 
			   ( ( ((char *)lns->items[j])[0] == ' ' ) ||
				 ( ((char *)lns->items[j])[0] == '\t' ) ) ) {
			++j;
		}
		rqstParseHeader( rqst, i, j - 1 );
		++i;
	}
	
	return rqst;
}

Request *rqstParse( Request *rqst )
{
	rqstParseRequestLine( rqst );
	
	
	rqstParseHeaders( rqst );
	
	
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

	if ( rqst->buffer )
		dsFree( rqst->buffer );
	
	if ( rqst->http )
		hmFree( rqst->http );

	if ( rqst->strings ) {
		while( rqst->strings->size ) {
			free( vcPop( rqst->strings ) );
		}
		vcFree( rqst->strings );
	}
	if ( rqst->dstrings ) {
		while( rqst->dstrings->size ) {
			dsFree( vcPop( rqst->dstrings ) );
		}
		vcFree( rqst->dstrings );
	}
	
	free( rqst );
}

Request* rqstReset(Request* rqst)
{
	rqst->buffer->size = 0;
	free( rqst->method );
	free( rqst->uri );
	free( rqst->httpversion );
	rqst->lastNLF = rqst->lastNCR = rqst->lns = rqst->method = rqst->uri = rqst->httpversion = 0;
	
	hmReset( rqst->http );
	rqst->http->type = HTTP_MESSAGE_REQUEST;
	
	//Todo rqst->strings
	
	while( rqst->strings->size ) {
		free( vcPop( rqst->strings ) );
	}
	while( rqst->dstrings->size ) {
		dsFree( vcPop( rqst->dstrings ) );
	}
	
	return rqst;
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
