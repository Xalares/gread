#include <gtk/gtk.h>
#include "gread-menu-dialog.h"
#include "gread-window.h"

struct _GreadMenuDialog {
  GtkDialog parent;
};

G_DEFINE_TYPE (GreadMenuDialog, gread_menu_dialog, GTK_TYPE_DIALOG)


GreadMenuDialog *
gread_menu_dialog_new(GreadAppWindow *win){
  return g_object_new(GREAD_MENU_DIALOG_TYPE, "transient-for", win, "use-header-bar", TRUE, NULL);
}
