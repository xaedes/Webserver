#ifndef __lines_header__
#define __lines_header__

#include "lib/vector.h"

/**
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

Lines *lnsAdd( Lines *lns, Line *ln );

/** \memberof Lines
 * \callergraph
 * \callgraph
 * 
 * Appends the given string with a specific length to the 
 * last line. If lns is empty, a new line will be created.
 * 
 * \param lns targeted Lines instance
 * \param str string
 * \param size string length
 * \return targeted Lines instance
 */
//Lines *lnsAppend( Lines *lns, char *str, int size );

/** \memberof Lines
 * 
 * Appends the given string with a specific length as a new 
 * line at the end. 
 * 
 * \param lns targeted Lines instance
 * \param str string
 * \param size string length
 * \return targeted Lines instance
 */
//Lines *lnsAppendLine( Lines *lns, char *str, int size );

/** \memberof Lines
 * 
 * Deletes the given Line object from the Lines object 
 * and frees it.
 * 
 * \param lns targeted Lines instance
 * \param ln Line instance managed by Lines
 * \return targeted Lines instance
 */
Lines *lnsDeleteLine( Lines *lns, Line *ln );

/** \memberof Lines
 * 
 * Parses the given memory area into the lines object.
 * All Lines point to locations in the given string after this.
 * This method will modify the given memory area!
 * All CR and LF will be replaced by 0
 * 
 * \param lns targeted Lines instance
 * \param str pointer to memory area
 * \param size size of memory area
 * \return targeted Lines instance
 */
Lines *lnsParse( Lines *lns, char *str, int size );

/** \memberof Lines
 * 
 * Appends the content of the buffer to the lines. CRLF and LF 
 * will be recognized and trigger a new line.
 * 
 * \todo add a cancel condition to stop parsing when CRLF CRLF is reached ; change method interface, so that the remaining buffer content can be determined
 * \todo 
 * 
 * \param lns targeted Lines instance
 * \param buffer buffer containing the data too be append 
 * \param size determines how many chars from the buffer will be append
 * \return targeted Lines instance
 */
//Lines *lnsAddBuffer( Lines *lns, char *buffer, int size);

/** \memberof Lines
 * 
 * Returns whether CRLF CRLF is reached
 * 
 * \param lns targeted Lines instance
 * \return targeted Lines instance
 */
//int lnsRqstComplete( Lines *lns );

/** \memberof Lines
 * 
 * Frees the Lines instance and all the lines managed by it.
 * 
 * \param lns targeted Lines instance
 */
void lnsFree( Lines *lns );

#endif