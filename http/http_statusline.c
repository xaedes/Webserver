
#include "http/http_statusline.h"
#include "http/http_enums.h"

#include "lib/dstring.h"
#include "lib/error.h"

#include <stdlib.h>
#include <string.h>

HttpStatusLine *hslInit()
{
	HttpStatusLine * hsl = malloc( sizeof( HttpStatusLine ) );
	if ( ! hsl ) {
		handle_error( "hshInit" );
	}
	memset( hsl, 0, sizeof( HttpStatusLine ) );
	
	hsl->httpversion = HTTP_VERSION_INVALID;
	hsl->statuscode = STATUS_INVALID;
	
	return hsl;
}

HttpStatusLine* hslReset( HttpStatusLine* hsl )
{
	hsl->httpversion = HTTP_VERSION_INVALID;
	hsl->statuscode = STATUS_INVALID;
}



void hslFree( HttpStatusLine* hsl )
{
	if ( !hsl ) 
		return;

	free( hsl );

}

DString* hslToString( HttpStatusLine* hsl, DString *dest )
{
	if ( !dest )
		dest = dsInit( 100 );
	dsCatF( dest, "%s %d %s", 	httpVersionToString( hsl->httpversion ), 
								(int) hsl->statuscode, 
								hscReasonphrase( hsl->statuscode ) );
	 
	return dest;
}

