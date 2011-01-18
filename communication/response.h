#ifndef __response_header__
#define __response_header__

#include "communication/requests.h"
#include "http/http.h"
#include "lib/dstring.h"
#include "lib/vector.h"
#include "server/serverconfig.h"
#include <stdio.h>

/**
 * Represents the file appendix for a response.
 */
typedef struct responseAppendix {
	DString *filename;	/**< filename of the file */
	int file;			/**< reference to opened file */
	char *buffer;		/**< file read buffer */
	int bufSize;		/**< file read buffer size */
	int bufFill;		/**< current size of buffer (how many bytes are sendable) */
	int sent; 			/**< number of sent bytes from current buffer content */
	int eof;			/**< is the end of file reached? \n i.e. do we need to reload buffer after complete sending? */
} ResponseAppendix;

/** \memberof ResponseAppendix
 * 
 * Contructor - Allocates memory for a new ResponseAppendix instance. 
 * Allocates memory for buffer, stores copy of filename and opens
 * the file. 
 * 
 * \param bufSize the desired file read buffer size
 * \param filename 
 * \return new ResponseAppendix instance
 */
ResponseAppendix *rspApdxInit( int bufSize, char *filename );

/** \memberof ResponseAppendix
 * 
 * Loads data from file to the buffer. If eof is reached, the corresponding
 * variable (rspApdx->eof) get set and the file get closed.
 * 
 * \param rspApdx targeted ResponseAppendix instance
 * \return count of read bytes
 */
int rspApdxLoad( ResponseAppendix *rspApdx );

/** \memberof ResponseAppendix
 * 
 * Resets a ResponseAppendix object for reuse
 * 
 * \param rspApdx targeted ResponseAppendix instance
 * \return targeted ResponseAppendix instance
 */
ResponseAppendix *rspApdxReset( ResponseAppendix *rspApdx );

/** \memberof ResponseAppendix
 * 
 * Opens a file
 * 
 * \param rspApdx targeted ResponseAppendix instance
 * \param filename
 * \return targeted ResponseAppendix instance
 */
ResponseAppendix *rspApdxOpen( ResponseAppendix *rspApdx, char *filename );


/** \memberof ResponseAppendix
 * 
 * Frees the ResponseAppendix instance and all the variables that where initialized in this context.
 * 
 * \param rspApdx targeted ResponseAppendix instance
 */
void rspApdxFree( ResponseAppendix *rspApdx );

/**
 * Represents a reponse message (without body)
 */
typedef struct ResponseMessage {
	HttpMessage *http;	/**< data object of msg */
	DString *data;		/**< data of msg */
	int sent;			/**< number of sent bytes from message header */
} ResponseMessage;

/** \memberof ResponseMessage
 * 
 * Contructor - Allocates memory for a new ResponseMessage instance. 
 *
 * \return new ResponseMessage instance
 */
ResponseMessage *rspmInit();

/** \memberof ResponseMessage
 * 
 * Frees the ResponseMessage instance and all the variables that where initialized in this context.
 * 
 * \param rspm targeted ResponseMessage instance
 */
void rspmFree( ResponseMessage *rspm );

/** \memberof ResponseMessage
 * 
 * Resets a ResponseMessage object for reuse
 * 
 * \param rspm targeted ResponseMessage instance
 * \return targeted ResponseMessage instance
 */
ResponseMessage *rspmReset( ResponseMessage *rspm );



/**
 * Generates and saves the response for a given Request object
 */
typedef struct Response {
	Vector *strings;	/**< Saves pointers to strings that have to be free'd */
	Vector *dstrings;	/**< Saves pointers to strings that have to be free'd */
	Request *rqst; 				/**< corresponding Request object */
	ResponseMessage *message;	/**< saves the generated message */
	int sendState; 				/**< stores whether currently sending message header ( == 0) or message body ( == 1) */
	int sendAppdx; 				/**< stores whether sending message body is scheduled */
	ResponseAppendix *appendix;	/**< stores the message body (a file) \n can be null */
} Response;


/** \memberof Response
 * 
 * Contructor - Allocates memory for a new Response instance. 
 * 
 * \return new Response instance
 */
Response *rspInit(  );

/** \memberof Response
 * 
 * Resets a Response object for reuse
 * 
 * \param rsp targeted Response instance
 * \return targeted Response instance
 */
Response *rspReset( Response *rsp );


/** \memberof Response
 * 
 * Generates the response data.
 * 
 * \param rsp targeted Response instance
 * \param cfg ServerConfigs object \n provides necessary info
 * \return targeted Response instance
 */
Response *rspBuild( Response *rsp, ServerConfigs *cfg );


/** \memberof Response
 * 
 * Frees the Response instance and all the variables that where initialized in this context.
 * 
 * \param rspm targeted Response instance
 */
void rspFree( Response *rsp );



#endif