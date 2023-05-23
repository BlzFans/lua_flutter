#include "lua_flutter_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>

namespace lua_flutter {

// static
void LuaFlutterPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "lua_flutter",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<LuaFlutterPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

LuaFlutterPlugin::LuaFlutterPlugin() {}

LuaFlutterPlugin::~LuaFlutterPlugin() {}

void LuaFlutterPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("getPlatformVersion") == 0) {
    std::ostringstream version_stream;
    version_stream << "Windows ";
    if (IsWindows10OrGreater()) {
      version_stream << "10+";
    } else if (IsWindows8OrGreater()) {
      version_stream << "8";
    } else if (IsWindows7OrGreater()) {
      version_stream << "7";
    }
    result->Success(flutter::EncodableValue(version_stream.str()));
  } else {
    result->NotImplemented();
  }
}

}  // namespace lua_flutter

#define LUA_LIB
#include "../lua/lua.hpp"

#define LUA_FLUTTER_API extern "C" __declspec(dllexport)

struct DartStateScope {
    DartStateScope() {
        //FlutterEngineDartEnterIsolate();
        //FlutterEngineDartEnterScope();
    };
    ~DartStateScope() {
        //FlutterEngineDartExitScope();
        //FlutterEngineDartExitIsolate();
    };
};

static int dart_method_call(lua_State* L)
{
    int64_t (*dartFunction)(void*);
    dartFunction = (decltype(dartFunction))lua_touserdata(L, lua_upvalueindex(1));

    int ret = 0;
    {
        DartStateScope scope;
        ret = (int)dartFunction(L);
    }

    if (lua_toboolean(L, lua_upvalueindex(2)))
    {
        lua_pushboolean(L, 0);
        lua_replace(L, lua_upvalueindex(2));
        return lua_error(L);
    }

    return ret;
}

int getLuaValue(lua_State* L, const char* key)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);       //[table]

    const char* ptr1 = key;
    const char* ptr2 = key;

    while (true)                                               //[table]
    {
        if (*ptr2 == '.')
        {
            lua_pushlstring(L, ptr1, ptr2 - ptr1);             //[table name]
            lua_gettable(L, -2);                               //[table value]
            if (!lua_istable(L, -1) && !lua_isuserdata(L, -1))
            {
                lua_pop(L, 2);
                lua_pushnil(L);                                //[nil]
                return LUA_TNIL;
            }

            lua_remove(L, -2);                                 //[newtable]
            ptr2++;
            ptr1 = ptr2;
        }
        else if (*ptr2 == '\0')
        {
            lua_pushlstring(L, ptr1, ptr2 - ptr1);             //[table name]
            int type = lua_gettable(L, -2);                    //[table, value]
            lua_remove(L, -2);                                 //[value]
            return type;
        }
        else
        {
            ptr2++;
        }
    }
}

bool setLuaValue(lua_State* L, const char* key)
{
    //[value]
    lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);   //[value table]

    const char* ptr1 = key;
    const char* ptr2 = key;

    while (true)                                           //[value table]
    {
        if (*ptr2 == '.')
        {
            //get table or create table
            lua_pushlstring(L, ptr1, ptr2 - ptr1);         //[value table name]
            lua_pushvalue(L, -1);                          //[value table name name]
            lua_rawget(L, -3);                             //[value table name value]
            if (lua_isnil(L, -1))
            {
                lua_pop(L, 1);                             //[value table name]
                lua_createtable(L, 0, 0);                  //[value table name newtable]
                lua_pushvalue(L, -2);                      //[value table name newtable name]
                lua_pushvalue(L, -2);                      //[value table name newtable name newtable]
                lua_rawset(L, -5);                         //[value table name newtable]
            }
            else if (!lua_istable(L, -1))
            {
                lua_pop(L, 4);
                return false;
            }

            lua_remove(L, -2);                             //[value table newtable]
            lua_remove(L, -2);                             //[value newtable]

            ptr2++;
            ptr1 = ptr2;
        }
        else if (*ptr2 == '\0')
        {
            lua_pushlstring(L, ptr1, ptr2 - ptr1);         //[value table name]
            lua_pushvalue(L, -3);                          //[value table name value]
            lua_rawset(L, -3);                             //[value table]
            lua_pop(L, 2);
            return true;
        }
        else
        {
            ptr2++;
        }
    }
}

