#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "cfg.h"
#include "debug.h"

char* cfg_get_path(void) {
	const char *user_home = getenv("HOME");
	if (!user_home) {
		debug("Could not get user HOME!\n");
		return NULL;
	}
	return g_strjoin("/", user_home, CFG_FILENAME, NULL);
}

void* cfg_load(const char *filename, size_t *cfg_sz) {
	int fd = open(filename, O_RDONLY);
	if (!fd) {
		debug("Can't open() config file!\n");
		return NULL;
	}

	struct stat st;
    if(stat(filename, &st)) {
		debug("Can't stat() config file!\n");
		return NULL;
	}

	void *cfg = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (!cfg) {
		debug("Can't mmap() config file\n");
		return NULL;
	}

	*cfg_sz = st.st_size;
	return cfg;
}
