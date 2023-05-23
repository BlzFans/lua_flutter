#include "include/lua_flutter/lua_flutter_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "lua_flutter_plugin.h"

void LuaFlutterPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  lua_flutter::LuaFlutterPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
