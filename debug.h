#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#ifdef __GNUC__
/* #define LOC  fprintf(stderr, "[in %s@line %d] %s:", \ */
/*                               __FILE__, __LINE__, __PRETTY_FUNCTION__); */
#define LOC  fprintf(stderr, "[in %s@line %d] %s: ", \
		     __FILE__, __LINE__, __FUNCTION__);
#else
#define LOC  fprintf(stderr, "[in %s@line %d] ", __FILE__, __LINE__); 
#endif //__GNUC__


#define ERR(...) do{LOC fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");} while(0)

#ifdef DEBUG
#define DBG(...) do{LOC fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");} while(0)
#define DBG_FUNC(...) do{__VA_ARGS__} while(0)
#else
#define DBG(...) do{;}while(0)
#define DBG_FUNC(...) do{;} while(0)
#endif //DEBUG

#define TEST(...) do{LOC fprintf(stdout, __VA_ARGS__); fprintf(stdout, "\n");}\
 while(0)

#endif //__DEBUG_H__
