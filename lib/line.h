#ifndef __line_header__
#define __line_header__


/**
 * Contains a single line, and pointers to the previous or next line
 */
typedef struct Line {
	char *str;			/**< points to the 0-terminated string for this line */
	struct Line *next;	/**< points to next line */
	struct Line *prev;	/**< points to previous line */
} Line;

/** \memberof Line
 * 
 * Contructor - Allocates memory for a new Line instance. 
 * The string for this lines will be allocated with 1 byte 
 * as a 0 length string (0-terminated).
 * 
 * \return new Line instance
 */
Line *lnInit();


/** \memberof Line
 * 
 * Sets the string pointer of this line to a given string.
 * 
 * \param ln targeted Line instance
 * \param str new string
 * \return targeted Line instance
 * 
 */
Line *lnSet( Line *ln, char *str );


/** \memberof Line
 * 
 * Frees this line and tries to free previous and next line.
 * 
 * \param ln targeted Line instance
 */
void lnFree( Line *ln );

#endif