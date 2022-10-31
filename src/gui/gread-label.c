#include <math.h>
#include <gtk/gtk.h>
#include <adwaita.h>
#include <glib/gi18n-lib.h>

#include "gread-label.h"

struct _GreadLabel {
  AdwBin parent;
  GtkLabel * label;
  guint value;
  guint digits;
};

typedef enum {
  PROP_DIGITS = 1,
  PROP_TEXT,
  N_PROPERTIES
} GreadLabelProperty;

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

G_DEFINE_TYPE (GreadLabel, gread_label, ADW_TYPE_BIN)

void
gread_label_roll(GreadLabel *self){
  guint zeros = (unsigned int) pow(10.0, self->digits-1);
  self->value = rand() % (9 * zeros) + zeros;
  char text[self->digits];
  sprintf(text, "%d", self->value);
  gtk_label_set_text(self->label, text);
}

const char *
gread_label_get_text(GreadLabel *self){
  return gtk_label_get_text(self->label);
}

guint
gread_label_get_value(GreadLabel *self){
  return self->value;
}

guint
gread_label_get_digits(GreadLabel *self){
  return self->digits;
}

void
gread_label_set_digits(GreadLabel *self, guint digits){
  self->digits = digits;
}

static void
gread_label_set_property(GObject *object, guint property_id,
                         const GValue *value, GParamSpec *pspec){

  GreadLabel *self = GREAD_LABEL(object);

  switch((GreadLabelProperty) property_id){
  case PROP_DIGITS:
    self->digits = g_value_get_uint(value);
    g_print("digits : %d\n", self->digits);
    break;

  case PROP_TEXT:
    gtk_label_set_text(self->label, g_value_dup_string(value));
    break;

  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    break;

  }
}

static void
gread_label_get_property(GObject *object, guint property_id,
                         GValue *value, GParamSpec *pspec){

  GreadLabel *self = GREAD_LABEL(object);

  switch((GreadLabelProperty) property_id){

  case PROP_DIGITS:
    g_value_set_uint(value, self->digits);
    break;

  case PROP_TEXT:
    g_value_set_string(value, gtk_label_get_text(self->label));
    break;

  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    break;

  }
}

static void
gread_label_dispose(GObject *object){
  GreadLabel *self;
  self = GREAD_LABEL(object);
  gtk_widget_unparent (GTK_WIDGET(self));
  G_OBJECT_CLASS(gread_label_parent_class)->dispose(object);
}

static void
gread_label_finalize(GObject *object){
  G_OBJECT_CLASS(gread_label_parent_class)->finalize (object);
}

static void
gread_label_class_init(GreadLabelClass *klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass *object_class = G_OBJECT_CLASS(klass);


  obj_properties[PROP_DIGITS] =
    g_param_spec_uint("digits",
                      "Digits",
                      "Number of digits for the random number to display.",
                      1,
                      10,
                      2,
                      G_PARAM_READWRITE);

  obj_properties[PROP_TEXT] =
    g_param_spec_string("text",
                        "Text",
                        "Default text to display.",
                        "Prêt ?",
                        G_PARAM_READWRITE);

  object_class->dispose = gread_label_dispose;
  object_class->finalize = gread_label_finalize;
  object_class->set_property = gread_label_set_property;
  object_class->get_property = gread_label_get_property;

  g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);

  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/org/gnome/gread/gui/gread-label.ui");
  gtk_widget_class_bind_template_child(widget_class, GreadLabel, label);
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

static void
gread_label_init(GreadLabel *self){
  self->digits = 2;
  gtk_widget_init_template(GTK_WIDGET(self));
  gtk_label_set_text(self->label, "Prêt ?");
}

GreadLabel *
gread_label_new(const char *text, const guint digits){
  return g_object_new(GREAD_LABEL_TYPE, "text", text, "digits", digits, NULL);
}
