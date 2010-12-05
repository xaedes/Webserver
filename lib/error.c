#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

//nur benutzen, wenn ein systemaufruf fehlschlug
void handle_error( char *msg )
{
	int x = 1;
	int y = x/(x-x);
	perror( msg );
	exit( EXIT_FAILURE );
}

void handle_fail( char *msg )
{
	int x = 1;
	int y = x/(x-x);
	fprintf( stderr, "%s", msg );
	exit( EXIT_FAILURE );
}
