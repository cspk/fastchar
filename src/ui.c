#include <stddef.h>

#include "ui.h"

void ui_init(GtkApplication *app, gpointer data) {
	char *chars = data;
	size_t char_cnt = g_utf8_strlen(data, -1);

	GtkWidget *window = gtk_application_window_new(app);
	GtkWidget *menu = gtk_menu_new();

	GtkWidget *menuitems[char_cnt];
	for (size_t i = 0; i < char_cnt; i++) {
		char *label = g_utf8_substring(chars, i, i + 1);
		menuitems[i] = gtk_menu_item_new_with_label(label);
		g_free(label);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitems[i]);
	}

	gtk_widget_show_all(GTK_WIDGET(menu));
	gtk_menu_popup_at_widget(GTK_MENU(menu), window, 0, 0, NULL);
}
