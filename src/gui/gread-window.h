#pragma once
#include <gtk/gtk.h>
#include "gread-application.h"

G_BEGIN_DECLS

#define GREAD_APP_WINDOW_TYPE (gread_app_window_get_type ())

G_DECLARE_FINAL_TYPE(GreadAppWindow, gread_app_window, GREAD, APP_WINDOW, AdwApplicationWindow)


void gread_app_window_set_digits(GreadAppWindow *win, guint digits);

void gread_app_window_set_display_time(GreadAppWindow *win, guint display_time);

GreadAppWindow* gread_app_window_new(GreadApplication *app);

G_END_DECLS
