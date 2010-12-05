#include "lib/general.h"
#include "lib/error.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>

int imax( int a, int b )
{
	return a > b ? a : b;
}

int imin( int a, int b )
{
	return a < b ? a : b;
}

int fexists( const char* filename )
{
	struct stat buf;
	int x = stat( filename, &buf );
	if ( x ) {
		if ( errno == ENOENT )
			return 0;
	} else return 1;
	
	/*
	FILE *f = fopen( filename, "r" );
	if ( !f ) {
		return 0;
	} else {
		fclose( f );
		return 1;
	}*/
}

int fisdir( const char* filename )
{
	struct stat buf;
	int x = stat( filename, &buf );
	if ( ( x == 0 ) &&
	     ( S_ISDIR( buf.st_mode )  ) )
		return 1;
	else
		return 0;
}

int fsize( const char* filename )
{
	struct stat buf;
	int x = stat( filename, &buf );
	if ( x == 0 ) 
		return buf.st_size;
	else
		return -1;
	/*
	int size = 0;
	FILE *f = fopen( filename, "r" );
	if ( f ) {
		fseek( f, 0, SEEK_END );
		size = ftell( f );
		fclose( f );
	} else
		handle_error( "fsize" );
	return size;
	*/
}

char *dynstrcat( char **a, const char *b )
{
	int x = ( a && *a ) ? strlen( *a ) : 0;
	int y = b ? strlen( b ) : 0;
	char *tmp = 0;
	if ( !a ) {
		a = &tmp;
	}
	*a = ( char * ) realloc( *a, x + y + 1 );
	if ( !*a )
		handle_error( "dynstrcat" );

	if ( b )
		strcat( *a, b );

	return *a;
}

char *cstring_memtrim( char **string ) 
{
	if ( string ) {
		if ( *string ) {
			*string = ( char * ) realloc( *string, strlen( *string ) + 1 );
		}
		return *string;
	} else {
		return 0;
	}
}


char last( char *s ) 
{
	if ( !s ) return 0;
	return s[ strlen( s ) - 1 ];
}


char* dynstrcpy( char **dest, const char *string )
{
	if ( !string )
		return 0;
	char *tmp = 0;
	if ( !dest ) {
		dest = &tmp;
	}
	
	int size = strlen( string );
	*dest = realloc( *dest, size + 1 );
	if ( !*dest )
		handle_error( "dynstrcpy" );
	strcpy( *dest, string );
	return *dest;
}

char * dynsprintf( char **dest, const char *fmt, ...)
{
	/* Guess we need no more than 100 bytes. */
	int n, size = 100;
	char *p, *np;
	va_list ap;
	
	char *tmp = 0;
	int len = (dest && *dest) ? strlen( *dest ) : 0;
	if ( !dest ) {
		dest = &tmp;
	}
		
	*dest = realloc( *dest, len + size + 1 );
	
	if ( !dest ) {
		handle_error( "dynsprintf" );
	}
	
	while (1) {
		/* Try to print in the allocated space. */
		va_start(ap, fmt);
		n = vsnprintf( ( *dest ) + len, size, fmt, ap);
		va_end(ap);
		/* If that worked, return the string. */
		if (n > -1 && n < size) {
			cstring_memtrim( dest );
			return *dest;
		}
		/* Else try again with more space. */
		/* Else try again with more space. */
		if (n > -1) {    /* glibc 2.1 */
			size = n + 1; /* precisely what is needed */
		} else {           /* glibc 2.0 */
			size *= 2;  /* twice the old size */
		}
		if ( ( *dest = realloc( *dest, size + len ) ) == 0 ) {
			handle_error( "dynsprintf" );
		}
	}
}

char *itoa( int i ) 
{
	char buf[20];
	int k = 0;
	while ( i || !k ) {
		buf[19-k]= '0'+(i % 10);
		i /= 10;
		++k;
	}
	char *s = malloc( k + 1 );
	memcpy( s, &buf[20-k], k );
	s[k] = 0;
	return s;
}

const char* fextension(const char* filename)
{
	if ( !filename )
		return filename;
	const char *pos = filename + strlen( filename ) - 1;
	const char *ext = filename + strlen( filename );	//zeigt auf die terminierende null
	while ( pos >= filename ) {
		if ( *pos == '.' ) {
			ext = pos + 1;
			break;
		} else if  ( *pos == '/' ) {
			break;
		}
		--pos;
	}
	return ext;
}

