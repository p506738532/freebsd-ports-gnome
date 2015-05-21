--- files/usr/lib/cinnamon-settings/cinnamon-settings.py.orig	2015-05-21 07:46:35.023228000 +0200
+++ files/usr/lib/cinnamon-settings/cinnamon-settings.py	2015-05-21 07:46:43.545072000 +0200
@@ -29,7 +29,7 @@
     print "No settings modules found!!"
     sys.exit(1)
 
-mod_files = [x.split('/')[5].split('.')[0] for x in mod_files]
+mod_files = [x.split('/')[6].split('.')[0] for x in mod_files]
 
 for mod_file in mod_files:
     if mod_file[0:3] != "cs_":
