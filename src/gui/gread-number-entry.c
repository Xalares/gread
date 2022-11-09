#include <gtk/gtk.h>
#include <glib/gi18n-lib.h>
#include <adwaita.h>
#include <string.h>

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

typedef enum {
  SIG_LIMIT_REACHED,
  SIG_INVALID_CHAR,
  ENTER_PRESSED,
  N_SIGNAL
} GreadNumberEntrySignal;

static guint obj_signal[N_SIGNAL] = {0, };
static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };
static void gread_number_entry_editable_init (GtkEditableInterface *iface);

G_DEFINE_TYPE_WITH_CODE (GreadNumberEntry, gread_number_entry, GTK_TYPE_WIDGET,
                         G_IMPLEMENT_INTERFACE(GTK_TYPE_EDITABLE, gread_number_entry_editable_init))

void
gread_number_entry_clear(GreadNumberEntry *self){
  guint16 digits = gtk_text_get_text_length(self->text);
  self->value = 0;
  gtk_editable_delete_text(GTK_EDITABLE(self->text), 0, digits);
}

guint
gread_number_entry_get_value(GreadNumberEntry *self){
  return self->value;
}

guint
gread_number_entry_set_digits(GreadNumberEntry *self, guint digits){
  gtk_editable_set_width_chars(self, digits);
  self->digits = digits;
}

guint
gread_number_entry_get_digits(GreadNumberEntry *self){
  return self->digits;
}

guint
gread_number_entry_digits_typed(GreadNumberEntry *self){
  return gtk_text_get_text_length(self->text);
}

static void
grab_focus(GreadNumberEntry *self){
  gtk_widget_grab_focus(GTK_WIDGET(self->text));
}

static void
insert_text_handler(GtkEditable *editable, const char *text, int length,
                    int *position, gpointer data){

  g_signal_handlers_block_by_func(editable, (gpointer)insert_text_handler, data);

  guint text_length = strlen(gtk_editable_get_text(editable));
  char *invalid_char = NULL;
  unsigned long value = 0;
  GreadNumberEntry *self = GREAD_NUMBER_ENTRY(data);

  if(text_length < GREAD_NUMBER_ENTRY(data)->digits){
    value = strtoul(text, &invalid_char, 10);

    if(*invalid_char == '\0'){
      gtk_editable_insert_text(editable, text, length, position);
      self->value = strtoul(gtk_editable_get_text(editable), NULL, 10);

    }else{
      *invalid_char = 0;
      g_signal_emit(self, obj_signal[SIG_INVALID_CHAR], 0);

    }
  }
  if(text_length == self->digits){
    g_signal_emit(self, obj_signal[SIG_LIMIT_REACHED], 0);
  }

  g_signal_handlers_unblock_by_func(editable, (gpointer)insert_text_handler, data);

  g_signal_stop_emission_by_name(editable, "insert_text");
}

static void
delete_text_handler(GtkEditable *editable, int start_pos, int end_pos, gpointer data){

  g_signal_handlers_block_by_func(editable, (gpointer)delete_text_handler, data);

  GreadNumberEntry *self = GREAD_NUMBER_ENTRY(data);
  gtk_editable_delete_text(editable, start_pos, end_pos);
  self->value = (guint) strtoul(gtk_editable_get_text(editable), NULL, 10);

  g_signal_handlers_unblock_by_func(editable, (gpointer)delete_text_handler, data);
  g_signal_stop_emission_by_name(editable, "delete_text");
}

static gboolean
key_pressed_handler(GtkEventControllerKey *controller, guint keyval,
                    guint keycode, GdkModifierType state, GreadNumberEntry *self){
  if(keyval == GDK_KEY_Return){
    g_signal_emit(self, obj_signal[ENTER_PRESSED], 0);
    return true;
  }
  return false;
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
  gtk_widget_unparent(GTK_WIDGET(self));
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

  obj_signal[SIG_LIMIT_REACHED] =
    g_signal_newv("limit-reached",
                G_TYPE_FROM_CLASS(object_class),
                G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                NULL,
                NULL,
                NULL,
                NULL,
                G_TYPE_NONE,
                0,
                NULL);

  obj_signal[SIG_INVALID_CHAR] =
    g_signal_newv("invalid-char",
                  G_TYPE_FROM_CLASS(object_class),
                  G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                  NULL,
                  NULL,
                  NULL,
                  NULL,
                  G_TYPE_NONE,
                  0,
                  NULL);

  obj_signal[ENTER_PRESSED] =
    g_signal_newv("enter-pressed",
                  G_TYPE_FROM_CLASS(object_class),
                  G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                  NULL,
                  NULL,
                  NULL,
                  NULL,
                  G_TYPE_NONE,
                  0,
                  NULL);

  widget_class->grab_focus = grab_focus;

  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
  gtk_editable_install_properties(object_class, N_PROPERTIES);

  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/org/gnome/gread/gui/gread-number-entry.ui");
  gtk_widget_class_bind_template_child(widget_class, GreadNumberEntry, text);
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

static void
gread_number_entry_init(GreadNumberEntry *self){
  GtkEventControllerKey *controller_key;
  gtk_widget_init_template(GTK_WIDGET(self));
  gtk_editable_init_delegate(GTK_EDITABLE(self));
  self->digits = 2;
  controller_key = gtk_event_controller_key_new();

  g_signal_connect(controller_key, "key-pressed", G_CALLBACK(key_pressed_handler), self);
  g_signal_connect(self->text, "insert-text", G_CALLBACK(insert_text_handler), self);
  g_signal_connect(self->text, "delete-text", G_CALLBACK(delete_text_handler), self);
  gtk_widget_add_controller(GTK_WIDGET(self->text), controller_key);
}
