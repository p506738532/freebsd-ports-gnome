--- capplets/display/xrandr-capplet.c.orig	2014-03-15 14:26:36.000000000 +0100
+++ capplets/display/xrandr-capplet.c	2015-01-19 12:43:53.831895476 +0100
@@ -49,7 +49,7 @@
     MateRRScreen       *screen;
     MateRRConfig  *current_configuration;
     MateRRLabeler *labeler;
-    MateOutputInfo         *current_output;
+    MateRROutputInfo         *current_output;
 
     GtkWidget	   *dialog;
     GtkWidget      *current_monitor_event_box;
@@ -91,13 +91,13 @@
 static void rebuild_gui (App *app);
 static void on_clone_changed (GtkWidget *box, gpointer data);
 static void on_rate_changed (GtkComboBox *box, gpointer data);
-static gboolean output_overlaps (MateOutputInfo *output, MateRRConfig *config);
+static gboolean output_overlaps (MateRROutputInfo *output, MateRRConfig *config);
 static void select_current_output_from_dialog_position (App *app);
 static void monitor_on_off_toggled_cb (GtkToggleButton *toggle, gpointer data);
-static void get_geometry (MateOutputInfo *output, int *w, int *h);
+static void get_geometry (MateRROutputInfo *output, int *w, int *h);
 static void apply_configuration_returned_cb (DBusGProxy *proxy, DBusGProxyCall *call_id, void *data);
 static gboolean get_clone_size (MateRRScreen *screen, int *width, int *height);
-static gboolean output_info_supports_mode (App *app, MateOutputInfo *info, int width, int height);
+static gboolean output_info_supports_mode (App *app, MateRROutputInfo *info, int width, int height);
 
 static void
 error_message (App *app, const char *primary_text, const char *secondary_text)
@@ -139,10 +139,10 @@
     MateRRConfig *current;
     App *app = data;
 
-    current = mate_rr_config_new_current (app->screen);
+    current = mate_rr_config_new_current (app->screen, NULL);
 
     if (app->current_configuration)
-	mate_rr_config_free (app->current_configuration);
+	g_object_unref (app->current_configuration);
 
     app->current_configuration = current;
     app->current_output = NULL;
@@ -278,7 +278,7 @@
 {
     MateRROutput *output;
 
-    if (app->current_configuration->clone)
+    if (mate_rr_config_get_clone (app->current_configuration))
     {
 	return mate_rr_screen_list_clone_modes (app->screen);
     }
@@ -287,8 +287,8 @@
 	if (!app->current_output)
 	    return NULL;
 
-	output = mate_rr_screen_get_output_by_name (
-	    app->screen, app->current_output->name);
+	output = mate_rr_screen_get_output_by_name (app->screen,
+	    mate_rr_output_info_get_name (app->current_output));
 
 	if (!output)
 	    return NULL;
@@ -317,20 +317,20 @@
 
     clear_combo (app->rotation_combo);
 
-    gtk_widget_set_sensitive (
-	app->rotation_combo, app->current_output && app->current_output->on);
+    gtk_widget_set_sensitive (app->rotation_combo,
+                              app->current_output && mate_rr_output_info_is_active (app->current_output));
 
     if (!app->current_output)
 	return;
 
-    current = app->current_output->rotation;
+    current = mate_rr_output_info_get_rotation (app->current_output);
 
     selection = NULL;
     for (i = 0; i < G_N_ELEMENTS (rotations); ++i)
     {
 	const RotationInfo *info = &(rotations[i]);
 
-	app->current_output->rotation = info->rotation;
+	mate_rr_output_info_set_rotation (app->current_output, info->rotation);
 
 	/* NULL-GError --- FIXME: we should say why this rotation is not available! */
 	if (mate_rr_config_applicable (app->current_configuration, app->screen, NULL))
@@ -342,7 +342,7 @@
 	}
     }
 
-    app->current_output->rotation = current;
+    mate_rr_output_info_set_rotation (app->current_output, current);
 
     if (!(selection && combo_select (app->rotation_combo, selection)))
 	combo_select (app->rotation_combo, _("Normal"));
@@ -357,7 +357,6 @@
 static void
 rebuild_rate_combo (App *app)
 {
-    GHashTable *rates;
     MateRRMode **modes;
     int best;
     int i;
@@ -365,27 +364,27 @@
     clear_combo (app->refresh_combo);
 
     gtk_widget_set_sensitive (
-	app->refresh_combo, app->current_output && app->current_output->on);
+	app->refresh_combo, app->current_output && mate_rr_output_info_is_active (app->current_output));
 
     if (!app->current_output
         || !(modes = get_current_modes (app)))
 	return;
 
-    rates = g_hash_table_new_full (
-	g_str_hash, g_str_equal, (GFreeFunc) g_free, NULL);
-
     best = -1;
     for (i = 0; modes[i] != NULL; ++i)
     {
 	MateRRMode *mode = modes[i];
 	int width, height, rate;
+	int output_width, output_height;
+
+	mate_rr_output_info_get_geometry (app->current_output, NULL, NULL, &output_width, &output_height);
 
 	width = mate_rr_mode_get_width (mode);
 	height = mate_rr_mode_get_height (mode);
 	rate = mate_rr_mode_get_freq (mode);
 
-	if (width == app->current_output->width		&&
-	    height == app->current_output->height)
+	if (width == output_width		&&
+	    height == output_height)
 	{
 	    add_key (app->refresh_combo,
 		     idle_free (make_rate_string (rate)),
@@ -396,7 +395,7 @@
 	}
     }
 
-    if (!combo_select (app->refresh_combo, idle_free (make_rate_string (app->current_output->rate))))
+    if (!combo_select (app->refresh_combo, idle_free (make_rate_string (mate_rr_output_info_get_refresh_rate (app->current_output)))))
 	combo_select (app->refresh_combo, idle_free (make_rate_string (best)));
 }
 
