#include <gtk/gtk.h>
#include <adwaita.h>
#include <glib/gi18n-lib.h>

#include "gread-activity-choice.h"

struct _GreadActivityChoice{
  GtkWidget parent;
  GtkWidget *activity_box;
};

G_DEFINE_TYPE (GreadActivityChoice, gread_activity_choice, GTK_TYPE_WIDGET)

static void
gread_activity_choice_class_init(GreadActivityChoiceClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  gtk_widget_class_set_template_from_resource(widget_class,
                                           "/org/gnome/gread/gui/gread-activity-choice.ui");
  gtk_widget_class_bind_template_child(widget_class, GreadActivityChoice, activity_box);
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

static void
gread_activity_choice_init(GreadActivityChoice *self)
{
  gtk_widget_init_template(GTK_WIDGET(self));
}
