#include "lib/lines.h"
#include "lib/line.h"
#include "lib/error.h"
#include <string.h>
#include <stdlib.h>


Lines *lnsInit()
{
	Lines * lns = malloc( sizeof( Lines ) );
	if ( ! lns )
		handle_error("lnsInit");
	memset( lns, 0, sizeof( Lines ) );
	return lns;
}

int lnsIsEmpty( Lines *lns )
{
	return ( !lns->head );
}

int lnsCount( Lines *lns )
{
	int s = 0;
	Line *x = lns->head;
	while ( x ) {
		x = x->next;
		s++;
	}
	return s;
}

Lines *lnsAdd( Lines *lns, Line *ln )
{
	if (!lns->head)
		lns->head = ln;
	if (lns->last)
		lns->last->next = ln;
	ln->prev = lns->last;
	ln->next = 0;
	lns->last = ln;
	return lns;
}

Lines *lnsAppend( Lines *lns, char *str, int size )
{
	if ( lnsIsEmpty( lns ) )
	{
		Line *ln = lnInit( );
		lnsAdd( lns, ln );
	}
	lnAppend( lns->last, str, size );
	//lnTrim( lns->last );
	return lns;
}

Lines *lnsAppendLine( Lines *lns, char *str, int size )
{
	Line *ln = lnInit( );
	lnAppend( ln, str, size );
	//lnTrim( ln );
	lnsAdd( lns, ln );
	
	return lns;
}


Lines* lnsDeleteLine(Lines* lns, Line* ln)
{
	if ( !ln )
		return lns;
	if ( !lns )
		return lns;
	
	if ( ln == lns->head ) {
		lns->head = ln->next;
	}
	if ( ln == lns->last ) {
		lns->last = ln->prev;
	}
	
	if ( ln->next ) {
		ln->next->prev = ln->prev;
	}
	if ( ln->prev ) {
		ln->prev->next = ln->next;
	}
	
	lnFree( ln );
}


Lines *lnsAddBuffer( Lines *lns, char *buffer, int size)
{
	char *last;
	char *pos;
	last = pos = buffer;
	do
	{
		switch ( *pos ) {
		case '\n':
			if ( lns->tmp == '\r' ) {//crlf (http)
				lnsAppend( lns, last, pos - last - 1 );	
			} else {//lf (unix, be tolerant)
				lnsAppend( lns, last, pos - last ); 
			}
			if ( !lns->head || ( strlen( lns->head->str ) > 0) )
				lnsAdd( lns, lnInit() ); //neue zeile einfügen
			last = pos + 1;
			break;
		default:
			/* void */
			break;
		}
		lns->tmp = *pos;
		pos ++;
	} while ( (int)( pos - buffer ) < size );
	if ( (int)( pos - buffer ) >= size ) {
		if ( lns->tmp == '\r' ) { //letztes zeichen
			//rest davor ausgeben
			lnsAppend( lns, last, pos - last - 1 );
		} else {
			lnsAppend( lns, last, pos - last );
		}
	}
	return lns;
}
// Lines *lnsAddBuffer( Lines *lns, char *buffer, int size)
// {
// 	char *last;
// 	char *pos;
// 	last = buffer;
// 	do
// 	{
// 		
// 		pos = index( last, '\n' );
// 		if ( pos ) { //ein zeilenumbruch wurde gefunden
// 			lnsAppend( lns, last, pos - last ); //von last bis zum zeilenumbruch an zeile anfügen
// 			lnsAdd( lns, lnInit() ); //neue zeile einfügen
// 		} else { //kein zeilenumbruch mehr gefunden -> bufferende
// 			lnsAppend( lns, last, size - (int) last + (int) buffer ); //von last bis zum bufferende an zeile anfügen
// 		}
// 		last = pos + 1;
// 	} while ( pos && (((int)last - (int)buffer ) < size) );
// 	
// 	return lns;
// }

int lnsRqstComplete( Lines *lns )
{
	if ( lnsCount( lns ) < 2 )
		return 0;
	return ( ( strlen( lns->last->str ) == 0 ) && ( strlen( lns->last->prev->str ) == 0 ) );
}

void lnsFree( Lines *lns )
{
	if ( !lns )
		return;
	
	while( lns->head ) {
		lnsDeleteLine( lns, lns->head );
	}

	free( lns );
}













