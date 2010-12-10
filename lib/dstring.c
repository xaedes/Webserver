#include "lib/dstring.h"
#include "lib/error.h"
#include "lib/tokenizer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "lib/vector.h"

#define DS_RECYCLE

#ifdef DS_RECYCLE


Vector *dsRecycled = 0;
const int dsNRecycleSizes = 4;
int dsRecycleSizes[4] = { 32, 128, 512, 2048 };
// const int dsNRecycleSizes = 1;
// int dsRecycleSizes[1] = { 2048 };


void dsInitRecycle() 
{
	
	dsRecycled = vcInit();
	int i = 0;
	for ( i = 0; i < dsNRecycleSizes; ++i ) {
		vcPush( dsRecycled, vcInit() );
	}
}

DString *dsGetRecycled( int reserved ) 
{
	if ( !dsRecycled )
		dsInitRecycle();
	int i = 0;
	for ( i = 0; i < dsNRecycleSizes; ++i ) {
		if ( ( reserved <= dsRecycleSizes[i] ) && ( ((Vector*)dsRecycled->items[i])->size ) ) {
			return vcPop( dsRecycled->items[i] );
		}
	}
	return 0;
}

int dsRecycle( DString* ds ) 
{
	if ( !dsRecycled )
		dsInitRecycle();

	int i = 0;
	for ( i = 0; i < dsNRecycleSizes; ++i ) {
		if ( ds->reserved <= dsRecycleSizes[i] ) {
			ds->size = 0;
			vcPush( dsRecycled->items[i], ds );
			return 1;
		}
	}
	return 0;	//nicht recycled
}

DString *dsInit ( int reserve ) 
{
	DString *ds = dsGetRecycled( reserve );
	
	if ( ds )
		return ds;
	
	ds = malloc( sizeof( DString ) );
	if ( !ds )
		handle_error( "dsInit" );
	
	memset( ds, 0, sizeof( DString ) );
	
	ds->size = 0;
	ds->reserved = reserve;
	ds->buffer = malloc( ds->reserved );
	if ( !ds->buffer ) 
		handle_error( "dsInit" );
	
	return ds;
}


void dsFree(DString* ds)
{
	if ( !dsRecycle( ds ) ) {
		free( ds->buffer );
		free( ds );
	}
}

void dsFreeKeep ( DString *ds )
{
	//kein recycle möglich
	free( ds );
}

#else

DString *dsInit ( int reserve ) {
	DString *ds = malloc( sizeof( DString ) );
	if ( !ds )
		handle_error( "dsInit" );
	
	memset( ds, 0, sizeof( DString ) );
	
	ds->size = 0;
	ds->reserved = reserve;
	ds->buffer = malloc( ds->reserved );
	if ( !ds->buffer ) 
		handle_error( "dsInit" );
	
	return ds;
}


void dsFree(DString* ds)
{
	free( ds->buffer );
	free( ds );
}

void dsFreeKeep ( DString *ds )
{
	free( ds );
}

#endif

DString *dsEnlarge( DString *ds, int n ) {
	int changed = 0;
	while( ds->size + n >= ds->reserved ) {
		ds->reserved *= 2;
		changed = 1;
	}
	if ( changed ) {
		ds->buffer = realloc( ds->buffer, ds->reserved );
		if ( !ds->buffer )
			handle_error( "dsEnlarge" );
		//memset( ds->buffer + ds->size, 0, ( ds->reserved - ds->size ) );
	}
	return ds;
}

DString *dsShrink( DString *ds ) {
	if ( ds->size <= ds->reserved/4 ) {
		ds->reserved /= 2;
		ds->buffer = realloc( ds->buffer, ds->reserved );
		if ( !ds->buffer )
			handle_error( "dsShrink" );
	}
	return ds;
}

DString *dsCatChar( DString *ds, const char c ) {
	dsEnlarge( ds, 1 );
	ds->buffer[ds->size] = c;
	++ds->size;
	return ds;
}

DString *dsCatString( DString *ds, const char *s ) {
	return s ? dsCatChars( ds, s, strlen( s ) ) : ds;
}

DString *dsCatChars( DString *ds, const char *s, int len ) {
	dsEnlarge( ds, len );
	memcpy( ds->buffer + ds->size, s, len );
	ds->size += len;
	return ds;
}

