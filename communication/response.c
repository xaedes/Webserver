#include "communication/response.h"
#include "communication/requests.h"
#include "lib/error.h"
#include "lib/dynstringstream.h"
#include "lib/general.h"
#include "server/serverconfig.h"
#include "http/http.h"
#include "lib/dstring.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

ResponseAppendix *rspApdxInit( int bufSize, char *filename )
{
	ResponseAppendix *rspApdx = malloc( sizeof( ResponseAppendix ) );
	if ( !rspApdx )
		handle_error( "rspApdxInit" );
	memset( rspApdx, 0, sizeof( ResponseAppendix ) );

	rspApdx->bufSize = bufSize;
	rspApdx->buffer = malloc( bufSize );
	if ( !rspApdx->buffer )
		handle_error( "rspApdxInit" );
	memset( rspApdx->buffer, 0, bufSize );
	rspApdx->filename = dsInit( 128 );
	rspApdxOpen( rspApdx, filename );
	return rspApdx;
}

ResponseAppendix* rspApdxReset( ResponseAppendix* rspApdx )
{
	//DString *filename;
	rspApdx->filename->size = 0;
	
	//FILE *file;
	if ( rspApdx->file >= 0 )
		close( rspApdx->file );
	rspApdx->file = -1;
	
	//char *buffer;
	//nothing to do
	
	//int bufSize;
	//nothing to do
	
	//int bufFill;
	rspApdx->bufFill = 0;
	
	//int sent;
	rspApdx->sent = 0;
	
	//int eof;
	rspApdx->eof = 0;
	
	return rspApdx;
}

ResponseAppendix* rspApdxOpen( ResponseAppendix* rspApdx, char* filename )
{
	dsCpyString( rspApdx->filename, filename );
	dsZeroTerminate( rspApdx->filename );
//	rspApdx->file = open( rspApdx->filename->buffer, O_RDONLY );
	rspApdx->file = open( rspApdx->filename->buffer, O_RDONLY | O_NONBLOCK );
	if ( rspApdx->file < 0 )
		handle_error( "rspApdxOpen" );
	rspApdx->eof = 0;
	rspApdx->sent = 0;
	rspApdx->bufFill = 0;
	return rspApdx;
}

//lädt bytes von der datei in den buffer
//return value : anzahl geladener bytes
//schließt datei automatisch, wenn dateiende erreicht
int rspApdxLoad( ResponseAppendix *rspApdx )
{
	rspApdx->bufFill = read( rspApdx->file, rspApdx->buffer, rspApdx->bufSize );
	
	
	if( rspApdx->bufFill == 0 ) {
		close( rspApdx->file );
		rspApdx->file = -1;
		rspApdx->eof = 1;
	} else if ( rspApdx->bufFill < 0 ) {
		if ( ( errno != EAGAIN ) && ( errno != EWOULDBLOCK ) )
			handle_error( "rspApdxLoad" );
		else
			rspApdx->bufFill = 0;
	}

	rspApdx->sent = 0;

	return rspApdx->bufFill;
}

void rspApdxFree( ResponseAppendix *rspApdx )
{
	if ( !rspApdx )
		return;

	if ( rspApdx->file >= 0 )
		close( rspApdx->file );
	if ( rspApdx->filename )
		dsFree( rspApdx->filename );
	if ( rspApdx->buffer )
		free( rspApdx->buffer );
	free( rspApdx );
}




ResponseMessage *rspmInit()
{
	ResponseMessage * rspm = malloc( sizeof( ResponseMessage ) );
	if ( ! rspm ) {
		handle_error( "rspmInit" );
	}
	memset( rspm, 0, sizeof( ResponseMessage ) );
	rspm->http = hmInit();
	rspm->http->type = HTTP_MESSAGE_RESPONSE;
	rspm->data = dsInit(256);
	return rspm;
}

void rspmFree( ResponseMessage* rspm )
{
	if ( !rspm )
		return;
	
	if ( rspm->data )
		dsFree( rspm->data );
	
	if ( rspm->http )
		hmFree( rspm->http );
	
	free( rspm );
}

ResponseMessage* rspmReset( ResponseMessage* rspm )
{
	//HttpMessage *http;
	hmReset( rspm->http );
	rspm->http->type = HTTP_MESSAGE_RESPONSE;
	//DString *data;
	rspm->data->size = 0;
	//int sent;
	rspm->sent = 0;
	
	
	
	return rspm;
}


