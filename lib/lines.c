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
	vcPush( lns, ln );
	return lns;
}

/*
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
*/

/*
Lines *lnsAppendLine( Lines *lns, char *str, int size )
{
	Line *ln = lnInit( );
	lnAppend( ln, str, size );
	//lnTrim( ln );
	lnsAdd( lns, ln );
	
	return lns;
}
*/

Lines* lnsDeleteLine( Lines* lns, int i )
{
	vcDel( lns, i );
	return lns;
}

/*
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
*/
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

/*
int lnsRqstComplete( Lines *lns )
{
	if ( lnsCount( lns ) < 2 )
		return 0;
	return ( ( strlen( lns->last->str ) == 0 ) && ( strlen( lns->last->prev->str ) == 0 ) );
}
*/

Lines* lnsParse( Lines *lns, char* str, int size )
{
	int i = 0;
	lnsAdd( lns, str );	//first line points to begin of the string
	while ( i < size ) {
		switch( str[i] ) {
		case '\r':
			str[i] = '\0';
			if ( i > 0 ) {
				switch( str[i-1] ) {
				case '\r':	//CR CR - after first CR begins a new line
					lnsAdd( lns, str + i );
					break;
				case '\n':	//LF CR - after LF begins a new line
					lnsAdd( lns, str + i );
					break;
				default:
					break;
				}
			} else if ( i + 1 < size ) {
				if ( ( str[i+1] != '\r' ) &&
					 ( str[i+1] != '\n' ) ) {
					lnsAdd( lns, str + i + 1 );
				}
				
			} else { //i == 0 | size == 1
				/* first line already lined up */
			}
			break;
		case '\n':
			str[i] = '\0';
			if ( i > 0 ) {
				switch( str[i-1] ) {
				case '\r':	//CR LF - after CR LF begins a new line
					if ( i + 1 < size )
						lnsAdd( lns, str + i + 1 );
					break;
				case '\n':	//LF LF - after first LF begins a new line
					lnsAdd( lns, str + i );
					break;
				default:
					break;
				}
			} else if ( i + 1 < size ) {
				if ( ( str[i+1] != '\r' ) &&
					 ( str[i+1] != '\n' ) ) {
					lnsAdd( lns, str + i + 1 );
				}
				
			} else { //i == 0 | size == 1
				/* first line already lined up */
			}
			break;
		default:
			break;
		}
		++i;
	}
	
	return lns;
}


void lnsFree( Lines *lns )
{
	vcFree( lns );
}













