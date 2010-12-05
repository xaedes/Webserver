#include "server/connections.h"
#include "communication/response.h"
#include "communication/requests.h"
#include "lib/lines.h"
#include "lib/error.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Client *clntInit()
// {
// 	Client *client = malloc( sizeof( Client ) );
// 	if ( !client )
// 		handle_error( "clntInit" );
// 	memset( client, 0, sizeof( Client ) );
// 	
// 	client->rqsts = rqstsInit();
// 	client->lnsRead = lnsInit();
// 	
// 	return client;
// }

Connections *consInit()
{
	Connections *cons = malloc( sizeof( Connections ) );
	if ( !cons )
		handle_error( "consInit" );
	memset( cons, 0, sizeof( Connections ) );
	cons->size = 0;
	cons->reserved = 10100;
	cons->clients = calloc( cons->reserved, sizeof( Client ) );
	if ( !cons->clients )
		handle_error( "consEnlarge" );
	cons->poll = calloc( cons->reserved, sizeof( struct pollfd ) );
	if ( !cons->clients )
		handle_error( "consEnlarge" );
	return cons;
}

Connections *consEnlarge( Connections *cons )
{
	cons->reserved *= 2;

	cons->clients = realloc( cons->clients, sizeof( Client ) * cons->reserved );
	if ( !cons->clients )
		handle_error( "consEnlarge" );
	cons->poll = realloc( cons->poll, sizeof( struct pollfd ) * cons->reserved );
	if ( !cons->clients )
		handle_error( "consEnlarge" );

	memset( cons->clients + cons->size, 0, ( cons->reserved - cons->size ) * sizeof( Client ) );
	memset( cons->poll + cons->size, 0, ( cons->reserved - cons->size ) * sizeof( struct pollfd ) );
}

int consAdd( Connections *cons )
{
	if ( cons->size == cons->reserved ) {
		consEnlarge( cons );
	}
	int i = cons->size++;
	
	
	return i;
}

void consDel( Connections *cons, int i )
{
	if ( cons->size )
	{
		//consConnectionReset( cons, i );
		consConnectionFree( cons, i );
		
		
		//letztes element mit zu löschender stelle tauschen
		
		//Client tmpClient = cons->clients[i];
		//struct pollfd tmpPoll = cons->poll[i];
		cons->clients[i] = cons->clients[cons->size-1];
		cons->poll[i] = cons->poll[cons->size-1];
		//cons->clients[cons->size-1] = tmpClient;
		//cons->poll[cons->size-1] = tmpPoll;

		memset( cons->clients + cons->size-1, 0, sizeof( Client ) );
		memset( cons->poll + cons->size-1, 0, sizeof( struct pollfd ) );
		
		--cons->size;
	}
}

Connections* consConnectionReset(Connections* cons, int i)
{
	Client *client = &cons->clients[i];
	
	cons->poll[i].fd = 0;
	cons->poll[i].revents = 0;
	//cons->clients[i].addr = addr;
	
	client->ios == ios_receive; //starteinstellung
	
	
	rspReset( client->rsp );

	if ( client->sfd )
		close( client->sfd ); 
	if ( client->lnsRead ) 
		lnsFree( client->lnsRead );
	if ( client->rqst )
		rqstFree( client->rqst );
	
	client->sfd = 0;
	client->lnsRead = 0;
	client->rqst = 0;
	
	return cons;
}


Connections *consConnectionSetup( Connections *cons, int i, int fd, struct sockaddr_in addr )
{
	cons->clients[i].sfd = fd;
	cons->clients[i].addr = addr;
	
	cons->poll[i].fd = fd;
	cons->clients[i].ios = ios_receive; //starteinstellung
	
	return cons;
}

Connections *consConnectionSetIOStatus( Connections *cons, int i, IO_Status ios )
{
	
	switch( ios ) {
	case ios_receive:
		if ( cons->clients[i].ios == ios_send ) {
			if ( cons->clients[i].rqst ) {
				rqstFree( cons->clients[i].rqst );
				cons->clients[i].rqst = 0;
			}
			if ( cons->clients[i].lnsRead ) {
				lnsFree( cons->clients[i].lnsRead );
				cons->clients[i].lnsRead = 0;
			}
		}
		
		cons->clients[i].ios = ios_receive;
		cons->poll[i].events = POLLIN;
		cons->clients[i].rqst = rqstInit();
		cons->clients[i].lnsRead = lnsInit();
		cons->clients[i].rqst->lns = cons->clients[i].lnsRead;
		break;
	case ios_send:
		cons->clients[i].ios = ios_send;
		cons->poll[i].events = POLLOUT;
		break;
	default:
		handle_fail( "consConnectionSetIOStatus: unimplemented IO_Status" );
	}
	
	return cons;
}

Connections *consConnectionFree( Connections *cons, int i )
{
	Client *client = &cons->clients[i];
	
	if ( client->sfd )
		close( client->sfd ); 
	if ( client->lnsRead ) 
		lnsFree( client->lnsRead );
	if ( client->rsp )
		rspFree( client->rsp ); 
	if ( client->rqst )
		rqstFree( client->rqst );
	
	client->sfd = 0;
	client->lnsRead = 0;
	client->rsp = 0;
	client->rqst = 0;
	
	return cons;
}

void consFree( Connections *cons )
{
	if ( !cons )
		return;
	
	while( cons->size ) {
		consConnectionFree( cons, 0 );
		consDel( cons, 0 );
	}
	
	
	
	if ( cons->clients )
		free( cons->clients );
	if ( cons->poll )
		free( cons->poll );
	
	free( cons );
}
