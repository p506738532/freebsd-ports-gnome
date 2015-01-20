--- src/gs-fade.c.orig	2014-10-03 07:57:43.000000000 +0200
+++ src/gs-fade.c	2015-01-20 07:14:51.525302438 +0100
@@ -629,8 +629,6 @@
 	screen_priv = &fade->priv->screen_priv[screen_idx];
 
 	screen_priv->rrscreen = mate_rr_screen_new (screen,
-	                        NULL,
-	                        NULL,
 	                        NULL);
 	if (!screen_priv->rrscreen)
 	{
@@ -988,7 +986,7 @@
 		{
 			if (!fade->priv->screen_priv[i].rrscreen)
 				continue;
-			mate_rr_screen_destroy (fade->priv->screen_priv[i].rrscreen);
+			g_object_unref (fade->priv->screen_priv[i].rrscreen);
 		}
 
 		g_free (fade->priv->screen_priv);