@@ -404,11 +403,11 @@
 count_active_outputs (App *app)
 {
     int i, count = 0;
+    MateRROutputInfo **outputs = mate_rr_config_get_outputs (app->current_configuration);
 
-    for (i = 0; app->current_configuration->outputs[i] != NULL; ++i)
+    for (i = 0; outputs[i] != NULL; ++i)
     {
-	MateOutputInfo *output = app->current_configuration->outputs[i];
-	if (output->on)
+	if (mate_rr_output_info_is_active (outputs[i]))
 	    count++;
     }
 
@@ -420,8 +419,9 @@
 count_all_outputs (MateRRConfig *config)
 {
     int i;
+    MateRROutputInfo **outputs = mate_rr_config_get_outputs (config);
 
-    for (i = 0; config->outputs[i] != NULL; i++)
+    for (i = 0; outputs[i] != NULL; i++)
 	;
 
     return i;
@@ -448,18 +448,17 @@
     if (have_clone_size) {
 	int i;
 	int num_outputs_with_clone_size;
+	MateRROutputInfo **outputs = mate_rr_config_get_outputs (app->current_configuration);
 
 	num_outputs_with_clone_size = 0;
 
-	for (i = 0; app->current_configuration->outputs[i] != NULL; i++)
+	for (i = 0; outputs[i] != NULL; i++)
 	{
-	    MateOutputInfo *output = app->current_configuration->outputs[i];
-
 	    /* We count the connected outputs that support the clone size.  It
 	     * doesn't matter if those outputs aren't actually On currently; we
 	     * will turn them on in on_clone_changed().
 	     */
-	    if (output->connected && output_info_supports_mode (app, output, clone_width, clone_height))
+	    if (mate_rr_output_info_is_connected (outputs[i]) && output_info_supports_mode (app, outputs[i], clone_width, clone_height))
 		num_outputs_with_clone_size++;
 	}
 
@@ -478,7 +477,7 @@
 
     g_signal_handlers_block_by_func (app->clone_checkbox, G_CALLBACK (on_clone_changed), app);
 
-    mirror_is_active = app->current_configuration && app->current_configuration->clone;
+    mirror_is_active = app->current_configuration && mate_rr_config_get_clone (app->current_configuration);
 
     /* If mirror_is_active, then it *must* be possible to turn mirroring off */
     mirror_is_supported = mirror_is_active || mirror_screens_is_supported (app);
@@ -493,18 +492,26 @@
 rebuild_current_monitor_label (App *app)
 {
 	char *str, *tmp;
+#if GTK_CHECK_VERSION (3, 0, 0)
+	GdkRGBA color;
+#else
 	GdkColor color;
+#endif
 	gboolean use_color;
 
 	if (app->current_output)
 	{
-	    if (app->current_configuration->clone)
+	    if (mate_rr_config_get_clone (app->current_configuration))
 		tmp = g_strdup (_("Mirror Screens"));
 	    else
-		tmp = g_strdup_printf (_("Monitor: %s"), app->current_output->display_name);
+		tmp = g_strdup_printf (_("Monitor: %s"), mate_rr_output_info_get_display_name (app->current_output));
 
 	    str = g_strdup_printf ("<b>%s</b>", tmp);
+#if GTK_CHECK_VERSION (3, 0, 0)
+	    mate_rr_labeler_get_rgba_for_output (app->labeler, app->current_output, &color);
+#else
 	    mate_rr_labeler_get_color_for_output (app->labeler, app->current_output, &color);
+#endif
 	    use_color = TRUE;
 	    g_free (tmp);
 	}
@@ -519,6 +526,17 @@
 
 	if (use_color)
 	{
+#if GTK_CHECK_VERSION (3, 0, 0)
+	    GdkRGBA black = { 0, 0, 0, 1.0 };
+
+	    gtk_widget_override_background_color (app->current_monitor_event_box, gtk_widget_get_state_flags (app->current_monitor_event_box), &color);
+
+	    /* Make the label explicitly black.  We don't want it to follow the
+	     * theme's colors, since the label is always shown against a light
+	     * pastel background.  See bgo#556050
+	     */
+	    gtk_widget_override_color (app->current_monitor_label, gtk_widget_get_state_flags (app->current_monitor_label), &black);
+#else
 	    GdkColor black = { 0, 0, 0, 0 };
 
 	    gtk_widget_modify_bg (app->current_monitor_event_box, gtk_widget_get_state (app->current_monitor_event_box), &color);
@@ -528,7 +546,9 @@
 	     * pastel background.  See bgo#556050
 	     */
 	    gtk_widget_modify_fg (app->current_monitor_label, gtk_widget_get_state (app->current_monitor_label), &black);
+#endif
 	}
+#if !GTK_CHECK_VERSION (3, 0, 0)
 	else
 	{
 	    /* Remove any modifications we did on the label's color */
@@ -537,6 +557,7 @@
 	    reset_rc_style = gtk_rc_style_new ();
 	    gtk_widget_modify_style (app->current_monitor_label, reset_rc_style); /* takes ownership of, and destroys, the rc style */
 	}
+#endif
 
 	gtk_event_box_set_visible_window (GTK_EVENT_BOX (app->current_monitor_event_box), use_color);
 }
@@ -555,14 +576,14 @@
     on_active = FALSE;
     off_active = FALSE;
 
-    if (!app->current_configuration->clone && app->current_output)
+    if (!mate_rr_config_get_clone (app->current_configuration) && app->current_output)
     {
-	if (count_active_outputs (app) > 1 || !app->current_output->on)
+	if (count_active_outputs (app) > 1 || !mate_rr_output_info_is_active (app->current_output))
 	    sensitive = TRUE;
 	else
 	    sensitive = FALSE;
 
-	on_active = app->current_output->on;
+	on_active = mate_rr_output_info_is_active (app->current_output);
 	off_active = !on_active;
     }
 
@@ -611,19 +632,22 @@
     int i;
     MateRRMode **modes;
     const char *current;
+    int output_width, output_height;
 
     clear_combo (app->resolution_combo);
 
     if (!(modes = get_current_modes (app))
 	|| !app->current_output
-	|| !app->current_output->on)
+	|| !mate_rr_output_info_is_active (app->current_output))
     {
 	gtk_widget_set_sensitive (app->resolution_combo, FALSE);
 	return;
     }
 
     g_assert (app->current_output != NULL);
-    g_assert (app->current_output->width != 0 && app->current_output->height != 0);
+
+    mate_rr_output_info_get_geometry (app->current_output, NULL, NULL, &output_width, &output_height);
+    g_assert (output_width != 0 && output_height != 0);
 
     gtk_widget_set_sensitive (app->resolution_combo, TRUE);
 
@@ -639,7 +663,7 @@
 		 width, height, 0, -1);
     }
 
