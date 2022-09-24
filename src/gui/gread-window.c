#include <gtk/gtk.h>
#include "gread-window.h"

struct _GreadWindow {
  AdwApplicationWindow parent;
  GtkWidget *main_box;
  GreadLabel *label;
  GtkEntry *entry;
}

G_DEFINE_TYPE (GreadWindow, gread_window, ADW_TYPE_APPLICATION_WINDOW)

static void gread_window_class_init(GreadWindowClass *klass){

}

static void gread_window_init(GreadWindow *self){

}
