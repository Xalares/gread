#ifndef _GREAD_APPLICATION_H_
#define _GREAD_APPLICATION_H_

#include <adwaita.h>

G_BEGIN_DECLS

#define GREAD_TYPE_APPLICATION (gread_application_get_type())
G_DECLARE_FINAL_TYPE (GreadApplication, gread_application, GREAD, APPLICATION, AdwApplication)

GreadApplication* gread_application_new(void);

G_END_DECLS

#endif
