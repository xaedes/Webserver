#ifndef __serverconfig_header__
#define __serverconfig_header__

#include "server/contenttype.h"

/**
 * Saves the configs for the server.
 */
typedef struct {
	int port;					/**< the port to listen at */
	int readBufferSize;			/**< the size of the buffer for receiving data from clients */
	int writeBufferSize;		/**< the size of the buffer for sending data to clients */
	int fileBufferSize;			/**< the size of the buffer for reading from files */
	const char *publicDir;		/**< the path (relativ to working directory) containing public data */
	const char *indexFile;		/**< the path of a index file, the browser is redirected to when trying * to access a directory (usually index.html) */
	
	ContentTypes *contentTypes;
	
	/**
	 * the http version the server is working with. 
	 * \todo should be placed in a struct ServerInfo, because this value shouldn't be configurable
	 */
	const char *httpversion;	/**<  */
	
	/**
	 * the servers public domain (e.g. http://foo.bar/ )
	 * e.g. used for redirecting. so should be set properly
	 * maybe this could be placed in a struct ServerInfo and retrieved automatically
	 */
	const char *serverDomain;	/**<  */
} ServerConfigs;

/** \memberof ServerConfigs
 * 
 * Contructor - Allocates memory for a new ServerConfigs instance.
 * 
 * \return new ServerConfigs instance
 */
ServerConfigs *srvCfgInit();

/** \memberof ServerConfigs
 * 
 * Loads the server configuration from programm arguments
 * Also sets default values
 * 
 * \todo should load configuration from file
 * 
 * \attention passed arguments will get modified by this method
 * 
 * \param cfg targeted ServerConfigs instance
 * \param argc number of passed arguments ( same as in main() )
 * \param argv programm arguments ( same as in main() )
 * \return targeted ServerConfigs instance
 */
ServerConfigs *srvCfgLoad( ServerConfigs *cfg, int argc, char *argv[] );

/** \memberof ServerConfigs
 * 
 * Frees the server configs.
 * 
 * \attention strings are not free'd, because they point to c string literals for now
 * 
 * \param cfg targeted ServerConfigs instance
 */
void srvCfgFree( ServerConfigs *cfg );

#endif