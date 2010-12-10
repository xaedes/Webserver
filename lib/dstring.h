#ifndef __dynstringbuffer_header__
#define __dynstringbuffer_header__

#include <stdarg.h>

/**
 * Represents a Dynamic String
 * Memory allocation is managed by the ds* methods
 */
typedef struct DString {
	int size;
	int reserved;
	char* buffer;
} DString;

/** \memberof DString
 * 
 * Contructor - Allocates memory for a new DString instance. 
 * 
 * \param reserve
 * \return new DString instance
 */
DString *dsInit ( int reserve );

//stellt sicher, dass platz für n chars ist
/** \memberof DString
 * 
 * Ensures that enough memory for n additional chars is available.
 * 
 * \param ds targeted DString instance
 * \return targeted DString instance
 */
DString *dsEnlarge( DString *ds, int n );

/** \memberof DString
 * 
 * When dstring uses less then 1/4 of its reserved memory, its reserved memory is cut in half
 * 
 * \param ds targeted DString instance
 * \return targeted DString instance
 */
DString *dsShrink( DString *ds );

/** \memberof DString
 * 
 * Concats a char to ds
 * 
 * \param ds targeted DString instance
 * \param char
 * \return targeted DString instance
 */
DString *dsCatChar( DString *ds, const char c );

/** \memberof DString
 * 
 * Concats a c string to ds
 * 
 * \param ds targeted DString instance
 * \param cstring
 * \return targeted DString instance
 */
DString *dsCatString( DString *ds, const char *s );

/** \memberof DString
 * 
 * Concats n chars to ds
 * 
 * \param ds targeted DString instance
 * \param chars
 * \param n
 * \return targeted DString instance
 */
DString *dsCatChars( DString *ds, const char *s, int len );

/** \memberof DString
 * 
 * Concats the dstring append to ds
 * 
 * \param ds targeted DString instance
 * \param append another DString instance
 * \return targeted DString instance
 */
DString *dsCat( DString *ds, DString *append );

/** \memberof DString
 * 
 * The content of ds will be replaced by a char
 * 
 * \param ds targeted DString instance
 * \param char
 * \return targeted DString instance
 */
DString *dsCpyChar( DString *ds, const char c );

/** \memberof DString
 * 
 * The content of ds will be replaced by a c string
 * 
 * \param ds targeted DString instance
 * \param cstring
 * \return targeted DString instance
 */
DString *dsCpyString( DString *ds, const char *s );

/** \memberof DString
 * 
 * The content of ds will be replaced by n chars
 * 
 * \param ds targeted DString instance
 * \param chars
 * \param n
 * \return targeted DString instance
 */
DString *dsCpyChars( DString *ds, const char *s, int len );

/** \memberof DString
 * 
 * The content of ds will be replaced by the content of another DString
 * 
 * \param ds targeted DString instance
 * \param ds2 another DString
 * \return targeted DString instance
 */
DString *dsCpy( DString *ds, DString *ds2 );

/** \memberof DString
 * 
 * The content of ds will be replaced by a formated string.
 * 
 * \see dsCatF
 * 
 * \param ds targeted DString instance
 * \param format
 * \param ... parameters for the formated string
 * \return targeted DString instance
 */
DString *dsCpyF( DString *ds, const char *format, ... );

/** \memberof DString
 * 
 * The content of ds will be replaced by a formated string.
 * 
 * \see dsCatF
 * 
 * \param ds targeted DString instance
 * \param format
 * \param va_list a va_list
 * \return targeted DString instance
 */
DString *dsCpyvF( DString *ds, const char *format, va_list ap );

/** \memberof DString
 * 
 * Concats a formated string to ds.
 * 
 * The string will be formated as with the printf functions.\n
 * Example:\n
 * dsCpy( ds, "Hello" );\n
 * dsCatF( ds, "Foo: %d - Bar: %s - Bongo: %S - %X %%\n", (int) 10, "Tower", ds, (int) 255 );\n
 * content of ds: "Foo: 10 - Bar: Tower - Bongo: Hello - FF %\n"
 * 
 * Recognizing:
 * - %d  - displays an integer in decimal notation
 * - %X  - displays an integer in hexadecimal notation
 * - %s  - displays a c string
 * - %S  - displays a DString
 * - %%  - displays a '%'
 * 
 * \todo implement integer display for signed values ( minus ) 
 * 
 * \param ds targeted DString instance
 * \param format
 * \param ... parameters for the formated string
 * \return targeted DString instance
 */
DString *dsCatF( DString *ds, const char *format, ... );

/** \memberof DString
 * 
 * Concats a formated string to ds.
 * 
 * \see dsCatF
 * 
 * \param ds targeted DString instance
 * \param format
 * \param .va_list a va_list
 * \return targeted DString instance
 */
DString* dsCatvF(DString* ds, const char* format, va_list ap );

/** \memberof DString
 * 
 * Do a c string like zero termination to the content of ds.
 * This is necessary if you want to use the buffer as a c string.
 * 
 * \param ds targeted DString instance
 * \return targeted DString instance
 */
DString *dsZeroTerminate( DString *ds );

/** \memberof DString
 * 
 * Returns the last character of ds
 * 
 * \param ds targeted DString instance
 * \return last character of ds
 */
char dsLast( DString *ds );

/** \memberof DString
 * 
 * Frees the DString instance and all the variables that where initialized in this context.
 * Content wont be available after this!
 * 
 * \param ds targeted DString instance
 */
void dsFree ( DString *ds );

/** \memberof DString
 * 
 * Frees the DString instance and all the variables that where initialized in this context,
 * except the buffer.
 * 
 * Content will be available after this.
 * 
 * \param ds targeted DString instance
 */
void dsFreeKeep ( DString *ds );

/** \memberof DString
 * 
 * Trims the allocated memory of this ds.
 * 
 * \param ds targeted DString instance
 * \return targeted DString instance
 */
void dsMemoryTrim ( DString *ds );


/** \memberof DString
 * 
 * Converts an integer to a dstring 
 * 
 * \param integer 
 * \return DString instance
 */
DString *itods( int i );

#endif