#include <gtk/gtk.h>
#include <adwaita.h>

#include "gread-number-entry.h"

struct _GreadNumberEntry {
  GtkWidget parent;
  GtkEntry *entry
  guint value;
};

G_DEFINE_TYPE (GreadNumberEntry, gread_number_entry, ADW_TYPE_BIN)

static void
gread_number_entry_class_init(GreadNumberEntry *klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
}

static void
gread_number_entry_init(GreadNumberEntry *self){

}
