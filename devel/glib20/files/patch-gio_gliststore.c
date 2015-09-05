--- gio/gliststore.c.orig	2015-09-05 18:24:25.301834000 +0200
+++ gio/gliststore.c	2015-09-05 18:25:19.589705000 +0200
@@ -334,7 +334,7 @@ g_list_store_sort (GListStore       *sto
   gint n_items;
 
   g_return_if_fail (G_IS_LIST_STORE (store));
-  g_return_val_if_fail (compare_func != NULL, 0);
+  g_return_if_fail (compare_func != NULL);
 
   g_sequence_sort (store->items, compare_func, user_data);
 
