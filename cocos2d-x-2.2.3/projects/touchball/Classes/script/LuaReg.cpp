#include "LuaReg.h"
#include <string>
#include "ScriptObject.h"

DECL_NAMESPACE_LUAREG_BEGIN



int callFunction(lua_State* L)
{
	void* vo = const_cast<void*>(lua_topointer(L, 1));
	Object* object = static_cast<Object*>(vo);
	return object->callFunction(L);
/*
	for (int i = 1; i <= lua_gettop(L); i++){
		switch (lua_type(L, i)){
		case 0:
			printf("NULL");
			break;
		case 1:
			printf("bool");
			b = lua_toboolean(L, i);
			break;
		case 2:
			printf("lightuserdata");
			break;
		case 3:
			printf("number");
			n = lua_tointeger(L, i);
			break;
		case 4:
			printf("string");
			s = lua_tostring(L, i);
			break;
		case 5:
			printf("table");
			break;
		case 6:
			printf("func");
			break;
		case 7:
			printf("userdata");
			o = lua_touserdata(L, i);
			break;
		default:
			printf("thread");
			break;
		}
	}
	return 0;
*/
}

	bool LuaReg::Init() {
		L = luaL_newstate();
		if (NULL == L) {
			return false;
		}
		luaL_openlibs(L);
		registerCommonFunction();
		return true;
}

void LuaReg::Clear() {
	lua_close(L);
}

void LuaReg::DoScript(const char* file) {
	luaL_dofile(L, file);
}

int LuaReg::Run(const char* code, const char* name)
{
	int result = 0;

//	code = "function onTimer(self, target)print(self);callFunction(self, target);end;";
//	result = luaL_loadbuffer(m_state, buff, strlen(buff), "line");

	if (name != ""){
		result = luaL_loadbuffer(L, code, strlen(code), name);
	}else{
		result = luaL_loadstring(L, code);
	}

	if (result == 0){
		result = lua_pcall(L, 0, LUA_MULTRET, 0);
		lua_pushstring(L, code);
	}

	if (result != 0){
		//this->error("script run error:" + result);
		//this->error(lua_tolstring(L, -1, NULL));
	}

	return result;
}

int LuaReg::excuteFunction(const char* funcName, void* self, void* gameScene, void* touchMap)
{
	lua_getglobal(L, funcName);
	lua_pushlightuserdata(L, self);
	lua_pushlightuserdata(L, gameScene);
	lua_pushlightuserdata(L, touchMap);
	return lua_pcall(L, 3, 0, NULL);
}

void LuaReg::registerCommonFunction()
{
	lua_pushcfunction(L, callFunction);
	lua_setglobal(L, "callFunction");
}

DECL_NAMESPACE_LUAREG_END

	NS_LUAREG::LuaReg* g_luaReg = NULL;

bool LuaInit() {
	g_luaReg = new NS_LUAREG::LuaReg;
	return g_luaReg->Init();
}

void LuaClear() {
	g_luaReg->Clear();
	delete g_luaReg;
	g_luaReg = NULL;
}
