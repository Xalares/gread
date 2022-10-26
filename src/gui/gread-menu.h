#pragma once
#include <gtk/gtk.h>
#include "gread-window.h"

G_BEGIN_DECLS

#define GREAD_MENU_TYPE (gread_menu_get_type ())


G_DECLARE_FINAL_TYPE (GreadMenu, gread_menu, GREAD, MENU, AdwBin)

void gread_menu_set_window(GreadMenu *self, GreadAppWindow *win);

GreadMenu * gread_menu_new(GreadAppWindow *win);

G_END_DECLS
