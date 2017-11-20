#include <stdlib.h>
#include <gio/gio.h>

#include "cfg.h"
#include "dbg.h"

char* cfg_get_path(void) {
	const char *user_home = getenv("HOME");
	if (!user_home) {
		DBG("Could not get user HOME!\n");
		return NULL;
	}
	return g_strjoin("/", user_home, CFG_FILENAME, NULL);
}

char* cfg_load(const char *filename) {
	GFile *file = g_file_new_for_path(filename);
	if (!g_file_query_exists(file, NULL)) {
		DBG("Config file not found\n");
		g_object_unref(file);
		return NULL;
	}

	GFileInputStream *s = g_file_read(file, NULL, NULL);
	if (!s) {
		DBG("Can't open config file\n");
		g_object_unref(file);
		return NULL;
	}

	GDataInputStream *data_s = g_data_input_stream_new(G_INPUT_STREAM(s));
	GDataStreamByteOrder byte_order = G_DATA_STREAM_BYTE_ORDER_BIG_ENDIAN;
	g_data_input_stream_set_byte_order(data_s, byte_order);

	gsize sz;
	char *data = g_data_input_stream_read_line_utf8(data_s, &sz, NULL, NULL);
	if (!data || sz == 0) {
		DBG("Can't read config file\n");
		g_input_stream_close(G_INPUT_STREAM(data_s), NULL, NULL);
		g_input_stream_close(G_INPUT_STREAM(s), NULL, NULL);
		g_object_unref(data_s);
		g_object_unref(s);
		g_object_unref(file);
		return NULL;
	}

	return data;
}
