#include "communication/response.h"
#include "communication/requests.h"
#include "lib/error.h"
#include "lib/dynstringstream.h"
#include "lib/general.h"
#include "server/serverconfig.h"
#include "http/http.h"
#include "lib/dstring.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
	if ( rspApdx->file )
		fclose( rspApdx->file );
	rspApdx->file = 0;
	dsCpyChars( rspApdx->filename, 0, 0 );
	rspApdx->eof = 0;
	rspApdx->sent = 0;
	rspApdx->bufFill = 0;
	return rspApdx;
}

ResponseAppendix* rspApdxOpen( ResponseAppendix* rspApdx, char* filename )
{
	dsCpyString( rspApdx->filename, filename );
	dsZeroTerminate( rspApdx->filename );
	rspApdx->file = fopen( rspApdx->filename->buffer, "r" );
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
	rspApdx->bufFill = fread( rspApdx->buffer, 1, rspApdx->bufSize, rspApdx->file );

	if( feof( rspApdx->file ) ) {
		fclose( rspApdx->file );
		rspApdx->file = 0;
		rspApdx->eof = 1;
	}

	rspApdx->sent = 0;

	return rspApdx->bufFill;
}

void rspApdxFree( ResponseAppendix *rspApdx )
{
	if ( !rspApdx )
		return;

	if ( rspApdx->file )
		fclose( rspApdx->file );
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
	rspm->sent = 0;
	hmReset( rspm->http );
	
	dsCpyChars( rspm->data, 0, 0 );
	
	return rspm;
}


Response *rspInit( ) {
	Response *rsp = malloc( sizeof( Response ) );
	if ( !rsp )
		handle_error( "rspInit" );
	memset( rsp, 0, sizeof( Response ) );
	rsp->message = rspmInit();
	rsp->strings = vcInit();
	
	return rsp;
}

Response* rspReset( Response* rsp )
{
	rsp->sendState = 0;
	rsp->sendAppdx = 0;
	rsp->rqst = 0;
	if ( rsp->appendix )
		rspApdxReset( rsp->appendix );
	rspmReset( rsp->message );
	
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
	
	free( rsp );
}


Response *rspBuild( Response *rsp, ServerConfigs *cfg ) {
	if ( !rsp->rqst )
		handle_fail( "rspBuild: request null poiner" );
	if ( !cfg )
		handle_fail( "rspBuild: server config null poiner" );

	
	if ( strcmp( rsp->rqst->httpversion, "" ) == 0 )
		dynstrcpy( &rsp->rqst->httpversion, "HTTP/1.0" );
	
	if ( ! (( strcmp( rsp->rqst->httpversion, "HTTP/1.1") == 0 ) ||
			( strcmp( rsp->rqst->httpversion, "HTTP/1.0") == 0 ) ) ) {
		rsp->message->http->statusLine->statuscode = STATUS_BAD_REQUEST;
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
					char *ct = ctGet( cfg->contentTypes, ext );
					if ( ct ) {
						rsp->message->http->statusLine->statuscode = STATUS_OK;
						
						char *size = itoa( fsize( filename->buffer ) );
						hmAddHeader( rsp->message->http, "Content-Length", size );
						
						ct = dynstrcpy( 0, ct );	//kopie anfertigen
						dynstrcat( &ct, "/" );
						dynstrcat( &ct, ext );
						
						hmAddHeader( rsp->message->http, "Content-Type", ct );
						
						vcPush( rsp->strings, size );
						vcPush( rsp->strings, ct );
						
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
			
			vcPush( rsp->strings, filename->buffer ); 
			dsFreeKeep( filename );
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



