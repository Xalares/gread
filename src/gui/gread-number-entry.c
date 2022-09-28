#include <gtk/gtk.h>
#include <adwaita.h>

#include "gread-number-entry.h"

struct _GreadNumberEntry {
  GtkEditable parent;
}

G_DEFINE_TYPE (GreadNumberEntry, gread_number_entry, ADW_TYPE_BIN)
