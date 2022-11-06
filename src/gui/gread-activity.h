#pragma once
#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GREAD_ACTIVITY_TYPE (gread_activity_get_type())

G_DECLARE_DERIVABLE_TYPE (GreadActivity, gread_activity, GREAD, ACTIVITY, GtkWidget)

struct _GreadActivityClass
{
  GtkWidgetClass parent_class;
  gboolean has_menu;
}

G_END_DECLS
