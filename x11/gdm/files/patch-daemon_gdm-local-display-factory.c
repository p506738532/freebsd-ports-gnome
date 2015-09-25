--- daemon/gdm-local-display-factory.c.orig	2015-09-21 16:12:33.000000000 +0200
+++ daemon/gdm-local-display-factory.c	2015-09-25 10:40:38.545961000 +0200
@@ -42,6 +42,7 @@
 
 #define GDM_LOCAL_DISPLAY_FACTORY_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GDM_TYPE_LOCAL_DISPLAY_FACTORY, GdmLocalDisplayFactoryPrivate))
 
+#define CK_SEAT1_PATH                       "/org/freedesktop/ConsoleKit/Seat1"
 #define SYSTEMD_SEAT0_PATH                  "seat0"
 
 #define GDM_DBUS_PATH                       "/org/gnome/DisplayManager"
@@ -59,8 +60,10 @@ struct GdmLocalDisplayFactoryPrivate
         /* FIXME: this needs to be per seat? */
         guint            num_failures;
 
+#ifdef WITH_SYSTEMD
         guint            seat_new_id;
         guint            seat_removed_id;
+#endif
 };
 
 enum {
@@ -190,8 +193,20 @@ store_display (GdmLocalDisplayFactory *f
 static const char *
 get_seat_of_transient_display (GdmLocalDisplayFactory *factory)
 {
+        const char *seat_id = NULL;
+
         /* FIXME: don't hardcode seat */
-        return SYSTEMD_SEAT0_PATH;
+#ifdef WITH_SYSTEMD
+        if (LOGIND_RUNNING() > 0) {
+                seat_id = SYSTEMD_SEAT0_PATH;
+        }
+#endif
+
+        if (seat_id == NULL) {
+                seat_id = CK_SEAT1_PATH;
+        }
+
+        return seat_id;
 }
 
 /*
@@ -216,7 +231,19 @@ gdm_local_display_factory_create_transie
 
         g_debug ("GdmLocalDisplayFactory: Creating transient display");
 
-        display = gdm_local_display_new ();
+#ifdef WITH_SYSTEMD
+        if (LOGIND_RUNNING() > 0) {
+                display = gdm_local_display_new ();
+        }
+#endif
+
+        if (display == NULL) {
+                guint32 num;
+
+                num = take_next_display_number (factory);
+
+                display = gdm_legacy_display_new (num);
+        }
 
         seat_id = get_seat_of_transient_display (factory);
         g_object_set (display,
@@ -372,12 +399,14 @@ create_display (GdmLocalDisplayFactory *
         g_debug ("GdmLocalDisplayFactory: Adding display on seat %s", seat_id);
 
 
+#ifdef WITH_SYSTEMD
         if (g_strcmp0 (seat_id, "seat0") == 0) {
                 display = gdm_local_display_new ();
                 if (session_type != NULL) {
                         g_object_set (G_OBJECT (display), "session-type", session_type, NULL);
                 }
         }
+#endif
 
         if (display == NULL) {
                 guint32 num;
@@ -402,6 +431,8 @@ create_display (GdmLocalDisplayFactory *
         return display;
 }
 
+#ifdef WITH_SYSTEMD
+
 static void
 delete_display (GdmLocalDisplayFactory *factory,
                 const char             *seat_id) {
@@ -571,17 +602,21 @@ on_display_removed (GdmDisplayStore     
 
         }
 }
+#endif
 
 static gboolean
 gdm_local_display_factory_start (GdmDisplayFactory *base_factory)
 {
         GdmLocalDisplayFactory *factory = GDM_LOCAL_DISPLAY_FACTORY (base_factory);
+        GdmDisplay		*display;
         GdmDisplayStore *store;
 
         g_return_val_if_fail (GDM_IS_LOCAL_DISPLAY_FACTORY (factory), FALSE);
 
         store = gdm_display_factory_get_display_store (GDM_DISPLAY_FACTORY (factory));
 
+// XXX signals?
+#ifdef WITH_SYSTEMD
         g_signal_connect (G_OBJECT (store),
                           "display-added",
                           G_CALLBACK (on_display_added),
@@ -592,8 +627,16 @@ gdm_local_display_factory_start (GdmDisp
                           G_CALLBACK (on_display_removed),
                           factory);
 
-        gdm_local_display_factory_start_monitor (factory);
-        return gdm_local_display_factory_sync_seats (factory);
+	if (LOGIND_RUNNING()) {
+        	gdm_local_display_factory_start_monitor (factory);
+        	return gdm_local_display_factory_sync_seats (factory);
+	}
+#endif
+
+ /* On ConsoleKit just create Seat1, and that's it. */
+        display = create_display (factory, CK_SEAT1_PATH, NULL, TRUE);
+
+        return display != NULL;
 }
 
 static gboolean
@@ -604,17 +647,20 @@ gdm_local_display_factory_stop (GdmDispl
 
         g_return_val_if_fail (GDM_IS_LOCAL_DISPLAY_FACTORY (factory), FALSE);
 
+#ifdef WITH_SYSTEMD
         gdm_local_display_factory_stop_monitor (factory);
+#endif
 
         store = gdm_display_factory_get_display_store (GDM_DISPLAY_FACTORY (factory));
 
+#ifdef WITH_SYSTEMD
         g_signal_handlers_disconnect_by_func (G_OBJECT (store),
                                               G_CALLBACK (on_display_added),
                                               factory);
         g_signal_handlers_disconnect_by_func (G_OBJECT (store),
                                               G_CALLBACK (on_display_removed),
                                               factory);
-
+#endif
         return TRUE;
 }
 
@@ -760,7 +806,9 @@ gdm_local_display_factory_finalize (GObj
 
         g_hash_table_destroy (factory->priv->used_display_numbers);
 
+#ifdef WITH_SYSTEMD
         gdm_local_display_factory_stop_monitor (factory);
+#endif
 
         G_OBJECT_CLASS (gdm_local_display_factory_parent_class)->finalize (object);
 }
