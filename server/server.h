#ifndef __server_header__
#define __server_header__

#include "server/connections.h"

#include "server/serverconfig.h"

/**
 * Represents the Server
 */
typedef struct Server {
	ServerConfigs *cfg;			/**< Configurations for the server */
	Connections *cons;	/**< Connections for the server */
	char *readBuffer;			/**< Central buffer for receiving data from clients */
	int socket;					/**< Holds the server socket file descriptor. */
} Server;

/** \memberof Server
 * 
 * Contructor - Allocates memory for a new Server instance.
 * 
 * \return new Server instance
 */
Server *srvInit ( );

/** \memberof Server
 * 
 * Starts the server by opening socket and allocating central buffers. \n
 * \link ServerConfigs Server configurations \endlink must be load at this time.
 * 
 * \param srv targeted Server instance
 * \return targeted Server instance
 */
Server *srvStart( Server *srv );

/** \memberof Server
 * 
 * Do a poll cycle for connection #i
 * 
 * \param srv targeted Server instance
 * \param i index of connection
 */
void srvPollCycle( Server *srv, int i );

/** \memberof Server
 * 
 * Accepts a client with accept()
 * 
 * \param srv targeted Server instance
 * \return targeted Server instance
 */
void srvAcceptClient( Server *srv );

/** \memberof Server
 * 
 * Do a response cycle for connection #i
 * 
 * \param srv targeted Server instance
 * \param i index of connection
 * \return bool whether continue with other stuff in this poll cycle or not
 */
int srvResponseCycle( Server *srv, int i );

/** \memberof Server
 * 
 * Do a request cycle for connection #i
 * 
 * \param srv targeted Server instance
 * \param i index of connection
 * \return bool whether continue with other stuff in this poll cycle or not
 */
int srvRequestCycle( Server *srv, int i );


/** \memberof Server
 * 
 * Frees the server.
 * 
 * \param srv targeted Server instance
 */
void srvFree( Server *srv );


#endif