Response *rspInit( ) {
	Response *rsp = malloc( sizeof( Response ) );
	if ( !rsp )
		handle_error( "rspInit" );
	memset( rsp, 0, sizeof( Response ) );
	rsp->message = rspmInit();
	rsp->strings = vcInit();
	rsp->dstrings = vcInit();
	
	return rsp;
}

Response* rspReset( Response* rsp )
{
	//Vector *strings;
	while( rsp->strings->size ) {
		free( vcPop( rsp->strings ) );
	}

	//Vector *dstrings;
	while( rsp->dstrings->size ) {
		dsFree( vcPop( rsp->dstrings ) );
	}
	
	//Request *rqst;
	rsp->rqst = 0;
	
	//ResponseMessage *message;
	rspmReset( rsp->message );
	
	//int sendState;
	rsp->sendState = 0;
	
	//int sendAppdx;
	rsp->sendAppdx = 0;
	
	//ResponseAppendix *appendix;
	if ( rsp->appendix )
		rspApdxReset( rsp->appendix );

	
	
	
	return rsp;
}

void rspFree( Response *rsp )
{
	if ( rsp->message )
		rspmFree( rsp->message );
	if ( rsp->appendix )
		rspApdxFree( rsp->appendix );
	
	if ( rsp->strings ) {
		while( rsp->strings->size ) {
			free( vcPop( rsp->strings ) );
		}
		vcFree( rsp->strings );
	}
	if ( rsp->dstrings ) {
		while( rsp->dstrings->size ) {
			dsFree( vcPop( rsp->dstrings ) );
		}
		vcFree( rsp->dstrings );
	}
	
	free( rsp );
}


Response *rspBuild( Response *rsp, ServerConfigs *cfg ) {
	if ( !rsp->rqst )
		handle_fail( "rspBuild: request null poiner" );
	if ( !cfg )
		handle_fail( "rspBuild: server config null poiner" );

	
	if ( strcmp( rsp->rqst->httpversion, "" ) == 0 ) {
		dynstrcpy( &rsp->rqst->httpversion, "HTTP/1.0" );
		rsp->message->http->statusLine->httpversion = HTTP_VERSION_1_0;
	}
	
	if ( ! (( strcmp( rsp->rqst->httpversion, "HTTP/1.1") == 0 ) ||
			( strcmp( rsp->rqst->httpversion, "HTTP/1.0") == 0 ) ) ) {
		rsp->message->http->statusLine->statuscode = STATUS_BAD_REQUEST;
		rsp->message->http->statusLine->httpversion = HTTP_VERSION_1_0;
		dynstrcpy( &rsp->rqst->httpversion, "HTTP/1.0" );
	} else {
		rsp->message->http->statusLine->httpversion = httpVersionFromString( rsp->rqst->httpversion );
		if ( strcmp( rsp->rqst->method, "GET" ) == 0 ) {
	
			
			DString *filename = dsInit( 256 );
			dsCpyString( filename, cfg->publicDir );
			dsCatString( filename, rsp->rqst->uri );
			dsZeroTerminate( filename );
			
			if ( fisdir( filename->buffer ) ) {
				if ( dsLast( filename ) == '/' ) {
					dsCpyString( filename, cfg->serverDomain );
					dsCatString( filename, ( rsp->rqst->uri[0] == '/' ? 1 : 0 ) + rsp->rqst->uri );
					dsCatString( filename, cfg->indexFile );
					
					rsp->message->http->statusLine->statuscode = STATUS_FOUND;
					
				} else {
					dsCpyString( filename, cfg->serverDomain );
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
					char *ct = smGet( cfg->contentTypes, ext );
					if ( ct ) {
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
							rsp->appendix = rspApdxInit( cfg->fileBufferSize, filename->buffer );
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
		} // if ( strcmp( rsp->rqst->method, "GET" ) == 0 )
		else {
			rsp->message->http->statusLine->statuscode = STATUS_BAD_REQUEST;
			dynstrcpy( &rsp->rqst->httpversion, "HTTP/1.0" );
			hmAddHeader( rsp->message->http, "Content-Length", "0" );
		}
	} //if <legal request http version > else
	
	hmToString( rsp->message->http, rsp->message->data );
	dsZeroTerminate( rsp->message->data );
	
	return rsp;
}



