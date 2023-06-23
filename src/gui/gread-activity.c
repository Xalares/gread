#include <gtk/gtk.h>
#include <adwaita.h>
#include <glib/gi18n-lib.h>

#include "gread-activity.h"
#include "gread-random-number-activity.h"

struct _GreadActivity{
  GtkWidget parent;
  GreadRandomNumberActivity *random_number_activity;
};

G_DEFINE_TYPE (GreadActivity, gread_activity, GTK_TYPE_WIDGET)


static void
gread_activity_dispose(GObject *object)
{
  GreadActivity *self;
  self = GREAD_ACTIVITY(object);
  gtk_widget_unparent(GTK_WIDGET(self->random_number_activity));
  G_OBJECT_CLASS(gread_activity_parent_class)->dispose(object);
}

static void
gread_activity_finalize(GObject *object)
{
  G_OBJECT_CLASS(gread_activity_parent_class)->finalize(object);
}

static void
gread_activity_class_init(GreadActivityClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->dispose = gread_activity_dispose;
  object_class->finalize = gread_activity_finalize;
  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/org/gnome/gread/gui/gread-activity.ui");
  gtk_widget_class_bind_template_child(widget_class, GreadActivity, random_number_activity);
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

static void
gread_activity_init(GreadActivity *self)
{
  g_type_ensure(GREAD_RANDOM_NUMBER_ACTIVITY_TYPE);
  gtk_widget_init_template(GTK_WIDGET(self));
}
