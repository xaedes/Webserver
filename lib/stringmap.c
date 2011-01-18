#include "lib/stringmap.h"
#include "lib/vector.h"
#include "lib/error.h"
#include <stdlib.h>
#include <string.h>

StringMap *smInit()
{
	StringMap * sm = malloc( sizeof( StringMap ) );
	if ( ! sm ) {
		handle_error( "smInit" );
	}
	memset( sm, 0, sizeof( StringMap ) );
	
	sm->ext = vcInit();
	sm->type = vcInit();
	
	return sm;
}

void smFree( StringMap *sm )
{
	if ( !sm )
		return;
	
	vcFree( sm->ext );
	vcFree( sm->type);
	
	free( sm );
}

StringMap *smAdd( StringMap *sm, const char *key, const char *value )
{
	vcPush( sm->ext, ( void * ) key );
	vcPush( sm->type, ( void * ) value );
}

const char *smGet( StringMap *sm, const char *key )
{
	int i;
	for( i = 0; i < sm->ext->size; ++i) {
		if ( strcmp( sm->ext->items[i], key ) == 0 ) {
			return sm->type->items[i];
		}
	}
	return 0;
}
