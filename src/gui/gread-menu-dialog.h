#pragma once
#include <gtk/gtk.h>
#include "gread-application.h"

G_BEGIN_DECLS
#define GREAD_MENU_DIALOG_TYPE(gread_menu_dialog_get_type())

G_DECLARE_FINAL_TYPE(GreadMenuDialog, gread_menu_dialog, GREAD, MENU_DIALOG, GtkDialog)

G_END_DECLS
