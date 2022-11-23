#include <gtk/gtk.h>
#include <adwaita.h>
#include <glib/gi18n-lib.h>

#include "gread-introduction.h"

struct _GreadIntroduction
{
  GtkWidget parent;
  GtkScrolledWindow *scrolled_window;
  GtkTextView *text_view;
  GtkButton *ok_button;
};

typedef enum {
  BOTTOM_REACHED,
  OK_PRESSED,
  N_SIGNAL
} GreadIntroductionSignal;


static guint obj_signal[N_SIGNAL] = {0, };
G_DEFINE_TYPE (GreadIntroduction, gread_introduction, GTK_TYPE_WIDGET)


static void
gread_intro_ok_handler (GreadIntroduction *self){
  g_signal_emit(self, obj_signal[OK_PRESSED], 0);
}

static void
gread_intro_edge_reached_handler(GreadIntroduction *self, GtkPositionType pos){
  if(pos == GTK_POS_BOTTOM){
    gtk_widget_set_sensitive(GTK_WIDGET(self->ok_button), true);
  }
}

static GtkSizeRequestMode
gread_introduction_request_mode(GtkWidget *widget){
  g_print("Request\n");
  return GTK_SIZE_REQUEST_HEIGHT_FOR_WIDTH;
}

static void
gread_introduction_measure(GtkWidget *widget,
                    GtkOrientation orientation,
                    int for_size,
                    int *minimum,
                    int *natural,
                    int *minimum_baseline,
                    int *natural_baseline){

  GtkWidget *child;

  for (child = gtk_widget_get_first_child(widget);
       child != NULL;
       child = gtk_widget_get_next_sibling(child)){

    if(gtk_widget_should_layout(child))
      {
        g_print("Measure\n");

        int child_min = 0;
        int child_nat = 0;
        int child_min_baseline = -1;
        int child_nat_baseline = -1;

        gtk_widget_measure(child, orientation, for_size,
                           &child_min, &child_nat,
                           &child_min_baseline, &child_nat_baseline);

        *minimum = MAX(*minimum, child_min);
        *natural = MAX(*natural, child_nat);

        if (child_min_baseline > -1)
          *minimum_baseline = MAX(*minimum_baseline, child_min_baseline);

        if (child_nat_baseline > -1)
          *natural_baseline = MAX(*natural_baseline, child_nat_baseline);

      }
  }

}

static void
gread_introduction_allocate(GtkWidget *widget, int width,
                            int height, int baseline){
  GtkWidget *child;

  for (child = gtk_widget_get_first_child(widget);
       child != NULL;
       child = gtk_widget_get_next_sibling(child))
    {

      if (child && gtk_widget_should_layout(child)){
        g_print("%s width:%d, height:%d\n", G_OBJECT_TYPE_NAME(child), width, height);
        gtk_widget_allocate(child, width, height, baseline, NULL);
        g_print("Allocate\n");
      }
    }
}

static void
gread_intro_dispose(GObject *object){
  GreadIntroduction *self = GREAD_INTRODUCTION(object);
  gtk_widget_unparent(GTK_WIDGET(self->text_view));
  gtk_widget_unparent(GTK_WIDGET(self->scrolled_window));
  gtk_widget_unparent(GTK_WIDGET(self));

  G_OBJECT_CLASS(gread_introduction_parent_class)->dispose(object);
}

static void
gread_intro_finalize(GObject *object){
  G_OBJECT_CLASS(gread_introduction_parent_class)->finalize(object);
}

static void
gread_introduction_class_init(GreadIntroductionClass *klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  widget_class->get_request_mode = gread_introduction_request_mode;
  widget_class->measure = gread_introduction_measure;
  widget_class->size_allocate = gread_introduction_allocate;

  object_class->dispose = gread_intro_dispose;
  object_class->finalize = (GObjectFinalizeFunc) gread_intro_finalize;

  obj_signal[BOTTOM_REACHED] =
    g_signal_newv("bottom-reached",
                  G_TYPE_FROM_CLASS(object_class),
                  G_SIGNAL_RUN_LAST|G_SIGNAL_NO_RECURSE|G_SIGNAL_NO_HOOKS,
                  NULL,
                  NULL,
                  NULL,
                  NULL,
                  G_TYPE_NONE,
                  0,
                  NULL);

  obj_signal[OK_PRESSED] =
    g_signal_newv("ok-pressed",
                  G_TYPE_FROM_CLASS(object_class),
                  G_SIGNAL_RUN_LAST|G_SIGNAL_NO_RECURSE|G_SIGNAL_NO_HOOKS,
                  NULL,
                  NULL,
                  NULL,
                  NULL,
                  G_TYPE_NONE,
                  0,
                  NULL);

  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/org/gnome/gread/gui/gread-introduction.ui");

  gtk_widget_class_bind_template_child(widget_class, GreadIntroduction, text_view);
  gtk_widget_class_bind_template_child(widget_class, GreadIntroduction, ok_button);
  gtk_widget_class_bind_template_child(widget_class, GreadIntroduction, scrolled_window);
}

static void
gread_introduction_init(GreadIntroduction *self){
  gtk_widget_init_template(GTK_WIDGET(self));
  gtk_widget_remove_css_class(GTK_WIDGET(self->text_view), "view");


  g_signal_connect_swapped(self->scrolled_window, "edge-reached",
                           G_CALLBACK(gread_intro_edge_reached_handler), self);
  g_signal_connect_swapped(self->ok_button, "clicked",
                           G_CALLBACK(gread_intro_ok_handler), self);
}
