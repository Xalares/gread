#include <gtk/gtk.h>
#include "gread-menu.h"

struct _GreadMenu {
  GtkWidget parent;
  GtkMenuButton *menu_button;
  /* GtkPopoverMenu *menu;
  GMenuModel *menu_model;
  GtkSpinButton *digits_spin;
  GtkLabel *test;*/
};

G_DEFINE_TYPE(GreadMenu, gread_menu, GTK_TYPE_WIDGET)

static void
gread_menu_class_init(GreadMenuClass *klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/org/gnome/gread/gui/gread-menu.ui");
  gtk_widget_class_bind_template_child(widget_class, GreadMenu, menu_button);
  //gtk_widget_class_bind_template_child(widget_class, GreadMenu, menu_model);
  //gtk_widget_class_bind_template_child(widget_class, GreadMenu, menu);

}

static void
gread_menu_init(GreadMenu *self){
  gtk_widget_init_template(GTK_WIDGET(self));
  //self->test = gtk_label_new("Test");
}
