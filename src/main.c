#include "gread-application.h"
#include <glib/gi18n.h>

gint main (gint argc, gchar **argv){
  GreadApplication * app = gread_application_new ();
  return g_application_run(G_APPLICATION(app), argc, argv);
}
