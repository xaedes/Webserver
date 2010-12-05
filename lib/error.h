#ifndef __error_header__
#define __error_header__

/**
 * Some useful functions
 */
struct Error { };


//nur benutzen, wenn ein systemaufruf fehlschlug
/** \memberof Error
 * 
 * Handles a fail of not-user code. (library call etc)
 * 
 */
void handle_error( char *msg );

/** \memberof Error
 * 
 * Handles a fail of user code. 
 * 
 */
void handle_fail( char *msg );

#endif