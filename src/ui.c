#include <stddef.h>
#include <stdbool.h>

#include "ui.h"
#include "x11.h"

static bool char_selected = false;

static void menu_on_hide(GtkApplication *app);
static void menu_on_activate(GtkWidget *menu);

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
	}


	g_signal_connect_swapped(menu, "hide", G_CALLBACK(menu_on_hide), app);
	g_signal_connect(menu, "activate-current", G_CALLBACK(menu_on_activate), NULL);

	gtk_widget_show_all(GTK_WIDGET(menu));
	gtk_menu_popup_at_widget(GTK_MENU(menu), window, 0, 0, NULL);
}



static void menu_on_hide(GtkApplication *app) {
	if (char_selected) {
		x11_simulate_paste();
		usleep(4000);
	}

	g_application_quit(G_APPLICATION(app));
}

static void menu_on_activate(GtkWidget *menu) {
	GtkWidget *item = gtk_menu_shell_get_selected_item(GTK_MENU_SHELL(menu));
	const char *label = gtk_menu_item_get_label(GTK_MENU_ITEM(item));

	GtkClipboard *primary = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
	GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	gtk_clipboard_set_text(primary, label, -1);
	gtk_clipboard_set_text(clipboard, label, -1);
	char_selected = true;
}
