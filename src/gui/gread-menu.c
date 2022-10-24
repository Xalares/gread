#include <gtk/gtk.h>
#include <adwaita.h>
#include "gread-menu.h"

struct _GreadMenu {
  AdwBin parent;
  GtkMenuButton *menu_button;
  GtkPopoverMenu *menu;
  GtkSpinButton *digits_spin;
};

G_DEFINE_TYPE(GreadMenu, gread_menu, ADW_TYPE_BIN)

static void
gread_menu_class_init(GreadMenuClass *klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  gtk_widget_class_set_template_from_resource(widget_class,
    "/org/gnome/gread/gui/gread-menu.ui");

  gtk_widget_class_bind_template_child(widget_class, GreadMenu, digits_spin);
  gtk_widget_class_bind_template_child(widget_class, GreadMenu, menu_button);
  gtk_widget_class_bind_template_child(widget_class, GreadMenu, menu);

}

static void
gread_menu_init(GreadMenu *self){
  gtk_widget_init_template(GTK_WIDGET(self));
  //self->menu_button = gtk_menu_button_new();
}
