#ifndef CFG_H
#define CFG_H

#include <stddef.h>
#include <glib.h>

#define CFG_DIRNAME "fastchar"
#define CFG_FILENAME "config"

char* cfg_get_path(void);
char* cfg_load(const char *filename);

#endif /* CFG_H */
