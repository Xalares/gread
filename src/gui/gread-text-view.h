#pragma once
#include <gtk/gtk.h>
G_BEGIN_DECLS

#define GREAD_TEXT_VIEW_TYPE (gread_text_view_get_type ())

G_DECLARE_FINAL_TYPE (GreadTextView, gread_text_view, GREAD, TEXT_VIEW, GtkWidget)

G_END_DECLS