DString *dsCat( DString *ds, DString *append ) {
	dsCatChars( ds, append->buffer, append->size );
	return ds;
}

DString* dsCatvF(DString* ds, const char* format, va_list ap )
{
	
	int escape = 0;
	char *buffer = ds->buffer + ds->size;
	int space = ds->reserved - ds->size;
	
	int i = 0;
	char *s = 0;
	DString *S = 0;
	
	const char *last = format;
	
	while ( *format ) {
		if ( escape ) {
			char buf[20];
			int k = 0;
			switch ( *format ) {
			case '%':
				dsCatChar( ds, '%' );
				break;
			case 's':
				s = va_arg( ap, char * );
				dsCatString( ds, s );
				break;
			case 'S':
				S = va_arg( ap, DString * );
				dsCat( ds, S );
				break;
			case 'd':
				i = va_arg( ap, int );
				
				k = 0;
				while ( i || !k ) {
					buf[19-k]= '0'+(i % 10);
					i /= 10;
					++k;
				}
				
				dsCatChars( ds, &buf[20-k], k );
				
				break;
			case 'X':
				i = va_arg( ap, int );
				
				k = 0;
				while ( i || !k ) {
					int x = (i % 16);
					buf[19-k]= (x <= 9) ? '0' + x : 'A' + x - 10;
					i /= 16;
					++k;
				}
				
				dsCatChars( ds, &buf[20-k], k );
				
				break;
			default:
				dsCatChar( ds, *format );
				break;
				
			}
			--escape;
			last = ++format;
		} else {
			switch ( *format ) {
			case '%':
				dsCatChars( ds, last, (int) ( format - last ) );
				++escape;
				break;
			default:

				break;
			}
			++format;
		}
		
	}
	if ( last < format ) {
		dsCatChars( ds, last, (int) ( format - last ) );
	}
	return ds;
}

DString* dsCatF(DString* ds, const char* format, ... )
{
	va_list ap;
	
	va_start( ap, format );
	dsCatvF( ds, format, ap );
	va_end( ap );
	
	return ds;
}


DString* dsCpyvF(DString* ds, const char* format, va_list ap)
{
	dsCpyChars( ds, 0, 0 );
	
	dsCatvF( ds, format, ap );
}


DString* dsCpyF(DString* ds, const char* format, ... )
{
	dsCpyChars( ds, 0, 0 );
	
	va_list ap;
	
	va_start( ap, format );
	dsCatvF( ds, format, ap );
	va_end( ap );
	
	return ds;
}

DString* dsCpy(DString* ds, DString* ds2)
{
	
	dsCpyChars( ds, ds2->buffer, ds2->size);
	
	return ds;
}

DString* dsCpyChars(DString* ds, const char* s, int len )
{
	int diff = len - ds->size;
	if ( diff > 0)
		dsEnlarge( ds, diff );
	
	if ( len )
		memcpy( ds->buffer, s, len );
	ds->size = len;
	
	if ( diff < 0)
		dsShrink( ds );
	
	return ds;
}

DString* dsCpyChar(DString* ds, const char c)
{
	return dsCpyChars( ds, &c, 1 );
}


DString* dsCpyString(DString* ds, const char* s)
{
	return dsCpyChars( ds, s, strlen( s ) );
}

DString* dsZeroTerminate(DString* ds)
{
	dsCatChar( ds, ((unsigned char) 0) );
	--ds->size;
	
	return ds;
}

char dsLast( DString *ds )
{
	if ( ds->size )
		return ds->buffer[ds->size-1];
	else
		return '\0';
}




void dsMemoryTrim(DString* ds)
{
	ds->reserved = ds->size;
	ds->buffer = realloc( ds->buffer, ds->reserved );
	if ( !ds->buffer )
		handle_fail( "dsMemoryTrim" );
}


DString* itods(int i)
{
	char buf[20];
	int k = 0;
	while ( i || !k ) {
		buf[19-k]= '0'+(i % 10);
		i /= 10;
		++k;
	}
	DString *ds = dsInit( 20 );
	dsCpyChars( ds, &buf[20-k], k );
	return ds;
}




