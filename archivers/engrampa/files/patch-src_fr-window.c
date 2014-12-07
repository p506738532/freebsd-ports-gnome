--- src/fr-window.c.orig	2014-11-19 21:02:45.000000000 +0100
+++ src/fr-window.c	2014-12-04 20:19:11.655038917 +0100
@@ -6629,7 +6629,7 @@
 }
 
 
-static gboolean _fr_window_ask_overwrite_dialog (OverwriteData *odata);
+static void _fr_window_ask_overwrite_dialog (OverwriteData *odata);
 
 
 static void
@@ -6680,7 +6680,7 @@
 }
 
 
-static gboolean
+static void
 _fr_window_ask_overwrite_dialog (OverwriteData *odata)
 {
 	gboolean do_not_extract = FALSE;
