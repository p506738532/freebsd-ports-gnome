--- daemon/gdm-session-worker.c.orig	Tue Apr 19 07:00:41 2016
+++ daemon/gdm-session-worker.c	Tue Apr 26 10:02:48 2016
@@ -28,9 +28,11 @@
 #include <string.h>
 #include <sys/types.h>
 #include <sys/wait.h>
+#ifdef WITH_SYSTEMD
 #include <sys/ioctl.h>
 #include <sys/vt.h>
 #include <sys/kd.h>
+#endif
 #include <errno.h>
 #include <grp.h>
 #include <pwd.h>
@@ -49,7 +51,9 @@
 
 #include <X11/Xauth.h>
 
+#ifdef WITH_SYSTEMD
 #include <systemd/sd-daemon.h>
+#endif
 
 #ifdef ENABLE_SYSTEMD_JOURNAL
 #include <systemd/sd-journal.h>
@@ -131,6 +135,10 @@ struct GdmSessionWorkerPrivate
 
         int               exit_code;
 
+#ifdef WITH_CONSOLE_KIT
+        char             *session_cookie;
+#endif
+
         pam_handle_t     *pam_handle;
 
         GPid              child_pid;
@@ -145,6 +153,7 @@ struct GdmSessionWorkerPrivate
         char             *hostname;
         char             *username;
         char             *log_file;
+        char             *session_type;
         char             *session_id;
         uid_t             uid;
         gid_t             gid;
@@ -207,6 +216,204 @@ G_DEFINE_TYPE_WITH_CODE (GdmSessionWorker,
                          G_IMPLEMENT_INTERFACE (GDM_DBUS_TYPE_WORKER,
                                                 worker_interface_init))
 
