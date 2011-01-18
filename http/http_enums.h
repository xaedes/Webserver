#ifndef __http_enums_header__
#define __http_enums_header__


typedef enum HttpVersion {
	HTTP_VERSION_INVALID,
	HTTP_VERSION_1_0 ,
	HTTP_VERSION_1_1
} HttpVersion;

/**
 * Gives the http version as string
 */
const char *httpVersionToString( HttpVersion hv );

/**
 * Returns a http version enum for a string
 */
HttpVersion httpVersionFromString( const char *s );

typedef enum HttpStatuscode {
	STATUS_INVALID = 0,
	STATUS_OK = 200,
	STATUS_NOT_FOUND = 404,
	STATUS_MOVED_PERM = 301,
	STATUS_FOUND = 302,
	STATUS_BAD_REQUEST = 400,
	STATUS_FORBIDDEN = 403
} HttpStatuscode;

/**
 * Returns the reasonphrase for a given statuscode
 */
const char *hscReasonphrase( HttpStatuscode status );


typedef enum HttpMessageType {
	HTTP_MESSAGE_INVALID,
	HTTP_MESSAGE_REQUEST,
	HTTP_MESSAGE_RESPONSE 
} HttpMessageType;


#endif