
#include "http/http_enums.h"
#include "lib/error.h"

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
