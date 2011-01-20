#ifndef __connection_header__
#define __connection_header__

#include "lib/lines.h"
#include "communication/requests.h"
#include "communication/response.h"
#include <poll.h>
#include <netinet/in.h>

/**
 * Connection State
 * 
 * State diagram:
 * \dot
digraph g{
  a -> d [label="header complete"];
  d -> b [label="built response"];
  d -> e [label="broadcast.service block"];
  d -> f [label="broadcast.service post"];
  f -> g [label="body complete"];
  b -> c [label="response has body"];
  b -> a [label="response has no body"];
  c -> a [label="response complete"];
  g -> a [label="broadcast"];
  e -> b [label="triggered by broadcast"];
  h -> a [label="start of a new connection"];
  a -> i [label="other side hung up"];
  b -> j [label="sent"];
  c -> j [label="sent"];
  j -> i [label="http/1.0"];
  j -> a [label=""];
  a [label="cs_receive_httpheaders",shape=box,style="filled,rounded"];
  b [label="cs_send_httpheaders",shape=box,style="filled,rounded"];
  c [label="cs_send_httpmsgbody",shape=box,style="filled,rounded"];
  d [label="cs_handle_requestheader",shape=box,style="filled,rounded"];
  e [label="cs_wait_forbroadcast",shape=box,style="filled,rounded"];
  f [label="cs_receive_httpmsgbody",shape=box,style="filled,rounded"];
  g [label="cs_handle_requestwithbody",shape=box,style="filled,rounded"];
  h [label="cs_none",shape=box,style="filled,rounded"];
  i [label="cs_lost",shape=box,style="filled,rounded"];
  j [label="cs_send_complete",shape=box,style="filled,rounded"];
  
  
}
 * \enddot
 */
typedef enum con_state {
	cs_none,						/**< default state - do nothing */
	cs_receive_httpheaders, cs_send_httpheaders, cs_send_httpmsgbody,	/**< normal http get traffic */
	cs_handle_requestheader,		/**< while in this state, a complete request header is handled */
	cs_handle_requestwithbody,		/**< while in this state, a complete request with body is handled */
	cs_wait_forbroadcast,			/**< special state for broadcasting service */
	cs_receive_httpmsgbody,			/**< for receiving a http message body ( POST method ) */
	cs_send_complete,				/**< send of header and body complete */
	cs_accept_clients,				/**< only for the special server connection */
	cs_lost							/**< connection lost */
} Con_State;

typedef struct Connections Connections;

/**
 * Stores Connection specific data
 * 
 * The socket file descriptor is stored here though it is already stored in the associated pollfd struct.
 * This is because in the feature we may not use pollfd at all, so its better to store this very important
 * information HERE as well.
 * 
 * When adding new members, a change in the following function is necessary:
 *  - conInit
 *  - conReset
 *  - conFree
 * 
 */
typedef struct Connection {
	Connections *cons;			/**< pointer to the Connections that manages this Connection, set by Connections */
	int index;					/**< index of the Connection in its Connections Object*/
	struct pollfd *poll;		/**< pointer to the poll struct for this connection, Connections takes care to set it properly */

void * begin;					/**< "memory label" to indicate begin of content managed by connections */
	int initialized;			/**< flag whether this client is initialized or not */
	
	
	
	int sfd;					/**< socket file descriptor to client */
	struct sockaddr_in addr;	/**< address given by accept() */
	
	Con_State state;			/**< connection status \see Con_State */
	
	
	Lines *lnsRead;				/**< stores read data from client */
	Request *rqst;				/**< stores the current request object from this client */
	Response *rsp;				/**< stores the current response object for this client*/
	
void * end;						/**< "memory label" to indicate end of content managed by connections */
} Connection;

/** \memberof Connection
 * 
 * Inits a connection. This means, it allocates memory etc.
 * After this the connection object is prepared to be set up.
 * 
 * \param con targeted Connection instance
 * \return targeted Connections instance
 */
Connection *conInit( Connection *con );

/** \memberof Connection
 * 
 * Sets up an already initialized connection 
 * 
 * \param con targeted Connection instance
 * \param fd socket file descriptor to client
 * \param addr client address info provided by accept()
 * \return targeted Connection instance
 */
Connection *conSetup( Connection *con, int fd, struct sockaddr_in *addr );

/** \memberof Connection
 * 
 * Resets a Connection. After this it is ready to be set up, as it were initialized with conInit().
 * This is used to improve performance by recycling already malloced memory.
 * 
 * \param con targeted Connection instance
 * \return targeted Connection instance
 */
Connection *conReset( Connection *con  );

/** \memberof Connection
 * 
 * Frees a connection and all the variables that where initialized in this context.
 * 
 * \param con targeted Connection instance
  * \return targeted Connection instance
 */
Connection *conFree( Connection *con );


/** \memberof Connection
 * 
 * Changes the state of a connection. This also manages the content of the pollfd struct.
 * It is possible, that this method is called recursive.
 * 
 * \param con targeted Connection instance
 * \param state new state \see Con_State
 * \return targeted Connection instance
 */
Connection *conChangeState( Connection *con, Con_State state );

/** \memberof Connection
 * 
 * Performs the necessary actions to exit a state for connection
 * 
 * \param con targeted Connection instance
 * \param state the state to be exited
 * \return targeted Connection instance
 */
Connection *conExitState( Connection *con, Con_State state );

/** \memberof Connection
 * 
 * Performs the necessary actions to enter a state for connection
 * 
 * \param con targeted Connection instance
 * \param state the state to be entered
 * \return targeted Connection instance
 */
Connection *conEnterState( Connection *con, Con_State state );

/** \memberof Connection
 * 
 * Handles PollIn events for the Connection
 * 
 * \param con targeted Connection instance
 * \return targeted Connection instance
 */
Connection *conOnPollIn( Connection *con );

/** \memberof Connection
 * 
 * Handles PollOut events for the Connection
 * 
 * \param con targeted Connection instance
 * \return targeted Connection instance
 */
Connection *conOnPollOut( Connection *con );

/** \memberof Connection
 * 
 * Handles PollHup events for the Connection
 * 
 * \param con targeted Connection instance
 * \return targeted Connection instance
 */
Connection *conOnPollHup( Connection *con );


/** \memberof Connection
 * 
 * Handles PollErr events for the Connection
 * 
 * \param con targeted Connection instance
 * \return targeted Connection instance
 */
Connection *conOnPollErr( Connection *con );




#endif