#ifndef __vector_header__
#define __vector_header__

/**
 * Dynamic Size Void* Vector
 */
typedef struct Vector {
	void **items;	/**< Items */
	int size;		/**< Occupied Size */
	int reserved;	/**< Reserved Size */
} Vector;

/** \memberof Vector
 * 
 * Contructor - Allocates memory for a new Vector instance. 
 * Initializes with starting reserved size 32.
 * 
 * \return new Vector instance
 */
Vector *vcInit();

/** \memberof Vector
 * 
 * Ensures that enough memory is allocated by enlarging.
 * 
 * \param vc targeted Vector instance
 * \return targeted Vector instance
 */
Vector *vcEnlarge( Vector *vc );

/** \memberof Vector
 * 
 * Ensures that not too much memory is allocated by shrinking.
 * 
 * \param vc targeted Vector instance
 * \return targeted Vector instance
 */
Vector *vcShrink( Vector *vc );

/** \memberof Vector
 * 
 * Gives index of next free item.
 * 
 * \param vc targeted Vector instance
 * \return index of next free item
 */
int vcAdd( Vector *vc );

/** \memberof Vector
 * 
 * Inserts new item p.
 * 
 * \param vc targeted Vector instance
 * \param p pointer to new item
 */
void vcPush( Vector *vc, void *p );

/** \memberof Vector
 * 
 * Deletes item at given index by swapping with last element and popping.
 * 
 * \param vc targeted Vector instance
 * \param i  index
 */
void vcDel( Vector *vc, int i );

/** \memberof Vector
 * 
 * Deletes last item and returns it.
 * 
 * \param vc targeted Vector instance
 * \return last item
 */
void *vcPop( Vector *vc );

/** \memberof Vector
 * 
 * Clears the Vector
 * 
 * \param vc targeted Vector instance
 * \return targeted Vector instance
 */
void vcClear( Vector *vc );

/** \memberof Vector
 * 
 * Frees the Vector instance and all the variables that where initialized in this context.
 * 
 * \param vc targeted Vector instance
 */
void vcFree( Vector *vc );



#endif