#ifndef FLUTTER_PLUGIN_LUA_FLUTTER_PLUGIN_H_
#define FLUTTER_PLUGIN_LUA_FLUTTER_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace lua_flutter {

class LuaFlutterPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  LuaFlutterPlugin();

  virtual ~LuaFlutterPlugin();

  // Disallow copy and assign.
  LuaFlutterPlugin(const LuaFlutterPlugin&) = delete;
  LuaFlutterPlugin& operator=(const LuaFlutterPlugin&) = delete;

  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

}  // namespace lua_flutter

#endif  // FLUTTER_PLUGIN_LUA_FLUTTER_PLUGIN_H_
