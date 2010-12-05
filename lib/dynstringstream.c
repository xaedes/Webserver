#include "lib/dynstringstream.h"
#include "lib/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

DynStringStream *dssInit() 
{
	DynStringStream *dss = malloc( sizeof( DynStringStream ) );
	if ( !dss )
		handle_error( "dssInit" );
	memset( dss, 0, sizeof( DynStringStream ) );
	
	dss->stream = open_memstream( &dss->buffer, &dss->size );
	if ( !dss->stream )
		handle_error( "dssInit" );
	
	return dss;
}

DynStringStream *dssCloseStream( DynStringStream *dss ) //schließt stream, aber löscht buffer nicht
{
	if ( fclose( dss->stream ) )
		handle_error( "dssCloseStream" );
	
	dss->stream = 0;
	
	return dss;
}

void dssFree( DynStringStream *dss )
{
	if ( dss->stream )
		dssCloseStream( dss );
	free( dss->buffer );
	free( dss );
}

void dssFreeKeepBuffer( DynStringStream *dss )
{
	if ( dss->stream )
		dssCloseStream( dss );
	free( dss );
}
