#include <gtk/gtk.h>
#include "gread-menu.h"

struct _GreadMenu {
  GtkWidget parent;
  GtkMenuButton *menu_button;
};

G_DEFINE_TYPE(GreadMenu, grea_menu, GTK_TYPE_BIN)
