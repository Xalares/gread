#include <gtk/gtk.h>
#include <glib/gi18n-lib.h>

#include "gread-label.h"
#include "gread-menu.h"
#include "gread-random-number-activity.h"
#include "gread-number-entry.h"

struct _GreadRandomNumberActivity {
  AdwBin parent;
  GreadMenu *menu_button;
  GtkWidget *main_box;
  GtkWidget *content_box;
  GtkWidget *top_box;
  GtkWidget *bottom_box;
  GtkWidget *progress_bar;
  GtkWidget *digit_selector;
  GreadLabel *label;
  GtkLabel *greeting_label;
  GtkLabel *score_label;
  GreadNumberEntry *number_entry;
  GtkButton *button_start;
  GtkButton *button_next;

  time_t time;
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
} GreadRandomNumberActivityProperty;

typedef enum {
  PROGRESS,
  TIMEOUT,
  N_TIMEOUT
} GreadRandomNumberActivityTimeout;

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static guint obj_timeout[N_TIMEOUT] = {0, };

G_DEFINE_TYPE (GreadRandomNumberActivity, gread_random_number_activity, ADW_TYPE_BIN)


static void
gread_random_number_activity_update_score(GreadRandomNumberActivity *self){
  gchar * str = malloc(20*sizeof(gchar));
  sprintf(str, "%s : %d/%d",gettext("Score"),self->result,self->try_number);
  gtk_label_set_text(self->score_label, str);
  g_free(str);
}

static void
gread_random_number_activity_update_random_seed(GreadRandomNumberActivity *self){
  srand((unsigned) time(&self->time));
}

