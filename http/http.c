
#include "http/http.h"
#include "lib/general.h"
#include "lib/error.h"
#include "lib/dstring.h"
#include <stdlib.h>
#include <string.h>

HttpVersion httpVersionFromString( const char *s )
{
	if ( strcmp( s, httpVersionToString( HTTP_VERSION_1_0 ) ) == 0 )
		return HTTP_VERSION_1_0;
	if ( strcmp( s, httpVersionToString( HTTP_VERSION_1_1 ) ) == 0 )
		return HTTP_VERSION_1_1;
	
	return HTTP_VERSION_INVALID;
}

const char *httpVersionToString( HttpVersion hv ) 
{
	switch ( hv ) {
		case HTTP_VERSION_INVALID:
			handle_fail("httpVersionToString: invalid");
			return "Invalid";
		case HTTP_VERSION_1_0:
			return "HTTP/1.0";
		case HTTP_VERSION_1_1:
			return "HTTP/1.1";
	}
	handle_fail( "httpVersionToString: unimplemented http version" );
}

const char *hscReasonphrase( HttpStatuscode status ) 
{
	const char *str;

	
	
	switch ( status ) {
	case STATUS_OK:
		str = "OK";
		break;
	case STATUS_NOT_FOUND:
		str = "Not Found";
		break;
	case STATUS_MOVED_PERM:
		str = "Moved Permanently";
		break;
	case STATUS_FOUND:
		str = "Found";
		break;
	case STATUS_BAD_REQUEST:
		str = "Bad Request";
		break;
	case STATUS_FORBIDDEN:
		str = "Forbidden";
		break;
	default:
		status  =(HttpStatuscode) ( 1 / (int) status);
		handle_fail( "hscReasonphrase: statuscode not implemented" );
	}
	
	return str;
}

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
	
	/** \todo richtigen reset einführen */
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


HttpMessageHeader *hmhInit()
{
	HttpMessageHeader * hmh = malloc( sizeof( HttpMessageHeader ) );
	if ( ! hmh ) {
		handle_error( "hmhInit" );
	}
	memset( hmh, 0, sizeof( HttpMessageHeader ) );
	
	hmh->fieldName = dsInit( 16 );
	hmh->fieldValue = dsInit( 100 );
		
	return hmh;
}

HttpMessageHeader* hmhInit2( const char* fieldName, const char* fieldValue )
{
	HttpMessageHeader *hmh = hmhInit();
	
	dsCpyString( hmh->fieldName, fieldName );
	dsCpyString( hmh->fieldValue, fieldValue );
	
	return hmh;
}

HttpMessageHeader* hmhReset( HttpMessageHeader* hmh )
{
	dsCpyChars( hmh->fieldName, 0, 0 );
	dsCpyChars( hmh->fieldValue, 0, 0 );
}


void hmhFree( HttpMessageHeader* hmh )
{
	if ( !hmh )
		return;
	
	if ( hmh->fieldName )
		dsFree( hmh->fieldName );
	
	if ( hmh->fieldValue )
		dsFree( hmh->fieldValue );
	
	free( hmh );
}

DString* hmhToString( HttpMessageHeader* hmh, DString *dest )
{
	if ( !dest )
		dest = dsInit( 100 );
	dsCatF( dest, "%S: %S", hmh->fieldName, hmh->fieldValue );
	 
	return dest;
}


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

