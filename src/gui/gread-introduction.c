#include <gtk/gtk.h>
#include <adwaita.h>
#include <glib/gi18n-lib.h>

#include "gread-introduction.h"

struct _GreadIntroduction
{
  AdwBin parent;
  GtkTextView *text_view;
}

G_DEFINE_TYPE (GreadIntroduction, gread_introduction, ADW_TYPE_BIN)
