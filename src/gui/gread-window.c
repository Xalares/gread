#include <gtk/gtk.h>
#include <glib/gi18n-lib.h>

#include "gread-label.h"
#include "gread-window.h"
#include "gread-menu.h"
#include "gread-number-entry.h"
#include "gread-random-number-activity.h"

struct _GreadAppWindow {
  AdwApplicationWindow parent;
  AdwHeaderBar *header_bar;
  GreadRandomNumberActivity * random_activity;
};

G_DEFINE_TYPE (GreadAppWindow, gread_app_window, ADW_TYPE_APPLICATION_WINDOW)

static void
gread_app_window_dispose(GObject *object){
  G_OBJECT_CLASS(gread_app_window_parent_class)->dispose(object);
}

static void
gread_app_window_finalize(GObject *object){
  G_OBJECT_CLASS(gread_app_window_parent_class)->finalize(object);
}

static void
gread_app_window_class_init(GreadAppWindowClass *klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->finalize = gread_app_window_finalize;
  object_class->dispose = gread_app_window_dispose;

  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass),
                                              "/org/gnome/gread/gui/gread-window.ui");

  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, header_bar);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, random_activity);
}

static void
gread_app_window_init(GreadAppWindow *self){
  g_type_ensure(GREAD_RANDOM_NUMBER_ACTIVITY_TYPE);
  gtk_widget_init_template(GTK_WIDGET(self));

}

GreadAppWindow *
gread_app_window_new(GreadApplication *app){
  return g_object_new(GREAD_APP_WINDOW_TYPE, "application", app, NULL);
}
