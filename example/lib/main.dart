
import 'package:flutter/foundation.dart';
import 'package:lua_flutter/lua_flutter.dart';

void main() {
  debugPrint("dart main");
  initLuaFlutter();
  /*
  dostring(g_L, 
    """
      local widget = Center {
          child = Text(
              "flutter_main", {
                  style = TextStyle {
                      fontWeight = FontWeight.bold,
                      color = Colors.blue,
                  },
                  textDirection = TextDirection.ltr,
              }
          )
      }

      runApp(widget)
    """
  );
  */

  dostring(g_L, "package.path = '?.lua;build/windows/runner/Debug/?.lua;build/windows/runner/Release/?.lua'");
  dostring(g_L, "require 'data/flutter_assets/assets/main'");
}
