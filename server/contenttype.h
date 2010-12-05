 #ifndef __contenttype_header__
 #define __contenttype_header__
 
#include "lib/vector.h"
 
/**
 * Content Types
 */
typedef struct ContentTypes {
	Vector *ext;
	Vector *type;
} ContentTypes;
 
/** \memberof ContentTypes
 * 
 * Contructor - Allocates memory for a new ContentTypes instance. 
 *
 * \return new ContentTypes instance
 */
ContentTypes *ctInit();
 
ContentTypes *ctAdd( ContentTypes *ct, const char *extension, const char *type );

const char *ctGet( ContentTypes *ct, const char *extension );

/** \memberof ContentTypes
 * 
 * Frees the ContentTypes instance and all the variables that where initialized in this context.
 * 
 * \param ct targeted ContentTypes instance
 */
void ctFree( ContentTypes *ct );
 
 
 #endif