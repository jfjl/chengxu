#ifndef _LUA_WRAPPER_HPP_
#define _LUA_WRAPPER_HPP_

#include "LuaRegDef.h"
#include "LuaRegClass.h"

DECL_NAMESPACE_LUAREG_BEGIN

//extern int callFunction(lua_State* L);

class LuaReg {
public:
	bool Init();

	void Clear();

	void DoScript(const char* file);
	int Run(const char* code, const char* name);
	int excuteFunction(const char* funcName, void* self, void* gameScene, void* touchMap);
	void registerCommonFunction();
public:
	template<typename T>
	void RegisterClass(const char* className) {
		RegisterClassToLua<T>(L, className);
	}

	template<typename F>
	void RegisterMethod(const char* funcName, F func) {
		RegisterMethodToLua(L, funcName, func);
	}

	template<typename T, typename V>
	void RegisterMember(const char* varName, V var) {
		RegisterMemberToLua<T>(L, varName, var);
	}

	template<typename T>
	void RegisterObject(const char* name, T* object) {
		RegisterObjectToLua<T>(L, name, object);
	}

private:
	lua_State* L;
};

DECL_NAMESPACE_LUAREG_END

extern NS_LUAREG::LuaReg* g_luaReg;
extern bool LuaInit();
extern void LuaClear();

#endif // _LUA_WRAPPER_HPP_