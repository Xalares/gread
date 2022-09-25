#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS

#define GREAD_LABEL_TYPE (gread_label_get_type())

G_DECLARE_FINAL_TYPE (GreadLabel, gread_label, GREAD, LABEL, GtkWidget)

void gread_label_roll(GreadLabel *label);

G_END_DECLS
