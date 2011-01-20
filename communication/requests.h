#ifndef __requests_header__
#define __requests_header__

#include "lib/lines.h"
#include "lib/vector.h"
#include "lib/dstring.h"
#include "http/http.h"

/**
 * Saves and parses an incoming request from a client
 */
typedef struct Request {
	int lastNCR;
	int lastNLF;
	DString *buffer;	/**< Buffer to read to */
	DString *buffer2;	/**< Buffer to read to */
	DString *backup;	
	Lines *lns;			/**< Lines to parse from */
	char *method;		/**< the parsed method from the request line */
	char *uri;			/**< the parsed uri from the request line */
	char *httpversion;	/**< the parsed httpversion from the request line */
	HttpMessage *http;
	Vector *strings;	/**< Saves pointers to strings that have to be free'd */
	Vector *dstrings;	/**< Saves pointers to dstrings that have to be free'd */
	//HTTPHeader *headers;
} Request;


/**
 * Returns the size of the string until a CR LF CR LF 
 * sequence is reached (excluding CR LF CR LF). 
 * If there is no CR LF CR LF sequence, size is
 * returned.
 */
int sizeUntilDoubleNewline( int *nCR, int *nLF, const char *string, int size );


/** \memberof Request
 *
 * Contructor - Allocates memory for a new Request instance.
 *
 * \return new Request instance
 */
Request *rqstInit( );

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
