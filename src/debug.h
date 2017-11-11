#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef DEBUG
#define debug(...) \
	do { \
		fprintf(stderr, "%s:%d: " , __FILE__, __LINE__); \
		fprintf(stderr, __VA_ARGS__); \
	} while (0);
#else
#define debug(...)
#endif /* DEBUG */

#endif /* DEBUG_H */
