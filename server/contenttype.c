#include "server/contenttype.h"
#include "lib/vector.h"
#include "lib/error.h"
#include <stdlib.h>
#include <string.h>

ContentTypes *ctInit()
{
	ContentTypes * ct = malloc( sizeof( ContentTypes ) );
	if ( ! ct ) {
		handle_error( "ctInit" );
	}
	memset( ct, 0, sizeof( ContentTypes ) );
	
	ct->ext = vcInit();
	ct->type = vcInit();
	
	return ct;
}

void ctFree( ContentTypes *ct )
{
	if ( !ct )
		return;
	
	vcFree( ct->ext );
	vcFree( ct->type);
	
	free( ct );
}

ContentTypes *ctAdd( ContentTypes *ct, const char *extension, const char *type )
{
	vcPush( ct->ext, ( void * ) extension );
	vcPush( ct->type, ( void * ) type );
}

const char *ctGet( ContentTypes *ct, const char *extension )
{
	int i;
	for( i = 0; i < ct->ext->size; ++i) {
		if ( strcmp( ct->ext->items[i], extension ) == 0 ) {
			return ct->type->items[i];
		}
	}
	return 0;
}
