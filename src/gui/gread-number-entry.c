#include <gtk/gtk.h>
#include <adwaita.h>

#include "gread-number-entry.h"

struct _GreadNumberEntry {
  AdwBin parent;
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
gread_number_entry_dispose(GObject *object){
    GreadNumberEntry *self;
  self = GREAD_NUMBER_ENTRY(object);
  gtk_widget_unparent (GTK_WIDGET (self));
  G_OBJECT_CLASS(gread_number_entry_parent_class)->dispose(object);
}

static void
gread_number_entry_finalize(GObject *object){
  G_OBJECT_CLASS(gread_number_entry_parent_class)->finalize(object);
}

static void
gread_number_entry_class_init(GreadNumberEntryClass *klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->dispose = gread_number_entry_dispose;
  object_class->finalize = gread_number_entry_finalize;

  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/org/gnome/gread/gui/gread-number-entry.ui");
  gtk_widget_class_bind_template_child(widget_class, GreadNumberEntry, entry);
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

static void
gread_number_entry_init(GreadNumberEntry *self){
  //g_signal_connect_swapped(self->entry, "insert-text", G_CALLBACK(digit_insert), NULL);
  gtk_widget_init_template(GTK_WIDGET(self));
}
