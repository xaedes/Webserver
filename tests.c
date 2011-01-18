
#include "lib/dstring.h"
#include "lib/tokenizer.h"
#include "lib/general.h"
#include "lib/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <uriparser/Uri.h>

void tokenizer( int argc, char *argv[] )
{
	char *string = argv[2];
	char *seps = argv[3];

	printf( "Tokenize \"%s\" by any of the chars of \"%s\":\n", string, seps );

	Tokenizer *tk = tkInitS( string, seps );

	char *token;

	while ( token = tkNext( tk ) ) {

		printf( "\"%s\"\n", token );

		free( token );
	}

	tkFree( tk );
}

void dstring1( int n )
{
	char *s1 = "Hallo";

	int i = 0;

	for ( i = 0; i < n; i++ ) {
		/*
		char *s2 = malloc ( 100 );
		sprintf(s2,"F Test : %sstrlen('%s') == '%d' --%%- ", s1,"Foobar",6);
		sprintf(s2,"F Test : %sstrlen('%s') == '%X' --%%- ", s1,"Foobar",6);
		free( s2 );
		*/
		char *s3 = 0;
		dynstrcpy( &s3, "Kartoffel" );
		dynstrcat( &s3, s1 );
		dynstrcat( &s3, s1 );
		dynstrcat( &s3, s1 );
		dynstrcat( &s3, s1 );
		dynstrcat( &s3, s1 );
		dynstrcat( &s3, s1 );
		dynstrcat( &s3, s1 );
		dynstrcat( &s3, s1 );
		dynstrcat( &s3, s1 );
		dynstrcat( &s3, s1 );
		dynstrcat( &s3, s1 );
		dynstrcat( &s3, s1 );
		free( s3 );
		//printf( "%s\n" , ds2->buffer );
	}

}

void dstring2( int n )
{
	DString *ds = dsInit( 100 );
	dsCpyString( ds, "Hallo" );
	dsZeroTerminate( ds );
	printf( "%s\n" , ds->buffer );

	int i = 0;

	for ( i = 0; i < n; i++ ) {
		/*
		DString *ds2 = dsInit( 100 );
		dsCpyF( ds2, "F Test : %Sstrlen('%s') == '%d' --%%- ", ds,"Foobar",6);
		dsCpyF( ds2, "F Test : %Sstrlen('%s') == '%X' --%%- ", ds,"Foobar",6);
		dsZeroTerminate( ds2 );
		dsFree( ds2 );
		*/
		//printf( "%s\n" , ds2->buffer );

		DString *ds3 = dsInit( 100 );
		dsCpyString( ds3, "Kartoffel" );
		dsCat( ds3, ds );
		dsCat( ds3, ds );
		dsCat( ds3, ds );
		dsCat( ds3, ds );
		dsCat( ds3, ds );
		dsCat( ds3, ds );
		dsCat( ds3, ds );
		dsCat( ds3, ds );
		dsCat( ds3, ds );
		dsCat( ds3, ds );
		dsCat( ds3, ds );
		dsCat( ds3, ds );
		dsFree( ds3 );
	}

	dsFree( ds );
}

void dstring( int argc, char *argv[] )
{
	printf( "Testing DString\n" );
	DString *ds = dsInit( 100 );

	char *s1 = "Hallo";

	dsCpyString( ds, s1 );
	dsZeroTerminate( ds );
	printf( "%s\n" , ds->buffer );

	int n = ( argc > 2 ) ? atoi( argv[2] ) : 10000;

	dstring1( n );
	dstring2( n );

	//dsZeroTerminate( ds );
	//printf( "%s\n" , ds->buffer );
}

void*thsys(void*cmd)
{
	system((char*)cmd);
}

void threadExec( char*cmd )
{
	pthread_t p;
	pthread_create (&p, 0, thsys, (void*)cmd);
	pthread_detach ( p );
	pthread_join (p, 0);
}

