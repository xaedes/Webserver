#include "server/serverconfig.h"
#include "lib/error.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

ServerConfigs *srvCfgInit()
{
	ServerConfigs *cfg = malloc( sizeof( ServerConfigs ) );
	if ( !cfg )
		handle_error( "srvCfgInit" );
	
	memset( cfg, 0, sizeof( ServerConfigs ) );

	cfg->contentTypes = smInit();
	cfg->cli = smInit();
	
	return cfg;
}

ServerConfigs *srvCfgLoad( ServerConfigs *cfg, int argc, char *argv[] )
{
	cfg->port = 8888;

	int c;
	while( ( c = getopt( argc, argv, "hp:" ) ) >= 0 ) {
		switch ( ( char )c ) {
		case 'p':


			errno = 0;
			long val = strtol( optarg, 0, 10 );
			if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
			                     || (errno != 0 && val == 0)) {
				handle_error( "port parameter parsing" );
			}

			if ( ( val < 0 ) || ( val > 65535 ) )
				handle_fail( "port parameter out of range - must be between 0 .. 65535" );

			cfg->port = val;

			break;
		case 'h':
			printHelp();
			return 0;
			break;
		case '?':
			if ( ( char )optopt == 'p' )	//Portangabe vergessen
			{
				//handle_error("-p requires an argument");
				printHelp();
				//exit(EXIT_FAILURE);
			}
			//else //unbekannte Optionen - einfach ignorieren


			break;
		default://unbekannte Optionen - einfach ignorieren
			break;
		}
	}

	cfg->readBufferSize = 200;
	cfg->writeBufferSize = 200;
	cfg->fileBufferSize = 200;
	cfg->publicDir = "../htdocs";
	cfg->indexFile = "index.html";
	cfg->httpversion = "HTTP/1.1";
	cfg->serverDomain = "http://xaedes.dyndns.org:8888/";
	cfg->pipeDir = "/home/martin/tmp";

	smAdd( cfg->contentTypes, "png", "image" );
	smAdd( cfg->contentTypes, "jpg", "image" );
	smAdd( cfg->contentTypes, "ico", "image" );
	smAdd( cfg->contentTypes, "html", "text" );
	smAdd( cfg->contentTypes, "css", "text" );
	smAdd( cfg->contentTypes, "", "application" );
	
	smAdd( cfg->cli, "php", "php" );
	
	return cfg;
}


void srvCfgFree( ServerConfigs* cfg )
{
	if ( !cfg )
		return;
	
	smFree( cfg->contentTypes );
	smFree( cfg->cli );
	
	free( cfg );
}











