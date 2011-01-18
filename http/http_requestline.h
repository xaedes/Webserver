#ifndef __http_requestline_header__
#define __http_requestline_header__


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
 * 
 * \param hrl targeted HttpRequestLine instance
 */
void hrlFree( HttpRequestLine *hrl );



#endif