void fifopoll( int argc, char *argv[] )
{
	char *filepipe = "/home/martin/tmp/file";
	char *outpipe = "/home/martin/tmp/out";

	/*
	remove(filepipe);
	remove(outpipe);

	mkfifo(filepipe,S_IRWXU | S_IRWXG | S_IRWXO); 
	mkfifo(outpipe,S_IRWXU | S_IRWXG | S_IRWXO);
	*/
	
	struct pollfd parr[4];

	threadExec("php < /home/martin/tmp/file > /home/martin/tmp/out");
	
	parr[0].fd = 1;
	parr[2].fd = 0;
	printf("waiting for %s\n", filepipe);
	parr[3].fd = open( filepipe, O_WRONLY  );
	printf("done\n");
	printf("waiting for %s\n", outpipe);
	parr[1].fd = open( outpipe, O_RDONLY | O_NONBLOCK );
	printf("done\n");
	
	parr[0].events = POLLOUT;
	parr[1].events = POLLIN;
	parr[2].events = POLLIN;
	parr[3].events = POLLOUT;
	
	char buf1[200];
	int size1 = 0;
	int sent1 = 0;
	
	char buf2[200];
	int size2 = 0;
	int sent2 = 0;
	
	memset( buf2, 0, 200);
	strcpy( buf2, "<? echo \"Hallo\n\"; ?>");
	size2 = strlen( buf2 );
	
	while(1){
		int p = poll(&parr[0],4,-1);
		if ( p < 0 )
			handle_error("poll");
		if ( p ) {
			int i;

			//if ( parr[0].revents & POLLOUT ) {
				if ( size1 > sent1 ) {
					int len = write( parr[0].fd, buf1, size1 - sent1 );
					if ( len < 0 )
						handle_error("write:0");
					if ( len )
						sent1+=len;
					printf("write to %d : %d\n", 0, len);
				}
			//}

			if ( parr[3].revents & POLLOUT ) {
				if ( size2 > sent2 ) {
					int len = write( parr[3].fd, buf2, size2 - sent2 );
					if ( len < 0 )
						handle_error("write:3");
					if ( len ) {
						sent2+=len;
						if ( sent2 == size2 ) {
							parr[3].events = 0;
							close( parr[3].fd );
						}
					}
					printf("write to %d : %d\n", 3, len);
				}
			}
			
			if ( parr[1].revents & POLLIN ) {
				if ( size1 <= sent1 ) {
					int len = read( parr[1].fd, buf1, 200 );
					if ( len < 0 )
						handle_error("read:1");
					if ( len ) {
						size1 = len;
						sent1 = 0;
					}
					printf("read from %d : %d\n", 1, len);
				}
			}
			if ( parr[1].events && parr[1].revents & POLLHUP ) {
				printf("outpipe hang up\n");
				parr[1].events = 0;
			}

			if ( parr[2].revents & POLLIN ) {
				if ( size2 <= sent2 ) {
					int len = read( parr[2].fd, buf2, 200 );
					if ( len < 0 )
						handle_error("read:2");
					if ( len ) {
						size2 = len;
						sent2 = 0;
					}
					printf("read from %d : %d\n", 2, len);
				}
			}
		}
	}
	
	close(parr[1].fd);
	close(parr[3].fd);
	
}

void uri( int argc, char *argv[] )
{
	UriParserStateA state;
	UriUriA uri; 
	

	state.uri = &uri;
	if (uriParseUriA(&state, argv[2]) != URI_SUCCESS) {
		/* Failure */
//		uriFreeUriMembersA(&uri);
	} else {
	}
	uriFreeUriMembersA(&uri);
}

void print_help()
{
	printf( "WebServer Testprogramm\n" );
	printf( "usage: \n" );
	printf( " test tokenizer string seperators \n" );
	printf( " test dstring [n benchmark = 10000] \n" );
	printf( " test fifopoll \n" );
	printf( " test uri \n" );
}

int main( int argc, char *argv[] )
{
	if ( argc == 1 ) {
		print_help();
		return 1;
	}

	char *test = argv[1];

	if ( strcmp( test, "tokenizer" ) == 0 )
		tokenizer( argc, argv );
	else
	if ( strcmp( test, "dstring" ) == 0 )
		dstring( argc, argv );
	else
	if ( strcmp( test, "fifopoll" ) == 0 )
		fifopoll( argc, argv );
	else
	if ( strcmp( test, "uri" ) == 0 )
		uri( argc, argv );
	else
		print_help();
	
	
	
	
	return 0;
}