-    current = idle_free (make_resolution_string (app->current_output->width, app->current_output->height));
+    current = idle_free (make_resolution_string (output_width, output_height));
 
     if (!combo_select (app->resolution_combo, current))
     {
@@ -665,7 +689,7 @@
     sensitive = app->current_output ? TRUE : FALSE;
 
 #if 0
-    g_debug ("rebuild gui, is on: %d", app->current_output->on);
+    g_debug ("rebuild gui, is on: %d", mate_rr_output_info_is_active (app->current_output));
 #endif
 
     rebuild_mirror_screens (app);
@@ -676,7 +700,7 @@
     rebuild_rotation_combo (app);
 
 #if 0
-    g_debug ("sensitive: %d, on: %d", sensitive, app->current_output->on);
+    g_debug ("sensitive: %d, on: %d", sensitive, mate_rr_output_info_is_active (app->current_output));
 #endif
     gtk_widget_set_sensitive (app->panel_checkbox, sensitive);
 
@@ -728,7 +752,7 @@
 	return;
 
     if (get_mode (app->rotation_combo, NULL, NULL, NULL, &rotation))
-	app->current_output->rotation = rotation;
+	mate_rr_output_info_set_rotation (app->current_output, rotation);
 
     foo_scroll_area_invalidate (FOO_SCROLL_AREA (app->area));
 }
@@ -743,7 +767,7 @@
 	return;
 
     if (get_mode (app->refresh_combo, NULL, NULL, &rate, NULL))
-	app->current_output->rate = rate;
+	rate = mate_rr_output_info_get_refresh_rate (app->current_output);
 
     foo_scroll_area_invalidate (FOO_SCROLL_AREA (app->area));
 }
@@ -753,11 +777,15 @@
 {
     MateRRMode **modes;
     int width, height;
+    int x, y;
+    mate_rr_output_info_get_geometry (app->current_output, &x, &y, NULL, NULL);
+
+    width = mate_rr_output_info_get_preferred_width (app->current_output);
+    height = mate_rr_output_info_get_preferred_height (app->current_output);
 
-    if (app->current_output->pref_width != 0 && app->current_output->pref_height != 0)
+    if (width != 0 && height != 0)
     {
-	app->current_output->width = app->current_output->pref_width;
-	app->current_output->height = app->current_output->pref_height;
+	mate_rr_output_info_set_geometry (app->current_output, x, y, width, height);
 	return;
     }
 
@@ -767,8 +795,7 @@
 
     find_best_mode (modes, &width, &height);
 
-    app->current_output->width = width;
-    app->current_output->height = height;
+    mate_rr_output_info_set_geometry (app->current_output, x, y, width, height);
 }
 
 static void
@@ -793,7 +820,7 @@
 	return;
     }
 
-    app->current_output->on = is_on;
+    mate_rr_output_info_set_active (app->current_output, is_on);
 
     if (is_on)
 	select_resolution_for_current_output (app); /* The refresh rate will be picked in rebuild_rate_combo() */
@@ -803,7 +830,7 @@
 }
 
 static void