static void
gread_random_number_activity_set_property(GObject *object, guint property_id,
                                   const GValue *value, GParamSpec *pspec){

  GreadRandomNumberActivity *self = GREAD_RANDOM_NUMBER_ACTIVITY(object);

  switch((GreadRandomNumberActivityProperty) property_id){

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
gread_random_number_activity_get_property(GObject *object, guint property_id,
                              GValue *value, GParamSpec *pspec){

  GreadRandomNumberActivity *self = GREAD_RANDOM_NUMBER_ACTIVITY(object);

  switch((GreadRandomNumberActivityProperty) property_id){

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
timeout(GreadRandomNumberActivity *self){
  gtk_widget_set_visible(GTK_WIDGET(self->label), false);
  gtk_widget_set_visible(GTK_WIDGET(self->number_entry), true);
  gtk_widget_grab_focus(GTK_WIDGET(self->number_entry));
  obj_timeout[TIMEOUT] = 0;
}

static gboolean
progress(GreadRandomNumberActivity *self){

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
start_cb(GreadRandomNumberActivity *self){

  if(!self->start){

    self->start = true;
    self->try_number = 1;

    gtk_widget_set_visible(GTK_WIDGET(self->progress_bar), true);
    gtk_widget_set_visible(GTK_WIDGET(self->score_label), true);
    gtk_widget_set_visible(GTK_WIDGET(self->button_next), true);
    gtk_widget_set_visible(GTK_WIDGET(self->greeting_label), false);

    gtk_widget_set_sensitive(GTK_WIDGET(self->button_next), false);
    gread_menu_lock(self->menu_button);
    gread_label_roll(self->label);
    gread_random_number_activity_update_score(self);

    gtk_button_set_label(self->button_start, gettext("Stop"));
    obj_timeout[PROGRESS] = g_timeout_add(self->prog_step, G_SOURCE_FUNC(progress), self);

  }else{

    self->start = false;
    gread_menu_unlock(self->menu_button);
    gtk_widget_set_visible(GTK_WIDGET(self->progress_bar), false);
    gtk_button_set_label(self->button_start, gettext("Start"));

    self->result = 0;
    self->progress = 0.0;
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(self->progress_bar), 0.0);
    gread_random_number_activity_update_random_seed(self);


    if(obj_timeout[PROGRESS]){
      g_source_remove(obj_timeout[PROGRESS]);
    }

    if(obj_timeout[TIMEOUT]){
      g_source_remove(obj_timeout[TIMEOUT]);
    }

    if(gtk_widget_is_visible(GTK_WIDGET(self->number_entry))){
      gtk_widget_set_visible(GTK_WIDGET(self->number_entry), false);
    }

    if(gtk_widget_is_visible(GTK_WIDGET(self->label))){
      gtk_widget_set_visible(GTK_WIDGET(self->label), false);
    }

    if(gtk_widget_is_visible(GTK_WIDGET(self->score_label))){
      gtk_widget_set_visible(GTK_WIDGET(self->score_label), false);
    }

    gread_number_entry_clear(self->number_entry);
    gtk_widget_remove_css_class(GTK_WIDGET(self->label), "correct");
    gtk_widget_remove_css_class(GTK_WIDGET(self->label), "wrong");
    gtk_widget_set_visible(GTK_WIDGET(self->greeting_label), true);
    gtk_widget_set_visible(GTK_WIDGET(self->button_next), false);
  }
}

static void
next_cb(GreadRandomNumberActivity *self){

  gread_number_entry_clear(self->number_entry);
  gtk_widget_remove_css_class(GTK_WIDGET(self->label), "correct");
  gtk_widget_remove_css_class(GTK_WIDGET (self->label), "wrong");
  gtk_widget_set_visible(GTK_WIDGET(self->number_entry), false);
  gtk_widget_set_visible(GTK_WIDGET(self->progress_bar), true);


  gread_label_roll(self->label);
  gtk_widget_set_visible(GTK_WIDGET(self->label), false);
  gtk_widget_set_visible(GTK_WIDGET(self->progress_bar), true);
  gtk_widget_set_sensitive(GTK_WIDGET(self->button_next), false);
  if(obj_timeout[TIMEOUT]){
    g_source_remove(obj_timeout[TIMEOUT]);
  }
  g_timeout_add(self->prog_step, G_SOURCE_FUNC(progress), self);
}

static void
enter_cb(GreadRandomNumberActivity *self){

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
      gread_random_number_activity_update_score(self);
      self->try_number += 1;
      gtk_widget_grab_focus(GTK_WIDGET(self->button_next));


    }else{

      gtk_widget_error_bell(GTK_WIDGET(self->number_entry));
    }

  }
}

void
gread_random_number_activity_set_digits(GreadRandomNumberActivity *self, guint digits){
  self->digits = digits;
  gread_label_set_digits(self->label, digits);
  gread_number_entry_set_digits(self->number_entry, digits);
}

void
gread_random_number_activity_set_display_time(GreadRandomNumberActivity *self, guint display_time){
  self->display_time = display_time;
}

static void
gread_random_number_activity_dispose(GObject *object){
  G_OBJECT_CLASS(gread_random_number_activity_parent_class)->dispose(object);
}

static void
gread_random_number_activity_finalize(GObject *object){
  G_OBJECT_CLASS(gread_random_number_activity_parent_class)->finalize(object);
}

static void
gread_random_number_activity_class_init(GreadRandomNumberActivityClass *klass){
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


  object_class->get_property = (GObjectGetPropertyFunc) gread_random_number_activity_get_property;
  object_class->set_property = (GObjectSetPropertyFunc) gread_random_number_activity_set_property;
  object_class->finalize = gread_random_number_activity_finalize;
  object_class->dispose = gread_random_number_activity_dispose;

  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass),
                                              "/org/gnome/gread/gui/gread-random-number-activity.ui");

  gtk_widget_class_bind_template_child(widget_class, GreadRandomNumberActivity, main_box);
  gtk_widget_class_bind_template_child(widget_class, GreadRandomNumberActivity, menu_button);
  gtk_widget_class_bind_template_child(widget_class, GreadRandomNumberActivity, content_box);
  gtk_widget_class_bind_template_child(widget_class, GreadRandomNumberActivity, progress_bar);
  gtk_widget_class_bind_template_child(widget_class, GreadRandomNumberActivity, top_box);
  gtk_widget_class_bind_template_child(widget_class, GreadRandomNumberActivity, bottom_box);
  gtk_widget_class_bind_template_child(widget_class, GreadRandomNumberActivity, label);
  gtk_widget_class_bind_template_child(widget_class, GreadRandomNumberActivity, greeting_label);
  gtk_widget_class_bind_template_child(widget_class, GreadRandomNumberActivity, score_label);
  gtk_widget_class_bind_template_child(widget_class, GreadRandomNumberActivity, number_entry);
  gtk_widget_class_bind_template_child(widget_class, GreadRandomNumberActivity, button_start);
  gtk_widget_class_bind_template_child(widget_class, GreadRandomNumberActivity, button_next);
}

static void
gread_random_number_activity_init(GreadRandomNumberActivity *self){
  g_type_ensure(GREAD_MENU_TYPE);
  g_type_ensure(GREAD_LABEL_TYPE);
  g_type_ensure(GREAD_NUMBER_ENTRY_TYPE);

  srand((unsigned) time(&self->time));
  self->start = false;
  self->try_number = 0;
  self->display_time = 500;
  self->prog_step = 175;
  self->progress = 0.0;
  self->digits = 2;
  self->result = 0;

  gtk_widget_init_template(GTK_WIDGET(self));
  gread_random_number_activity_update_score(self);


  g_signal_connect_swapped(self->number_entry, "invalid-char",
                           G_CALLBACK(gtk_widget_error_bell), self);

  g_signal_connect_swapped(self->number_entry, "limit-reached",
                           G_CALLBACK(gtk_widget_error_bell), self);

  g_signal_connect_swapped(self->button_start, "clicked", G_CALLBACK(start_cb), self);
  g_signal_connect_swapped(self->button_next, "clicked", G_CALLBACK(next_cb), self);
  g_signal_connect_swapped(self->number_entry, "enter-pressed", G_CALLBACK(enter_cb), self);
}

GreadRandomNumberActivity *
gread_random_number_activity_new(GreadApplication *app){
  return g_object_new(GREAD_RANDOM_NUMBER_ACTIVITY_TYPE, "application", app, NULL);
}
