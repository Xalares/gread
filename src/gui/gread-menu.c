#include <gtk/gtk.h>
#include "gread-menu.h"
#include "gread-window.h"

struct _GreadMenu {
  GtkDialog parent;
};

G_DEFINE_TYPE (GreadMenu, gread_menu, GTK_TYPE_MENU)


static void
gread_menu_class_init(GreadMenuClass *klass){

}

GreadMenuDialog *
gread_menu_new(GreadAppWindow *win){
  return g_object_new(GREAD_MENU_TYPE, "transient-for", win, "use-header-bar", TRUE, NULL);
}
