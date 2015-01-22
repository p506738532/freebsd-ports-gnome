From 1cf8ae1fea1e3ac8eed6d7d35a718dcfa2e415f8 Mon Sep 17 00:00:00 2001
From: Bastien Nocera <hadess@hadess.net>
Date: Thu, 22 Jan 2015 15:48:31 +0100
Subject: [PATCH] common: Fix build on non-Linux systems

Only define HAVE_WAYLAND when GDK_WINDOWING_WAYLAND is defined.

https://bugzilla.gnome.org/show_bug.cgi?id=743266
---
 panels/common/gnome-settings-bus.h | 4 ++++
 1 file changed, 4 insertions(+)

diff --git a/panels/common/gnome-settings-bus.h b/panels/common/gnome-settings-bus.h
index 0698f01..ce58f58 100644
--- panels/common/gnome-settings-bus.h
+++ panels/common/gnome-settings-bus.h
@@ -3,6 +3,8 @@
 
 #include <gdk/gdkx.h>
 
+#ifdef GDK_WINDOWING_WAYLAND
+
 #define HAVE_WAYLAND 1
 
 static inline gboolean
@@ -10,3 +12,5 @@ gnome_settings_is_wayland (void)
 {
   return !GDK_IS_X11_DISPLAY (gdk_display_get_default ());
 }
+
+#endif /* GDK_WINDOWING_WAYLAND */
-- 
2.1.0
