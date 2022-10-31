#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS

#define GREAD_NUMBER_ENTRY_TYPE (gread_number_entry_get_type())

G_DECLARE_FINAL_TYPE (GreadNumberEntry, gread_number_entry, GREAD, NUMBER_ENTRY, GtkWidget)

void gread_number_entry_clear(GreadNumberEntry *self);

guint gread_number_entry_get_value(GreadNumberEntry *self);

guint gread_number_entry_set_digits(GreadNumberEntry *self, guint digits);

guint gread_number_entry_get_digits(GreadNumberEntry *self);

guint gread_number_entry_digits_typed(GreadNumberEntry *self);

G_END_DECLS