LUA_FLUTTER_API int luaGetValue(lua_State* L, const char* name)
{
    return getLuaValue(L, name);
}

LUA_FLUTTER_API void luaSetValue(lua_State* L, const char* name)
{
    setLuaValue(L, name);
}

static void callstack(lua_State* L, int n)
{
    lua_Debug ar;
    if (lua_getstack(L, n, &ar) == 1)
    {
        lua_getinfo(L, "nSlu", &ar);
        if (ar.name)
        {
            printf("--->%s %d %s\n", ar.source, ar.currentline, ar.name);
        }
        else
        {
            printf("--->%s %d\n", ar.source, ar.currentline);
        }

        callstack(L, n + 1);
    }
}

int onError(lua_State* L)
{
    printf("--->\n");
    printf("---> %s\n", lua_tostring(L, -1));
    printf("--->\n");
    callstack(L, 0);
    printf("--->\n");

    return 1;
}

LUA_FLUTTER_API int lua_pcall_dart(lua_State* L, int nargs, int nresults, int errfunc)
{
    if (errfunc != 0)
    {
        lua_pushcclosure(L, onError, 0);
        lua_replace(L, errfunc);
    }
    return lua_pcall(L, nargs, nresults, errfunc);
}

LUA_FLUTTER_API void luaL_error_dart(lua_State* L, const char* error)
{
    lua_pushboolean(L, 1);
    lua_replace(L, lua_upvalueindex(2));
    lua_pushstring(L, error);
}

LUA_FLUTTER_API void luaRegisterMethod(lua_State* L, const char* className, const char* method, int64_t address)
{
    assert(className);
    int type = getLuaValue(L, className);
    assert(type == LUA_TTABLE);
    if (type != LUA_TTABLE)
    {
        lua_pop(L, 1);
        return;
    }

    lua_pushstring(L, method);
    lua_pushlightuserdata(L, (void*)address);
    lua_pushboolean(L, 0);                              //lua_error
    lua_pushcclosure(L, dart_method_call, 2);
    lua_rawset(L, -3);

    lua_pop(L, 1);
}

LUA_FLUTTER_API void luaRegisterFunction(lua_State* L, const char* function, int64_t address)
{
    lua_pushlightuserdata(L, (void*)address);
    lua_pushboolean(L, 0);                              //lua_error
    lua_pushcclosure(L, dart_method_call, 2);
    setLuaValue(L, function);
}

static int dartGetProp(lua_State* L)
{
    int64_t(*dartFunction)(void*);
    dartFunction = (decltype(dartFunction))lua_touserdata(L, lua_upvalueindex(2));
    if (dartFunction != nullptr)
    {
        DartStateScope scope;
        return (int)dartFunction(L);
    }

    lua_pushnil(L);
    return 1;
}

