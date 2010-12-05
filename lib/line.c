
#include "lib/line.h"
#include "lib/error.h"
#include "lib/general.h"
#include <string.h>
#include <stdlib.h>

Line *lnInit()
{
	Line * ln = malloc( sizeof( Line ) );
	if ( ! ln )
		handle_error("lnInit");
	memset( ln, 0, sizeof( Line ) );
	ln->str = malloc( 1 );
	if ( !ln->str )
		handle_error("lnInit");
	ln->str[0] = 0;
	return ln;
}

//entfernt vorhande \r und \n aus ln->str
Line *lnTrim( Line *ln )
{
	int len = strlen( ln->str );
	int i;
	
	//i = strcspn( ln->str, "\r\n" );
	
	while ((i = strcspn( ln->str, "\r\n" )) < len) {
		memcpy( ln->str + i, ln->str + i + 1, len - i - 1 );
		ln->str[--len] = 0; //stringtermination
	}
	 
	return ln;
}

//wenn string in ln NULL war, wird gleich speicher geholt
//kopiert den uebergebenen string nach ln
Line *lnSet( Line *ln, char *str, int size )
{
	size = imax( 0, size );
	if ( !str )
		return ln;
	ln->str = realloc( ln->str, size + 1 );
	if ( ! ln->str )
		handle_error("lnSet");
	memcpy( ln->str, str, size );
	ln->str[size]=0; //stringtermination
	return ln;
}

//wenn string in ln NULL war, wird gleich speicher geholt
//kopiert uebergebenen string und hängt in an ln 
Line *lnAppend( Line *ln, char *str, int size )
{
	size = imax( 0, size );
	if ( !str )
		return ln;
	int a;
	if ( ln->str ) {
		a = strlen( ln->str );
	} else {
		a = 0;
	}
	ln->str = realloc( ln->str, a + size + 1 );
	
	if ( ! ln->str )
		handle_error("lnAppend");
		
	memcpy( ln->str + a, str, size );
	ln->str[a+size]=0; //stringtermination
	return ln;
}

void lnFree( Line *ln )
{
	if ( !ln )
		return;
	if ( ln->str )
		free( ln->str );

	free( ln );
}