+#ifdef WITH_CONSOLE_KIT
+static gboolean
+open_ck_session (GdmSessionWorker  *worker)
+{
+        GDBusConnection  *system_bus;
+        GVariantBuilder   builder;
+        GVariant         *parameters;
+        GVariant         *in_args;
+        struct passwd    *pwent;
+        GVariant         *reply;
+        GError           *error = NULL;
+        const char       *display_name;
+        const char       *display_device;
+        const char       *display_hostname;
+        const char       *session_type;
+        gint32            uid;
+
+        g_assert (worker->priv->session_cookie == NULL);
+
+        if (worker->priv->x11_display_name != NULL) {
+                display_name = worker->priv->x11_display_name;
+        } else {
+                display_name = "";
+        }
+        if (worker->priv->hostname != NULL) {
+                display_hostname = worker->priv->hostname;
+        } else {
+                display_hostname = "";
+        }
+        if (worker->priv->display_device != NULL) {
+                display_device = worker->priv->display_device;
+        } else {
+                display_device = "";
+        }
+
+        if (worker->priv->session_type != NULL) {
+                session_type = worker->priv->session_type;
+        } else {
+                session_type = "";
+        }
+
+        g_assert (worker->priv->username != NULL);
+
+        gdm_get_pwent_for_name (worker->priv->username, &pwent);
+        if (pwent == NULL) {
+                goto out;
+        }
+
+        uid = (gint32) pwent->pw_uid;
+
+        error = NULL;
+        system_bus = g_bus_get_sync (G_BUS_TYPE_SYSTEM, NULL, &error);
+
+        if (system_bus == NULL) {
+                g_warning ("Couldn't create connection to system bus: %s",
+                           error->message);
+
+                g_error_free (error);
+                goto out;
+        }
+
+        g_variant_builder_init (&builder, G_VARIANT_TYPE ("a(sv)"));
+        g_variant_builder_add_parsed (&builder, "('unix-user', <%i>)", uid);
+        g_variant_builder_add_parsed (&builder, "('x11-display-device', <%s>)", display_device);
+        g_variant_builder_add_parsed (&builder, "('x11-display', <%s>)", display_name);
+        g_variant_builder_add_parsed (&builder, "('remote-host-name', <%s>)", display_hostname);
+        g_variant_builder_add_parsed (&builder, "('is-local', <%b>)", worker->priv->display_is_local);
+        g_variant_builder_add_parsed (&builder, "('session-type', <%s>)", session_type);
+
+        parameters = g_variant_builder_end (&builder);
+        in_args = g_variant_new_tuple (&parameters, 1);
+
+        reply = g_dbus_connection_call_sync (system_bus,
+                                             "org.freedesktop.ConsoleKit",
+                                             "/org/freedesktop/ConsoleKit/Manager",
+                                             "org.freedesktop.ConsoleKit.Manager",
+                                             "OpenSessionWithParameters",
+                                             in_args,
+                                             G_VARIANT_TYPE ("(s)"),
+                                             G_DBUS_CALL_FLAGS_NONE,
+                                             -1,
+                                             NULL,
+                                             &error);
+
+        if (! reply) {
+                g_warning ("%s\n", error->message);
+                g_clear_error (&error);
+                goto out;
+        }
+
+        g_variant_get (reply, "(s)", &worker->priv->session_cookie);
+
+        g_variant_unref (reply);
+
+out:
+        return worker->priv->session_cookie != NULL;
+}
+
+static void
+close_ck_session (GdmSessionWorker *worker)
+{
+        GDBusConnection  *system_bus;
+        GVariant         *reply;
+        GError           *error = NULL;
+        gboolean          was_closed;
+
+        if (worker->priv->session_cookie == NULL) {
+                return;
+        }
+
+        error = NULL;
+        system_bus = g_bus_get_sync (G_BUS_TYPE_SYSTEM, NULL, &error);
+
+        if (system_bus == NULL) {
+                g_warning ("Couldn't create connection to system bus: %s",
+                           error->message);
+
+                g_error_free (error);
+                goto out;
+        }
+
+        reply = g_dbus_connection_call_sync (system_bus,
+                                             "org.freedesktop.ConsoleKit",
+                                             "/org/freedesktop/ConsoleKit/Manager",
+                                             "org.freedesktop.ConsoleKit.Manager",
+                                             "CloseSession",
+                                             g_variant_new ("(s)", worker->priv->session_cookie),
+                                             G_VARIANT_TYPE ("(b)"),
+                                             G_DBUS_CALL_FLAGS_NONE,
+                                             -1,
+                                             NULL,
+                                             &error);
+
+        if (! reply) {
+                g_warning ("%s", error->message);
+                g_clear_error (&error);
+                goto out;
+        }
+
+        g_variant_get (reply, "(b)", &was_closed);
+
+        if (!was_closed) {
+                g_warning ("Unable to close ConsoleKit session");
+        }
+
+        g_variant_unref (reply);
+
+out:
+        g_clear_pointer (&worker->priv->session_cookie,
+                         (GDestroyNotify) g_free);
+}
+
+static char *
+get_ck_session_id (GdmSessionWorker *worker)
+{
+        GDBusConnection  *system_bus;
+        GVariant         *reply;
+        GError           *error = NULL;
+        char             *session_id = NULL;
+
+        error = NULL;
+        system_bus = g_bus_get_sync (G_BUS_TYPE_SYSTEM, NULL, &error);
+
+        if (system_bus == NULL) {
+                g_warning ("Couldn't create connection to system bus: %s",
+                           error->message);
+
+                g_error_free (error);
+                goto out;
+        }
+
+        reply = g_dbus_connection_call_sync (system_bus,
+                                             "org.freedesktop.ConsoleKit",
+                                             "/org/freedesktop/ConsoleKit/Manager",
+                                             "org.freedesktop.ConsoleKit.Manager",
+                                             "GetSessionForCookie",
+                                             g_variant_new ("(s)", worker->priv->session_cookie),
+                                             G_VARIANT_TYPE ("(o)"),
+                                             G_DBUS_CALL_FLAGS_NONE,
+                                             -1,
+                                             NULL,
+                                             &error);
+
+        if (reply == NULL) {
+                g_warning ("%s", error->message);
+                g_clear_error (&error);
+                goto out;
+        }
+
+        g_variant_get (reply, "(o)", &session_id);
+
+        g_variant_unref (reply);
+
+out:
+        return session_id;
+}
+#endif
+
 /* adapted from glib script_execute */
 static void
 script_execute (const gchar *file,
@@ -754,6 +961,7 @@ gdm_session_worker_stop_auditor (GdmSessionWorker *wor
         worker->priv->auditor = NULL;
 }
 
+#ifdef WITH_SYSTEMD
 static void
 on_release_display (int signal)
 {
@@ -879,6 +1087,7 @@ jump_to_vt (GdmSessionWorker  *worker,
 
         close (active_vt_tty_fd);
 }
+#endif
 
 static void
 gdm_session_worker_uninitialize_pam (GdmSessionWorker *worker,
@@ -909,9 +1118,11 @@ gdm_session_worker_uninitialize_pam (GdmSessionWorker 
 
         gdm_session_worker_stop_auditor (worker);
 
+#ifdef WITH_SYSTEMD
         if (worker->priv->login_vt != worker->priv->session_vt) {
                 jump_to_vt (worker, worker->priv->login_vt);
         }
+#endif
 
         worker->priv->login_vt = 0;
         worker->priv->session_vt = 0;
@@ -963,32 +1174,6 @@ _get_xauth_for_pam (const char *x11_authority_file)
 #endif
 
 static gboolean
-ensure_login_vt (GdmSessionWorker *worker)
-{
-        int fd;
-        struct vt_stat vt_state = { 0 };
-        gboolean got_login_vt = FALSE;
-
-        fd = open ("/dev/tty0", O_RDWR | O_NOCTTY);
-
-        if (fd < 0) {
-                g_debug ("GdmSessionWorker: couldn't open VT master: %m");
-                return FALSE;
-        }
-
-        if (ioctl (fd, VT_GETSTATE, &vt_state) < 0) {
-                g_debug ("GdmSessionWorker: couldn't get current VT: %m");
-                goto out;
-        }
-
-        worker->priv->login_vt = vt_state.v_active;
-        got_login_vt = TRUE;
-out:
-        close (fd);
-        return got_login_vt;
-}
-
-static gboolean
 gdm_session_worker_initialize_pam (GdmSessionWorker *worker,
                                    const char       *service,
                                    const char       *username,
@@ -1002,7 +1187,6 @@ gdm_session_worker_initialize_pam (GdmSessionWorker *w
 {
         struct pam_conv        pam_conversation;
         int                    error_code;
-        char tty_string[256];
 
         g_assert (worker->priv->pam_handle == NULL);
 
@@ -1063,10 +1247,12 @@ gdm_session_worker_initialize_pam (GdmSessionWorker *w
                 }
         }
 
+#ifdef WITH_SYSTEMD
         /* set seat ID */
-        if (seat_id != NULL && seat_id[0] != '\0') {
+        if (seat_id != NULL && seat_id[0] != '\0' && LOGIND_RUNNING()) {
                 gdm_session_worker_set_environment_variable (worker, "XDG_SEAT", seat_id);
         }
+#endif
 
         if (strcmp (service, "gdm-launch-environment") == 0) {
                 gdm_session_worker_set_environment_variable (worker, "XDG_SESSION_CLASS", "greeter");
@@ -1075,14 +1261,6 @@ gdm_session_worker_initialize_pam (GdmSessionWorker *w
         g_debug ("GdmSessionWorker: state SETUP_COMPLETE");
         worker->priv->state = GDM_SESSION_WORKER_STATE_SETUP_COMPLETE;
 
-        /* Temporarily set PAM_TTY with the currently active VT (login screen) 
-           PAM_TTY will be reset with the users VT right before the user session is opened */
-        ensure_login_vt (worker);
-        g_snprintf (tty_string, 256, "/dev/tty%d", worker->priv->login_vt);
-        pam_set_item (worker->priv->pam_handle, PAM_TTY, tty_string);
-        if (!display_is_local)
-                worker->priv->password_is_required = TRUE;
-
  out:
         if (error_code != PAM_SUCCESS) {
                 gdm_session_worker_uninitialize_pam (worker, error_code);
@@ -1629,6 +1807,26 @@ gdm_session_worker_get_environment (GdmSessionWorker *
         return (const char * const *) pam_getenvlist (worker->priv->pam_handle);
 }
 
+#ifdef WITH_CONSOLE_KIT
+static void
+register_ck_session (GdmSessionWorker *worker)
+{
+#ifdef WITH_SYSTEMD
+        if (LOGIND_RUNNING()) {
+                return;
+        }
+#endif
+
+        open_ck_session (worker);
+
+        if (worker->priv->session_cookie != NULL) {
+                gdm_session_worker_set_environment_variable (worker,
+                                                             "XDG_SESSION_COOKIE",
+                                                             worker->priv->session_cookie);
+        }
+}
+#endif
+
 static gboolean
 run_script (GdmSessionWorker *worker,
             const char       *dir)
@@ -1659,6 +1857,9 @@ session_worker_child_watch (GPid              pid,
                  : WIFSIGNALED (status) ? WTERMSIG (status)
                  : -1);
 
+#ifdef WITH_CONSOLE_KIT
+        close_ck_session (worker);
+#endif
 
         gdm_session_worker_uninitialize_pam (worker, PAM_SUCCESS);
 
@@ -1847,12 +2048,14 @@ gdm_session_worker_start_session (GdmSessionWorker  *w
 
         error_code = PAM_SUCCESS;
 
+#ifdef WITH_SYSTEMD
         /* If we're in new vt mode, jump to the new vt now. There's no need to jump for
          * the other two modes: in the logind case, the session will activate itself when
          * ready, and in the reuse server case, we're already on the correct VT. */
         if (worker->priv->display_mode == GDM_SESSION_DISPLAY_MODE_NEW_VT) {
                 jump_to_vt (worker, worker->priv->session_vt);
         }
+#endif
 
         session_pid = fork ();
 
@@ -1899,6 +2102,7 @@ gdm_session_worker_start_session (GdmSessionWorker  *w
                         _exit (2);
                 }
 
+#ifdef WITH_SYSTEMD
                 /* Take control of the tty
                  */
                 if (needs_controlling_terminal) {
@@ -1906,6 +2110,7 @@ gdm_session_worker_start_session (GdmSessionWorker  *w
                                 g_debug ("GdmSessionWorker: could not take control of tty: %m");
                         }
                 }
+#endif
 
 #ifdef HAVE_LOGINCAP
                 if (setusercontext (NULL, passwd_entry, passwd_entry->pw_uid, LOGIN_SETALL) < 0) {
@@ -2050,11 +2255,13 @@ gdm_session_worker_start_session (GdmSessionWorker  *w
         return TRUE;
 }
 
+#ifdef WITH_SYSTEMD
 static gboolean
 set_up_for_new_vt (GdmSessionWorker *worker)
 {
         int fd;
         char vt_string[256], tty_string[256];
+        struct vt_stat vt_state = { 0 };
         int session_vt = 0;
 
         fd = open ("/dev/tty0", O_RDWR | O_NOCTTY);
@@ -2064,6 +2271,11 @@ set_up_for_new_vt (GdmSessionWorker *worker)
                 return FALSE;
         }
 
+        if (ioctl (fd, VT_GETSTATE, &vt_state) < 0) {
+                g_debug ("GdmSessionWorker: couldn't get current VT: %m");
+                goto fail;
+        }
+
         if (worker->priv->display_is_initial) {
                 session_vt = atoi (GDM_INITIAL_VT);
         } else {
@@ -2073,6 +2285,7 @@ set_up_for_new_vt (GdmSessionWorker *worker)
                 }
         }
 
+        worker->priv->login_vt = vt_state.v_active;
         worker->priv->session_vt = session_vt;
 
         close (fd);
@@ -2100,6 +2313,7 @@ fail:
         close (fd);
         return FALSE;
 }
+#endif
 
 static gboolean
 set_up_for_current_vt (GdmSessionWorker  *worker,
@@ -2188,6 +2402,7 @@ gdm_session_worker_open_session (GdmSessionWorker  *wo
                         return FALSE;
                 }
                 break;
+#ifdef WITH_SYSTEMD
         case GDM_SESSION_DISPLAY_MODE_NEW_VT:
         case GDM_SESSION_DISPLAY_MODE_LOGIND_MANAGED:
                 if (!set_up_for_new_vt (worker)) {
@@ -2198,6 +2413,7 @@ gdm_session_worker_open_session (GdmSessionWorker  *wo
                         return FALSE;
                 }
                 break;
+#endif
         }
 
         flags = 0;
@@ -2227,7 +2443,9 @@ gdm_session_worker_open_session (GdmSessionWorker  *wo
         g_debug ("GdmSessionWorker: state SESSION_OPENED");
         worker->priv->state = GDM_SESSION_WORKER_STATE_SESSION_OPENED;
 
+#ifdef WITH_SYSTEMD
         session_id = gdm_session_worker_get_environment_variable (worker, "XDG_SESSION_ID");
+#endif
 
         /* FIXME: should we do something here?
          * Note that error return status from PreSession script should
@@ -2237,6 +2455,14 @@ gdm_session_worker_open_session (GdmSessionWorker  *wo
          */
         run_script (worker, GDMCONFDIR "/PreSession");
 
+#ifdef WITH_CONSOLE_KIT
+        register_ck_session (worker);
+
+        if (session_id == NULL) {
+                session_id = get_ck_session_id (worker);
+        }
+#endif
+
         if (session_id != NULL) {
                 g_free (worker->priv->session_id);
                 worker->priv->session_id = session_id;
@@ -2341,6 +2567,19 @@ gdm_session_worker_handle_set_session_name (GdmDBusWor
 }
 
 static gboolean
+gdm_session_worker_handle_set_session_type (GdmDBusWorker         *object,
+                                            GDBusMethodInvocation *invocation,
+                                            const char            *session_type)
+{
+        GdmSessionWorker *worker = GDM_SESSION_WORKER (object);
+        g_debug ("GdmSessionWorker: session type set to %s", session_type);
+        g_free (worker->priv->session_type);
+        worker->priv->session_type = g_strdup (session_type);
+        gdm_dbus_worker_complete_set_session_type (object, invocation);
+        return TRUE;
+}
+
+static gboolean
 gdm_session_worker_handle_set_session_display_mode (GdmDBusWorker         *object,
                                                     GDBusMethodInvocation *invocation,
                                                     const char            *str)
@@ -3147,6 +3386,7 @@ worker_interface_init (GdmDBusWorkerIface *interface)
         interface->handle_open = gdm_session_worker_handle_open;
         interface->handle_set_language_name = gdm_session_worker_handle_set_language_name;
         interface->handle_set_session_name = gdm_session_worker_handle_set_session_name;
+        interface->handle_set_session_type = gdm_session_worker_handle_set_session_type;
         interface->handle_set_session_display_mode = gdm_session_worker_handle_set_session_display_mode;
         interface->handle_set_environment_variable = gdm_session_worker_handle_set_environment_variable;
         interface->handle_start_program = gdm_session_worker_handle_start_program;
