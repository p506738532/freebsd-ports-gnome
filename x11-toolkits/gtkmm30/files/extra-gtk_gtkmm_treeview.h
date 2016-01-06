--- gtk/gtkmm/treeview.h.orig	2016-01-06 13:35:41.771569000 +0100
+++ gtk/gtkmm/treeview.h	2016-01-06 13:39:40.531489000 +0100
@@ -2453,7 +2453,7 @@
       //new_value << astream; //Get it out of the stream as the numerical type.
 
       //Convert the text to a number, using the same logic used by GtkCellRendererText when it stores numbers.
-      auto new_value = static_cast<ColumnType>( std::stod(new_text) );
+      auto new_value = static_cast<ColumnType>( strtod(new_text.c_str(), NULL) );
 
       //Store the user's new text in the model:
       Gtk::TreeRow row = *iter;
