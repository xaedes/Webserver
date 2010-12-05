
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
	return ln;
}


//wenn string in ln NULL war, wird gleich speicher geholt
//kopiert den uebergebenen string nach ln
Line *lnSet( Line *ln, char *str )
{
	ln->str = str;
	return ln;
}

void lnFree( Line *ln )
{
	if ( !ln )
		return;

	free( ln );
}


