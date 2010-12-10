#ifndef __lines_header__
#define __lines_header__

#include "lib/vector.h"

/** \class Lines
 * Contains multiple lines.
 * The lines are managed as a double linked list.
 * Each element points to a c string representing the line
 */
typedef Vector Lines;

/** \memberof Lines
 * 
 * Contructor - Allocates memory for a new Lines instance. 
 * 
 * \return new Lines instance
 */
Lines *lnsInit();

/** \memberof Lines
 * 
 * Returns whether the double linked list is empty.
 * 
 * \param lns targeted Lines instance
 */
int lnsIsEmpty( Lines *lns );

/** \memberof Lines
 * 
 * Counts the number of lines stored in this double linked list.
 * 
 * \param lns targeted Lines instance
 * \return number of lines
 */
int lnsCount( Lines *lns );

/** \memberof Lines
 *
 * Adds the given line at the end of the double linked list.
 * 
 * \param lns targeted Lines instance
 * \param ln Line to append at lns
 * \return targeted Lines instance
 */

Lines *lnsAdd( Lines *lns, char *ln );


/** \memberof Lines
 * 
 * Deletes a line.
 * 
 * \param lns targeted Lines instance
 * \param i index of line
 * \return targeted Lines instance
 */
Lines *lnsDeleteLine( Lines *lns, int i );

/** \memberof Lines
 * 
 * Parses the given memory area into the lines object.
 * All Lines point to locations in the given string after this.
 * This method will modify the given memory area!
 * All CR and LF will be replaced by 0.
 * The given string should be zero terminated and size must not 
 * include the zero.
 * 
 * \param lns targeted Lines instance
 * \param str pointer to memory area
 * \param size size of memory area
 * \return targeted Lines instance
 */
Lines *lnsParse( Lines *lns, char *str, int size );

Lines *lnsClear( Lines *lns );

/** \memberof Lines
 * 
 * Frees the Lines instance and all the lines managed by it.
 * 
 * \param lns targeted Lines instance
 */
void lnsFree( Lines *lns );

#endif