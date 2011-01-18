
#include "http/http_requestline.h"
#include "lib/error.h"

#include <stdlib.h>
#include <string.h>

HttpRequestLine* hrlInit()
{
	HttpRequestLine * hrl = malloc( sizeof( HttpRequestLine ) );
	if ( ! hrl ) {
		handle_error( "hrlInit" );
	}
	memset( hrl, 0, sizeof( HttpRequestLine ) );
	
	
	return hrl;
}

void hrlFree(HttpRequestLine* hrl)
{
	free( hrl );
}

HttpRequestLine* hrlReset(HttpRequestLine* hrl)
{
	memset( hrl, 0, sizeof( HttpRequestLine ) );
	return hrl;
}

