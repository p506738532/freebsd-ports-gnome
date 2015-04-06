--- src/procproperties.cpp.orig	2015-01-23 09:15:58.000000000 +0100
+++ src/procproperties.cpp	2015-04-06 20:55:39.473119000 +0200
@@ -26,7 +26,7 @@
 #include <glibtop/procstate.h>
 #if defined (__linux__)
 #include <asm/param.h>
-#elif defined (__OpenBSD__)
+#elif defined (__OpenBSD__) || defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
 #include <sys/param.h>
 #include <sys/sysctl.h>
 #endif
@@ -114,7 +114,7 @@
 
     get_process_memory_info(info);
 
-#if defined (__OpenBSD__)
+#if defined (__OpenBSD__) || defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
     struct clockinfo cinf;
     size_t size = sizeof (cinf);
     int HZ;
