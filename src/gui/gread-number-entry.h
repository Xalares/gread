#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS

#define GREAD_NUMBER_ENTRY_TYPE (gread_label_get_type())

G_DECLARE_FINAL_TYPE (GreadNumberEntry, gread_number_entry, GREAD, NUMBER_ENTRY, GtkWidget)

G_END_DECLS
