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
gread_app_window_dispose(GObject *object){
  G_OBJECT_CLASS(gread_app_window_parent_class)->dispose(object);
}

static void
gread_app_window_finalize(GObject *object){
  GreadAppWindow *self = GREAD_APP_WINDOW(object);
  g_clear_object(&self->main_box);
  g_clear_object(&self->content_box);
  g_clear_object(&self->header_bar);
  g_clear_object(&self->entry);
  g_clear_object(&self->button);
  g_clear_object(&self->label);
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
  win->label = g_object_new(GREAD_LABEL_TYPE, NULL);
  gtk_widget_init_template(GTK_WIDGET(win));
}

GreadAppWindow *
gread_app_window_new(GreadApplication *app){
  return g_object_new(GREAD_APP_WINDOW_TYPE, "application", app, NULL);
}
