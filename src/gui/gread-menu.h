#pragma once
#include <gtk/gtk.h>
#include "gread-window.h"

G_BEGIN_DECLS
#define GREAD_MENU_TYPE (gread_menu_get_type ())

G_DECLARE_FINAL_TYPE(GreadMenu, gread_menu, GREAD, MENU, GtkPopover)

GreadMenu * gread_menu_new(GreadAppWindow *app);

G_END_DECLS
