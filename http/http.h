#ifndef __http_header__
#define __http_header__

#include "lib/vector.h"
#include "lib/dstring.h"
//http://www.w3.org/Protocols/rfc2616/rfc2616.html

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


/**
 * HTTP StatusLine
 */
typedef struct HttpStatusLine {
	HttpVersion httpversion;	/**<  */
	HttpStatuscode statuscode;	/**<  */
} HttpStatusLine;				/**<  */

/** \memberof HttpStatusLine
 * 
 * Contructor - Allocates memory for a new HttpStatusLine instance. 
 *
 * \return new HttpStatusLine instance
 */
HttpStatusLine *hslInit();

/** \memberof HttpStatusLine
 * 
 * Resets a HttpStatusLine object for reuse
 * 
 * \param hsl targeted HttpStatusLine instance
 * \return targeted HttpStatusLine instance
 */
HttpStatusLine *hslReset( HttpStatusLine *hsl );


/** \memberof HttpStatusLine
 * 
 * Frees the HttpStatusLine instance and all the variables that where initialized in this context.
 * 
 * \param hsl targeted HttpStatusLine instance
 */
void hslFree( HttpStatusLine *hsl );

/** \memberof HttpStatusLine
 * 
 * Convert hsl to c string.
 * 
 * \param hsl targeted HttpStatusLine instance
 * \param dest pointer to already existing dstring to write to, if dest == 0, new memory will be allocated
 * \return string 
 */
DString *hslToString( HttpStatusLine *hsl, DString *dest );

/**
 * HTTP Messageheader
 */
typedef struct HttpMessageHeader {
	DString *fieldName;
	DString *fieldValue;
} HttpMessageHeader;

/** \memberof HttpMessageHeader
 * 
 * Contructor - Allocates memory for a new HttpMessageHeader instance. 
 *
 * \return new HttpMessageHeader instance
 */
HttpMessageHeader *hmhInit();

/** \memberof HttpMessageHeader
 * 
 * Contructor - Allocates memory for a new HttpMessageHeader instance. 
 * Initializes with given values.
 * Values are copied.
 *
 * \param fieldName
 * \param fieldValue
 * \return new HttpMessageHeader instance
 */
HttpMessageHeader *hmhInit2( const char *fieldName, const char *fieldValue);

/** \memberof HttpMessageHeader
 * 
 * Resets a HttpMessageHeader object for reuse
 * 
 * \param hmh targeted HttpMessageHeader instance
 * \return targeted HttpMessageHeader instance
 */
HttpMessageHeader *hmhReset( HttpMessageHeader *hmh );


/** \memberof HttpMessageHeader
 * 
 * Frees the HttpMessageHeader instance and all the variables that where initialized in this context.
 * 
 * \param hmh targeted HttpMessageHeader instance
 */
void hmhFree( HttpMessageHeader *hmh );

/** \memberof HttpMessageHeader
 * 
 * Convert hml to c string.
 * 
 * \param hmh targeted HttpMessageHeader instance
 * \param dest pointer to already existing dstring to write to, if dest == 0, new memory will be allocated
 * \return string 
 */
DString *hmhToString( HttpMessageHeader *hmh, DString *dest );

/**
 * HTTP RequestLine
 */
typedef struct HttpRequestLine {
	char *method;		/**< the method  */
	char *uri;			/**< the uri  */
	char *httpversion;	/**< the httpversion  */
} HttpRequestLine;

/** \memberof HttpRequestLine
 * 
 * Contructor - Allocates memory for a new HttpRequestLine instance. 
 *
 * \todo Write implementation. 
 * 
 * \return new HttpRequestLine instance
 */
HttpRequestLine *hrlInit();

/** \memberof HttpRequestLine
 * 
 * Resets a HttpRequestLine object for reuse
 * 
 * \param hrl targeted HttpRequestLine instance
 * \return targeted HttpRequestLine instance
 */
HttpRequestLine *hrlReset( HttpRequestLine *hrl );


/** \memberof HttpRequestLine
 * 
 * Frees the HttpRequestLine instance and all the variables that where initialized in this context.
 * 
 * \todo Write implementation. 
 * 
 * \param hrl targeted HttpRequestLine instance
 */
void hrlFree( HttpRequestLine *hrl );



/**
 * HTTP Message (without body)
 */
typedef struct HttpMessage {
	HttpStatusLine *statusLine;		/**<  */
	HttpRequestLine *requestLine;	/**<  */
	Vector *headerNames;				/**<  */
	Vector *headerValues;			/**<  */
} HttpMessage;

/** \memberof HttpMessage
 * 
 * Contructor - Allocates memory for a new HttpMessage instance. 
 *
 * \return new HttpMessage instance
 */
HttpMessage *hmInit();

/** \memberof HttpMessage
 * 
 * Resets a HttpMessage object for reuse
 * 
 * \param hm targeted HttpMessage instance
 * \return targeted HttpMessage instance
 */
HttpMessage *hmReset( HttpMessage *hm );

/** \memberof HttpMessage
 * 
 * Frees the HttpMessage instance and all the variables that where initialized in this context.
 * 
 * \param hm targeted HttpMessage instance
 */
void hmFree( HttpMessage *hm );


HttpMessage *hmAddHeader( HttpMessage *hm, const char *name, const char *value );


/** \memberof HttpMessage
 * 
 * Convert hml to c string.
 * 
 * \param hm targeted HttpMessage instance
 * \param dest pointer to already existing dstring to write to, if dest == 0, new memory will be allocated
 * \return string
 */
DString *hmToString( HttpMessage *hm, DString *dest );


#endif