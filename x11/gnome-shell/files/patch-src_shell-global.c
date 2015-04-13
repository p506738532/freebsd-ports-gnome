From 45583812f6c4ae2bed931a80aeeec9d268a5b842 Mon Sep 17 00:00:00 2001
From: Ting-Wei Lan <lantw@src.gnome.org>
Date: Mon, 13 Apr 2015 12:07:35 +0800
Subject: [PATCH] shell_global_reexec_self: add support for FreeBSD

https://bugzilla.gnome.org/show_bug.cgi?id=747788
---
 src/shell-global.c | 28 ++++++++++++++++++++++++++--
 1 file changed, 26 insertions(+), 2 deletions(-)

diff --git a/src/shell-global.c b/src/shell-global.c
index af99747..acd5c80 100644
--- src/shell-global.c	
+++ src/shell-global.c	
@@ -39,7 +39,7 @@ 
 #include <malloc.h>
 #endif
 
-#ifdef __OpenBSD__
+#if defined __OpenBSD__ || defined __FreeBSD__
 #include <sys/sysctl.h>
 #endif
 
@@ -1279,6 +1279,30 @@ shell_global_reexec_self (ShellGlobal *global)
   }
 
   g_ptr_array_add (arr, NULL);
+#elif defined __FreeBSD__
+  char *buf;
+  char *buf_p;
+  char *buf_end;
+  gint mib[] = { CTL_KERN, KERN_PROC, KERN_PROC_ARGS, getpid() };
+
+  if (sysctl (mib, G_N_ELEMENTS (mib), NULL, &len, NULL, 0) == -1)
+    return;
+
+  buf = g_malloc0 (len);
+
+  if (sysctl (mib, G_N_ELEMENTS (mib), buf, &len, NULL, 0) == -1) {
+    g_warning ("failed to get command line args: %d", errno);
+    g_free (buf);
+    return;
+  }
+
+  buf_end = buf+len;
+  arr = g_ptr_array_new ();
+  /* The value returned by sysctl is NUL-separated */
+  for (buf_p = buf; buf_p < buf_end; buf_p = buf_p + strlen (buf_p) + 1)
+    g_ptr_array_add (arr, buf_p);
+
+  g_ptr_array_add (arr, NULL);
 #else
   return;
 #endif
@@ -1297,7 +1321,7 @@ shell_global_reexec_self (ShellGlobal *global)
   execvp (arr->pdata[0], (char**)arr->pdata);
   g_warning ("failed to reexec: %s", g_strerror (errno));
   g_ptr_array_free (arr, TRUE);
-#if defined __linux__
+#if defined __linux__ || defined __FreeBSD__
   g_free (buf);
 #elif defined __OpenBSD__
   g_free (args);
