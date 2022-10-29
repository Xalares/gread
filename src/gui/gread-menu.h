#pragma once
#include <gtk/gtk.h>
#include "gread-window.h"

G_BEGIN_DECLS

#define GREAD_MENU_TYPE (gread_menu_get_type ())


G_DECLARE_FINAL_TYPE (GreadMenu, gread_menu, GREAD, MENU, AdwBin)

void gread_menu_set_window(GreadMenu *self, GreadAppWindow *win);

void gread_menu_lock(GreadMenu *self);

void gread_menu_unlock(GreadMenu *self);

gboolean gread_menu_is_locked(GreadMenu *self);

GreadMenu * gread_menu_new(GreadAppWindow *win);

G_END_DECLS
