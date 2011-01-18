 #ifndef __stringmap_header__
 #define __stringmap_header__
 
#include "lib/vector.h"
 
/**
 * Content Types
 */
typedef struct StringMap {
	Vector *ext;
	Vector *type;
} StringMap;
 
/** \memberof StringMap
 * 
 * Contructor - Allocates memory for a new StringMap instance. 
 *
 * \return new StringMap instance
 */
StringMap *smInit();
 
StringMap *smAdd( StringMap *sm, const char *key, const char *value );

const char *smGet( StringMap *sm, const char *key );

/** \memberof StringMap
 * 
 * Frees the StringMap instance and all the variables that where initialized in this context.
 * 
 * \param sm targeted StringMap instance
 */
void smFree( StringMap *sm );
 
 
 #endif