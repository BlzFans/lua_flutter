# The Flutter tooling requires that developers have a version of Visual Studio
# installed that includes CMake 3.14 or later. You should not increase this
# version, as doing so will cause the plugin to fail to compile for some
# customers of the plugin.
cmake_minimum_required(VERSION 3.14)

# Project-level configuration.
set(PROJECT_NAME "lua_flutter")
project(${PROJECT_NAME})

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "lua_flutter_plugin")

# Any new source files that you add to the plugin should be added here.
list(APPEND PLUGIN_SOURCES
  "lua_flutter_plugin.cpp"
  "lua_flutter_plugin.h"
)

set(LUA_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lapi.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lapi.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lauxlib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lauxlib.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lbaselib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lcode.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lcode.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lcorolib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lctype.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lctype.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/ldblib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/ldebug.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/ldebug.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/ldo.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/ldo.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/ldump.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lfunc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lfunc.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lgc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lgc.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/linit.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/liolib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/ljumptab.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/llex.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/llex.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/llimits.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lmathlib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lmem.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lmem.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/loadlib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lobject.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lobject.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lopcodes.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lopcodes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lopnames.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/loslib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lparser.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lparser.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lprefix.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lstate.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lstate.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lstring.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lstring.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lstrlib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/ltable.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/ltable.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/ltablib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/ltm.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/ltm.h"
    #"${CMAKE_CURRENT_SOURCE_DIR}/../lua/lua.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lua.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lua.hpp"
    #"${CMAKE_CURRENT_SOURCE_DIR}/../lua/luac.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/luaconf.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lualib.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lundump.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lundump.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lutf8lib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lvm.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lvm.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lzio.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lzio.h"
)

add_definitions("/wd4244" "/wd4324" "/wd4310" "/wd4701" "/wd4702")
add_definitions(-DLUA_BUILD_AS_DLL)

# Define the plugin library target. Its name must not be changed (see comment
# on PLUGIN_NAME above).
add_library(${PLUGIN_NAME} SHARED
  "include/lua_flutter/lua_flutter_plugin_c_api.h"
  "lua_flutter_plugin_c_api.cpp"
  ${PLUGIN_SOURCES}
  ${LUA_SOURCES}
)

# Apply a standard set of build settings that are configured in the
# application-level CMakeLists.txt. This can be removed for plugins that want
# full control over build settings.
apply_standard_settings(${PLUGIN_NAME})

# Symbols are hidden by default to reduce the chance of accidental conflicts
# between plugins. This should not be removed; any symbols that should be
# exported should be explicitly exported with the FLUTTER_PLUGIN_EXPORT macro.
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)

# Source include directories and library dependencies. Add any plugin-specific
# dependencies here.
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter flutter_wrapper_plugin)

# List of absolute paths to libraries that should be bundled with the plugin.
# This list could contain prebuilt libraries, or libraries created by an
# external build triggered from this build file.
set(lua_flutter_bundled_libraries
  ""
  PARENT_SCOPE
)
