#ifndef __debug_header__
#define __debug_header__

#define DEBUG

#ifdef DEBUG
#define LOG(string, args...) \
printf ( string, ##args )
#else
#define LOG(string, args...) 


#endif

#endif

