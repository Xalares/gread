#include <gtk/gtk.h>
#include <adwaita.h>
#include <glib/gi18n-lib.h>

#include "gread-introduction.h"

struct _GreadIntroduction
{
  GtkWidget parent;
  GtkScrolledWindow *scrolled_window;
  GtkOverlay *overlay;
  GtkWidget *box;
  GtkScrollbar *scrollbar;
  GtkAdjustment *adjustment;
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
gread_intro_vadjustment(GreadIntroduction *self){
  double upper = gtk_adjustment_get_upper(self->adjustment);
  double lower = gtk_adjustment_get_lower(self->adjustment);
  double page_size = gtk_adjustment_get_page_size(self->adjustment);
  double value = gtk_adjustment_get_value(self->adjustment);
  if(0>=(upper-page_size)){
    g_print("Max-content width: %d\n", gtk_scrolled_window_get_max_content_width(self->scrolled_window));
    g_print("Adjustment %f, upper: %f, value: %f, page_size: %f, lower:%f\n",
            upper-page_size,upper, value, page_size, lower);
  }
}

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

static void
gread_intro_dispose(GObject *object){
  GreadIntroduction *self = GREAD_INTRODUCTION(object);
  gtk_widget_unparent(GTK_WIDGET(self->text_view));
  gtk_widget_unparent(GTK_WIDGET(self->scrolled_window));
  gtk_widget_unparent(GTK_WIDGET(self->box));
  gtk_widget_unparent(GTK_WIDGET(self->overlay));
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
  gtk_widget_class_bind_template_child(widget_class, GreadIntroduction, box);
  gtk_widget_class_bind_template_child(widget_class, GreadIntroduction, overlay);
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

static void
gread_introduction_init(GreadIntroduction *self){
  gtk_widget_init_template(GTK_WIDGET(self));
  gtk_widget_remove_css_class(GTK_WIDGET(self->text_view), "view");
  self->adjustment = gtk_scrolled_window_get_vadjustment(self->scrolled_window);

  g_signal_connect_swapped(self->adjustment, "changed",
                           G_CALLBACK(gread_intro_vadjustment), self);

  g_signal_connect_swapped(self->scrolled_window, "edge-reached",
                         G_CALLBACK(gread_intro_edge_reached_handler), self);

  g_signal_connect_swapped(self->ok_button, "clicked",
                           G_CALLBACK(gread_intro_ok_handler), self);
}
