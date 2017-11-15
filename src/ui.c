#include <stddef.h>

#include "ui.h"

static void menu_on_hide(GtkWidget *menu, gpointer data);
static void menuitem_on_activate(GtkWidget *menuitem, gpointer data);

void ui_init(char *cfg) {
	char *chars = cfg;
	size_t char_cnt = g_utf8_strlen(chars, -1);

	GtkApplication *app = (GtkApplication*)g_application_get_default();
	GtkWidget *window = gtk_application_window_new(app);
	GtkWidget *menu = gtk_menu_new();

	GtkWidget *menuitems[char_cnt];
	for (size_t i = 0; i < char_cnt; i++) {
		char *label = g_utf8_substring(chars, i, i + 1);
		menuitems[i] = gtk_menu_item_new_with_label(label);
		g_free(label);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitems[i]);
		g_signal_connect(menuitems[i], "activate",
			G_CALLBACK(menuitem_on_activate), NULL);
	}

	g_signal_connect(menu, "hide", G_CALLBACK(menu_on_hide), app);

	gtk_widget_show_all(GTK_WIDGET(menu));
	gtk_menu_popup_at_widget(GTK_MENU(menu), window, 0, 0, NULL);
}



static void menu_on_hide(GtkWidget *menu, gpointer data) {
	(void)menu;

	g_application_quit(G_APPLICATION(data));
}

static void menuitem_on_activate(GtkWidget *menuitem, gpointer data) {
	(void)menuitem;
	(void)data;
}
