#include <gtk/gtk.h>
#include <adwaita.h>

#include "gread-scrolled-window.h"

enum {
  PROP_0,
  PROP_ORIENTATION,
  LAST_PROP = PROP_ORIENTATION
};

struct _GreadScrolledWindow {
  GtkBox parent;
  GtkWidget * scrollbar;
  GtkWidget * child;
};


static void gread_scrolled_window_buildable_init(GtkBuildableIface *iface);
void gread_scrolled_window_set_child(GreadScrolledWindow *self, GtkWidget *child);

G_DEFINE_TYPE_WITH_CODE (GreadScrolledWindow, gread_scrolled_window, GTK_TYPE_WIDGET,
                         G_IMPLEMENT_INTERFACE (GTK_TYPE_BUILDABLE,
                                                gread_scrolled_window_buildable_init)
                         G_IMPLEMENT_INTERFACE (GTK_TYPE_ORIENTABLE, NULL))


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

static void
gread_scrolled_window_buildable_init (GtkBuildableIface *iface)
{
  parent_buildable_iface = g_type_interface_peek_parent (iface);

  iface->add_child = gread_scrolled_window_buildable_add_child;
}

void
gread_scrolled_window_set_child(GreadScrolledWindow *self, GtkWidget *child){
  self->child = child;
  gtk_widget_insert_after(child, GTK_WIDGET (self), NULL);
}

static void
gread_scrolled_window_measure(GtkWidget *widget,
                              GtkOrientation orientation,
                              int for_size,
                              int *minimum,
                              int *natural,
                              int *minimum_baseline,
                              int *natural_baseline){

  GtkWidget *child;

  for (child = gtk_widget_get_first_child(widget);
       child != NULL;
       child = gtk_widget_get_next_sibling(child)){

    if(gtk_widget_should_layout(child))
      {
        g_print("Mesure widget %s %d\n", G_OBJECT_CLASS_NAME(G_OBJECT_GET_CLASS(child)), *minimum);

        int child_min = 0;
        int child_nat = 0;
        int child_min_baseline = -1;
        int child_nat_baseline = -1;

        gtk_widget_measure(child, orientation, for_size,
                           &child_min, &child_nat,
                           &child_min_baseline, &child_nat_baseline);

        *minimum = MAX(*minimum, child_min);
        *natural = MAX(*natural, child_nat);

        if (child_min_baseline > -1)
          *minimum_baseline = MAX(*minimum_baseline, child_min_baseline);

        if (child_nat_baseline > -1)
          *natural_baseline = MAX(*natural_baseline, child_nat_baseline);

      }
  }
}

static void
gread_scrolled_window_allocate(GtkWidget *widget, int width,
                               int height, int baseline){
  GtkWidget *child;
  for (child = gtk_widget_get_first_child(widget);
       child != NULL;
       child = gtk_widget_get_next_sibling(child)){

    if(child && gtk_widget_should_layout(child)){
      gtk_widget_allocate(child, width, height, baseline, NULL);
    }
  }
}

static void
gread_scrolled_window_class_init(GreadScrolledWindowClass *klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass   *object_class = G_OBJECT_CLASS(klass);

  widget_class->size_allocate = gread_scrolled_window_allocate;
  widget_class->measure = gread_scrolled_window_measure;

  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/org/gnome/gread/gui/gread-scrolled-window.ui");
  gtk_widget_class_bind_template_child(widget_class, GreadScrolledWindow, scrollbar);

}


static void
gread_scrolled_window_init(GreadScrolledWindow *self){
  gtk_widget_init_template(GTK_WIDGET(self));
  //gtk_widget_add_css_class(GTK_WIDGET(self->scrollbar), "right");
}
