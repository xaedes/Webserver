
#include "http/http.h"
#include "http/http_enums.h"
#include "http/http_statusline.h"
#include "http_requestline.h"

#include "lib/general.h"
#include "lib/error.h"
#include "lib/dstring.h"

#include <stdlib.h>
#include <string.h>



HttpMessage *hmInit()
{
	HttpMessage * hm = malloc( sizeof( HttpMessage ) );
	if ( ! hm ) {
		handle_error( "hmInit" );
	}
	memset( hm, 0, sizeof( HttpMessage ) );
	
	hm->statusLine = hslInit();
	hm->requestLine = hrlInit();
	hm->headerNames = vcInit();
	hm->headerValues = vcInit();
	
	return hm;
}

HttpMessage* hmReset( HttpMessage* hm )
{
	hslReset( hm->statusLine );
	hrlReset( hm->requestLine );
	
	vcClear( hm->headerNames );
	vcClear( hm->headerValues );
	
	hm->type = HTTP_MESSAGE_INVALID;
	
	return hm;
}


void hmFree( HttpMessage* hm )
{
	if ( !hm ) 
		return;
	
	if ( hm->headerNames ) 
		vcFree( hm->headerNames );
	if ( hm->headerValues ) 
		vcFree( hm->headerValues );
	
	if ( hm->statusLine )
		hslFree( hm->statusLine );
	if ( hm->requestLine )
		hrlFree( hm->requestLine );
	
	free( hm );
}

HttpMessage* hmAddHeader(HttpMessage* hm, const char* name, const char* value)
{
	vcPush( hm->headerNames, name );
	vcPush( hm->headerValues, value );
	return hm;
}


DString* hmToString( HttpMessage* hm, DString *dest )
{
	if ( !hm )
		return 0;
	
	switch( hm->type ) {
	case HTTP_MESSAGE_INVALID:	
		handle_fail("hmToString: invalid message type");
		break;
	case HTTP_MESSAGE_RESPONSE:
		hslToString( hm->statusLine, dest );
		break;
	case HTTP_MESSAGE_REQUEST:
		handle_fail("hmToString: not applicable to message type");
		break;
	default:
		handle_fail("hmToString: message type not implemented");
		break;
	}
	
	dsCatString( dest, "\r\n" );
	
	int i;
	for ( i = 0; i < hm->headerNames->size ; ++i ) {
		
		dsCatF( dest, "%s: %s\r\n", hm->headerNames->items[i], hm->headerValues->items[i] );
	}
	
	dsCatString( dest, "\r\n" );
	
	return dest;
}


