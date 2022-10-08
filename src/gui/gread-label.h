#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS

#define GREAD_LABEL_TYPE (gread_label_get_type())

G_DECLARE_FINAL_TYPE (GreadLabel, gread_label, GREAD, LABEL, GtkWidget)

void gread_label_roll(GreadLabel *label);

void gread_label_set_text(GreadLabel *label, char *str);

const char * gread_label_get_text(GreadLabel *label);

guint gread_label_get_value(GreadLabel *label);

guint gread_label_get_digits(GreadLabel *label);

void gread_label_set_digits(GreadLabel *label, guint digits);

G_END_DECLS
