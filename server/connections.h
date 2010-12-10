#ifndef __connections_header__
#define __connections_header__

#include "lib/lines.h"
#include "communication/requests.h"
#include "communication/response.h"
#include <poll.h>
#include <netinet/in.h>

typedef enum io_status {
	ios_none, ios_receive, ios_send
} IO_Status;

/**
 * Stores client spezific data
 */
typedef struct Client {
	int initialized;
	int sfd;					/**< socket file descriptor to client */
	struct sockaddr_in addr;	/**< address given by accept() */
	Lines *lnsRead;				/**< stores read data from client */
	Request *rqst;				/**< stores the current request object from this client */
	Response *rsp;				/**< stores the current response object for this client*/
	IO_Status ios;				/**< either we receive data from client, or we are sending data to client */
} Client;

//Client *clntInit();

/**
 * Manages all connections
 * For poll() we need a struct pollfd array and we have to store additional information for each client in another client.
 * Connections keeps the two array sync.
 */
typedef struct connections{
	int size;				/**< current number of managed connections */
	int reserved;			/**< current maximum number of connections */
	Client *clients;		/**< array for additional client information  */
	struct pollfd *poll;	/**< array for poll() */
} Connections;

/** \memberof Connections
 * 
 * Contructor - Allocates memory for a new Connections instance. 
 * Sets starting size for arrays to 10 
 * 
 * \return new Connections instance
 */
Connections *consInit();

/** \memberof Connections
 * 
 * Enlarges the the arrays.
 * 
 * \param cons targeted Connections instance
 * \return targeted Connections instance
 */
Connections *consEnlarge( Connections *cons );

/** \memberof Connections
 * 
 * Makes place for a new connection and return index of free connection slot.
 * 
 * \param cons targeted Connections instance
 * \return index of free connection
 */
int consAdd( Connections *cons );

/** \memberof Connections
 * 
 * Deletes the connection at the given index and frees it.
 * 
 * \param cons targeted Connections instance
 * \param i index
 * \return targeted Connections instance
 */
void consDel( Connections *cons, int i ); 

/** \memberof Connections
 * 
 * Inits a connection.
 * 
 * \param cons targeted Connections instance
 * \param i index of connection
 * \return targeted Connections instance
 */
Connections *consConnectionInit( Connections *cons, int i );

/** \memberof Connections
 * 
 * Sets up the connection at index i
 * 
 * \param cons targeted Connections instance
 * \param i index
 * \param fd socket file descriptor to client
 * \param addr client address info provided by accept()
 * \return targeted Connections instance
 */
Connections *consConnectionSetup( Connections *cons, int i, int fd, struct sockaddr_in );

/** \memberof Connections
 * 
 * Changes the io status of a connection. Switch between send and receive.
 * 
 * \param cons targeted Connections instance
 * \param i index
 * \param ios new io status ( send | receive )
 * \return targeted Connections instance
 */
Connections *consConnectionSetIOStatus( Connections *cons, int i, IO_Status ios );

/** \memberof Connections
 * 
 * Resets a connection slot
 * 
 * \param cons targeted Connections instance
 * \param i index
 * \return targeted Connections instance
 */
Connections *consConnectionReset( Connections *cons, int i );

/** \memberof Connections
 * 
 * Frees a connection and all the variables that where initialized in this context.
 * 
 * \param cons targeted Connections instance
 * \param i index of connection
 * \return targeted Connections instance
 */
Connections *consConnectionFree( Connections *cons, int i );

/** \memberof Connections
 * 
 * Frees the Connections instance and all the variables that where initialized in this context.
 * 
 * \param cons targeted Connections instance
 */
void consFree( Connections *cons );

#endif
