#include <glib/gi18n.h>
#include <locale.h>

#include "config.h"
#include "gread-application.h"

gint main (gint argc, gchar **argv){

  setlocale (LC_ALL, "");
  bindtextdomain (GETTEXT_PACKAGE, DATADIR "/locale");
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);

  GreadApplication * app = gread_application_new ();

  return g_application_run(G_APPLICATION(app), argc, argv);
}