static int meta_index(lua_State* L)                //[userdata key]
{
    if (lua_type(L, 2) != LUA_TSTRING)
        return 0;

    lua_pushvalue(L, 2);                           //[userdata key key]
    int type = lua_rawget(L, lua_upvalueindex(1)); //[userdata key value] upvalue1: metatable
    if (type != LUA_TNIL) {
        return 1;
    }

    lua_pop(L, 1);                                //[userdata key]
    int nresult = dartGetProp(L);                 //[userdata key value]
    assert(nresult == 1);
    type = lua_type(L, 3);
    if (type != LUA_TNIL)
        return nresult;

    lua_pop(L, nresult);                          //[userdata key]

    //call super class meta_index

    lua_pushvalue(L, lua_upvalueindex(3));        //[userdata key super__meta__index]
    type = lua_type(L, 3);
    if (type == LUA_TFUNCTION) {
        lua_pushvalue(L, -3);                     //[userdata key super__meta__index userdata]
        lua_pushvalue(L, -3);                     //[userdata key super__meta__index userdata key]
        if (LUA_OK == lua_pcall(L, 2, 1, 0))      //[userdata key super__meta__index value]
        {
            lua_remove(L, -2);                    //[userdata key value]
        }
        else
        {
            lua_pop(L, 2);                        //[userdata key]
            lua_pushnil(L);                       //[userdata key nil]
        }

        return 1;
    }

    lua_pop(L, 1);
    lua_pushnil(L);                               //[userdata key nil]
    return 1;
}

LUA_FLUTTER_API void luaRegisterClass(lua_State* L, const char* className, const char* superClass, int64_t getProp)
{
    int type = getLuaValue(L, className);
    lua_pop(L, 1);
    assert(type == LUA_TNIL);

    lua_createtable(L, 0, 3);                      //[table]
    lua_pushvalue(L, -1);                          //[table table]
    lua_setmetatable(L, -2);                       //[table]

    //__index
    lua_pushstring(L, "__index");                  //[table "__index"]
    lua_pushvalue(L, -2);                          //[table "__index" table]
    lua_pushlightuserdata(L, (void*)getProp);      //[table "__index" table getProp]
    if (superClass && superClass[0]) {
        type = getLuaValue(L, superClass);         //[table "__index" table getProp superClass]
        assert(type == LUA_TTABLE || type == LUA_TNIL);

        if (type == LUA_TTABLE) {
            lua_pushstring(L, "__index");                 //[table "__index" table getProp superClass "__index"]
            type = lua_rawget(L, -2);                     //[table "__index" table getProp superClass super__index]
            assert(type == LUA_TFUNCTION);
            lua_remove(L, -2);                            //[table "__index" table getProp super__index]
        }
    }
    else {
        lua_pushnil(L);                            //[table "__index" table getProp nil]
    }
    lua_pushcclosure(L, meta_index, 3);            //[table "__index" meta_index]
    lua_rawset(L, -3);                             //[table]

    //__gc
    lua_pushstring(L, "__gc");                     //[table "__gc"]
    getLuaValue(L, "dart_metal.__gc");      //[table "__gc" dart_metal.__gc]
    lua_rawset(L, -3);                             //[table]

    //__tostring
    lua_pushstring(L, "__tostring");               //[table "__tostring"]
    getLuaValue(L, "dart_metal.__tostring");//[table "__tostring" dart_metal.__tostring]
    lua_rawset(L, -3);                             //[table]

    //__eq
    lua_pushstring(L, "__eq");                     //[table "__eq"]
    getLuaValue(L, "dart_metal.__eq");      //[table "__eq" dart_metal.__eq]
    lua_rawset(L, -3);                             //[table]

    //name
    lua_pushstring(L, "name");                     //[table "name"]
    lua_pushstring(L, className);                  //[table "name" className]
    lua_rawset(L, -3);                             //[table]

    //__super
    if (superClass && superClass[0]) {
        lua_pushstring(L, "__super");              //[table "__super"]
        type = getLuaValue(L, superClass);  //[table "__super" superClass]
        assert(type == LUA_TTABLE);

        lua_rawset(L, -3);                         //[table]
    }

    setLuaValue(L, className);              //[]
}

LUA_FLUTTER_API void luaPushObject(lua_State* L, int64_t id, int metaTable)
{
    void* userData = lua_newuserdata(L, sizeof(int64_t));
    *(int64_t*)userData = id;

    int type = lua_rawgeti(L, LUA_REGISTRYINDEX, metaTable);
    assert(type == LUA_TTABLE);
    lua_setmetatable(L, -2);
}

