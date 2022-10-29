#include <gtk/gtk.h>
#include "gread-label.h"
#include "gread-window.h"
#include "gread-menu.h"
#include "gread-number-entry.h"

struct _GreadAppWindow {
  AdwApplicationWindow parent;
  AdwHeaderBar *header_bar;
  GreadMenu *menu_button;
  GtkWidget *main_box;
  GtkWidget *content_box;
  GtkWidget *top_box;
  GtkWidget *bottom_box;
  GtkWidget *progress_bar;
  GtkWidget *digit_selector;
  GreadLabel *label;
  GtkLabel *greeting_label;
  GtkLabel *try_number_label;
  GreadNumberEntry *number_entry;
  GtkButton *button_start;
  GtkButton *button_next;

  gdouble progress;
  guint result;
  guint prog_step;
  guint try_number;
  guint display_time;
  guint digits;
  gboolean start;
};

typedef enum {
  PROP_DIGITS = 1,
  PROP_DISPLAY_TIME,
  N_PROPERTIES
} GreadAppWindowProperty;

typedef enum {
  PROGRESS,
  TIMEOUT,
  N_TIMEOUT
} GreadAppWindowTimeout;

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static guint obj_timeout[N_TIMEOUT] = {0, };

G_DEFINE_TYPE (GreadAppWindow, gread_app_window, ADW_TYPE_APPLICATION_WINDOW)


static void
gread_app_window_set_property(GObject *object, guint property_id,
                                   const GValue *value, GParamSpec *pspec){

  GreadAppWindow *self = GREAD_APP_WINDOW(object);

  switch((GreadAppWindowProperty) property_id){

  case PROP_DIGITS:
    self->digits = g_value_get_uint(value);
    break;

  case PROP_DISPLAY_TIME:
    self->display_time = g_value_get_uint(value);
    break;

  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    break;

  }
}

static void
gread_app_window_get_property(GObject *object, guint property_id,
                              GValue *value, GParamSpec *pspec){

  GreadAppWindow *self = GREAD_APP_WINDOW(object);

  switch((GreadAppWindowProperty) property_id){

  case PROP_DIGITS:
    g_value_set_uint(value, self->digits);
    break;

  case PROP_DISPLAY_TIME:
    g_value_set_uint(value, self->display_time);
    break;

  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    break;

  }
}

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

  obj_timeout[TIMEOUT] = g_timeout_add_once(self->display_time, (GSourceOnceFunc)timeout, self);
  obj_timeout[PROGRESS] = 0;
  return false;
}


//callbacks
static void
start_cb(GreadAppWindow *win){

  if(!win->start){

    win->start = true;
    win->try_number = 1;

    gtk_widget_set_visible(GTK_WIDGET(win->progress_bar), true);
    gtk_widget_set_visible(GTK_WIDGET(win->try_number_label), true);
    gtk_widget_set_visible(GTK_WIDGET(win->button_next), true);
    gtk_widget_set_visible(GTK_WIDGET(win->greeting_label), false);

    gtk_widget_set_sensitive(GTK_WIDGET(win->button_next), false);
    gread_menu_lock(win->menu_button);
    gread_label_roll(win->label);

    gtk_button_set_label(win->button_start, "Stop");
    obj_timeout[PROGRESS] = g_timeout_add(win->prog_step, G_SOURCE_FUNC(progress), win);

  }else{

    win->start = false;
    gread_menu_unlock(win->menu_button);
    gtk_widget_set_visible(GTK_WIDGET(win->progress_bar), false);
    gtk_button_set_label(win->button_start,"Start");

    win->progress = 0.0;
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(win->progress_bar), 0.0);

    if(obj_timeout[PROGRESS]){
      g_source_remove(obj_timeout[PROGRESS]);
    }

    if(obj_timeout[TIMEOUT]){
      g_source_remove(obj_timeout[TIMEOUT]);
    }

    if(gtk_widget_is_visible(GTK_WIDGET(win->number_entry))){
      gtk_widget_set_visible(GTK_WIDGET(win->number_entry), false);
    }

    if(gtk_widget_is_visible(GTK_WIDGET(win->label))){
      gtk_widget_set_visible(GTK_WIDGET(win->label), false);
    }

    if(gtk_widget_is_visible(GTK_WIDGET(win->try_number_label))){
      gtk_widget_set_visible(GTK_WIDGET(win->try_number_label), true);
    }

    float r = (float) win->result / (float) win->try_number;
    g_print("pourcentage %f, %d/%d\n", r, win->result, win->try_number);
    gread_number_entry_clear(win->number_entry);
    gtk_widget_remove_css_class(GTK_WIDGET(win->label), "correct");
    gtk_widget_remove_css_class(GTK_WIDGET(win->label), "wrong");
    gtk_widget_set_visible(GTK_WIDGET(win->greeting_label), true);
    gtk_widget_set_visible(GTK_WIDGET(win->button_next), false);
  }
}

