#include "lib/lines.h"
#include "lib/error.h"
#include <string.h>
#include <stdlib.h>


Lines *lnsInit()
{
	Lines * lns = vcInit();
	return lns;
}

int lnsIsEmpty( Lines *lns )
{
	return ( !lns->size );
}

int lnsCount( Lines *lns )
{
	return lns->size;
}

Lines *lnsAdd( Lines *lns, char *ln )
{
	if ( ln == 0)
		handle_fail( "PENG" );
	vcPush( lns, ln );
	return lns;
}


Lines* lnsDeleteLine( Lines* lns, int i )
{
	vcDel( lns, i );
	return lns;
}


Lines* lnsParse( Lines *lns, char* str, int size )
{
	int i = 0;
	if ( lns->size )
		handle_fail( "lnsParse: lns not empty" );
	lnsAdd( lns, str );	//first line points to begin of the string
	while ( i < size ) {
		switch( str[i] ) {
		case '\n':
			str[i] = '\0';
			lnsAdd( lns, str + i + 1 );
			break;
		case '\r':
			str[i] = '\0';
			break;
		default:
			/* void */
			break;
		}
		++i;
	}
	
	return lns;
}

Lines* lnsClear(Lines* lns)
{
	vcClear( lns );
}


void lnsFree( Lines *lns )
{
	vcFree( lns );
}













