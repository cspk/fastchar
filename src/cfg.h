#ifndef CFG_H
#define CFG_H

#include <stddef.h>
#include <glib.h>

#define CFG_FILENAME "." PROJECT_NAME

char* cfg_get_path(void);
char* cfg_load(const char *filename, size_t *cfg_sz);

#endif /* CFG_H */
