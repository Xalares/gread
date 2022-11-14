#include <gtk/gtk.h>
#include <adwaita.h>

#include "gread-scrolled-window.h"

struct _GreadScrolledWindow {
  AdwBin parent;
  GtkWidget * scrollbar;
  GtkWidget * content;
};


static void gread_scrolled_window_buildable_init(GtkBuildableIface *iface);

G_DEFINE_TYPE_WITH_CODE (GreadScrolledWindow, gread_scrolled_window, ADW_TYPE_BIN,
                         G_IMPLEMENT_INTERFACE (GTK_TYPE_BUILDABLE,
                                                gread_scrolled_window_buildable_init))

static GtkBuildableIface *parent_buildable_iface;

static void
  gread_scrolled_window_buildable_add_child(GtkBuildable *buildable,
                                            GtkBuilder   *builder,
                                            GObject      *child,
                                            const char   *type){
  if(GTK_IS_WIDGET(child))
    gread_scrolled_window_set_child(GREAD_SCROLLED_WINDOW(buildable), GTK_WIDGET(child));
  else
    parent_buildable_iface->add_child(buildable, builder, child, type);
}

void
gread_scrolled_window_set_child(GreadScrolledWindow *self, GtkWidget *child){

}

static void
gread_scrolled_window_class_init(GreadScrolledWindowClass *klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass   *object_class = G_OBJECT_CLASS(klass);
  gtk_widget_class_set_template_from_resource(widget_class,
                                               "/org/gnome/gread/gui/gread-scrolled-window.ui");
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

static void
gread_scrolled_window_init(GreadScrolledWindow *self){
  gtk_widget_init_template(GTK_WIDGET(self));
}
