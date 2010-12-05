#ifndef __bitset_header__
#define __bitset_header__


/**
 * Implements a bit set
 */
typedef struct BitSet {
	char *bits;		/**< Memory area for the bits */
	int bitSize;	/**< number of bits to be managed, e.g. the number of different items in the set */
} BitSet;

/** \memberof BitSet
 * 
 * Contructor - Allocates memory for a new BitSet instance. 
 *
 * \param nBits number of bits to be managed, e.g. the number of different items in the set
 * \return new BitSet instance
 */
BitSet *bsInit( int nBits );

/** \memberof BitSet
 * 
 * Returns the number of bytes necessary for the bitset 
 * Not the struct, but the actually memory area containing
 * the bits
 * 
 * \param bs targeted BitSet instance
 * \return number of bytes
 */
int bsByteSize( BitSet *bs ); 

/** \memberof BitSet
 * 
 * Adds the n-th element/bit to the bitset
 * 
 * \param bs targeted BitSet instance
 * \param n
 * \return targeted BitSet instance
 */
BitSet *bsAdd( BitSet *bs, int n );


/** \memberof BitSet
 * 
 * Removes the n-th element/bit from the bitset
 * 
 * \param bs targeted BitSet instance
 * \param n
 * \return targeted BitSet instance
 */
BitSet *bsRemove( BitSet *bs, int n );

/** \memberof BitSet
 * 
 * Sets the status of the n-th element/bit of the bitset
 * 
 * \param bs targeted BitSet instance
 * \param n
 * \param value 0 : not in set ; any other : in set
 * \return targeted BitSet instance
 */
BitSet *bsSet( BitSet *bs, int n, int value );

/** \memberof BitSet
 * 
 * Checks whether the n-th element/bit is included in the bitset
 * 
 * \param bs targeted BitSet instance
 * \param n
 * \return 0 : not in set ; 1 : in set
 */
int bsGet( BitSet *bs, int n );

/** \memberof BitSet
 * 
 * Frees the BitSet instance and all the variables that where initialized in this context.
 * 
 * \param bs targeted BitSet instance
 */
void bsFree( BitSet *bs );





#endif
