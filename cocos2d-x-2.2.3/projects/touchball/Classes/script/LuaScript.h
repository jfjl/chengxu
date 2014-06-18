#pragma once
#ifndef __LUASCRIPT_H__
#define __LUASCRIPT_H__

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "tolua_fix.h"

//这里就是关联metatable的__index域，我们所有的操作都会转换到函数class_index_event进行
extern int class_index_event(lua_State *ls);
}

#include <fstream>
#include <map>
#include "ScriptObject.h"

using namespace std;

typedef struct lua_State lua_State;

#define XM_SCRIPT 1000;
#define XM_SCRIPTQUERY XM_SCRIPT + 0;
#define XM_SCRIPTSET   XM_SCRIPT + 1;
#define XM_SCRIPTINDEX XM_SCRIPT + 2;

class LuaScript;

struct ScriptMessage
{
	int msg;
	LuaScript* script;
	string query;
};

struct ScriptIndex
{
	int msg;
	LuaScript* script;
	int index;
};

typedef void (*ScriptEvent)(ScriptObject* sender, const char* str);
typedef int (*ScriptProc)(LuaScript* script);
//typedef int (Object::*ScriptMethod)(LuaScript* script);

typedef int (LuaScript::*classFunc)(lua_State *ls);

/*注册userdata的函数，在这里我们必须明确，对于我们自定义的类型的实例，比如obj，
我们是通过指针传递给lua的，这其实是一个light userdata，而在lua里面，
light userdata不具有meta机制，但是我们又要设定该类型的meta为类A的meta，
这样我们就要实现一下包装，我们把这个obj 首先赋值给一个full userdata，
在lua里面full userdata具有meta属性，然后存贮在一个注册table里面，
这样其实我们返回的不是这个obj的指针了，而是包装了这个obj的一个full userdata 指针。
在lua5.0里面定义了lua_unboxpointer和lua_boxpointer宏，但不知怎么5.1里面没有了，这两个宏如下：
*/
#define lua_boxpointer(L,u) (*(void **)(lua_newuserdata(L, sizeof(void *))) = (u))
#define lua_unboxpointer(L,i)    (*(void **)(lua_touserdata(L, i)))
//	当我们需要取出obj指针时候，我们通过(*(void **)转换得到。

class LuaScript
{
private:
	lua_State* m_state;
	static LuaScript* m_Script;

protected:
	void init();

	void createObjectMetatable(lua_State* ls);
	void addMethod(const char* name, lua_CFunction func, int index);
	void setMetatable(const char* name);

	void openLuaState();
	void closeLuaState();
	int funcCall(int arg, int ret);
public:
	LuaScript(void);
	~LuaScript(void);

	void clear();
	void pop(int n);
	int open(const char* fileName);
	int run(const char* code, const char* name);
	void error(const char* value);
	int scriptEvent(const char* strProc, ScriptObject* selfObj, ScriptObject* sender);
	void setStackSize(int n);
	int getStackSize();

	void registerFunction(const char* funcName, ScriptProc* proc, int tableIndex);
	void registerMethod(const char* funcName, ScriptObject* sender, ScriptMethod* method, int tableIndex);
	void registerObject(const char* objectName, ScriptObject* object, int tableIndex=LUA_GLOBALSINDEX);
	void objectMT(const char* name);
	
	int call(const char* proc, int arg, int ret);
	int call(int arg, int ret);
	

	int     getType(int index);
	bool    getBoolean(int index);
	int     getInteger(int index);
	double  getFloat(int index);
	ScriptObject* getObject(int index);
	const char* getString(int index);
	const void* getPointer(int index);
	int getTable(const char* tokens);

	int pushNil();
	int pushBool(bool value);
	int pushInt(int value);
	int pushFloat(float value);
	int pushString(const char* data);
	int pushString(const char* data, int length);
	int pushObject(ScriptObject *value);
	int pushFunction(ScriptProc* proc);
	bool pushFunction(int nHandler);
	int pushMethod(ScriptObject* sender, ScriptMethod method);
	int pushPointer(void* param);

	int createTable();
	int createTable(int narr, int nrec);
	int setTable(int tableIndex);
	int setTableArray(int tableIndex);
	int setTableSet(int tableIndex);
	void releaseTable(int tableIndex);
	int getField(const char* fieldName, int tableIndex);
	void setField(const char* fieldName, int tableIndex);
	int getArray(int index, int tableIndex);
	void setArray(int index, int tableIndex);

	const char* getFieldValueStr(const char* fieldName, int tableIndex);
	int  getFieldValueInt(const char* fieldName, int tableIndex);
	void setFieldValue(const char* fieldName, const char* value, int tableIndex);
	void setFieldValue(const char* fieldName, int value, int tableIndex);
	void setFieldValue(const char* fieldName, double value, int tableIndex);
	void setFieldValue(const char* fieldName, bool value, int tableIndex);

	int excuteFunction(const char* funcName, ScriptObject* self, ScriptObject* target);
	lua_State* getState();

	/////////test
	//这个函数把我们类注册进lua中，即是把我们的类当成一个metatable
	//registry type to lua
	 int reg_type(lua_State *ls, const char *name);
	 void classevent(lua_State *ls);
	 //这里就是关联metatable的__index域，我们所有的操作都会转换到函数class_index_event进行
	 //int class_index_event(lua_State *ls);
	 //同时我们提供了设定继承关系的函数
	 int reg_class(lua_State *ls, const char *name, const char *base);
	 //注册函数的函数，使函数与特定的metatable关联：
	 int reg_function(lua_State* ls, const char * type, const char* name, lua_CFunction func);
	 int reg_userdata(lua_State* ls, void* value, const char* type);
	 //这个函数只是注册一个table供我们操作。
	 int script_open(lua_State *ls);
};

class ScriptMethodTable
{
private:
	ScriptObject* m_owner;
	map<const char*, ScriptMethod*> *m_list;
public:
	ScriptMethodTable(ScriptObject* owner);
	~ScriptMethodTable();

	void clear();

	void addMethod(const char* name, ScriptMethod* method);
	ScriptMethod* findMethod(ScriptObject* object, const char* name);
	bool doScriptQuery(ScriptObject* object, ScriptMessage* message);
};

#endif