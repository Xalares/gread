#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS

#define GREAD_SCROLLABLE_WINDOW_TYPE (gread_scrollable_window_get_type())

G_DECLARE_FINAL_TYPE (GreadScrollableWindow, gread_scrollable_window, GREAD, SCROLLABLE_WINDOW, GtkWidget)

G_END_DECLS
