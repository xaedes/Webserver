#ifndef __http_header__
#define __http_header__

#include "lib/vector.h"
#include "lib/dstring.h"

//http://www.w3.org/Protocols/rfc2616/rfc2616.html

#include "http/http_enums.h"
#include "http/http_statusline.h"
#include "http/http_requestline.h"






/**
 * HTTP Message (without body)
 */
typedef struct HttpMessage {
	HttpMessageType type; 
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