import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'lua_flutter_platform_interface.dart';

/// An implementation of [LuaFlutterPlatform] that uses method channels.
class MethodChannelLuaFlutter extends LuaFlutterPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('lua_flutter');

  @override
  Future<String?> getPlatformVersion() async {
    final version = await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }
}
