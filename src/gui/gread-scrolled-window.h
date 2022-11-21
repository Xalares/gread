#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS

#define GREAD_SCROLLED_WINDOW_TYPE         (gread_scrolled_window_get_type ())

G_DECLARE_FINAL_TYPE (GreadScrolledWindow, gread_scrolled_window, GREAD, SCROLLED_WINDOW, GtkWidget)

G_END_DECLS
