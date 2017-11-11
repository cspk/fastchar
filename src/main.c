#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "cfg.h"
#include "ui.h"

int main(int argc, char *argv[]) {
	const char *user_home = getenv("HOME");
	if (!user_home) {
		fprintf(stderr, "Could not get user HOME!\n");
		return 1;
	}
	gchar *cfg_path = g_strjoin("/", user_home, CFG_FILENAME, NULL);

	size_t cfg_sz;
	void *cfg = cfg_load(cfg_path, &cfg_sz);
	if (!cfg) {
		fprintf(stderr, "Error loading config file!\n");
		return 1;
	}
	g_free(cfg_path);

	GtkApplication *app = gtk_application_new("org.gtk."PROJECT_NAME,
		G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(ui_init), NULL);
	int status = g_application_run(G_APPLICATION(app), argc, argv);

	g_object_unref(app);
	return status;
}
