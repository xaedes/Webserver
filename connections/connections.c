
#include "connections.h"
#include "lib/error.h"
#include <stdlib.h>
#include <string.h>

int consAdd ( Connections* cons )
{
	if ( cons->size == cons->reserved ) {
		consEnlarge ( cons );
	}
	int i = cons->size++;
	
	cons->cons[i].index = i;
	cons->cons[i].poll = &cons->poll[i];
	cons->cons[i].cons = cons;

	if ( !cons->cons[i].initialized )
		conInit( &cons->cons[i] );
	

	return i;
}

void consDel ( Connections* cons, int i )
{
	if ( cons->size ) {

		//*
		conReset ( &cons->cons[i] );

		//letztes element mit zu löschender stelle tauschen
		Connection tmpClient = cons->cons[i];
		struct pollfd tmpPoll = cons->poll[i];

		Connection lst = cons->cons[cons->size-1];

		/*		if ( lst.ios == ios_forward_receiver) {
					cons->clients[lst.iForwardSource].iForwardDest = i;
				} else if ( lst.ios == ios_forward_sender) {
					cons->clients[lst.iForwardDest].iForwardSource = i;
				}*/

		cons->cons[i] = lst;
		cons->cons[i].index = i;
		cons->cons[i].poll = &cons->poll[i];
		cons->poll[i] = cons->poll[cons->size-1];
		


		cons->cons[cons->size-1] = tmpClient;
		cons->cons[cons->size-1].index = cons->size - 1;
		cons->cons[cons->size-1].poll = &cons->poll[cons->size-1];
		cons->poll[cons->size-1] = tmpPoll;

		/*/

		consConnectionFree( cons, i );


		//letztes element mit zu löschender stelle tauschen

		cons->clients[i] = cons->clients[cons->size-1];
		cons->poll[i] = cons->poll[cons->size-1];

		memset( cons->clients + cons->size-1, 0, sizeof( Client ) );
		memset( cons->poll + cons->size-1, 0, sizeof( struct pollfd ) );

		//*/
		--cons->size;
	}
}

Connections* consEnlarge ( Connections* cons )
{
	cons->reserved *= 2;

	cons->cons = realloc ( cons->cons, sizeof ( Connection ) * cons->reserved );
	if ( !cons->cons )
		handle_error ( "consEnlarge" );
	cons->poll = realloc ( cons->poll, sizeof ( struct pollfd ) * cons->reserved );
	if ( !cons->cons )
		handle_error ( "consEnlarge" );

	memset ( cons->cons + cons->size, 0, ( cons->reserved - cons->size ) * sizeof ( Connection ) );
	memset ( cons->poll + cons->size, 0, ( cons->reserved - cons->size ) * sizeof ( struct pollfd ) );

	return cons;
}

void consFree ( Connections* cons )
{
	if ( !cons )
		return;

	while ( cons->size ) {
		conFree ( &cons->cons[0] );
		consDel ( cons, 0 );
	}



	if ( cons->cons )
		free ( cons->cons );
	if ( cons->poll )
		free ( cons->poll );

	free ( cons );
}

Connections* consInit()
{
	Connections *cons = malloc ( sizeof ( Connections ) );
	if ( !cons )
		handle_error ( "consInit" );
	memset ( cons, 0, sizeof ( Connections ) );
	cons->size = 0;
	cons->reserved = 1000;
	cons->cons = calloc ( cons->reserved, sizeof ( Connection ) );
	if ( !cons->cons )
		handle_error ( "consInit" );
	cons->poll = calloc ( cons->reserved, sizeof ( struct pollfd ) );
	if ( !cons->cons )
		handle_error ( "consInit" );
	return cons;

}
