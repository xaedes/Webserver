#ifndef __response_factory_header__
#define __response_factory_header__

#include "connection.h"
#include "connections.h"
#include "server/serverconfig.h"

/**
 * Response Factory
 */
struct ResponseFactory {
	ServerConfigs *cfg;		/**< Configurations for the server */
	Connections *cons;		/**< Reference to Connections */
};

/** \memberof ResponseFactory
 * 
 * Contructor - Allocates memory for a new ResponseFactory instance. 
 *
 * \return new ResponseFactory instance
 */
ResponseFactory *rfInit();

/** \memberof ResponseFactory
 * 
 * Frees the ResponseFactory instance and all the variables that where initialized in this context.
 * 
 * \param rf targeted ResponseFactory instance
 */
void rfFree( ResponseFactory *rf );

/** \memberof ResponseFactory
 * 
 * Produces Response(s) for the given Connection 
 * 
 * \param rf targeted ResponseFactory instance
 * \param con Connection
 * \return targeted ResponseFactory instance
 */
ResponseFactory *rfProduce( ResponseFactory *rf, Connection *con );


#endif