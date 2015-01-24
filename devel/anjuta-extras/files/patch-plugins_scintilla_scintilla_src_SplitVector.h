--- plugins/scintilla/scintilla/src/SplitVector.h.orig	2015-01-24 19:35:53.869017933 +0100
+++ plugins/scintilla/scintilla/src/SplitVector.h	2015-01-24 19:36:05.243185563 +0100
@@ -9,6 +9,8 @@
 #ifndef SPLITVECTOR_H
 #define SPLITVECTOR_H
 
+#include <algorithm>
+
 template <typename T>
 class SplitVector {
 protected:
