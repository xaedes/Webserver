#include "lib/bitset.h"
#include "lib/error.h"
#include <string.h>
#include <stdlib.h>

BitSet *bsInit( int nBits )
{
	BitSet * bs = malloc( sizeof( BitSet ) );
	if ( ! bs )
		handle_error( "bsInit" );
	memset( bs, 0, sizeof( BitSet ) );
	bs->bitSize = nBits;
	bs->bits = malloc( bsByteSize( bs ) );
	if ( !bs->bits )
		handle_error( "bsInit" );
	return bs;
}

int bsByteSize( BitSet* bs )
{
	if ( !bs )
		return -1;
	return ( bs->bitSize / 8 ) + ( bs->bitSize % 8 ? 1 : 0 );
}

BitSet* bsSet( BitSet* bs, int n, int value )
{
	if ( ( n < 0 ) || ( n >= bs->bitSize ) )
		return bs;
	
	char *b = bs->bits;
	b += ( n / 8 );
	
	if ( value ) {
		*b |= ( char ) 1 << ( n % 8 );
	} else {
		*b &= ~( ( char ) 1 << ( n % 8 ) );
	}
	
	return bs;
}

BitSet* bsAdd( BitSet* bs, int n )
{
	return bsSet( bs, n, 1 );
}

BitSet* bsRemove( BitSet* bs, int n )
{
	return bsSet( bs, n, 0 );

}


int bsGet( BitSet* bs, int n )
{
	if ( ( n < 0 ) || ( n >= bs->bitSize ) )
		return 0;
	
	char *b = bs->bits;
	b += ( n / 8 );

	return ( *b & ( ( char ) 1 << ( n % 8 ) ) ) ? 1 : 0;
}



void bsFree( BitSet* bs )
{
	if ( !bs )
		return;
	
	if ( bs->bits )
		free( bs->bits );
	
	free( bs );
}
