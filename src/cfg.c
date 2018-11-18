#include <stdlib.h>
#include <gio/gio.h>

#include "cfg.h"
#include "dbg.h"

char* cfg_get_path(void) {
	const gchar *cfg_dir = g_get_user_config_dir();

	return g_strjoin("/", cfg_dir, CFG_DIRNAME "/" CFG_FILENAME, NULL);
}

char* cfg_load(const char *filename) {
	char *ret = NULL;

	GFile *file = g_file_new_for_path(filename);
	if (!g_file_query_exists(file, NULL)) {
		DBG("Config file not found\n");
		goto err_query;
	}

	GFileInputStream *s = g_file_read(file, NULL, NULL);
	if (!s) {
		DBG("Can't open config file\n");
		goto err_open;
	}

	GDataInputStream *data_s = g_data_input_stream_new(G_INPUT_STREAM(s));
	g_assert(data_s);
	GDataStreamByteOrder byte_order = G_DATA_STREAM_BYTE_ORDER_BIG_ENDIAN;
	g_data_input_stream_set_byte_order(data_s, byte_order);

	gsize sz;
	char *data = g_data_input_stream_read_line_utf8(data_s, &sz, NULL, NULL);
	if (!data || sz == 0) {
		DBG("Can't read config file (probably it's empty)\n");
		goto err_read;
	}

	ret = data;

err_read:
	g_input_stream_close(G_INPUT_STREAM(data_s), NULL, NULL);
	g_input_stream_close(G_INPUT_STREAM(s), NULL, NULL);
	g_object_unref(data_s);
	g_object_unref(s);
err_query:
err_open:
	g_object_unref(file);

	return ret;
}
