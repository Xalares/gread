#include <gtk/gtk.h>
#include <adwaita.h>

#include "gread-number-entry.h"

struct _GreadNumberEntry {
  GtkWidget parent;
  GtkText *text;
  guint value;
  guint digits;
};

typedef enum {
  PROP_DIGITS = 1,
  PROP_VALUE,
  N_PROPERTIES
} GreadNumberEntryProperty;

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

G_DEFINE_TYPE (GreadNumberEntry, gread_number_entry, GTK_TYPE_WIDGET)

void
gread_number_entry_clear(GreadNumberEntry *self){
}

static void
digit_insert(GreadNumberEntry *self){
}

static void
gread_number_entry_get_property(GObject *object, guint property_id,
                                GValue *value, GParamSpec *pspec){

  if(gtk_editable_delegate_get_property(object, property_id, value, pspec))
    return;

  GreadNumberEntry *self = GREAD_NUMBER_ENTRY(object);

  switch((GreadNumberEntryProperty) property_id){

  case PROP_DIGITS:
    g_value_set_uint(value, self->digits);
    break;

  case PROP_VALUE:
    self->value = gtk_text_get_text_length(self->text);
    g_value_set_uint(value, self->value);
    break;

  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
  }

}

static void
gread_number_entry_set_property(GObject *object, guint property_id,
                                const GValue *value, GParamSpec *pspec){

  if(gtk_editable_delegate_set_property(object, property_id, value, pspec))
    return;

  GreadNumberEntry *self = GREAD_NUMBER_ENTRY(object);

  switch((GreadNumberEntryProperty) property_id){
  case PROP_DIGITS:
    self->digits = g_value_get_uint(value);
    g_print("GreadNumberEntry digits : %d", self->digits);
    break;

  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
  }
}

static void
gread_number_entry_dispose(GObject *object){
    GreadNumberEntry *self;
  self = GREAD_NUMBER_ENTRY(object);
  gtk_editable_finish_delegate(GTK_EDITABLE(self));
  GtkWidget *text = GTK_WIDGET(self->text);
  g_clear_pointer(&text, gtk_widget_unparent);
  G_OBJECT_CLASS(gread_number_entry_parent_class)->dispose(object);
}

static void
gread_number_entry_finalize(GObject *object){
  G_OBJECT_CLASS(gread_number_entry_parent_class)->finalize(object);
}

GtkEditable *
get_editable_delegate(GtkEditable *editable){
  return GTK_EDITABLE(GREAD_NUMBER_ENTRY(editable)->text);
}

static void
gread_number_entry_editable_init(GtkEditableInterface *iface){
  iface->get_delegate = get_editable_delegate;
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
    g_param_spec_ulong("value",
                      "Value",
                      "Value of the number written.",
                      0,
                      9999999999,
                      10,
                      G_PARAM_READABLE);

  object_class->dispose = gread_number_entry_dispose;
  object_class->finalize = gread_number_entry_finalize;
  object_class->set_property = gread_number_entry_set_property;
  object_class->get_property = gread_number_entry_get_property;

  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
  gtk_editable_install_properties(object_class, N_PROPERTIES);

  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/org/gnome/gread/gui/gread-number-entry.ui");
  gtk_widget_class_bind_template_child(widget_class, GreadNumberEntry, text);
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

static void
gread_number_entry_init(GreadNumberEntry *self){
  gtk_widget_init_template(GTK_WIDGET(self));
  gtk_editable_init_delegate(GTK_EDITABLE(self));
  //g_signal_connect_swapped(self->text, "insert-text", G_CALLBACK(digit_insert), self);
}
