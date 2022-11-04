#include <gtk/gtk.h>
#include <adwaita.h>
#include <glib/gi18n-lib.h>

#include "gread-window.h"
#include "gread-menu.h"

struct _GreadMenu {
  AdwBin parent;
  GreadAppWindow *window;
  GtkMenuButton *menu_button;
  GtkPopoverMenu *menu;
  GtkSpinButton *digits_spin;
  GtkSpinButton *display_time_spin;
  gboolean locked;
  guint digits;
  guint display_time;
};

typedef enum {
  PROP_WINDOW = 1,
  PROP_LOCKED,
  N_PROPERTIES,
} GreadMenuProperty;

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

G_DEFINE_TYPE(GreadMenu, gread_menu, ADW_TYPE_BIN)

//calbacks
static void
gread_menu_digits_changed(GtkSpinButton *button, GreadMenu *self){
  self->window  = GREAD_APP_WINDOW(gtk_widget_get_root(GTK_WIDGET(self)));
  g_assert(GREAD_APP_WINDOW_TYPE == G_OBJECT_TYPE(self->window));
  gread_app_window_set_digits(self->window,
    (guint)gtk_spin_button_get_value_as_int(button));
}

static void
gread_menu_display_time_changed(GtkSpinButton *button, GreadMenu *self){
  self->window  = GREAD_APP_WINDOW(gtk_widget_get_root(GTK_WIDGET(self)));
  g_assert(GREAD_APP_WINDOW_TYPE == G_OBJECT_TYPE(self->window));
  gread_app_window_set_display_time(self->window,
                                    (guint)gtk_spin_button_get_value_as_int(button));

}

static void
gread_menu_set_property(GObject *object, guint property_id,
                        const GValue *value, GParamSpec *pspec){

  GreadMenu *self = GREAD_MENU(object);

  switch((GreadMenuProperty) property_id){

  case PROP_WINDOW:
    self->window = g_value_get_object(value);
    break;

  case PROP_LOCKED:
    self->locked = g_value_get_boolean(value);

    if(self->locked){
      gtk_widget_set_sensitive(GTK_WIDGET(self->digits_spin), false);
      gtk_widget_set_sensitive(GTK_WIDGET(self->display_time_spin), false);
      break;
    }

    gtk_widget_set_sensitive(GTK_WIDGET(self->digits_spin), true);
    gtk_widget_set_sensitive(GTK_WIDGET(self->display_time_spin), true);

    break;

  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    break;

  }

}

static void
gread_menu_get_property(GObject *object, guint property_id,
                        GValue *value, GParamSpec *pspec){

  GreadMenu *self = GREAD_MENU(object);

  switch((GreadMenuProperty) property_id){

  case PROP_WINDOW:
    g_value_set_object(value, self->window);
    break;

  case PROP_LOCKED:
    g_value_set_boolean(value, self->locked);
    break;

  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    break;

  }

}

void
gread_menu_set_window(GreadMenu *self,GreadAppWindow *win){
  self->window = win;
}

void
gread_menu_lock(GreadMenu *self){
  GValue value = G_VALUE_INIT;
  g_value_init(&value, G_TYPE_BOOLEAN);
  g_value_set_boolean(&value, true);
  g_object_set_property(G_OBJECT(self), "locked", &value);
}

void
gread_menu_unlock(GreadMenu *self){
  GValue value = G_VALUE_INIT;
  g_value_init(&value, G_TYPE_BOOLEAN);
  g_value_set_boolean(&value, false);
  g_object_set_property(G_OBJECT(self), "locked", &value);
}

gboolean
gread_menu_is_locked(GreadMenu *self){
  return self->locked;
}

static void
gread_menu_class_init(GreadMenuClass *klass){

  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass *object_class = G_OBJECT_CLASS(klass);


  obj_properties[PROP_WINDOW] =
    g_param_spec_object("window",
                        "Window",
                        "Window for which the parameters are set",
                        GREAD_APP_WINDOW_TYPE,
                        G_PARAM_WRITABLE);

  obj_properties[PROP_LOCKED] =
    g_param_spec_boolean("locked",
                         "Locked",
                         "Check if the menu is locked",
                         false,
                         G_PARAM_READWRITE);

  object_class->get_property = (GObjectGetPropertyFunc) gread_menu_get_property;
  object_class->set_property = (GObjectSetPropertyFunc) gread_menu_set_property;

  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);

  gtk_widget_class_set_template_from_resource(widget_class,
    "/org/gnome/gread/gui/gread-menu.ui");

  gtk_widget_class_bind_template_child(widget_class, GreadMenu, digits_spin);
  gtk_widget_class_bind_template_child(widget_class, GreadMenu, display_time_spin);
  gtk_widget_class_bind_template_child(widget_class, GreadMenu, menu_button);
  gtk_widget_class_bind_template_child(widget_class, GreadMenu, menu);
}

static void
gread_menu_init(GreadMenu *self){
  g_type_ensure(GREAD_APP_WINDOW_TYPE);

  gtk_widget_init_template(GTK_WIDGET(self));

  g_signal_connect(self->digits_spin, "value-changed",
                   G_CALLBACK(gread_menu_digits_changed), self);

  g_signal_connect(self->display_time_spin, "value-changed",
                   G_CALLBACK(gread_menu_display_time_changed), self);
}

GreadMenu *
gread_menu_new(GreadAppWindow *win){
  return g_object_new(GREAD_MENU_TYPE, "window", win, NULL);
}
