#pragma once
#include <gtk/gtk.h>
#include <adwaita.h>

G_BEGIN_DECLS

#define GREAD_INTRODUCTION_TYPE (gread_introduction_get_type())

G_DECLARE_FINAL_TYPE (GreadIntroduction, gread_introduction, GREAD, INTRODUCTION, AdwBin)

G_END_DECLS
