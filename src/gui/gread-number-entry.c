#include <gtk/gtk.h>
#include <adwaita.h>

#include "gread-number-entry.h"

struct _GreadNumberEntry {
  AdwBin parent;
  GtkEntry *entry;
  GtkEntryBuffer *entry_buffer;
  guint value;
};

G_DEFINE_TYPE (GreadNumberEntry, gread_number_entry, ADW_TYPE_BIN)

void
gread_number_entry_clear(GreadNumberEntry *self){
  gtk_entry_buffer_delete_text(self->entry_buffer, 0, -1);
}

static void
digit_insert(GreadNumberEntry *self){
  gtk_entry_buffer_set_text(self->entry_buffer, "y", 1);
  gtk_entry_set_buffer(self->entry_buffer, self->entry_buffer);
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
  gtk_widget_init_template(GTK_WIDGET(self));
  self->entry_buffer = gtk_entry_get_buffer(self->entry);
  g_signal_connect_swapped(self->entry, "insert-text", G_CALLBACK(digit_insert), self);
}
