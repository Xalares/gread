#include "gread-application.h"
#include "gread-window.h"
#include <gtk/gtk.h>

struct _GreadApplication {
  AdwApplication parent;
  GtkWidget *window;
};

G_DEFINE_TYPE (GreadApplication, gread_application, ADW_TYPE_APPLICATION)

static void
gread_application_activate (GApplication *app){
  GreadApplication *self;
  self = GREAD_APPLICATION(app);
  self->window = GTK_WIDGET(gread_app_window_new(self));
  gtk_window_present(GTK_WINDOW(self->window));
}

static void
gread_application_class_init(GreadApplicationClass *klass){
  G_APPLICATION_CLASS(klass)->activate = gread_application_activate;
}

static void
gread_application_init(GreadApplication *app){

}

GreadApplication * gread_application_new(void){
  return g_object_new(GREAD_APPLICATION_TYPE,
                      "application-id",
                      "org.gnome.gread",
                      NULL);
}
