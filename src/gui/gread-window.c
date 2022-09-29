#include <gtk/gtk.h>
#include "gread-label.h"
#include "gread-window.h"
#include "gread-number-entry.h"

struct _GreadAppWindow {
  AdwApplicationWindow parent;
  GtkWidget *main_box;
  GtkWidget *content_box;
  GtkWidget *bottom_box;
  AdwHeaderBar *header_bar;
  GreadLabel *label;
  GreadNumberEntry *entry;
  GtkButton *button_start;
  GtkButton *button_next;
  gboolean start;
};

G_DEFINE_TYPE (GreadAppWindow, gread_app_window, ADW_TYPE_APPLICATION_WINDOW)

static void
gread_app_window_dispose(GObject *object){
  GreadAppWindow *self = GREAD_APP_WINDOW(object);
  gtk_widget_unparent(GTK_WIDGET(self->label));
  G_OBJECT_CLASS(gread_app_window_parent_class)->dispose(object);
}

//callbacks
static void
on_start(GreadAppWindow *win){

  if(!win->start){
    gread_label_roll(win->label);
    gtk_widget_set_visible(GTK_WIDGET(win->button_next), true);
    gtk_button_set_label(win->button_start, "Stop");
    win->start = true;
  }else{
    gtk_button_set_label(win->button_start,"Start");
    gtk_widget_set_visible(GTK_WIDGET(win->button_next), false);
    win->start = false;
}
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

  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, main_box);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, header_bar);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, content_box);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, bottom_box);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, label);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, entry);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, button_start);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, button_next);
}

static void
gread_app_window_init(GreadAppWindow *win){
  win->start = false;
  g_type_ensure(GREAD_LABEL_TYPE);
  //win->label = g_object_new(GREAD_LABEL_TYPE, NULL);
  gtk_widget_init_template(GTK_WIDGET(win));
  g_signal_connect_swapped(win->button_start, "clicked", G_CALLBACK(on_start), win);
}

GreadAppWindow *
gread_app_window_new(GreadApplication *app){
  return g_object_new(GREAD_APP_WINDOW_TYPE, "application", app, NULL);
}
