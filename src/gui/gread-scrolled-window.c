#include <gtk/gtk.h>
#include <adwaita.h>

#include "gread-scrolled-window.h"

struct _GreadScrolledWindow {
  AdwBin parent;
  GtkWidget * scrollbar;
  GtkWidget * child;
};


static void gread_scrolled_window_buildable_init(GtkBuildableIface *iface);
void gread_scrolled_window_set_child(GreadScrolledWindow *self, GtkWidget *child);

G_DEFINE_TYPE_WITH_CODE (GreadScrolledWindow, gread_scrolled_window, ADW_TYPE_BIN,
                         G_IMPLEMENT_INTERFACE (GTK_TYPE_BUILDABLE,
                                                gread_scrolled_window_buildable_init))


static GtkBuildableIface *parent_buildable_iface;

static void
gread_scrolled_window_buildable_add_child(GtkBuildable *buildable,
                                          GtkBuilder   *builder,
                                          GObject      *child,
                                          const char   *type){
  if(GTK_IS_WIDGET(child))
    gread_scrolled_window_set_child(GREAD_SCROLLED_WINDOW(buildable), GTK_WIDGET(child));
  else
    parent_buildable_iface->add_child(buildable, builder, child, type);
}

static void
gread_scrolled_window_buildable_init (GtkBuildableIface *iface)
{
  parent_buildable_iface = g_type_interface_peek_parent (iface);

  iface->add_child = gread_scrolled_window_buildable_add_child;
}

void
gread_scrolled_window_set_child(GreadScrolledWindow *self, GtkWidget *child){
  //g_return_if_fail (GREAD_IS_SCROLLED_WINDOW (scrolled_window));

  if (self->child)
    {
      /*if (priv->auto_added_viewport)
        {
          gtk_viewport_set_child (GTK_VIEWPORT (priv->child), NULL);
          }*/

      /*g_object_set (self->child,
                    "hadjustment", NULL,
                    "vadjustment", NULL,
                    NULL);*/

      g_clear_pointer (&self->child, gtk_widget_unparent);
    }

  if (child)
    {
      // GtkAdjustment *hadj, *vadj;

      /* gtk_scrolled_window_set_[hv]adjustment have the side-effect
       * of creating the scrollbars
       */
      /*if (!priv->hscrollbar)
        gtk_scrolled_window_set_hadjustment (scrolled_window, NULL);

      if (!priv->vscrollbar)
        gtk_scrolled_window_set_vadjustment (scrolled_window, NULL);

      hadj = gtk_scrollbar_get_adjustment (GTK_SCROLLBAR (priv->hscrollbar));
      vadj = gtk_scrollbar_get_adjustment (GTK_SCROLLBAR (priv->vscrollbar));

      if (GTK_IS_SCROLLABLE (child))
        {
          scrollable_child = child;
          priv->auto_added_viewport = FALSE;
        }
      else
        {
          scrollable_child = gtk_viewport_new (hadj, vadj);
          gtk_viewport_set_child (GTK_VIEWPORT (scrollable_child), child);
          priv->auto_added_viewport = TRUE;
          }*/

      self->child = child;
      gtk_widget_insert_after (child, GTK_WIDGET (self), NULL);

      /*g_object_set (scrollable_child,
                    "hadjustment", hadj,
                    "vadjustment", vadj,
                    NULL);*/
    }
  /*
  if (priv->child)
    {
      gtk_accessible_update_relation (GTK_ACCESSIBLE (priv->hscrollbar),
                                      GTK_ACCESSIBLE_RELATION_CONTROLS, priv->child, NULL,
                                      -1);
      gtk_accessible_update_relation (GTK_ACCESSIBLE (priv->vscrollbar),
                                      GTK_ACCESSIBLE_RELATION_CONTROLS, priv->child, NULL,
                                      -1);
    }
  else
    {
      gtk_accessible_reset_relation (GTK_ACCESSIBLE (priv->hscrollbar),
                                     GTK_ACCESSIBLE_RELATION_CONTROLS);
      gtk_accessible_reset_relation (GTK_ACCESSIBLE (priv->vscrollbar),
                                     GTK_ACCESSIBLE_RELATION_CONTROLS);
    }

    g_object_notify_by_pspec (G_OBJECT (scrolled_window), properties[PROP_CHILD]);*/

}
static void
gread_scrolled_window_measure(GtkWidget *widget,
                              GtkOrientation orientation,
                              int for_size,
                              int *minimum,
                              int *natural,
                              int *minimum_baseline,
                              int *natural_baseline){

}

static void
gread_scrolled_window_allocate(GtkWidget *widget, int width,
                               int height, int baseline){

}

static void
gread_scrolled_window_class_init(GreadScrolledWindowClass *klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass   *object_class = G_OBJECT_CLASS(klass);

  widget_class->size_allocate = gread_scrolled_window_allocate;
  widget_class->measure = gread_scrolled_window_measure;

  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/org/gnome/gread/gui/gread-introduction.ui");

}


static void
gread_scrolled_window_init(GreadScrolledWindow *self){
  gtk_widget_init_template(GTK_WIDGET(self));
}
