#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

//nur benutzen, wenn ein systemaufruf fehlschlug
void handle_error( char *msg )
{
	perror( msg );
	int x = 1;
	int y = x/(x-x);
	exit( EXIT_FAILURE );
}

void handle_fail( char *msg )
{
	fprintf( stderr, "%s", msg );
	int x = 1;
	int y = x/(x-x);
	exit( EXIT_FAILURE );
}
