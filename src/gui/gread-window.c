#include <gtk/gtk.h>
#include "gread-label.h"
#include "gread-window.h"
#include "gread-number-entry.h"

struct _GreadAppWindow {
  AdwApplicationWindow parent;
  AdwHeaderBar *header_bar;
  GMenuModel *menu;
  GtkWidget *main_box;
  GtkWidget *content_box;
  GtkWidget *bottom_box;
  GtkWidget *progress_bar;
  GreadLabel *label;
  GreadNumberEntry *number_entry;
  GtkButton *button_start;
  GtkButton *button_next;

  gdouble progress;
  guint prog_step;
  guint interval;
  guint digits;
  gboolean start;
};

typedef enum {
  PROGRESS,
  TIMEOUT,
  N_TIMEOUT
} GreadAppWindowTimeout;

static guint *obj_timeout[N_TIMEOUT] = {NULL, };

G_DEFINE_TYPE (GreadAppWindow, gread_app_window, ADW_TYPE_APPLICATION_WINDOW)

//timeouts
static void
timeout(GreadAppWindow *self){
  gtk_widget_set_visible(GTK_WIDGET(self->label), false);
  gtk_widget_set_visible(GTK_WIDGET(self->number_entry), true);
  gtk_widget_grab_focus(GTK_WIDGET(self->number_entry));
  obj_timeout[TIMEOUT] = 0;
}

static gboolean
progress(GreadAppWindow *self){

  if(self->progress <= 1.0){
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(self->progress_bar), self->progress);
    self->progress += 0.25;
    return true;
  }
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(self->progress_bar), 0.0);
  gtk_widget_set_visible(GTK_WIDGET(self->progress_bar), false);
  self->progress = 0.0;
  gtk_widget_set_visible(GTK_WIDGET(self->label), true);
  gtk_widget_set_sensitive(GTK_WIDGET(self->button_next), true);

  obj_timeout[TIMEOUT] = g_timeout_add_once(self->interval, timeout, self);
  obj_timeout[PROGRESS] = 0;
  return false;
}


//callbacks
static void
start_cb(GreadAppWindow *win){

  if(!win->start){
    win->start = true;
    gtk_widget_set_visible(GTK_WIDGET(win->label), false);
    gread_label_roll(win->label);
    gtk_widget_set_visible(win->button_next, true);
    gtk_widget_set_sensitive(GTK_WIDGET(win->button_next), false);

    gtk_button_set_label(win->button_start, "Stop");
    gtk_widget_set_visible(win->progress_bar, true);
    obj_timeout[PROGRESS] = g_timeout_add(win->prog_step, progress, win);

  }else{
    win->start = false;
    gtk_widget_set_visible(win->progress_bar, false);
    gtk_button_set_label(win->button_start,"Start");

    win->progress = 0.0;
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(win->progress_bar), 0.0);

    if(obj_timeout[PROGRESS]){
      g_print("source progress with id:%d removed\n", obj_timeout[PROGRESS]);

      g_source_remove(obj_timeout[PROGRESS]);
    }

    if(obj_timeout[TIMEOUT]){
      g_print("source timeout with id:%d removed\n", obj_timeout[TIMEOUT]);

      g_source_remove(obj_timeout[TIMEOUT]);
    }

    if(gtk_widget_is_visible(GTK_WIDGET(win->number_entry))){
      gtk_widget_set_visible(GTK_WIDGET(win->number_entry), false);
    }

    if(!gtk_widget_is_visible(GTK_WIDGET(win->label))){
      gtk_widget_set_visible(GTK_WIDGET(win->label), true);
    }
    gread_number_entry_clear(win->number_entry);
    gtk_widget_remove_css_class(win->label, "correct");
    gtk_widget_remove_css_class(win->label, "wrong");
    gread_label_set_text(win->label, "Prêt ?");
    gtk_widget_set_visible(GTK_WIDGET(win->button_next), false);
  }
}

static void
next_cb(GreadAppWindow *self){

  gread_number_entry_clear(self->number_entry);
  gtk_widget_remove_css_class(self->label, "correct");
  gtk_widget_remove_css_class(self->label, "wrong");
  gtk_widget_set_visible(GTK_WIDGET(self->number_entry), false);
  gtk_widget_set_visible(GTK_WIDGET(self->progress_bar), true);

  gread_label_roll(self->label);
  gtk_widget_set_visible(GTK_WIDGET(self->label), false);
  gtk_widget_set_visible(GTK_WIDGET(self->progress_bar), true);
  gtk_widget_set_sensitive(GTK_WIDGET(self->button_next), false);
  g_timeout_add(self->prog_step, progress, self);
}

static void
enter_cb(GreadAppWindow *self){

  if(gtk_widget_is_visible(GTK_WIDGET(self->number_entry))){

    if (gread_number_entry_digits_typed(self->number_entry) == self->digits){

      gtk_widget_set_visible(GTK_WIDGET(self->number_entry), false);
      gtk_widget_set_visible(GTK_WIDGET(self->label), true);

      if(gread_number_entry_get_value(self->number_entry) ==
         gread_label_get_value(self->label)){

        gtk_widget_add_css_class(self->label, "correct");
      }else{

        gtk_widget_add_css_class(self->label, "wrong");
      }

      //gtk_widget_grab_focus(self->button_next);
    }else{
      gtk_widget_error_bell(self->number_entry);
    }
  }
}

static void
gread_app_window_dispose(GObject *object){
  GreadAppWindow *self = GREAD_APP_WINDOW(object);
  g_clear_pointer(&self->label, gtk_widget_unparent);
  g_clear_pointer(&self->number_entry, gtk_widget_unparent);
  g_clear_pointer(&self->button_next, gtk_widget_unparent);
  g_clear_pointer(&self->button_start, gtk_widget_unparent);
  g_clear_pointer(&self->progress_bar, gtk_widget_unparent);
  g_clear_pointer(&self->header_bar, gtk_widget_unparent);

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

  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, main_box);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, header_bar);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, menu);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, content_box);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, progress_bar);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, bottom_box);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, label);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, number_entry);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, button_start);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, button_next);
}

static void
gread_app_window_init(GreadAppWindow *self){
  g_type_ensure(GREAD_LABEL_TYPE);
  g_type_ensure(GREAD_NUMBER_ENTRY_TYPE);

  self->start = false;
  self->interval = 500;
  self->prog_step = 175;
  self->progress = 0.0;
  self->digits = 2;

  gtk_widget_init_template(GTK_WIDGET(self));

  g_signal_connect_swapped(self->number_entry, "invalid-char",
                           G_CALLBACK(gtk_widget_error_bell), self);
  g_signal_connect_swapped(self->number_entry, "limit-reached",
                           G_CALLBACK(gtk_widget_error_bell), self);
  g_signal_connect_swapped(self->button_start, "clicked", G_CALLBACK(start_cb), self);
  g_signal_connect_swapped(self->button_next, "clicked", G_CALLBACK(next_cb), self);
  g_signal_connect_swapped(self->number_entry, "enter-pressed", G_CALLBACK(enter_cb), self);
}

GreadAppWindow *
gread_app_window_new(GreadApplication *app){
  return g_object_new(GREAD_APP_WINDOW_TYPE, "application", app, NULL);
}
