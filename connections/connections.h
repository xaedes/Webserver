#ifndef __connections_header__
#define __connections_header__

#include "connection.h"
#include <poll.h>

typedef struct ResponseFactory ResponseFactory;
typedef struct Server Server;

/**
 * Manages a bunch of connections together with a pollfd array
 * For poll() we need a struct pollfd array and we have to store additional information for each connection.
 * Connections keeps the two arrays ( Connection[] and pollfd[] ) sync.
 */
struct Connections{
	int size;				/**< current number of managed connections */
	int reserved;			/**< current maximum number of connections */
	Connection *cons;		/**< array for additional connection information  */
	struct pollfd *poll;	/**< array for poll() */
	
	ResponseFactory *responseFactory;	/**< Pointer to ResponseFactory for this Connections */
	Server *srv;
};

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
 * Frees the Connections instance and all the variables that where initialized in this context.
 * 
 * \param cons targeted Connections instance
 */
void consFree( Connections *cons );

#endif
