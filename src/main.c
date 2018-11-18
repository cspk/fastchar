#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "cfg.h"
#include "ui.h"

static gboolean arg_version;

static GOptionEntry params[] =
{
  { "version", 0, 0, G_OPTION_ARG_NONE, &arg_version, "Show version", NULL },
  { NULL }
};

static int g_main(void);

int main(int argc, char *argv[]) {
	const char *app_name = "org.gtk.fastchar";
	GApplicationFlags app_flags = G_APPLICATION_FLAGS_NONE;
	GtkApplication *app = gtk_application_new(app_name, app_flags);

	GOptionContext *context = g_option_context_new("- type in arbitrary symbols");
	g_option_context_add_main_entries(context, params, NULL);
	g_option_context_add_group(context, gtk_get_option_group(TRUE));

	GError *error;
	if (!g_option_context_parse(context, &argc, &argv, &error)) {
		fprintf(stderr, "error parsing program arguments\n");
		return 1;
	}

	if (arg_version) {
		printf("v%s\n", PROJECT_VERSION);
		g_object_unref(app);
		return 0;
	}
	
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

	free(cfg);
	return 0;
}
