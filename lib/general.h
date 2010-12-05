#ifndef __httpheader_header__
#define __httpheader_header__

/**
 * Some useful functions
 */
struct General { };

/** \memberof General
 * 
 * Returns the maximum of two values.
 * 
 * \param a 
 * \param b 
 * \return maximum of a and b
 */
int imax( int a, int b );

/** \memberof General
 * 
 * Returns the minimum of two values.
 * 
 * \param a 
 * \param b 
 * \return minimum of a and b
 */
int imin( int a, int b );

/** \memberof General
 * 
 * Checks whether a file exists.
 * 
 */
int fexists( const char* filename );

/** \memberof General
 * 
 * Checks whether a filename points to a directory or not.
 * 
 */
int fisdir( const char* filename );

/** \memberof General
 * 
 * Return the size of filename.
 * 
 */
int fsize( const char* filename );

/** \memberof General
 * 
 * Return the concatenation of two cstrings.
 * Memory of first string will be reallocated for the result.
 * Its your duty to free it!
 * 
 * \param a a pointer to the first string (concat b to an existing string variable)
 * \param b the second string
 * \return result 
 */
char *dynstrcat( char **a, const char *b );


/** \memberof General
 * 
 * Reallocates memory for a cstring, so that it fits perfectly in.
 * 
 */
char *cstring_memtrim( char **string );


/** \memberof General
 * 
 * Returns the last char of a cstring.
 * 
 */
char last( char *s );

/** \memberof General
 * 
 * Reallocs enough memory to store a copy of string and 
 * returns a copy to string.
 * 
 * \param a a pointer to the first string (copy b to an existing string variable)
 * \return copy of string 
 */
char *dynstrcpy( char **dest, const char *string );


/** \memberof General
 * 
 * Like sprintf, but mallocs enough space.
 * Its your duty to free it!
 * 
 * \param dest pointer to already existing c string to write to, if dest == 0, or *dest == 0, new memory will be allocated
 * \param format Formatstring for printf
 * \param ... Arguments for printf
 * \return string
 */
char * dynsprintf( char **dest, const char *fmt, ...);

/** \memberof General
 * 
 * Converts an integer to a string 
 * 
 * \param integer 
 * \return string
 */
char *itoa( int i );

/** \memberof General
 * 
 * Returns the a pointer to the extension part of a filename cstring
 * 
 * \param filename 
 * \return filename extension excluding the dot
 */
const char *fextension( const char *filename );

#endif