#pragma once
#ifndef __LUASCRIPT_H__
#define __LUASCRIPT_H__

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "tolua_fix.h"

//������ǹ���metatable��__index���������еĲ�������ת��������class_index_event����
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

/*ע��userdata�ĺ��������������Ǳ�����ȷ�����������Զ�������͵�ʵ��������obj��
������ͨ��ָ�봫�ݸ�lua�ģ�����ʵ��һ��light userdata������lua���棬
light userdata������meta���ƣ�����������Ҫ�趨�����͵�metaΪ��A��meta��
�������Ǿ�Ҫʵ��һ�°�װ�����ǰ����obj ���ȸ�ֵ��һ��full userdata��
��lua����full userdata����meta���ԣ�Ȼ�������һ��ע��table���棬
������ʵ���Ƿ��صĲ������obj��ָ���ˣ����ǰ�װ�����obj��һ��full userdata ָ�롣
��lua5.0���涨����lua_unboxpointer��lua_boxpointer�꣬����֪��ô5.1����û���ˣ������������£�
*/
#define lua_boxpointer(L,u) (*(void **)(lua_newuserdata(L, sizeof(void *))) = (u))
#define lua_unboxpointer(L,i)    (*(void **)(lua_touserdata(L, i)))
//	��������Ҫȡ��objָ��ʱ������ͨ��(*(void **)ת���õ���

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
	//���������������ע���lua�У����ǰ����ǵ��൱��һ��metatable
	//registry type to lua
	 int reg_type(lua_State *ls, const char *name);
	 void classevent(lua_State *ls);
	 //������ǹ���metatable��__index���������еĲ�������ת��������class_index_event����
	 //int class_index_event(lua_State *ls);
	 //ͬʱ�����ṩ���趨�̳й�ϵ�ĺ���
	 int reg_class(lua_State *ls, const char *name, const char *base);
	 //ע�ắ���ĺ�����ʹ�������ض���metatable������
	 int reg_function(lua_State* ls, const char * type, const char* name, lua_CFunction func);
	 int reg_userdata(lua_State* ls, void* value, const char* type);
	 //�������ֻ��ע��һ��table�����ǲ�����
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