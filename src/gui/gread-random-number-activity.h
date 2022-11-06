#pragma once
#include <gtk/gtk.h>
#include "gread-application.h"

G_BEGIN_DECLS

#define GREAD_RANDOM_NUMBER_ACTIVITY_TYPE (gread_random_number_activity_get_type ())

G_DECLARE_FINAL_TYPE(GreadRandomNumberActivity, gread_random_number_activity, GREAD, RANDOM_NUMBER_ACTIVITY, AdwBin)


  void gread_random_number_activity_set_digits(GreadRandomNumberActivity *win, guint digits);

void gread_random_number_activity_set_display_time(GreadRandomNumberActivity *win, guint display_time);

GreadRandomNumberActivity* gread_random_number_activity_new(GreadApplication *app);

G_END_DECLS
