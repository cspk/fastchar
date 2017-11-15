#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "cfg.h"
#include "ui.h"

static int g_main(void);

int main(int argc, char *argv[]) {
	GtkApplication *app = gtk_application_new("org.gtk."PROJECT_NAME,
		G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(g_main), NULL);
	int status = g_application_run(G_APPLICATION(app), argc, argv);

	g_object_unref(app);
	return status;
}



static int g_main(void) {
	char *cfg_path = cfg_get_path();
	if (!cfg_path) {
		fprintf(stderr, "Could not locate config file!\n");
		return 1;
	}

	char *cfg = cfg_load(cfg_path);
	if (!cfg) {
		fprintf(stderr, "Error loading config file!\n");
		return 1;
	}
	g_free(cfg_path);

	ui_init(cfg);

	return 0;
}
