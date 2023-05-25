#include <gtk/gtk.h>
#include <adwaita.h>
#include <glib/gi18n-lib.h>

#include "gread-activity-choice.h"

struct _GreadActivityChoice{
  GtkWidget parent;
};

static void
gread_activity_choice_class_init(GreadActivityChoiceClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  gtk_widget_class_set_layout_manager_type(widget_class,
                                           "/org/gnome/gread/gui/gread-activity-choice.ui");
}

static void
gread_activity_choice_init(GreadActivityChoice *self)
{

}
