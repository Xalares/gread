#include <gtk/gtk.h>
#include <adwaita.h>
#include <glib/gi18n-lib.h>

#include "gread-scrolled-window.h"
#include "gread-introduction.h"
#include "gread-text-view.h"

struct _GreadIntroduction
{
  GtkWidget parent;
  GtkScrolledWindow *scrolled_window;
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
  gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

static void
gread_introduction_init(GreadIntroduction *self){
  g_type_ensure(GREAD_TEXT_VIEW_TYPE);
  gtk_widget_init_template(GTK_WIDGET(self));
  gtk_widget_remove_css_class(GTK_WIDGET(self->text_view), "view");


  g_signal_connect_swapped(self->scrolled_window, "edge-reached",
                         G_CALLBACK(gread_intro_edge_reached_handler), self);
  g_signal_connect_swapped(self->ok_button, "clicked",
                           G_CALLBACK(gread_intro_ok_handler), self);
}
