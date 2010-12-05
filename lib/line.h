#ifndef __line_header__
#define __line_header__


/**
 * Contains a single line, and pointers to the previous or next line
 */
typedef struct Line {
	char *str;			/**< contains the 0-terminated string as data for this line */
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

//entfernt vorhande CR und LF aus ln->str
/** \memberof Line
 * 
 * removes any \r and \\n characters from string
 * 
 * \param ln targeted Line instance
 * \return targeted Line instance
 */
Line *lnTrim( Line *ln );

/** \memberof Line
 * 
 * Sets the string value of this line to a given string with a specific length.
 * The memory of the existing string value is reallocated.
 * If existing string value was set to NULL, new memory is allocated.
 * 
 * \param ln targeted Line instance
 * \param str new string value (will be copied)
 * \param size size of new string value
 * \return targeted Line instance
 * 
 */
Line *lnSet( Line *ln, char *str, int size );

/** \memberof Line
 * 
 * Appends the given string value with a specific length to the string value 
 * of this line .
 * The memory of the existing string value is reallocated.
 * If existing string value was set to NULL, new memory is allocated.
 * 
 * \param ln targeted Line instance
 * \param str string value to append (will be copied)
 * \param size size of string value to append
 * \return targeted Line instance
 * 
 */
Line *lnAppend( Line *ln, char *str, int size );

/** \memberof Line
 * 
 * Frees this line and tries to free previous and next line.
 * 
 * \param ln targeted Line instance
 */
void lnFree( Line *ln );

#endif