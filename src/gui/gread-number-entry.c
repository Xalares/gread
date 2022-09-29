#include <gtk/gtk.h>
#include <adwaita.h>

#include "gread-number-entry.h"

struct _GreadNumberEntry {
  GtkWidget parent;
  GtkEntry *entry;
  guint value;
};

G_DEFINE_TYPE (GreadNumberEntry, gread_number_entry, ADW_TYPE_BIN)

static void
digit_insert(GtkEditable *edit, gchar *new_text,
             gint new_length, gpointer position, gpointer data){

  (void)new_text; (void)new_length; (void)position; (void)data;

  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(edit));
  gchar *content = gtk_entry_buffer_get_text(buffer);
  gtk_entry_buffer_set_text(buffer, "y", -1);
  gtk_entry_set_buffer(GTK_ENTRY(edit), buffer);
  g_free(content);
}

/*static void
digit_delete(GtkEditable *edit, gint start_pos,
             gint end_pos, gpointer data){

             }*/

static void
gread_number_entry_class_init(GreadNumberEntryClass *klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
}

static void
gread_number_entry_init(GreadNumberEntry *self){
  g_signal_connect(self->entry, "insert-text", G_CALLBACK(digit_insert), NULL);
}