LUA_FLUTTER_API int64_t luaToObject(lua_State* L, int32_t idx)
{
    void* userData = lua_touserdata(L, idx);
    if (userData) {
        return *(int64_t*)userData;
    }

    return 0;
}

bool _doBuffer(lua_State* L, const char* buffer, size_t size)
{
    int err = luaL_loadbuffer(L, buffer, size, "");
    if (err)
    {
        const char* str = lua_tostring(L, -1);
        printf("doBuffer luaL_loadbuffer %s\n", str);
        lua_pop(L, 1);
        return false;
    }

    if (lua_isfunction(L, -1))
    {
        err = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (err)
        {
            const char* str = lua_tostring(L, -1);
            printf("doBuffer lua_pcall %s\n", str);
            lua_pop(L, 1);
        }
    }

    return err == LUA_OK ? true : false;
}

struct stack_reset
{
    stack_reset(lua_State* L)
        : m_L(L)
        , m_top(lua_gettop(L))
    {
    }

    ~stack_reset()
    {
        lua_settop(m_L, m_top);
    }

private:
    lua_State* m_L;
    int m_top;
};

LUA_FLUTTER_API void luaDoString(lua_State* L, const char* str)
{
    stack_reset stack(L);
    _doBuffer(L, str, strlen(str));
}

LUA_FLUTTER_API void luaDoFile(lua_State* L, const char* name)
{
    stack_reset stack(L);
    int err = luaL_loadfile(L, name);
    if (err)
    {
        const char* str = lua_tostring(L, -1);
        printf("doFile luaL_loadfile %s\n", str);
        return;
    }

    if (lua_isfunction(L, -1))
    {
        err = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (err)
        {
            const char* str = lua_tostring(L, -1);
            printf("doFile lua_pcall %s\n", str);
        }
    }
}

static void l_message(const char* pname, const char* msg) {
    if (pname) lua_writestringerror("%s: ", pname);
    lua_writestringerror("%s\n", msg);
}


/*
** Check whether 'status' is not OK and, if so, prints the error
** message on the top of the stack. It assumes that the error object
** is a string, as it was either generated by Lua or by 'msghandler'.
*/
static int report(lua_State* L, int status) {
    if (status != LUA_OK) {
        const char* msg = lua_tostring(L, -1);
        l_message("lua", msg);
        lua_pop(L, 1);  /* remove message */
    }
    return status;
}

static int msghandler(lua_State* L) {
    const char* msg = lua_tostring(L, 1);
    if (msg == NULL) {  /* is error object not a string? */
        if (luaL_callmeta(L, 1, "__tostring") &&  /* does it have a metamethod */
            lua_type(L, -1) == LUA_TSTRING)  /* that produces a string? */
            return 1;  /* that is the message */
        else
            msg = lua_pushfstring(L, "(error object is a %s value)",
                luaL_typename(L, 1));
    }
    luaL_traceback(L, L, msg, 1);  /* append a standard traceback */
    return 1;  /* return the traceback */
}

static int docall(lua_State* L, int narg, int nres) {
    int status;
    int base = lua_gettop(L) - narg;  /* function index */
    lua_pushcfunction(L, msghandler);  /* push message handler */
    lua_insert(L, base);  /* put it under function and args */
    status = lua_pcall(L, narg, nres, base);
    lua_remove(L, base);  /* remove message handler from the stack */
    return status;
}

static int dochunk(lua_State* L, int status) {
    if (status == LUA_OK) status = docall(L, 0, 0);
    return report(L, status);
}


LUA_FLUTTER_API int dofile(lua_State* L, const char* name) {
    return dochunk(L, luaL_loadfile(L, name));
}

LUA_FLUTTER_API int dostring(lua_State* L, const char* s) {
    return dochunk(L, luaL_loadbuffer(L, s, strlen(s), ""));
}
