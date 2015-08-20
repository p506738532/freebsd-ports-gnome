--- src/miners/fs/tracker-miner-files-peer-listener.c.orig	2015-08-20 12:55:57.927428000 +0200
+++ src/miners/fs/tracker-miner-files-peer-listener.c	2015-08-20 12:57:29.444966000 +0200
@@ -446,8 +446,8 @@
 {
 	TrackerMinerFilesPeerListenerPrivate *priv;
 
-	g_return_if_fail (TRACKER_IS_MINER_FILES_PEER_LISTENER (listener));
-	g_return_if_fail (G_IS_FILE (file));
+	g_return_val_if_fail (TRACKER_IS_MINER_FILES_PEER_LISTENER (listener), FALSE);
+	g_return_val_if_fail (G_IS_FILE (file), FALSE);
 
 	priv = tracker_miner_files_peer_listener_get_instance_private (listener);
 
