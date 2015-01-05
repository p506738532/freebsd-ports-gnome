From 1fbb2b1fcc7e945fcd7bfd1c5afa15a501976c35 Mon Sep 17 00:00:00 2001
From: Lionel Landwerlin <llandwerlin@gmail.com>
Date: Sun, 4 Jan 2015 23:34:39 +0000
Subject: [PATCH] auto-video-sink: fix crash when clutter is not initialized

We must prevent sink creation when Clutter's initialization has
failed.

https://bugzilla.gnome.org/show_bug.cgi?id=742279
---
 clutter-gst/clutter-gst-auto-video-sink.c | 34 ++++++++++++++-----------------
 1 file changed, 15 insertions(+), 19 deletions(-)

diff --git a/clutter-gst/clutter-gst-auto-video-sink.c b/clutter-gst/clutter-gst-auto-video-sink.c
index 02aca5b..3a37a70 100644
--- clutter-gst/clutter-gst-auto-video-sink.c
+++ clutter-gst/clutter-gst-auto-video-sink.c
@@ -62,12 +62,6 @@ G_DEFINE_TYPE (ClutterGstAutoVideoSink3,
                clutter_gst_auto_video_sink,
                GST_TYPE_BIN)
 
-/* static GstStaticPadTemplate sink_template = */
-/*   GST_STATIC_PAD_TEMPLATE ("sink", */
-/*                            GST_PAD_SINK, */
-/*                            GST_PAD_ALWAYS, */
-/*                            GST_STATIC_CAPS_ANY); */
-
 static ClutterInitError _clutter_initialized = CLUTTER_INIT_ERROR_UNKNOWN;
 
 static void
@@ -116,19 +110,20 @@ clutter_gst_auto_video_sink_class_init (ClutterGstAutoVideoSink3Class *klass)
                                                         CLUTTER_GST_TYPE_CONTENT,
                                                         G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
 
-  GstElement *clutter_sink = clutter_gst_create_video_sink ();
+  if (_clutter_initialized) {
+    GstElement *clutter_sink = clutter_gst_create_video_sink ();
 
-  gst_element_class_add_pad_template (eklass,
-                                      gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS (clutter_sink),
-                                                                          "sink"));
-  /* gst_static_pad_template_get (&sink_template)); */
-  gst_element_class_set_static_metadata (eklass,
-                                         "Clutter Auto Video Sink",
-                                         "Sink/Video",
-                                         "Video sink using the Clutter scene graph as output",
-                                         "Lionel Landwerlin <lionel.g.landwerlin@linux.intel.com>");
+    gst_element_class_add_pad_template (eklass,
+                                        gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS (clutter_sink),
+                                                                            "sink"));
+    gst_element_class_set_static_metadata (eklass,
+                                           "Clutter Auto Video Sink",
+                                           "Sink/Video",
+                                           "Video sink using the Clutter scene graph as output",
+                                           "Lionel Landwerlin <lionel.g.landwerlin@linux.intel.com>");
 
-  g_object_unref (clutter_sink);
+    g_object_unref (clutter_sink);
+  }
 }
 
 static void
@@ -162,6 +157,9 @@ clutter_gst_auto_video_sink_reset (ClutterGstAutoVideoSink3 *sink)
 {
   GstPad *targetpad;
 
+  if (!_clutter_initialized)
+    return;
+
   /* Remove any existing element */
   clutter_gst_auto_video_sink_clear_kid (sink);
 
@@ -186,8 +184,6 @@ clutter_gst_auto_video_sink_init (ClutterGstAutoVideoSink3 *sink)
 
   clutter_gst_auto_video_sink_reset (sink);
 
-
-
   /* mark as sink */
   GST_OBJECT_FLAG_SET (sink, GST_ELEMENT_FLAG_SINK);
 }
-- 
2.2.1

