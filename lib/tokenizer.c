
#include "lib/tokenizer.h"
#include "lib/bitset.h"
#include "lib/general.h"
#include "lib/error.h"

#include <string.h>
#include <stdlib.h>

Tokenizer *tkInit( const char* start, int size, const char *seperators )
{
	Tokenizer * tk = malloc( sizeof( Tokenizer ) );
	if ( ! tk )
		handle_error( "tkInit" );
	memset( tk, 0, sizeof( Tokenizer ) );
	tk->start = start;
	tk->size = size;
	tk->end = start + size;
	tk->current = start;
	tk->charset = bsInit( 256 );
	if ( !tk->charset )
		handle_error( "tkInit" );
	
	int n = strlen( seperators );
	int i = 0;
	for ( i = 0 ; i < n ; ++i ) {
		tkAddSeperator( tk, seperators[i] );
	}
	
	return tk;
}

Tokenizer* tkInitS( const char* string, const char* seperators )
{
	if ( !string )
		return 0;
	
	return tkInit( string, strlen( string ), seperators );
}

int tkIsSeperator( Tokenizer* tk, char c )
{
	unsigned char uc = ( unsigned char ) c;
	return bsGet( tk->charset, uc );
}

Tokenizer* tkAddSeperator( Tokenizer* tk, char c )
{
	unsigned char uc = ( unsigned char ) c;
	bsAdd( tk->charset, uc );
	return tk;
}

Tokenizer* tkRemoveSeperator( Tokenizer* tk, char c )
{
	unsigned char uc = ( unsigned char ) c;
	bsRemove( tk->charset, uc );
	return tk;
}


char* tkNext( Tokenizer* tk )
{
	int pos = 0;
	int max = ( int ) ( tk->end - tk->current );
	if ( max < 0 )
		return 0;
	char *parsed = malloc( max + 1 );
	if ( !parsed ) 
		handle_error( "tkNext" );
	while ( ( pos < max ) && 
			! tkIsSeperator( tk, tk->current[pos] ) ) {
		parsed[pos] = tk->current[pos];
		++pos;
	}
	parsed[pos] = 0; //termination
	
	tk->current += pos + 1;
	cstring_memtrim( &parsed );
	
	return parsed;
}

Tokenizer* tkReset( Tokenizer* tk )
{
	tk->current = tk->start;
	return tk;
}


void tkFree( Tokenizer* tk )
{
	if ( !tk )
		return;
	
	if ( tk->charset )
		bsFree( tk->charset );
	
	free( tk );
}

