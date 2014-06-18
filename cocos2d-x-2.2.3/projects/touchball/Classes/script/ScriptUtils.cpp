#include "ScriptUtils.h"
#include <cstdlib>


int global_writeLog(lua_State* ls)
{
	LuaScript* script = (LuaScript*) lua_topointer(ls, lua_upvalueindex(1));
	for (int i = 1; i <= lua_gettop(ls); i++){
		switch (lua_type(ls, i)){
		case 0:
			script->error("null");
			break;
		case 1:
			//char * value;
			//itoa(lua_toboolean(ls, i), value, 10);
			script->error("bool");
			break;
		case 2:
			script->error("lightUserData");
			break;
		case 3:
			script->error("table");
			break;
		case 4:
			script->error("function");
			break;
		case 5:
			script->error("userData");
			break;
		case 6:
			script->error("thread");
			break;
		default:
			script->error(lua_tostring(ls, i));
			break;
		}
	}
	return 0;
}

int global_alert(lua_State* ls)
{
	LuaScript* script = (LuaScript*) lua_topointer(ls, lua_upvalueindex(1));
	const char* str = lua_tostring(ls, -1);
	script->error(str);

	return 0;
}

int __scriptRANDOM(lua_State* ls)
{
	//srand(time(0));
	int value = rand() % lua_tointeger(ls, 1);
	lua_pushinteger(ls, value);
	return 1;
}

void loadGlobalUtilFunctions(lua_State* ls)
{
	//lua_register(ls, 'RANDOM', __scriptRANDOM);
}

char* readFile(const char* fileName)
{
	FILE* file = fopen(fileName, "r");
	if (file){
		fseek(file, 0, SEEK_END);	//移到尾部 
		int len = ftell(file);		//提取长度
		rewind(file);               //回归原位 

		char* result = (char*)malloc(sizeof(char) * len + 1); //要获取的字符串 //分配result空间  
		if (! result) return NULL;

		//读取文件  
		//读取进的result，单位大小，长度，文件指针  
		int rLen = fread(result, sizeof(char), len, file);  
		result[rLen] = '\0'; 

		fclose(file);  
		return result;  
	}else
		return NULL;
}

int global_loadScript(lua_State* ls)
{
	LuaScript* script = (LuaScript*) lua_topointer(ls, lua_upvalueindex(1));
	const char* fileName = lua_tostring(ls, 1);
	const char *fileStream = readFile(fileName);
	if (fileStream == NULL) return 1;

	int stackIndex = lua_gettop(ls);
	//PChar(Cardinal(Stream.DataPointer) + Stream.DataSize)^ := #0;
	int result = luaL_loadbuffer(ls, fileStream, strlen(fileStream), fileName);
	delete fileStream;

	if (result == 0){
		result = lua_pcall(ls, 0, LUA_MULTRET, 0);
	}

	if (result != 0){
		script->error("script loadscript error");
		const char* errorMsg = lua_tolstring(ls, -1, NULL);
		script->error(errorMsg);
	}

	lua_settop(ls, stackIndex);
	return result;
}

int global__call(lua_State* ls)
{
	LuaScript* script = (LuaScript*) lua_topointer(ls, lua_upvalueindex(1));
	ScriptProc* proc = (ScriptProc*) lua_topointer(ls, lua_upvalueindex(2));
	return (*proc)(script);
}

int global__method(lua_State* ls)
{
	LuaScript* script = (LuaScript*) lua_topointer(ls, lua_upvalueindex(1));
	ScriptMethod* method = (ScriptMethod*) lua_topointer(ls, lua_upvalueindex(2));
	ScriptObject* sender = (ScriptObject*) lua_topointer(ls, lua_upvalueindex(3));
	if (method != NULL && sender != NULL){
		return (sender->**method)(script);
	}
	return -1;
}

void initRandomSeed(lua_State* ls)
{
	/*
	lua_getfield(ls, LUA_GLOBALSINDEX, "math");
	lua_getfield(ls, -1, "randomseed");
	lua_pushnumber(ls, system("time"));
	lua_pcall(ls, 1, 0, 0);
	lua_pop(ls, 1);
	*/
}

int object__index(lua_State* ls)
{
	int result = 0;
	ScriptObject *sender = (ScriptObject *)lua_touserdata(ls, 1);
	if (sender == NULL) return result;
	int stackIndex = lua_gettop(ls);
	ScriptIndex msgIndex;
	ScriptIndex *pmsgIndex = &msgIndex;
	ScriptMessage smsg;
	ScriptMessage* psmsg = &smsg;

	switch (lua_type(ls, 2)){
	case LUA_TNUMBER:
		pmsgIndex->msg = XM_SCRIPTINDEX;
		pmsgIndex->script = (LuaScript*)lua_topointer(ls, lua_upvalueindex(1));
		pmsgIndex->index = lua_tointeger(ls, 2);
		sender->scriptIndex(pmsgIndex);
		result = lua_gettop(ls) - stackIndex;
		break;
	case LUA_TSTRING:
		psmsg->msg = XM_SCRIPTQUERY;
		psmsg->script = (LuaScript*)lua_topointer(ls, lua_upvalueindex(1));
		psmsg->query = lua_tostring(ls, 2);
		sender->scriptQuery(psmsg);
		result = lua_gettop(ls) - stackIndex;
		break;
	}
	return result;
}

int object__newindex(lua_State* ls)
{
	int result = 0;
	ScriptObject *sender = (ScriptObject *) lua_touserdata(ls, 1);
	ScriptMessage scriptMessage;
	ScriptMessage *pscriptMessage = &scriptMessage;
	pscriptMessage->msg = XM_SCRIPTSET;
	pscriptMessage->script = (LuaScript*) lua_topointer(ls, lua_upvalueindex(1));
	pscriptMessage->query = lua_tostring(ls, 2);
	sender->scriptSet(pscriptMessage);
	result = 1;
	return result;
}

int object__equal(lua_State* ls)
{
	int result = 0;
	if (lua_type(ls, 1) != lua_type(ls, 2)) return result;
	ScriptObject *obj1 = (ScriptObject*) lua_touserdata(ls, 1);
	ScriptObject *obj2 = (ScriptObject*) lua_touserdata(ls, 2);
	lua_pushboolean(ls, obj1 == obj2);
	result = 1;
	return result;
}
