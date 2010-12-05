#ifndef __requests_header__
#define __requests_header__

#include "lib/lines.h"
#include "lib/vector.h"

/**
 * Saves and parses an incoming request from a client
 */

typedef struct Request {
	Lines *lns;			/**< Lines to parse from */
	char *method;		/**< the parsed method from the request line */
	char *uri;			/**< the parsed uri from the request line */
	char *httpversion;	/**< the parsed httpversion from the request line */
	//HTTPHeader *headers;
} Request;


/** \memberof Request
 *
 * Contructor - Allocates memory for a new Request instance.
 *
 * \return new Request instance
 */
Request *rqstInit( Lines *lns );

/** \memberof Request
 *
 * Parses the request line and stores the parsed data into the targeted object.
 *
 * \param rqst targeted Request instance
 * \return targeted Request instance
 */
Request *rqstParseRequestLine( Request *rqst );

/** \memberof Request
 *
 * Parses the whole request
 *
 * \param rqst targeted Request instance
 * \return targeted Request instance
 */
Request *rqstParse( Request *rqst );

/** \memberof Request
 *
 * Resets a Request object for reuse
 *
 * \param rqst targeted Request instance
 * \return targeted Request instance
 */
Request *rqstReset( Request *rqst );

/** \memberof Request
 *
 * Frees the Request instance and all the variables that where initialized in this context.
 *
 * \param rqst targeted Request instance
 */
void rqstFree( Request *rqst );

/*

typedef struct requests {
	Vector *vc;
} Requests;

Requests *rqstsInit();

Requests *rqstsAdd( Requests *rqsts, Request *rqst );

*/

#endif
