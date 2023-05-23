import 'package:flutter_test/flutter_test.dart';
import 'package:lua_flutter/lua_flutter.dart';
import 'package:lua_flutter/lua_flutter_platform_interface.dart';
import 'package:lua_flutter/lua_flutter_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockLuaFlutterPlatform
    with MockPlatformInterfaceMixin
    implements LuaFlutterPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  final LuaFlutterPlatform initialPlatform = LuaFlutterPlatform.instance;

  test('$MethodChannelLuaFlutter is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelLuaFlutter>());
  });

  test('getPlatformVersion', () async {
    LuaFlutter luaFlutterPlugin = LuaFlutter();
    MockLuaFlutterPlatform fakePlatform = MockLuaFlutterPlatform();
    LuaFlutterPlatform.instance = fakePlatform;

    expect(await luaFlutterPlugin.getPlatformVersion(), '42');
  });
}
