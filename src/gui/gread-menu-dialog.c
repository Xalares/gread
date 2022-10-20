#include <gtk/gtk.h>
#include "gread-menu-dialog.h"

struct _GreadMenuDialog {
  GtkDialog parent;
};

G_DEFINE_TYPE (GreadMenuDialog, gread_menu_dialog, GTK_TYPE_DIALOG)
