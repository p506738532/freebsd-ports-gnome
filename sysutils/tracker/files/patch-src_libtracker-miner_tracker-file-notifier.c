--- src/libtracker-miner/tracker-file-notifier.c.orig	2015-03-18 20:47:53.529615000 +0100
+++ src/libtracker-miner/tracker-file-notifier.c	2015-03-18 20:48:20.774719000 +0100
@@ -1732,8 +1732,8 @@
 	TrackerFileNotifierPrivate *priv;
 	GFile *canonical;
 
-	g_return_val_if_fail (TRACKER_IS_FILE_NOTIFIER (notifier), NULL);
-	g_return_val_if_fail (G_IS_FILE (file), NULL);
+	g_return_if_fail (TRACKER_IS_FILE_NOTIFIER (notifier));
+	g_return_if_fail (G_IS_FILE (file));
 
 	priv = notifier->priv;
 	canonical = tracker_file_system_get_file (priv->file_system,
