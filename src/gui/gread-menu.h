#pragma once
#include <gtk/gtk.h>
#include "gread-application.h"

G_BEGIN_DECLS
#define GREAD_MENU_TYPE(gread_menu_get_type())

G_DECLARE_FINAL_TYPE(GreadMenu, gread_menu, GREAD, MENU, GtkPopover)

G_END_DECLS
