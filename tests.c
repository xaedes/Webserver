
#include "lib/dstring.h"
#include "lib/tokenizer.h"
#include "lib/general.h"
#include "lib/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void print_help()
{
	printf( "WebServer Testprogramm\n" );
	printf( "usage: \n" );
	printf( " test tokenizer string seperators \n" );
	printf( " test dstring [n benchmark = 10000] \n" );
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
			print_help();



	return 0;
}
