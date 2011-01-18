
#include "server/responsefactory.h"
#include "cli/cli.h"
#include "lib/general.h"
#include "lib/error.h"


#include <stdlib.h>
#include <string.h>


ResponseFactory *rfInit()
{
	ResponseFactory * rf = malloc( sizeof( ResponseFactory ) );
	if ( ! rf ) {
		handle_error( "rfInit" );
	}
	memset( rf, 0, sizeof( ResponseFactory ) );
	return rf;
}

void rfFree(ResponseFactory* rf)
{
	free( rf );
}

ResponseFactory* rfProduce(ResponseFactory* rf, int i)
{
	Client *client = &rf->cons->clients[i];
	
	Response *rsp = client->rsp;
	
	consConnectionSetIOStatus( rf->cons, i, ios_send_respond );

	if ( strcasecmp( rsp->rqst->httpversion, "" ) == 0 ) {
		dynstrcpy( &rsp->rqst->httpversion, "HTTP/1.0" );
		rsp->message->http->statusLine->httpversion = HTTP_VERSION_1_0;
	}
	
	if ( ! (( strcasecmp( rsp->rqst->httpversion, "HTTP/1.1") == 0 ) ||
			( strcasecmp( rsp->rqst->httpversion, "HTTP/1.0") == 0 ) ) ) {
		rsp->message->http->statusLine->statuscode = STATUS_BAD_REQUEST;
		rsp->message->http->statusLine->httpversion = HTTP_VERSION_1_0;
		dynstrcpy( &rsp->rqst->httpversion, "HTTP/1.0" );
	} else {
		rsp->message->http->statusLine->httpversion = httpVersionFromString( rsp->rqst->httpversion );
		if ( strcasecmp( rsp->rqst->method, "GET" ) == 0 ) {
	
			
			if ( strcasecmp( rsp->rqst->uri, "/broadcast.service" ) == 0 ) {
				
			} else {
				DString *filename = dsInit( 256 );
				dsCpyString( filename, rf->cfg->publicDir );
				dsCatString( filename, rsp->rqst->uri );
				dsZeroTerminate( filename );
				
				if ( fisdir( filename->buffer ) ) {
					if ( dsLast( filename ) == '/' ) {
						dsCpyString( filename, rf->cfg->serverDomain );
						dsCatString( filename, ( rsp->rqst->uri[0] == '/' ? 1 : 0 ) + rsp->rqst->uri );
						dsCatString( filename, rf->cfg->indexFile );
						
						rsp->message->http->statusLine->statuscode = STATUS_FOUND;
						
					} else {
						dsCpyString( filename, rf->cfg->serverDomain );
						dsCatString( filename, ( rsp->rqst->uri[0] == '/' ? 1 : 0 ) + rsp->rqst->uri );
						dsCatString( filename, "/" );

						rsp->message->http->statusLine->statuscode = STATUS_MOVED_PERM;
					}
					dsZeroTerminate( filename );

					hmAddHeader( rsp->message->http, "Location", filename->buffer );
					hmAddHeader( rsp->message->http, "Content-Length", "0" );
					
				} else { //if ( fisdir( filename ) )
					if ( fexists( filename->buffer ) ) {
						char *ext = fextension( filename->buffer );
						char *cli = smGet( rf->cfg->cli, ext );
						char *ct = smGet( rf->cfg->contentTypes, ext );
						if ( cli ) {
/*							if ( strcasecmp( cli, "php" ) == 0 ) {
								cliPHP( rf->cfg, rf->cons, i, filename );
							}*/
						} else if ( ct ) {
							rsp->message->http->statusLine->statuscode = STATUS_OK;
							
							DString *size = itods( fsize( filename->buffer ) );
							dsZeroTerminate( size );
							hmAddHeader( rsp->message->http, "Content-Length", size->buffer );
							
							DString *dct = dsInit( 16 );
							dsCpyString( dct, ct );
							dsCatChar( dct, '/' );
							dsCatString( dct, ext );
							dsZeroTerminate( dct );
							
							hmAddHeader( rsp->message->http, "Content-Type", dct->buffer );
							
							vcPush( rsp->dstrings, size );
							vcPush( rsp->dstrings, dct );
							
							if ( !rsp->appendix )
								rsp->appendix = rspApdxInit( rf->cfg->fileBufferSize, filename->buffer );
							else
								rsp->appendix = rspApdxOpen( rsp->appendix, filename->buffer );
							rsp->sendAppdx = 1;
						} else {	//extension von angeforderter datei ist nicht configuriert
							rsp->message->http->statusLine->statuscode = STATUS_FORBIDDEN;
							hmAddHeader( rsp->message->http, "Content-Length", "0" );
						}
					} else {
						rsp->message->http->statusLine->statuscode = STATUS_NOT_FOUND;
						hmAddHeader( rsp->message->http, "Content-Length", "0" );
					}
				} // if ( fisdir( filename ) ) else
			
				vcPush( rsp->dstrings, filename ); 
			}
		} // if ( strcmp( rsp->rqst->method, "GET" ) == 0 )
		else {
			rsp->message->http->statusLine->statuscode = STATUS_BAD_REQUEST;
			dynstrcpy( &rsp->rqst->httpversion, "HTTP/1.0" );
			hmAddHeader( rsp->message->http, "Content-Length", "0" );
		}
	} //if <legal request http version > else
	
	hmToString( rsp->message->http, rsp->message->data );
	dsZeroTerminate( rsp->message->data );
	
	
	return rf;
}
