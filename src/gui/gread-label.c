#include <math.h>
#include <gtk/gtk.h>

#include "gread-label.h"

struct _GreadLabel {
  GtkWidget parent;
  GtkLabel * label;
  guint value;
  guint digits;
};

G_DEFINE_TYPE (GreadLabel, gread_label, GTK_TYPE_WIDGET)

void
gread_label_roll(GreadLabel *self){
  guint zeros = (unsigned int) pow(10.0,self->digits-1);
  self->value = rand() % 9 * (unsigned int) pow(10.0,zeros) + zeros;
}

static void
gread_label_finalize(GObject *object){
  GreadLabel *self = (GreadLabel *)object;
  g_clear_object(&self->label);
  G_OBJECT_CLASS(gread_label_parent_class)->finalize (object);
}

static void
gread_label_class_init(GreadLabelClass *klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->finalize = gread_label_finalize;
  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/org/gnome/gread/gui/gread-label.ui");
  gtk_widget_class_bind_template_child(widget_class, GreadLabel, label);
}

static void
gread_label_init(GreadLabel *self){
  gtk_widget_init_template(GTK_WIDGET(self));
}


/*GreadLabel *
gread_label_new (char * text){
  return g_object_new(GREAD_LABEL_TYPE, NULL);
  }*/
