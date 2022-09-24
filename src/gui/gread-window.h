#pragma once

#include "gread-application.h"

G_BEGIN_DECLS

#define GREAD_TYPE_WINDOW(gread_window_get_type())

G_DECLARE_FINAL_TYPE(GreadWindow, gread_window, GREAD, WINDOW, AdwApplicationWindow)

G_END_DECLS
