--- gst-libs/gst/adaptivedemux/gstadaptivedemux.c.orig	2016-02-20 12:11:12.539859000 +0100
+++ gst-libs/gst/adaptivedemux/gstadaptivedemux.c	2016-02-20 12:14:45.329220000 +0100
@@ -1335,7 +1335,7 @@
       if (IS_SNAP_SEEK (flags) && demux_class->stream_seek) {
         GstAdaptiveDemuxStream *stream =
             gst_adaptive_demux_find_stream_for_pad (demux, pad);
-        GstClockTime ts;
+        GstClockTime ts = 0;
         GstSeekFlags stream_seek_flags = flags;
 
         /* snap-seek on the stream that received the event and then
