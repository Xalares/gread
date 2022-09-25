#include <gtk/gtk.h>
#include "gread-label.h"
#include "gread-window.h"

struct _GreadAppWindow {
  AdwApplicationWindow parent;
  GtkWidget *main_box;
  GtkWidget *content_box;
  AdwHeaderBar *header_bar;
  GreadLabel *label;
  GtkEntry *entry;
  GtkButton *button;
};

G_DEFINE_TYPE (GreadAppWindow, gread_app_window, ADW_TYPE_APPLICATION_WINDOW)

static void
gread_app_window_class_init(GreadAppWindowClass *klass){

  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass),
                                              "/org/gnome/gread/gui/gread-window.ui");

  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GreadAppWindow, main_box);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GreadAppWindow, header_bar);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GreadAppWindow, content_box);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GreadAppWindow, label);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GreadAppWindow, entry);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), GreadAppWindow, button);
}

static void
gread_app_window_init(GreadAppWindow *win){
  g_type_ensure(GREAD_LABEL_TYPE);
  gtk_widget_init_template(GTK_WIDGET(win));
}

GreadAppWindow *
gread_app_window_new(GreadApplication *app){
  return g_object_new(GREAD_APP_WINDOW_TYPE, "application", app, NULL);
}
