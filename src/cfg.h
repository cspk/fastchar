#ifndef CFG_H
#define CFG_H

#include <stddef.h>
#include <glib.h>

#define CFG_FILENAME ".fastchar"

char* cfg_get_path(void);
char* cfg_load(const char *filename);

#endif /* CFG_H */
