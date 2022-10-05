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
  GreadNumberEntry *number_entry;
  GtkButton *button_start;
  GtkButton *button_next;
  guint interval;
  gboolean start;
};

G_DEFINE_TYPE (GreadAppWindow, gread_app_window, ADW_TYPE_APPLICATION_WINDOW)

static void
gread_app_window_dispose(GObject *object){
  GreadAppWindow *self = GREAD_APP_WINDOW(object);
  gtk_widget_unparent(GTK_WIDGET(self->label));
  G_OBJECT_CLASS(gread_app_window_parent_class)->dispose(object);
}

static void timeout(GreadAppWindow *self){
  gtk_widget_set_visible(GTK_WIDGET(self->label), false);
  gtk_widget_set_visible(GTK_WIDGET(self->number_entry), true);
}

//callbacks
static void
start(GreadAppWindow *win){
  if(!win->start){
    gread_label_roll(win->label);
    gtk_widget_set_visible(GTK_WIDGET(win->button_next), true);
    gtk_button_set_label(win->button_start, "Stop");
    g_timeout_add_once(win->interval, timeout, win);
    win->start = true;
  }else{
    gtk_button_set_label(win->button_start,"Start");

    if(gtk_widget_is_visible(GTK_WIDGET(win->number_entry))){
      gtk_widget_set_visible(GTK_WIDGET(win->number_entry), false);
    }

    if(!gtk_widget_is_visible(GTK_WIDGET(win->label))){
      gtk_widget_set_visible(GTK_WIDGET(win->label), true);
    }
    gread_label_set_text(win->label, "Prêt ?");
    gtk_widget_set_visible(GTK_WIDGET(win->button_next), false);
    win->start = false;
  }
}

static void
next(GreadAppWindow *self){
  if(gtk_widget_is_visible(GTK_WIDGET(self->number_entry))){
    gtk_widget_set_visible(GTK_WIDGET(self->number_entry), false);
  }
  gread_label_roll(self->label);
  gtk_widget_set_visible(GTK_WIDGET(self->label), true);
  g_timeout_add_once(self->interval, timeout, self);
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
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, number_entry);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, button_start);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, button_next);
}

static void
gread_app_window_init(GreadAppWindow *self){
  self->start = false;
  g_type_ensure(GREAD_LABEL_TYPE);
  g_type_ensure(GREAD_NUMBER_ENTRY_TYPE);
  //win->label = g_object_new(GREAD_LABEL_TYPE, NULL);
  self->interval = 500;
  gtk_widget_init_template(GTK_WIDGET(self));
  g_signal_connect_swapped(self->button_start, "clicked", G_CALLBACK(start), self);
  g_signal_connect_swapped(self->button_next, "clicked", G_CALLBACK(next), self);
}

GreadAppWindow *
gread_app_window_new(GreadApplication *app){
  return g_object_new(GREAD_APP_WINDOW_TYPE, "application", app, NULL);
}
