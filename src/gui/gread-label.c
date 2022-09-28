#include <math.h>
#include <gtk/gtk.h>
#include <adwaita.h>

#include "gread-label.h"

struct _GreadLabel {
  AdwBin parent;
  GtkLabel * label;
  guint value;
  guint digits;
};

G_DEFINE_TYPE (GreadLabel, gread_label, ADW_TYPE_BIN)

void
gread_label_roll(GreadLabel *self){
  guint zeros = (unsigned int) pow(10.0,self->digits-1);
  self->value = rand() % (unsigned int) (9 * zeros) + zeros;
  char text[self->digits];
  sprintf(text, "%d", self->value);
  gtk_label_set_text(self->label, text);
}

static void
gread_label_dispose(GObject *object){
  GreadLabel *self;
  self = GREAD_LABEL(object);
  gtk_widget_unparent(GTK_WIDGET(self->label));
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

  object_class->dispose = gread_label_dispose;
  object_class->finalize = gread_label_finalize;
  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/org/gnome/gread/gui/gread-label.ui");
  gtk_widget_class_bind_template_child(widget_class, GreadLabel, label);
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

static void
gread_label_init(GreadLabel *self){
  self->digits = 1;
  gtk_widget_init_template(GTK_WIDGET(self));
}
