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
	cons->reserved = 1000;
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
		
		//*
		consConnectionReset( cons, i );
		
		//letztes element mit zu löschender stelle tauschen
		Client tmpClient = cons->clients[i];
		struct pollfd tmpPoll = cons->poll[i];
		
		Client lst = cons->clients[cons->size-1];
		
/*		if ( lst.ios == ios_forward_receiver) {
			cons->clients[lst.iForwardSource].iForwardDest = i;
		} else if ( lst.ios == ios_forward_sender) {
			cons->clients[lst.iForwardDest].iForwardSource = i;
		}*/
		
		cons->clients[i] = lst;
		cons->poll[i] = cons->poll[cons->size-1];
		
		cons->clients[cons->size-1] = tmpClient;
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

// Connections* consConnectionForward(Connections* cons, int from, int to)
// {
// 	cons->clients[from].iForwardDest = to;
// 	consConnectionSetIOStatus( cons, from, ios_forward_sender );
// 	cons->clients[to].iForwardSource = from;
// 	consConnectionSetIOStatus( cons, to, ios_forward_receiver );
// 	
// 	return cons;
// }


Connections* consConnectionReset(Connections* cons, int i)
{
	Client *client = &cons->clients[i];
	
	
	memset( cons->poll + i, 0, sizeof( struct pollfd ) );
	
	
	
	//int sfd;
	
	if ( client->sfd )
		close( client->sfd ); 
	client->sfd=0;
	
	//struct sockaddr_in addr;
	
	memset( &client->addr, 0, sizeof( struct sockaddr_in ) );
	
	//Lines *lnsRead;
	
	if ( client->lnsRead ) 
		lnsClear( client->lnsRead );
	else
		client->lnsRead = lnsInit();
	
	
	//Request *rqst;
	
	if ( client->rqst )
		rqstReset( client->rqst );
	else
		client->rqst = rqstInit();
	
	client->rqst->lns = client->lnsRead;
		
	//Response *rsp;

	if ( client->rsp )
		rspReset( client->rsp ); 
	else
		client->rsp = rspInit();
	client->rsp->rqst = client->rqst;
	
	
	//IO_Status ios;
	
	client->ios = ios_none;
	
	//int initialized;
	
	client->initialized = 1;
	
	//DString *forward;
	
	client->forward->size = 0;
	
	//int iForwardSource;

	client->iForwardSource = -1;
	
	//int iForwardDest;
	
	client->iForwardDest = -1;
	
	//int forwardEOF;
	
	client->forwardEOF = 0;
	
	//void (*forwardHangUp)(int);
	client->forwardHangUp = 0;
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
	if ( client->forward )
		dsFree( client->forward );
	
	memset( client, 0, sizeof( Client ) );
	
	return cons;
}

Connections* consConnectionInit(Connections* cons, int i)
{
	if ( cons->clients[i].initialized )
		handle_fail( "consConnectionInit: already initialized" );
	
	memset( cons->clients + i, 0, sizeof( Client ) );
	memset( cons->poll + i, 0, sizeof( struct pollfd ) );
	
	cons->clients[i].lnsRead = lnsInit();
	cons->clients[i].rqst = rqstInit();
	cons->clients[i].rqst->lns = cons->clients[i].lnsRead;
	cons->clients[i].rsp = rspInit();
	cons->clients[i].rsp->rqst = cons->clients[i].rqst;
	cons->clients[i].forward = dsInit( 512 );
	cons->clients[i].iForwardSource = -1;
	cons->clients[i].iForwardDest = -1;
	cons->clients[i].forwardEOF = 0;
	cons->clients[i].forwardHangUp = 0;
	cons->clients[i].initialized = 1;
	
	return cons;
}


Connections *consConnectionSetup( Connections *cons, int i, int fd, struct sockaddr_in *addr )
{
	cons->clients[i].sfd = fd;
	if ( addr )
		cons->clients[i].addr = *addr;
	
	cons->poll[i].fd = fd;
	
	return cons;
}

Connections *consConnectionSetIOStatus( Connections *cons, int i, IO_Status ios )
{
	
	switch( ios ) {
	case ios_receive_httpheaders:
		cons->clients[i].ios = ios_receive_httpheaders;
		cons->poll[i].events = POLLIN;
		
		rqstReset( cons->clients[i].rqst );
		lnsClear( cons->clients[i].lnsRead );
		cons->clients[i].rqst->lns = cons->clients[i].lnsRead;
		break;
	case ios_send_respond:
		cons->clients[i].ios = ios_send_respond;
		cons->poll[i].events = POLLOUT;
		
		rspReset( cons->clients[i].rsp );
		cons->clients[i].rsp->rqst = cons->clients[i].rqst;
		
		break;
	case ios_receive_messagebody:
		cons->clients[i].ios = ios_receive_messagebody;
		cons->poll[i].events = POLLIN;
		
/*		rqstReset( cons->clients[i].rqst );
		lnsClear( cons->clients[i].lnsRead );*/
		
		
		break;
/*	case ios_forward_sender:
		cons->clients[i].ios = ios_forward_sender;
		
		//der forward_sender liest von seinem fd und stellt es seinem forward partner zur verfügung
		cons->poll[i].events = POLLIN;	
		
		break;
	case ios_forward_receiver:
		cons->clients[i].ios = ios_forward_receiver;
		
		//der forward_receiver schreibt auf seinem fd und bekommt seine daten von seinem forward partner
		cons->poll[i].events = POLLOUT;
		
		break;*/
	default:
		handle_fail( "consConnectionSetIOStatus: unimplemented IO_Status" );
	}
	
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
