#ifndef DBG_H
#define DBG_H

#include <stdio.h>

#ifdef DEBUG
#define DBG(...) \
	do { \
		fprintf(stderr, "%s:%d: " , __FILE__, __LINE__); \
		fprintf(stderr, __VA_ARGS__); \
	} while (0);
#else
#define DBG(...)
#endif /* DEBUG */

#endif /* DBG_H */
