From f97690b633b8d432cf5a3207a112e83be7d28013 Mon Sep 17 00:00:00 2001
From: Benjamin Otte <otte@redhat.com>
Date: Sat, 24 Jan 2015 03:21:04 +0100
Subject: [PATCH] window: Track padding for geometry

Changes in padding need to cause an update of geometry. And with GTK
3.15 padding actually does change causing shrinkage of the window when
(un)focusing.

https://bugzilla.gnome.org/show_bug.cgi?id=743395
---
 src/terminal-window.c | 23 +++++++++++++----------
 1 file changed, 13 insertions(+), 10 deletions(-)

diff --git a/src/terminal-window.c b/src/terminal-window.c
index cf9ba80..1dacc93 100644
--- src/terminal-window.c
+++ src/terminal-window.c
@@ -74,6 +74,8 @@ struct _TerminalWindowPrivate
   TerminalScreen *active_screen;
   int old_char_width;
   int old_char_height;
+  int old_base_width;
+  int old_base_height;
   void *old_geometry_widget; /* only used for pointer value as it may be freed */
 
   GtkWidget *confirm_close_dialog;
@@ -2616,6 +2618,8 @@ terminal_window_init (TerminalWindow *window)
 
   priv->old_char_width = -1;
   priv->old_char_height = -1;
+  priv->old_base_width = -1;
+  priv->old_base_height = -1;
   priv->old_geometry_widget = NULL;
 
   /* GAction setup */
@@ -3479,6 +3483,7 @@ terminal_window_update_geometry (TerminalWindow *window)
   TerminalWindowPrivate *priv = window->priv;
   GtkWidget *widget;
   GdkGeometry hints;
+  GtkBorder padding;
   int char_width;
   int char_height;
   
@@ -3494,20 +3499,16 @@ terminal_window_update_geometry (TerminalWindow *window)
    */
   terminal_screen_get_cell_size (priv->active_screen, &char_width, &char_height);
   
+  gtk_style_context_get_padding(gtk_widget_get_style_context(widget),
+                                gtk_widget_get_state_flags(widget),
+                                &padding);
+
   if (char_width != priv->old_char_width ||
       char_height != priv->old_char_height ||
+      padding.left + padding.right != priv->old_base_width ||
+      padding.top + padding.bottom != priv->old_base_height ||
       widget != (GtkWidget*) priv->old_geometry_widget)
     {
-      GtkBorder padding;
-      
-      /* FIXME Since we're using xthickness/ythickness to compute
-       * padding we need to change the hints when the theme changes.
-       */
-
-      gtk_style_context_get_padding(gtk_widget_get_style_context(widget),
-                                    gtk_widget_get_state_flags(widget),
-                                    &padding);
-
       hints.base_width = padding.left + padding.right;
       hints.base_height = padding.top + padding.bottom;
 
@@ -3540,6 +3541,8 @@ terminal_window_update_geometry (TerminalWindow *window)
       
       priv->old_char_width = hints.width_inc;
       priv->old_char_height = hints.height_inc;
+      priv->old_base_width = hints.base_width;
+      priv->old_base_height = hints.base_height;
       priv->old_geometry_widget = widget;
     }
   else
-- 
2.1.0
