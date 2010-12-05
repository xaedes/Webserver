#include "lib/vector.h"
#include "lib/error.h"
#include <stdlib.h>
#include <string.h>

Vector *vcInit()
{
	Vector *vc = malloc( sizeof( Vector ) );
	if ( !vc )
		handle_error( "vcInit" );
	memset( vc, 0, sizeof( Vector ) );
	vc->reserved = 32;
	vc->items = malloc( sizeof( void * ) * vc->reserved );
	
	return vc;
}

Vector *vcEnlarge( Vector *vc )
{
	if ( vc->size == vc->reserved ) {
		vc->reserved *= 2;
		vc->items = realloc( vc->items, sizeof( void * ) * vc->reserved );
		if ( !vc->items )
			handle_error( "vcEnlarge" );
		memset( vc->items + vc->size, 0, sizeof( void * ) * ( vc->reserved - vc->size ) );
	}
	return vc;
}

Vector *vcShrink( Vector *vc )
{
	if ( vc->size == vc->reserved/4 ) {
		vc->reserved /= 2;
		vc->items = realloc( vc->items, sizeof( void * ) * vc->reserved );
		if ( !vc->items )
			handle_error( "vcShrink" );
	}
	return vc;
}

int vcAdd( Vector *vc )
{
	vcEnlarge( vc );
	return vc->size++;
}

void vcPush( Vector* vc, void* p )
{
	vc->items[ vcAdd( vc ) ] = p;
}

void vcDel( Vector *vc, int i )
{
	vc->items[i] = vc->items[vc->size-1];
	vcPop( vc );
}

void *vcPop( Vector *vc )
{
	void *p = vc->items[--vc->size];
	vc->items[--vc->size] = 0;
	vcShrink( vc );
	return p;
}

void vcFree( Vector *vc )
{
	if ( !vc )
		return;
	
	free ( vc->items );
	
	free ( vc );
	
}
