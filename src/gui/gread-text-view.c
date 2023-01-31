#include <gtk/gtk.h>
#include <adwaita.h>

#include "gread-text-view.h"

struct _GreadTextView {
  GtkWidget parent;
  GtkWidget *text_view;
};

G_DEFINE_TYPE (GreadTextView, gread_text_view, GTK_TYPE_WIDGET)

/*GtkSizeRequestMode
gread_get_request_mode(GtkWidget *widget){

}*/

static void
gread_text_view_measure(GtkWidget *widget,
                              GtkOrientation orientation,
                              int for_size,
                              int *minimum,
                              int *natural,
                              int *minimum_baseline,
                              int *natural_baseline){

  GreadTextView *self = GREAD_TEXT_VIEW(widget);
  GtkWidget *child = self->text_view;


  if(gtk_widget_should_layout(child))
  {

    int child_min = 0;
    int child_nat = 0;
    int child_min_baseline = -1;
    int child_nat_baseline = -1;

    gtk_widget_measure(child, orientation, for_size,
                       &child_min, &child_nat,
                       &child_min_baseline, &child_nat_baseline);
    *minimum = MAX(*minimum, child_min);
    *natural = MAX(*natural, child_nat);
    g_print("Mesure widget %s %d\n", G_OBJECT_CLASS_NAME(G_OBJECT_GET_CLASS(self)), *natural);

    if (child_min_baseline > -1)
      *minimum_baseline = MAX(*minimum_baseline, child_min_baseline);

    if (child_nat_baseline > -1)
      *natural_baseline = MAX(*natural_baseline, child_nat_baseline);
  }
}

static void
gread_text_view_allocate(GtkWidget *widget, int width,
                               int height, int baseline){
  GreadTextView *self = GREAD_TEXT_VIEW(widget);
  GtkWidget *child = self->text_view;
  g_print("Mesure widget %s %d\n", G_OBJECT_CLASS_NAME(G_OBJECT_GET_CLASS(self)), height);

  if(child && gtk_widget_should_layout(child)){
    gtk_widget_allocate(child, width, height, baseline, NULL);
  }
}

static void
gread_text_view_class_init(GreadTextViewClass *klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass   *object_class = G_OBJECT_CLASS(klass);

  widget_class->size_allocate = gread_text_view_allocate;
  widget_class->measure = gread_text_view_measure;

  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/org/gnome/gread/gui/gread-text-view.ui");
  gtk_widget_class_bind_template_child(widget_class, GreadTextView, text_view);

}


static void
gread_text_view_init(GreadTextView *self){
  gtk_widget_init_template(GTK_WIDGET(self));
}
