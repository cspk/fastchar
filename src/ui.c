#include <stddef.h>
#include <stdbool.h>

#include "ui.h"
#include "x11.h"

static const char *selected_char = NULL;

static void menu_on_hide(GtkApplication *app);
static void menu_on_activate(GtkWidget *menu);
static gboolean menu_key_event(GtkWidget *menu, GdkEvent *event, gpointer data);

static void menuitem_capitalize_label(GtkWidget *item, gpointer data);

void ui_init(const char *cfg) {
	const char *chars = cfg;
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

	gtk_widget_set_events(menu, GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);
	g_signal_connect(menu, "key-press-event", G_CALLBACK(menu_key_event), NULL);
	g_signal_connect(menu, "key-release-event", G_CALLBACK(menu_key_event), NULL);

	gtk_widget_show_all(GTK_WIDGET(menu));
	gtk_menu_popup_at_widget(GTK_MENU(menu), window, 0, 0, NULL);
	gtk_menu_shell_select_first(GTK_MENU_SHELL(menu), FALSE);
}



static void menu_on_hide(GtkApplication *app) {
	if (selected_char) {
		x11_type_char(selected_char);
	}

	g_application_quit(G_APPLICATION(app));
}

static void menu_on_activate(GtkWidget *menu) {
	GtkWidget *item = gtk_menu_shell_get_selected_item(GTK_MENU_SHELL(menu));
	selected_char = gtk_menu_item_get_label(GTK_MENU_ITEM(item));
}

static gboolean menu_key_event(GtkWidget *menu, GdkEvent *event, gpointer data) {
	(void)data;

	GdkEventKey *key_event = (GdkEventKey*)event;
	switch (key_event->keyval) {
	case GDK_KEY_Shift_L:
	case GDK_KEY_Shift_R: ;
		bool b = (key_event->type == GDK_KEY_PRESS) ? true : false;
		gtk_container_foreach(GTK_CONTAINER(menu), menuitem_capitalize_label, &b);

		return TRUE;
		break;

	case GDK_KEY_Return:
		if ((key_event->type == GDK_KEY_PRESS) &&
		    (key_event->state & GDK_SHIFT_MASK)) {
			g_signal_emit_by_name(menu, "activate-current");

			return TRUE;
		}
		break;

	case GDK_KEY_Up:
	case GDK_KEY_Down:
		if ((key_event->type == GDK_KEY_PRESS) &&
		    (key_event->state & GDK_SHIFT_MASK)) {
			GtkMenuDirectionType dir = (key_event->keyval == GDK_KEY_Up) ?
				GTK_MENU_DIR_PREV : GTK_MENU_DIR_NEXT;
			g_signal_emit_by_name(menu, "move-current", dir);

			return TRUE;
		}
		break;
	}

	return FALSE;
}

static void menuitem_capitalize_label(GtkWidget *item, gpointer data) {
	bool capitalize = *(bool*)data;

	const char *old_label = gtk_menu_item_get_label(GTK_MENU_ITEM(item));
	char *new_label;
	if (capitalize) {
		new_label = g_utf8_strup(old_label, -1);
	}
	else {
		new_label = g_utf8_strdown(old_label, -1);
	}

	gtk_menu_item_set_label(GTK_MENU_ITEM(item), new_label);
	g_free(new_label);
}
