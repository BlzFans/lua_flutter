import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'lua_flutter_method_channel.dart';

abstract class LuaFlutterPlatform extends PlatformInterface {
  /// Constructs a LuaFlutterPlatform.
  LuaFlutterPlatform() : super(token: _token);

  static final Object _token = Object();

  static LuaFlutterPlatform _instance = MethodChannelLuaFlutter();

  /// The default instance of [LuaFlutterPlatform] to use.
  ///
  /// Defaults to [MethodChannelLuaFlutter].
  static LuaFlutterPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [LuaFlutterPlatform] when
  /// they register themselves.
  static set instance(LuaFlutterPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }
}
