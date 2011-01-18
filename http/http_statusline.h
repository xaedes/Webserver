#ifndef __http_statusline_header__
#define __http_statusline_header__

#include "http/http_enums.h"
#include "lib/dstring.h"

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


#endif