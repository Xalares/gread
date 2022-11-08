#include <gtk/gtk.h>
#include <adwaita.h>
#include <glib/gi18n-lib.h>

#include "gread-introduction.h"

struct _GreadIntroduction
{
  AdwBin parent;
  GtkTextView *text_view;
  GtkButton *start_button;
};

G_DEFINE_TYPE (GreadIntroduction, gread_introduction, ADW_TYPE_BIN)

static void
gread_introduction_class_init(GreadIntroductionClass *klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/org/gnome/gread/gui/gread-introduction.ui");

  gtk_widget_class_bind_template_child(widget_class, GreadIntroduction, text_view);
  gtk_widget_class_bind_template_child(widget_class, GreadIntroduction, start_button);

  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

static void
gread_introduction_init(GreadIntroduction *self){
  gtk_widget_init_template(GTK_WIDGET(self));
}
