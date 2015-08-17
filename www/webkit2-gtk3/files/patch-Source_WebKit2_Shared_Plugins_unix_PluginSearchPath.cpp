--- Source/WebKit2/Shared/Plugins/unix/PluginSearchPath.cpp.orig	2015-08-07 00:15:19.000000000 +0200
+++ Source/WebKit2/Shared/Plugins/unix/PluginSearchPath.cpp	2015-08-07 00:15:37.000000000 +0200
@@ -39,6 +39,7 @@ Vector<String> pluginsDirectories()
 #if ENABLE(NETSCAPE_PLUGIN_API)
     result.append(homeDirectoryPath() + "/.mozilla/plugins");
     result.append(homeDirectoryPath() + "/.netscape/plugins");
+    result.append("%%BROWSER_PLUGINS_DIR%%");
     result.append("/usr/lib/browser/plugins");
     result.append("/usr/local/lib/mozilla/plugins");
     result.append("/usr/lib/firefox/plugins");