static void
next_cb(GreadAppWindow *self){

  gread_number_entry_clear(self->number_entry);
  gtk_widget_remove_css_class(GTK_WIDGET(self->label), "correct");
  gtk_widget_remove_css_class(GTK_WIDGET (self->label), "wrong");
  gtk_widget_set_visible(GTK_WIDGET(self->number_entry), false);
  gtk_widget_set_visible(GTK_WIDGET(self->progress_bar), true);

  gread_label_roll(self->label);
  gtk_widget_set_visible(GTK_WIDGET(self->label), false);
  gtk_widget_set_visible(GTK_WIDGET(self->progress_bar), true);
  gtk_widget_set_sensitive(GTK_WIDGET(self->button_next), false);
  g_timeout_add(self->prog_step, G_SOURCE_FUNC(progress), self);
}

static void
enter_cb(GreadAppWindow *self){

  if(gtk_widget_is_visible(GTK_WIDGET(self->number_entry))){

    if (gread_number_entry_digits_typed(self->number_entry) == self->digits){

      gtk_widget_set_visible(GTK_WIDGET(self->number_entry), false);
      gtk_widget_set_visible(GTK_WIDGET(self->label), true);

      if(gread_number_entry_get_value(self->number_entry) ==
         gread_label_get_value(self->label)){
        self->result += 1;
        gtk_widget_add_css_class(GTK_WIDGET(self->label), "correct");

      }
      else{

        gtk_widget_add_css_class(GTK_WIDGET(self->label), "wrong");

      }
      self->try_number += 1;
      gtk_widget_grab_focus(self->button_next);

    }else{

      gtk_widget_error_bell(GTK_WIDGET(self->number_entry));
    }

  }
}

void
gread_app_window_set_digits(GreadAppWindow *self, guint digits){
  self->digits = digits;
  gread_label_set_digits(self->label, digits);
  gread_number_entry_set_digits(self->number_entry, digits);
}

void
gread_app_window_set_display_time(GreadAppWindow *self, guint display_time){
  self->display_time = display_time;
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


  obj_properties[PROP_DIGITS] =
    g_param_spec_uint("digits",
                      "Digits",
                      "Number of digits for the exercise difficulty",
                      1,
                      10,
                      1,
                      G_PARAM_READWRITE);

  obj_properties[PROP_DISPLAY_TIME] =
    g_param_spec_uint("display_time",
                      "Display time",
                      "Set the time to display the digits",
                      90,
                      700,
                      500,
                      G_PARAM_READWRITE);


  object_class->get_property = (GObjectGetPropertyFunc) gread_app_window_get_property;
  object_class->set_property = (GObjectSetPropertyFunc) gread_app_window_set_property;
  object_class->finalize = gread_app_window_finalize;
  object_class->dispose = gread_app_window_dispose;

  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass),
                                              "/org/gnome/gread/gui/gread-window.ui");

  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, main_box);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, header_bar);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, menu_button);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, content_box);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, progress_bar);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, top_box);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, bottom_box);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, label);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, greeting_label);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, try_number_label);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, number_entry);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, button_start);
  gtk_widget_class_bind_template_child(widget_class, GreadAppWindow, button_next);
}

static void
gread_app_window_init(GreadAppWindow *self){
  g_type_ensure(GREAD_MENU_TYPE);
  g_type_ensure(GREAD_LABEL_TYPE);
  g_type_ensure(GREAD_NUMBER_ENTRY_TYPE);
  time_t t;
  srand((unsigned) time(&t));
  self->start = false;
  self->try_number = 0;
  self->display_time = 500;
  self->prog_step = 175;
  self->progress = 0.0;
  self->digits = 2;
  self->result = 0;

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
