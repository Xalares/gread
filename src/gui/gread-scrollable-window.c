#include <gtk/gtk.h>

#include "gread-scrollable-window.h"

struct _GreadScrollableWindow {
  AdwBin parent;
};

G_DEFINE_TYPE (GreadScrollableWindow, gread_scrollable_window)





static void
gread_scrollable_window_class_init(GreadScrollableWindowClass *klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass   *object_class = G_OBJECT_CLASS(klass);
  gtk_widget_class_set_template_from_ressource(widget_class,
                                               "/org/gnome/gread/gui/gread-scrollable-window.ui");
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

static void
gread_scrollable_window(GreadScrollableWindow *self){
  gtk_widget_init_template(GTK_WIDGET(self));
}
