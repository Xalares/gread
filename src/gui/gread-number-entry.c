#include <gtk/gtk.h>
#include <adwaita.h>

#include "gread-number-entry.h"

struct _GreadNumberEntry {
  AdwBin parent;
  GtkText *text;
  guint value;
  guint digits;
};

typedef enum {
  PROP_DIGITS = 1,
  PROP_VALUE,
  N_PROPERTIES
} GreadNumberEntryProperty;

G_DEFINE_TYPE (GreadNumberEntry, gread_number_entry, ADW_TYPE_BIN)

void
gread_number_entry_clear(GreadNumberEntry *self){
}

static void
digit_insert(GreadNumberEntry *self){
}

static void
gread_number_entry_get_property(GObject *object, guint property_id,
                                GValue *value, GParamSpec *pspec){

  GreadNumberEntry *self = GREAD_NUMBER_ENTRY(object);

  switch((GreadNumberEntryProperty) property_id){

  case PROP_DIGITS:
    g_value_set_uint(value, self->digits);
    break;

  case PROP_VALUE:
    gtk_text_get_text_length(self->text);
    g_value_set_uint(value, self->value);
    break;

  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
  }

}

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

  obj_properties[PROP_DIGITS] =
    g_param_spec_uint("digits",
                      "Digits",
                      "Number of digits needed to be entered.",
                      1,
                      10,
                      2,
                      G_PARAM_READWRITE);

  obj_properties[PROP_VALUE] =
    g_param_spec_uint("value",
                      "Value",
                      "Value of the number written.",
                      0,
                      G_PARAM_READABLE);

  object_class->dispose = gread_number_entry_dispose;
  object_class->finalize = gread_number_entry_finalize;

  g_object_class_install_properties(GObjectClass *oclass, guint n_pspecs, GParamSpec **pspecs)
  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/org/gnome/gread/gui/gread-number-entry.ui");
  gtk_widget_class_bind_template_child(widget_class, GreadNumberEntry, text);
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

static void
gread_number_entry_init(GreadNumberEntry *self){
  gtk_widget_init_template(GTK_WIDGET(self));
  g_signal_connect_swapped(self->entry, "insert-text", G_CALLBACK(digit_insert), self);
}
