#ifndef __tokenizer_header__
#define __tokenizer_header__

#include "lib/bitset.h"

/**
 * Tokenizes a given memory area by multiple possible chars
 */
typedef struct Tokenizer {
	const char *start;		/**< points to the begin of the memory area */
	int size;			/**< size of the memory area */
	const char *end;			/**< points to the end of the memory area */
	const char *current;			/**< current position - used for tkNext */
	BitSet *charset;	/**< bit map with 256 bits (=32 byte), masking the seperator chars */
} Tokenizer;

/** \memberof Tokenizer
 * 
 * Contructor - Allocates memory for a new Tokenizer instance. 
 *
 * \param start points to the begin of the memory area
 * \param size size of the memory area
 * \param seperators 
 * \return new Tokenizer instance
 */
Tokenizer *tkInit( const char* start, int size, const char *seperators );


/** \memberof Tokenizer
 * 
 * Contructor - Allocates memory for a new Tokenizer instance. 
 *
 * \param string points to a c string
 * \param seperators 
 * \return new Tokenizer instance
 */
Tokenizer *tkInitS( const char* string, const char *seperators );

/** \memberof Tokenizer
 * 
 * Checks whether a given char c is a seperator in tk
 * 
 * \param tk targeted Tokenizer instance
 * \param c char
 * \return targeted Tokenizer instance
 */
int tkIsSeperator( Tokenizer *tk, char c );

/** \memberof Tokenizer
 * 
 * Adds a char c to the seperators of tk.
 * 
 * \param tk targeted Tokenizer instance
 * \param c char
 * \return targeted Tokenizer instance
 */
Tokenizer *tkAddSeperator( Tokenizer *tk, char c );

/** \memberof Tokenizer
 * 
 * Removes a char c from the seperators of tk.
 * 
 * \param tk targeted Tokenizer instance
 * \param c char
 * \return targeted Tokenizer instance
 */
Tokenizer *tkRemoveSeperator( Tokenizer *tk, char c );

/** \memberof Tokenizer
 * 
 * Gets the next token. New memory is allocated for this.
 * Its your duty to free it.
 * 
 * \param tk targeted Tokenizer instance
 * \return next token - when there is no more token, null is returned
 */
char *tkNext( Tokenizer *tk );

/** \memberof Tokenizer
 * 
 * Resets the tokenizer, so that you can read all tokens again.
 * 
 * \param tk targeted Tokenizer instance
 * \return targeted Tokenizer instance
 */
Tokenizer *tkReset( Tokenizer *tk );

/** \memberof Tokenizer
 * 
 * Frees the Tokenizer instance and all the variables that where initialized in this context.
 * 
 * \param tk targeted Tokenizer instance
 */
void tkFree( Tokenizer *tk );


#endif