-realign_outputs_after_resolution_change (App *app, MateOutputInfo *output_that_changed, int old_width, int old_height)
+realign_outputs_after_resolution_change (App *app, MateRROutputInfo *output_that_changed, int old_width, int old_height)
 {
     /* We find the outputs that were below or to the right of the output that
      * changed, and realign them; we also do that for outputs that shared the
@@ -814,38 +841,45 @@
     int i;
     int old_right_edge, old_bottom_edge;
     int dx, dy;
+    int x, y, width, height;
+    MateRROutputInfo **outputs;
 
     g_assert (app->current_configuration != NULL);
 
-    if (output_that_changed->width == old_width && output_that_changed->height == old_height)
+    mate_rr_output_info_get_geometry (output_that_changed, &x, &y, &width, &height);
+    if (width == old_width && height == old_height)
 	return;
 
-    old_right_edge = output_that_changed->x + old_width;
-    old_bottom_edge = output_that_changed->y + old_height;
+    old_right_edge = x + old_width;
+    old_bottom_edge = y + old_height;
 
-    dx = output_that_changed->width - old_width;
-    dy = output_that_changed->height - old_height;
+    dx = width - old_width;
+    dy = height - old_height;
 
-    for (i = 0; app->current_configuration->outputs[i] != NULL; i++) {
-	MateOutputInfo *output;
+    outputs = mate_rr_config_get_outputs (app->current_configuration);
+
+    for (i = 0; outputs[i] != NULL; i++)
+      {
+        int output_x, output_y;
 	int output_width, output_height;
 
-	output = app->current_configuration->outputs[i];
+	if (outputs[i] == output_that_changed || mate_rr_output_info_is_connected (outputs[i]))
+	  continue;
 
-	if (output == output_that_changed || !output->connected)
-	    continue;
+	mate_rr_output_info_get_geometry (outputs[i], &output_x, &output_y, &output_width, &output_height);
 
-	get_geometry (output, &output_width, &output_height);
+	if (output_x >= old_right_edge)
+	  output_x += dx;
+	else if (output_x + output_width == old_right_edge)
+	  output_x = x + width - output_width;
 
-	if (output->x >= old_right_edge)
-	    output->x += dx;
-	else if (output->x + output_width == old_right_edge)
-	    output->x = output_that_changed->x + output_that_changed->width - output_width;
-
-	if (output->y >= old_bottom_edge)
-	    output->y += dy;
-	else if (output->y + output_height == old_bottom_edge)
-	    output->y = output_that_changed->y + output_that_changed->height - output_height;
+
+	if (output_y >= old_bottom_edge)
+	    output_y += dy;
+	else if (output_y + output_height == old_bottom_edge)
+	    output_y = y + height - output_height;
+
+	mate_rr_output_info_set_geometry (outputs[i], output_x, output_y, output_width, output_height);
     }
 }
 
@@ -854,24 +888,23 @@
 {
     App *app = data;
     int old_width, old_height;
+    int x, y;
     int width;
     int height;
 
     if (!app->current_output)
 	return;
 
-    old_width = app->current_output->width;
-    old_height = app->current_output->height;
+    mate_rr_output_info_get_geometry (app->current_output, &x, &y, &old_width, &old_height);
 
     if (get_mode (app->resolution_combo, &width, &height, NULL, NULL))
     {
-	app->current_output->width = width;
-	app->current_output->height = height;
+	mate_rr_output_info_set_geometry (app->current_output, x, y, width, height);
 
 	if (width == 0 || height == 0)
-	    app->current_output->on = FALSE;
+	    mate_rr_output_info_set_active (app->current_output, FALSE);
 	else
-	    app->current_output->on = TRUE;
+	    mate_rr_output_info_set_active (app->current_output, TRUE);
     }
 
     realign_outputs_after_resolution_change (app, app->current_output, old_width, old_height);
@@ -887,6 +920,7 @@
 {
     int i;
     int x;
+    MateRROutputInfo **outputs;
 
     /* Lay out all the monitors horizontally when "mirror screens" is turned
      * off, to avoid having all of them overlapped initially.  We put the
@@ -896,30 +930,29 @@
     x = 0;
 
     /* First pass, all "on" outputs */
+    outputs = mate_rr_config_get_outputs (app->current_configuration);
 
-    for (i = 0; app->current_configuration->outputs[i]; ++i)
+    for (i = 0; outputs[i]; ++i)
     {
-	MateOutputInfo *output;
-
-	output = app->current_configuration->outputs[i];
-	if (output->connected && output->on) {
-	    output->x = x;
-	    output->y = 0;
-	    x += output->width;
+	int width, height;
+	if (mate_rr_output_info_is_connected (outputs[i]) &&mate_rr_output_info_is_active (outputs[i]))
+	{
+	    mate_rr_output_info_get_geometry (outputs[i], NULL, NULL, &width, &height);
+	    mate_rr_output_info_set_geometry (outputs[i], x, 0, width, height);
+	    x += width;
 	}
     }
 
     /* Second pass, all the black screens */
 
-    for (i = 0; app->current_configuration->outputs[i]; ++i)
+    for (i = 0; outputs[i]; ++i)
     {
-	MateOutputInfo *output;
-
-	output = app->current_configuration->outputs[i];
-	if (!(output->connected && output->on)) {
-	    output->x = x;
-	    output->y = 0;
-	    x += output->width;
+	int width, height;
+	if (!(mate_rr_output_info_is_connected (outputs[i]) && mate_rr_output_info_is_active (outputs[i])))
+	  {
+	    mate_rr_output_info_get_geometry (outputs[i], NULL, NULL, &width, &height);
+	    mate_rr_output_info_set_geometry (outputs[i], x, 0, width, height);
+	    x += width;
 	}
     }
 
@@ -964,16 +997,16 @@
 }
 
 static gboolean
-output_info_supports_mode (App *app, MateOutputInfo *info, int width, int height)
+output_info_supports_mode (App *app, MateRROutputInfo *info, int width, int height)
 {
     MateRROutput *output;
     MateRRMode **modes;
     int i;
 
-    if (!info->connected)
+    if (!mate_rr_output_info_is_connected (info))
 	return FALSE;
 
-    output = mate_rr_screen_get_output_by_name (app->screen, info->name);
+    output = mate_rr_screen_get_output_by_name (app->screen, mate_rr_output_info_get_name (info));
     if (!output)
 	return FALSE;
 
@@ -993,19 +1026,19 @@
 {
     App *app = data;
 
-    app->current_configuration->clone =
-	gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (app->clone_checkbox));
+    mate_rr_config_set_clone (app->current_configuration, gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (app->clone_checkbox)));
 
-    if (app->current_configuration->clone)
+    if (mate_rr_config_get_clone (app->current_configuration))
     {
 	int i;
 	int width, height;
+	MateRROutputInfo **outputs = mate_rr_config_get_outputs (app->current_configuration);
 
-	for (i = 0; app->current_configuration->outputs[i]; ++i)
+	for (i = 0; outputs[i]; ++i)
 	{
-	    if (app->current_configuration->outputs[i]->connected)
+	    if (mate_rr_output_info_is_connected(outputs[i]))
 	    {
-		app->current_output = app->current_configuration->outputs[i];
+		app->current_output = outputs[i];
 		break;
 	    }
 	}
@@ -1017,11 +1050,12 @@
 
 	get_clone_size (app->screen, &width, &height);
 
-	for (i = 0; app->current_configuration->outputs[i]; i++) {
-	    if (output_info_supports_mode (app, app->current_configuration->outputs[i], width, height)) {
-		app->current_configuration->outputs[i]->on = TRUE;
-		app->current_configuration->outputs[i]->width = width;
-		app->current_configuration->outputs[i]->height = height;
+	for (i = 0; outputs[i]; i++) {
+	    int x, y;
+	    if (output_info_supports_mode (app, outputs[i], width, height)) {
+		mate_rr_output_info_set_active (outputs[i], TRUE);
+		mate_rr_output_info_get_geometry (outputs[i], &x, &y, NULL, NULL);
+		mate_rr_output_info_set_geometry (outputs[i], x, y, width, height);
 	    }
 	}
     }
@@ -1035,19 +1069,21 @@
 }
 
 static void
-get_geometry (MateOutputInfo *output, int *w, int *h)
+get_geometry (MateRROutputInfo *output, int *w, int *h)
 {
-    if (output->on)
+    MateRRRotation rotation;
+
+    if (mate_rr_output_info_is_active (output))
     {
-	*h = output->height;
-	*w = output->width;
+	mate_rr_output_info_get_geometry (output, NULL, NULL, w, h);
     }
     else
     {
-	*h = output->pref_height;
-	*w = output->pref_width;
+	*h = mate_rr_output_info_get_preferred_height (output);
+	*w = mate_rr_output_info_get_preferred_width (output);
     }
-   if ((output->rotation & MATE_RR_ROTATION_90) || (output->rotation & MATE_RR_ROTATION_270))
+   rotation = mate_rr_output_info_get_rotation (output);
+   if ((rotation & MATE_RR_ROTATION_90) || (rotation & MATE_RR_ROTATION_270))
    {
         int tmp;
         tmp = *h;
@@ -1064,6 +1100,7 @@
 {
     int i, dummy;
     GList *result = NULL;
+    MateRROutputInfo **outputs;
 
     if (!total_w)
 	total_w = &dummy;
@@ -1072,17 +1109,17 @@
 
     *total_w = 0;
     *total_h = 0;
-    for (i = 0; app->current_configuration->outputs[i] != NULL; ++i)
-    {
-	MateOutputInfo *output = app->current_configuration->outputs[i];
 
-	if (output->connected)
+    outputs = mate_rr_config_get_outputs(app->current_configuration);
+    for (i = 0; outputs[i] != NULL; ++i)
+    {
+	if (mate_rr_output_info_is_connected (outputs[i]))
 	{
 	    int w, h;
 
-	    result = g_list_prepend (result, output);
+	    result = g_list_prepend (result, outputs[i]);
 
-	    get_geometry (output, &w, &h);
+	    get_geometry (outputs[i], &w, &h);
 
 	    *total_w += w;
 	    *total_h += h;
@@ -1128,7 +1165,7 @@
 
 typedef struct Edge
 {
-    MateOutputInfo *output;
+    MateRROutputInfo *output;
     int x1, y1;
     int x2, y2;
 } Edge;
@@ -1141,7 +1178,7 @@
 } Snap;
 
 static void
-add_edge (MateOutputInfo *output, int x1, int y1, int x2, int y2, GArray *edges)
+add_edge (MateRROutputInfo *output, int x1, int y1, int x2, int y2, GArray *edges)
 {
     Edge e;
 
@@ -1155,13 +1192,11 @@
 }
 
 static void
-list_edges_for_output (MateOutputInfo *output, GArray *edges)
+list_edges_for_output (MateRROutputInfo *output, GArray *edges)
 {
     int x, y, w, h;
 
-    x = output->x;
-    y = output->y;
-    get_geometry (output, &w, &h);
+    mate_rr_output_info_get_geometry (output, &x, &y, &w, &h);
 
     /* Top, Bottom, Left, Right */
     add_edge (output, x, y, x + w, y, edges);
@@ -1174,13 +1209,12 @@
 list_edges (MateRRConfig *config, GArray *edges)
 {
     int i;
+    MateRROutputInfo **outputs = mate_rr_config_get_outputs (config);
 
-    for (i = 0; config->outputs[i]; ++i)
+    for (i = 0; outputs[i]; ++i)
     {
-	MateOutputInfo *output = config->outputs[i];
-
-	if (output->connected)
-	    list_edges_for_output (output, edges);
+	if (mate_rr_output_info_is_connected (outputs[i]))
+	    list_edges_for_output (outputs[i], edges);
     }
 }
 
@@ -1265,7 +1299,7 @@
 }
 
 static void
-list_snaps (MateOutputInfo *output, GArray *edges, GArray *snaps)
+list_snaps (MateRROutputInfo *output, GArray *edges, GArray *snaps)
 {
     int i;
 
@@ -1321,7 +1355,7 @@
 }
 
 static gboolean
-output_is_aligned (MateOutputInfo *output, GArray *edges)
+output_is_aligned (MateRROutputInfo *output, GArray *edges)
 {
     gboolean result = FALSE;
     int i;
@@ -1358,35 +1392,28 @@
 }
 
 static void
-get_output_rect (MateOutputInfo *output, GdkRectangle *rect)
+get_output_rect (MateRROutputInfo *output, GdkRectangle *rect)
 {
-    int w, h;
-
-    get_geometry (output, &w, &h);
-
-    rect->width = w;
-    rect->height = h;
-    rect->x = output->x;
-    rect->y = output->y;
+    mate_rr_output_info_get_geometry (output, &rect->x, &rect->y, &rect->width, &rect->height);
 }
 
 static gboolean
-output_overlaps (MateOutputInfo *output, MateRRConfig *config)
+output_overlaps (MateRROutputInfo *output, MateRRConfig *config)
 {
     int i;
     GdkRectangle output_rect;
+    MateRROutputInfo **outputs;
 
     get_output_rect (output, &output_rect);
 
-    for (i = 0; config->outputs[i]; ++i)
+    outputs = mate_rr_config_get_outputs (config);
+    for (i = 0; outputs[i]; ++i)
     {
-	MateOutputInfo *other = config->outputs[i];
-
-	if (other != output && other->connected)
+	if (outputs[i] != output && mate_rr_output_info_is_connected (outputs[i]))
 	{
 	    GdkRectangle other_rect;
 
-	    get_output_rect (other, &other_rect);
+	    get_output_rect (outputs[i], &other_rect);
 	    if (gdk_rectangle_intersect (&output_rect, &other_rect, NULL))
 		return TRUE;
 	}
@@ -1400,17 +1427,17 @@
 {
     int i;
     gboolean result = TRUE;
+    MateRROutputInfo **outputs;
 
-    for (i = 0; config->outputs[i]; ++i)
+    outputs = mate_rr_config_get_outputs(config);
+    for (i = 0; outputs[i]; ++i)
     {
-	MateOutputInfo *output = config->outputs[i];
-
-	if (output->connected)
+	if (mate_rr_output_info_is_connected (outputs[i]))
 	{
-	    if (!output_is_aligned (output, edges))
+	    if (!output_is_aligned (outputs[i], edges))
 		return FALSE;
 
-	    if (output_overlaps (output, config))
+	    if (output_overlaps (outputs[i], config))
 		return FALSE;
 	}
     }
@@ -1515,14 +1542,14 @@
 		 FooScrollAreaEvent *event,
 		 gpointer data)
 {
-    MateOutputInfo *output = data;
+    MateRROutputInfo *output = data;
     App *app = g_object_get_data (G_OBJECT (area), "app");
 
     /* If the mouse is inside the outputs, set the cursor to "you can move me".  See
      * on_canvas_event() for where we reset the cursor to the default if it
      * exits the outputs' area.
      */
-    if (!app->current_configuration->clone && get_n_connected (app) > 1)
+    if (!mate_rr_config_get_clone (app->current_configuration) && get_n_connected (app) > 1)
 	set_cursor (GTK_WIDGET (area), GDK_FLEUR);
 
     if (event->type == FOO_BUTTON_PRESS)
@@ -1534,17 +1561,20 @@
 	rebuild_gui (app);
 	set_monitors_tooltip (app, TRUE);
 
-	if (!app->current_configuration->clone && get_n_connected (app) > 1)
+	if (!mate_rr_config_get_clone (app->current_configuration) && get_n_connected (app) > 1)
 	{
+	    int output_x, output_y;
+	    mate_rr_output_info_get_geometry (output, &output_x, &output_y, NULL, NULL);
+
 	    foo_scroll_area_begin_grab (area, on_output_event, data);
 
 	    info = g_new0 (GrabInfo, 1);
 	    info->grab_x = event->x;
 	    info->grab_y = event->y;
-	    info->output_x = output->x;
-	    info->output_y = output->y;
+	    info->output_x = output_x;
+	    info->output_y = output_y;
 
-	    output->user_data = info;
+	    g_object_set_data (G_OBJECT (output), "grab-info", info);
 	}
 
 	foo_scroll_area_invalidate (area);
@@ -1553,20 +1583,19 @@
     {
 	if (foo_scroll_area_is_grabbed (area))
 	{
-	    GrabInfo *info = output->user_data;
+	    GrabInfo *info = g_object_get_data (G_OBJECT (output), "grab-info");
 	    double scale = compute_scale (app);
 	    int old_x, old_y;
+	    int width, height;
 	    int new_x, new_y;
 	    int i;
 	    GArray *edges, *snaps, *new_edges;
 
-	    old_x = output->x;
-	    old_y = output->y;
+	    mate_rr_output_info_get_geometry (output, &old_x, &old_y, &width, &height);
 	    new_x = info->output_x + (event->x - info->grab_x) / scale;
 	    new_y = info->output_y + (event->y - info->grab_y) / scale;
 
-	    output->x = new_x;
-	    output->y = new_y;
+	    mate_rr_output_info_set_geometry (output, new_x, new_y, width, height);
 
 	    edges = g_array_new (TRUE, TRUE, sizeof (Edge));
 	    snaps = g_array_new (TRUE, TRUE, sizeof (Snap));
@@ -1577,16 +1606,14 @@
 
 	    g_array_sort (snaps, compare_snaps);
 
-	    output->x = info->output_x;
-	    output->y = info->output_y;
+	    mate_rr_output_info_set_geometry (output, new_x, new_y, width, height);
 
 	    for (i = 0; i < snaps->len; ++i)
 	    {
 		Snap *snap = &(g_array_index (snaps, Snap, i));
 		GArray *new_edges = g_array_new (TRUE, TRUE, sizeof (Edge));
 
-		output->x = new_x + snap->dx;
-		output->y = new_y + snap->dy;
+		mate_rr_output_info_set_geometry (output, new_x + snap->dx, new_y + snap->dy, width, height);
 
 		g_array_set_size (new_edges, 0);
 		list_edges (app->current_configuration, new_edges);
@@ -1598,8 +1625,7 @@
 		}
 		else
 		{
-		    output->x = info->output_x;
-		    output->y = info->output_y;
+		    mate_rr_output_info_set_geometry (output, info->output_x, info->output_y, width, height);
 		}
 	    }
 
@@ -1612,8 +1638,8 @@
 		foo_scroll_area_end_grab (area);
 		set_monitors_tooltip (app, FALSE);
 
-		g_free (output->user_data);
-		output->user_data = NULL;
+		g_free (g_object_get_data (G_OBJECT (output), "grab-info"));
+		g_object_set_data (G_OBJECT (output), "grab-info", NULL);
 
 #if 0
 		g_debug ("new position: %d %d %d %d", output->x, output->y, output->width, output->height);
@@ -1639,11 +1665,11 @@
 
 static PangoLayout *
 get_display_name (App *app,
-		  MateOutputInfo *output)
+		  MateRROutputInfo *output)
 {
     const char *text;
 
-    if (app->current_configuration->clone) {
+    if (mate_rr_config_get_clone (app->current_configuration)) {
 	/* Translators:  this is the feature where what you see on your laptop's
 	 * screen is the same as your external monitor.  Here, "Mirror" is being
 	 * used as an adjective, not as a verb.  For example, the Spanish
@@ -1651,7 +1677,7 @@
 	 */
 	text = _("Mirror Screens");
     } else
-	text = output->display_name;
+	text = mate_rr_output_info_get_display_name (output);
 
     return gtk_widget_create_pango_layout (
 	GTK_WIDGET (app->area), text);
@@ -1663,17 +1689,36 @@
 {
     GdkRectangle viewport;
     GtkWidget *widget;
+#if GTK_CHECK_VERSION (3, 0, 0)
+    GtkStyleContext *widget_style;
+    GdkRGBA *base_color = NULL;
+    GdkRGBA dark_color;
+#else
     GtkStyle *widget_style;
+#endif
 
     widget = GTK_WIDGET (area);
 
     foo_scroll_area_get_viewport (area, &viewport);
+
+#if GTK_CHECK_VERSION (3, 0, 0)
+    widget_style = gtk_widget_get_style_context (widget);
+#else
     widget_style = gtk_widget_get_style (widget);
+#endif
 
+#if GTK_CHECK_VERSION (3, 0, 0)
+    gtk_style_context_get (widget_style, GTK_STATE_FLAG_SELECTED,
+			   GTK_STYLE_PROPERTY_BACKGROUND_COLOR, &base_color,
+			   NULL);
+    gdk_cairo_set_source_rgba(cr, base_color);
+    gdk_rgba_free (base_color);
+#else
     cairo_set_source_rgb (cr,
                           widget_style->base[GTK_STATE_SELECTED].red / 65535.0,
                           widget_style->base[GTK_STATE_SELECTED].green / 65535.0,
                           widget_style->base[GTK_STATE_SELECTED].blue / 65535.0);
+#endif
 
     cairo_rectangle (cr,
 		     viewport.x, viewport.y,
@@ -1683,10 +1728,15 @@
 
     foo_scroll_area_add_input_from_fill (area, cr, on_canvas_event, NULL);
 
+#if GTK_CHECK_VERSION (3, 0, 0)
+    mate_desktop_gtk_style_get_dark_color (widget_style, GTK_STATE_FLAG_SELECTED, &dark_color);
+    gdk_cairo_set_source_rgba (cr, &dark_color);
+#else
     cairo_set_source_rgb (cr,
                           widget_style->dark[GTK_STATE_SELECTED].red / 65535.0,
                           widget_style->dark[GTK_STATE_SELECTED].green / 65535.0,
                           widget_style->dark[GTK_STATE_SELECTED].blue / 65535.0);
+#endif
 
     cairo_stroke (cr);
 }
@@ -1697,13 +1747,19 @@
     int w, h;
     double scale = compute_scale (app);
     double x, y;
+    int output_x, output_y;
+    MateRRRotation rotation;
     int total_w, total_h;
     GList *connected_outputs = list_connected_outputs (app, &total_w, &total_h);
-    MateOutputInfo *output = g_list_nth (connected_outputs, i)->data;
+    MateRROutputInfo *output = g_list_nth_data (connected_outputs, i);
     PangoLayout *layout = get_display_name (app, output);
     PangoRectangle ink_extent, log_extent;
     GdkRectangle viewport;
+#if GTK_CHECK_VERSION (3, 0, 0)
+    GdkRGBA output_color;
+#else
     GdkColor output_color;
+#endif
     double r, g, b;
     double available_w;
     double factor;
@@ -1722,8 +1778,9 @@
     viewport.height -= 2 * MARGIN;
     viewport.width -= 2 * MARGIN;
 
-    x = output->x * scale + MARGIN + (viewport.width - total_w * scale) / 2.0;
-    y = output->y * scale + MARGIN + (viewport.height - total_h * scale) / 2.0;
+    mate_rr_output_info_get_geometry (output, &output_x, &output_y, NULL, NULL);
+    x = output_x * scale + MARGIN + (viewport.width - total_w * scale) / 2.0;
+    y = output_y * scale + MARGIN + (viewport.height - total_h * scale) / 2.0;
 
 #if 0
     g_debug ("scaled: %f %f", x, y);
@@ -1733,18 +1790,17 @@
     g_debug ("%f %f %f %f", x, y, w * scale + 0.5, h * scale + 0.5);
 #endif
 
-    cairo_save (cr);
-
     cairo_translate (cr,
 		     x + (w * scale + 0.5) / 2,
 		     y + (h * scale + 0.5) / 2);
 
     /* rotation is already applied in get_geometry */
 
-    if (output->rotation & MATE_RR_REFLECT_X)
+    rotation = mate_rr_output_info_get_rotation (output);
+    if (rotation & MATE_RR_REFLECT_X)
 	cairo_scale (cr, -1, 1);
 
-    if (output->rotation & MATE_RR_REFLECT_Y)
+    if (rotation & MATE_RR_REFLECT_Y)
 	cairo_scale (cr, 1, -1);
 
     cairo_translate (cr,
@@ -1755,12 +1811,19 @@
     cairo_rectangle (cr, x, y, w * scale + 0.5, h * scale + 0.5);
     cairo_clip_preserve (cr);
 
+#if GTK_CHECK_VERSION (3, 0, 0)
+    mate_rr_labeler_get_rgba_for_output (app->labeler, output, &output_color);
+    r = output_color.red;
+    g = output_color.green;
+    b = output_color.blue;
+#else
     mate_rr_labeler_get_color_for_output (app->labeler, output, &output_color);
     r = output_color.red / 65535.0;
     g = output_color.green / 65535.0;
     b = output_color.blue / 65535.0;
+#endif
 
-    if (!output->on)
+    if (!mate_rr_output_info_is_active (output))
     {
 	/* If the output is turned off, just darken the selected color */
 	r *= 0.2;
@@ -1806,7 +1869,7 @@
 
     cairo_scale (cr, factor, factor);
 
-    if (output->on)
+    if (mate_rr_output_info_is_active (output))
 	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
     else
 	cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
@@ -1818,12 +1881,19 @@
     g_object_unref (layout);
 }
 
+#if GTK_CHECK_VERSION (3, 0, 0)
+static void
+on_area_paint (FooScrollArea *area,
+	       cairo_t	     *cr,
+	       gpointer	      data)
+#else
 static void
 on_area_paint (FooScrollArea *area,
 	       cairo_t	     *cr,
 	       GdkRectangle  *extent,
 	       GdkRegion     *region,
 	       gpointer	      data)
+#endif
 {
     App *app = data;
     double scale;
@@ -1846,7 +1916,7 @@
     {
 	paint_output (app, cr, g_list_position (connected_outputs, list));
 
-	if (app->current_configuration->clone)
+	if (mate_rr_config_get_clone (app->current_configuration))
 	    break;
     }
 }
@@ -1889,19 +1959,19 @@
 {
     int i;
     int width, height;
+    int output_x, output_y, output_width, output_height;
+    MateRROutputInfo **outputs;
 
     width = height = 0;
 
-    for (i = 0; config->outputs[i] != NULL; i++)
+    outputs = mate_rr_config_get_outputs (config);
+    for (i = 0; outputs[i] != NULL; i++)
     {
-	MateOutputInfo *output;
-
-	output = config->outputs[i];
-
-	if (output->on)
+	if (mate_rr_output_info_is_active (outputs[i]))
 	{
-	    width = MAX (width, output->x + output->width);
-	    height = MAX (height, output->y + output->height);
+	    mate_rr_output_info_get_geometry (outputs[i], &output_x, &output_y, &output_width, &output_height);
+	    width = MAX (width, output_x + output_width);
+	    height = MAX (height, output_y + output_height);
 	}
     }
 
@@ -2000,15 +2070,15 @@
          * that there *will* be a backup file in the end.
          */
 
-        rr_screen = mate_rr_screen_new (gdk_screen_get_default (), NULL, NULL, NULL); /* NULL-GError */
+        rr_screen = mate_rr_screen_new (gdk_screen_get_default (), NULL); /* NULL-GError */
         if (!rr_screen)
                 return;
 
-        rr_config = mate_rr_config_new_current (rr_screen);
+        rr_config = mate_rr_config_new_current (rr_screen, NULL);
         mate_rr_config_save (rr_config, NULL); /* NULL-GError */
 
-        mate_rr_config_free (rr_config);
-        mate_rr_screen_destroy (rr_screen);
+        g_object_unref (rr_config);
+        g_object_unref (rr_screen);
 }
 
 /* Callback for dbus_g_proxy_begin_call() */
@@ -2121,7 +2191,7 @@
      * or is mate_rr_screen_new()'s return value sufficient?
      */
 
-    return (count_all_outputs (config) == 1 && strcmp (config->outputs[0]->name, "default") == 0);
+    return (count_all_outputs (config) == 1 && strcmp (mate_rr_output_info_get_name (mate_rr_config_get_outputs (config)[0]), "default") == 0);
 }
 #endif
 
@@ -2154,37 +2224,38 @@
 			   gtk_toggle_button_get_active (tb));
 }
 
-static MateOutputInfo *
+static MateRROutputInfo *
 get_nearest_output (MateRRConfig *configuration, int x, int y)
 {
     int i;
     int nearest_index;
     int nearest_dist;
+    MateRROutputInfo **outputs;
 
     nearest_index = -1;
     nearest_dist = G_MAXINT;
 
-    for (i = 0; configuration->outputs[i] != NULL; i++)
+    outputs = mate_rr_config_get_outputs (configuration);
+    for (i = 0; outputs[i] != NULL; i++)
     {
-	MateOutputInfo *output;
 	int dist_x, dist_y;
+	int output_x, output_y, output_width, output_height;
 
-	output = configuration->outputs[i];
-
-	if (!(output->connected && output->on))
+	if (!(mate_rr_output_info_is_connected(outputs[i]) && mate_rr_output_info_is_active (outputs[i])))
 	    continue;
 
-	if (x < output->x)
-	    dist_x = output->x - x;
-	else if (x >= output->x + output->width)
-	    dist_x = x - (output->x + output->width) + 1;
+	mate_rr_output_info_get_geometry (outputs[i], &output_x, &output_y, &output_width, &output_height);
+	if (x < output_x)
+	    dist_x = output_x - x;
+	else if (x >= output_x + output_width)
+	    dist_x = x - (output_x + output_width) + 1;
 	else
 	    dist_x = 0;
 
-	if (y < output->y)
-	    dist_y = output->y - y;
-	else if (y >= output->y + output->height)
-	    dist_y = y - (output->y + output->height) + 1;
+	if (y < output_y)
+	    dist_y = output_y - y;
+	else if (y >= output_y + output_height)
+	    dist_y = y - (output_y + output_height) + 1;
 	else
 	    dist_y = 0;
 
@@ -2196,7 +2267,7 @@
     }
 
     if (nearest_index != -1)
-	return configuration->outputs[nearest_index];
+	return outputs[nearest_index];
     else
 	return NULL;
 
@@ -2205,13 +2276,14 @@
 /* Gets the output that contains the largest intersection with the window.
  * Logic stolen from gdk_screen_get_monitor_at_window().
  */
-static MateOutputInfo *
+static MateRROutputInfo *
 get_output_for_window (MateRRConfig *configuration, GdkWindow *window)
 {
     GdkRectangle win_rect;
     int i;
     int largest_area;
     int largest_index;
+    MateRROutputInfo **outputs;
 
 #if GTK_CHECK_VERSION (3, 0, 0)
     gdk_window_get_geometry (window, &win_rect.x, &win_rect.y, &win_rect.width, &win_rect.height);
@@ -2223,19 +2295,14 @@
     largest_area = 0;
     largest_index = -1;
 
-    for (i = 0; configuration->outputs[i] != NULL; i++)
+    outputs = mate_rr_config_get_outputs (configuration);
+    for (i = 0; outputs[i] != NULL; i++)
     {
-	MateOutputInfo *output;
 	GdkRectangle output_rect, intersection;
 
-	output = configuration->outputs[i];
-
-	output_rect.x	   = output->x;
-	output_rect.y	   = output->y;
-	output_rect.width  = output->width;
-	output_rect.height = output->height;
+	mate_rr_output_info_get_geometry (outputs[i], &output_rect.x, &output_rect.y, &output_rect.width, &output_rect.height);
 
-	if (output->connected && gdk_rectangle_intersect (&win_rect, &output_rect, &intersection))
+	if (mate_rr_output_info_is_connected (outputs[i]) && gdk_rectangle_intersect (&win_rect, &output_rect, &intersection))
 	{
 	    int area;
 
@@ -2249,7 +2316,7 @@
     }
 
     if (largest_index != -1)
-	return configuration->outputs[largest_index];
+	return outputs[largest_index];
     else
 	return get_nearest_output (configuration,
 				   win_rect.x + win_rect.width / 2,
@@ -2422,8 +2489,8 @@
 	return;
     }
 
-    app->screen = mate_rr_screen_new (gdk_screen_get_default (),
-				       on_screen_changed, app, &error);
+    app->screen = mate_rr_screen_new (gdk_screen_get_default (), &error);
+    g_signal_connect (app->screen, "changed", G_CALLBACK (on_screen_changed), app);
     if (!app->screen)
     {
 	error_message (NULL, _("Could not get screen information"), error->message);
@@ -2553,7 +2620,7 @@
     }
 
     gtk_widget_destroy (app->dialog);
-    mate_rr_screen_destroy (app->screen);
+    g_object_unref (app->screen);
     g_object_unref (app->settings);
 }
 
