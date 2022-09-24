#include "gread-application.h"
#include "gread-window.h"
#include <gtk/gtk.h>

struct _GreadApplication {
  AdwApplication parent;
  GtkWidget *widget;
}
