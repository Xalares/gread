#include <gtk/gtk.h>
#include <adwaita.h>
#include <glib/gi18n-lib.h>

#include "gread-activity-choice.h"
#include "gread-random-number-activity.h"

struct _GreadActivityChoice{
  GtkWidget parent;
  GtkWidget *activity_box;
  GreadRandomNumberActivity *random_number_activity;
  GtkWidget *focus_activity_button;
  GtkWidget *random_number_activity_button;
};

G_DEFINE_TYPE (GreadActivityChoice, gread_activity_choice, GTK_TYPE_WIDGET)

//Calbacks

static void
random_number_activity_cb(GreadActivityChoice *self)
{
  gtk_widget_set_visible(self->activity_box, false);
  gtk_widget_set_visible(GTK_WIDGET(self->random_number_activity), true);

}
//Class functions
static void
gread_activity_choice_dispose(GObject *object)
{
  GreadActivityChoice *self;
  self = GREAD_ACTIVITY_CHOICE(object);

  gtk_widget_unparent(GTK_WIDGET(self->focus_activity_button));
  gtk_widget_unparent(GTK_WIDGET(self->random_number_activity_button));
  gtk_widget_unparent(GTK_WIDGET(self->random_number_activity));
  gtk_widget_unparent(GTK_WIDGET(self->activity_box));

  G_OBJECT_CLASS(gread_activity_choice_parent_class)->dispose(object);
}

static void
gread_activity_choice_finalize(GObject *object)
{
  G_OBJECT_CLASS(gread_activity_choice_parent_class)->finalize(object);
}

static void
gread_activity_choice_class_init(GreadActivityChoiceClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->dispose = gread_activity_choice_dispose;
  object_class->finalize = gread_activity_choice_finalize;

  gtk_widget_class_set_template_from_resource(widget_class,
                                           "/org/gnome/gread/gui/gread-activity-choice.ui");
  gtk_widget_class_bind_template_child(widget_class, GreadActivityChoice, activity_box);
  gtk_widget_class_bind_template_child(widget_class, GreadActivityChoice, random_number_activity);
  gtk_widget_class_bind_template_child(widget_class, GreadActivityChoice, focus_activity_button);
  gtk_widget_class_bind_template_child(widget_class, GreadActivityChoice, random_number_activity_button);
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

static void
gread_activity_choice_init(GreadActivityChoice *self)
{
  g_type_ensure(GREAD_RANDOM_NUMBER_ACTIVITY_TYPE);
  gtk_widget_init_template(GTK_WIDGET(self));

  g_signal_connect_swapped(self->random_number_activity_button, "clicked", G_CALLBACK(random_number_activity_cb), self);

}
