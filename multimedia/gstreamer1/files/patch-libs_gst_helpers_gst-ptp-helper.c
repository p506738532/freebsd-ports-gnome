--- libs/gst/helpers/gst-ptp-helper.c.orig	2015-06-04 19:05:43.000000000 +0200
+++ libs/gst/helpers/gst-ptp-helper.c	2015-06-07 19:50:45.373792000 +0200
@@ -41,7 +41,7 @@
 #include <netinet/in.h>
 #include <string.h>
 
-#ifdef __APPLE__
+#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__DragonFly__)
 #include <ifaddrs.h>
 #include <net/if_dl.h>
 #endif
@@ -298,7 +298,7 @@ setup_sockets (void)
   if (clock_id == (guint64) - 1) {
     gboolean success = FALSE;
 
-#ifndef __APPLE__
+#if !defined(__APPLE__) && !defined(__FreeBSD__) && !defined(__DragonFly__)
     struct ifreq ifr;
 
     if (ifaces) {
