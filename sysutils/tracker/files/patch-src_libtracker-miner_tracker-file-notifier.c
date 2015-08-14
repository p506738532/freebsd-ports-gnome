From 789462f3c0b731a5188c3ae89d356c097d3bcf32 Mon Sep 17 00:00:00 2001
From: Ting-Wei Lan <lantw@src.gnome.org>
Date: Mon, 27 Jul 2015 01:14:35 +0800
Subject: build: Fix return value error for tracker_file_notifier_get_file_type

https://bugzilla.gnome.org/show_bug.cgi?id=752900

diff --git a/src/libtracker-miner/tracker-file-notifier.c b/src/libtracker-miner/tracker-file-notifier.c
index 2862490..49a0998 100644
--- src/libtracker-miner/tracker-file-notifier.c
+++ src/libtracker-miner/tracker-file-notifier.c
@@ -1767,8 +1767,8 @@ tracker_file_notifier_get_file_type (TrackerFileNotifier *notifier,
 	TrackerFileNotifierPrivate *priv;
 	GFile *canonical;
 
-	g_return_if_fail (TRACKER_IS_FILE_NOTIFIER (notifier));
-	g_return_if_fail (G_IS_FILE (file));
+	g_return_val_if_fail (TRACKER_IS_FILE_NOTIFIER (notifier), G_FILE_TYPE_UNKNOWN);
+	g_return_val_if_fail (G_IS_FILE (file), G_FILE_TYPE_UNKNOWN);
 
 	priv = notifier->priv;
 	canonical = tracker_file_system_get_file (priv->file_system,
-- 
cgit v0.10.2

