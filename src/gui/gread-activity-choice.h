#ifndef _GREAD_ACTIVITY_CHOICE_H_
#define _GREAD_ACTIVITY_CHOICE_H_

#include <adwaita.h>

G_BEGIN_DECLS

#define GREAD_APPLICATION_TYPE (gread_application_get_type())
G_DECLARE_FINAL_TYPE (GreadActivityChoice, gread_activity_choice, GREAD, ACTIVITY_CHOICE, GtkWidget)

GreadActivityChoice* gread_application_new(void);

G_END_DECLS

#endif
