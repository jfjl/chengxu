#include "LuaScript.h"
#include "ScriptUtils.h"

extern "C"
{
	int class_index_event(lua_State *ls)
	{
		int t = lua_type(ls, 1);
		if (t == LUA_TUSERDATA){
			//这里我们只考虑了函数类型，所以简单起见没有进行很多其他的处理
			//我们会得到metatable，然后找到该metatable对应的函数，进行调用
			lua_getmetatable(ls, 1);
			lua_pushvalue(ls, 2);
			lua_rawget(ls, -2);
			if(! lua_isnil(ls, -1)) return 1;
		}

		return 1;
	}
}

LuaScript::LuaScript(void)
	: m_state(NULL)
{
	openLuaState();
}


LuaScript::~LuaScript(void)
{
	closeLuaState();
}

void LuaScript::openLuaState()
{
	m_state = lua_open();
	luaL_openlibs(m_state);
	toluafix_open(m_state);
	init();
}

void LuaScript::closeLuaState()
{
	if (m_state != NULL) lua_close(m_state);

	m_state = NULL;
}

void LuaScript::clear()
{
	closeLuaState();
	openLuaState();
}

void LuaScript::addMethod(const char* name, lua_CFunction func, int index)
{
	lua_pushstring(m_state, name);
	lua_pushlightuserdata(m_state, this);
	lua_pushcclosure(m_state, func, 1);
	lua_rawset(m_state, index);
}

void LuaScript::setMetatable(const char* name)
{
	lua_getfield(m_state, LUA_REGISTRYINDEX, name);
	lua_setmetatable(m_state, -2);
}

void LuaScript::init()
{
	// Load Basic Library
	luaopen_base(m_state);
	luaopen_string(m_state);
	luaopen_math(m_state);
	luaopen_table(m_state);

	addMethod("alert", global_alert, LUA_GLOBALSINDEX);
	addMethod("writeLog", global_writeLog, LUA_GLOBALSINDEX);
	// Add global functions
	loadGlobalUtilFunctions(m_state);
	initRandomSeed(m_state);
	addMethod("dofile", global_loadScript, LUA_GLOBALSINDEX);
	// Create Default Metatable
	createObjectMetatable(m_state);
}

void LuaScript::error(const char* name)
{

}

int LuaScript::open(const char* fileName)
{
	int result = 1;
	const char *fileStream = readFile(fileName);
	if (fileStream == NULL) return result;

	int stackIndex = lua_gettop(m_state);
	result = luaL_loadbuffer(m_state, fileStream, strlen(fileStream), fileName);
	delete fileStream;
    
	if (result == 0){
		result = lua_pcall(m_state, 0, LUA_MULTRET, 0);
	}

	if (result != 0){
		this->error("script open error on");
		const char* errorMsg = lua_tolstring(m_state, -1, NULL);
		this->error(errorMsg);
	}

	lua_settop(m_state, stackIndex);

	return result;
}

int LuaScript::run(const char* code, const char* name)
{
	int result = 0;

	//const char* buff = "function onTimer(self, target)print(\"hello\");end;";
	//result = luaL_loadbuffer(m_state, buff, strlen(buff), "line");

	if (name != ""){
		result = luaL_loadbuffer(m_state, code, strlen(code), name);
	}else{
		result = luaL_loadstring(m_state, code);
	}

	if (result == 0){
		result = lua_pcall(m_state, 0, LUA_MULTRET, 0);
		lua_pushstring(m_state, code);
	}

	if (result != 0){
		this->error("script run error:" + result);
		this->error(lua_tolstring(m_state, -1, NULL));
	}

	return result;
}

void LuaScript::registerFunction(const char* funcName, ScriptProc* proc, int tableIndex)
{
	pushString(funcName);
	pushFunction(proc);
	lua_rawset(m_state, tableIndex);
}

void LuaScript::registerMethod(const char* funcName, ScriptObject*sender, ScriptMethod* method, int tableIndex)
{
	pushString(funcName);
	pushMethod(sender, *method);
	lua_rawset(m_state, tableIndex);
}

void LuaScript::registerObject(const char* objectName, ScriptObject* object, int tableIndex)
{
	pushString(objectName);
	pushObject(object);
	lua_rawset(m_state, tableIndex);
}

int LuaScript::funcCall(int arg, int ret)
{
	int result = lua_pcall(m_state, arg, ret, 0);
	if (result != 0){
		error(lua_tostring(m_state, -1));
		lua_remove(m_state, -1);
	}
	return result;
}

int LuaScript::scriptEvent(const char* strProc, ScriptObject* selfObj, ScriptObject* sender)
{
	lua_getfield(m_state, LUA_GLOBALSINDEX, strProc);
	if (lua_type(m_state, -1) != LUA_TFUNCTION){
		lua_remove(m_state, -1);
		this->error("invalid call");
		return -1;
	}

	pushObject(selfObj);
	pushObject(sender);

	int result = funcCall(2, 0);
	if (result != 0){
		this->error("lua call error");
	}
	return result;
}

int LuaScript::call(const char* proc, int arg, int ret)
{
	getTable(proc);
	lua_insert(m_state, (-arg)-1);

	int result = funcCall(arg, ret);
	if (result != 0)
		error("Lua Call(%s) Error: ");

	return result;
}

int LuaScript::call(int arg, int ret)
{
	return funcCall(arg, ret);
}

void LuaScript::pop(int n)
{
	lua_pop(m_state, n);
}

int LuaScript::getStackSize()
{
	return lua_gettop(m_state);
}

void LuaScript::setStackSize(int n)
{
	lua_settop(m_state, n);
}

bool LuaScript::getBoolean(int index)
{
	return lua_toboolean(m_state, index) != 0;
}

int LuaScript::getInteger(int index)
{
	return (int)(lua_tonumber(m_state, index));
}

double LuaScript::getFloat(int index)
{
	return lua_tonumber(m_state, index);
}

ScriptObject* LuaScript::getObject(int index)
{
	return (ScriptObject*) lua_topointer(m_state, index);
}

const void* LuaScript::getPointer(int index)
{
	return lua_topointer(m_state, index);
}

const char* LuaScript::getString(int index)
{
	return lua_tolstring(m_state, index, NULL);
}

int LuaScript::getType(int index)
{
	return lua_type(m_state, index);
}

int LuaScript::pushNil()
{
	lua_pushnil(m_state);
	return lua_gettop(m_state);
}

int LuaScript::pushBool(bool value)
{
	lua_pushboolean(m_state, value);
	return lua_gettop(m_state);
}

int LuaScript::pushInt(int value)
{
	lua_pushinteger(m_state, value);
	return lua_gettop(m_state);
}

int LuaScript::pushFloat(float value)
{
	lua_pushnumber(m_state, value);
	return lua_gettop(m_state);
}

int LuaScript::pushString(const char* data)
{
	lua_pushlstring(m_state, data, strlen(data));
	return lua_gettop(m_state);
}

int LuaScript::pushString(const char* data, int length)
{
	lua_pushlstring(m_state, data, length);
	return lua_gettop(m_state);
}

int LuaScript::pushObject(ScriptObject *value)
{
	if (value != NULL){
		int size = sizeof(void* );
		void* pData = lua_newuserdata(m_state, size);
		pData = value;
		setMetatable("OM");
	}else{
		lua_pushnil(m_state);
	}
	return lua_gettop(m_state);
}

int LuaScript::pushFunction(ScriptProc* proc)
{
	lua_pushlightuserdata(m_state, this);
	lua_pushlightuserdata(m_state, proc);
	lua_pushcclosure(m_state, global__call, 2);
	return lua_gettop(m_state);
}

int LuaScript::pushMethod(ScriptObject* sender, ScriptMethod method)
{
	lua_pushlightuserdata(m_state, this);
	lua_pushlightuserdata(m_state, &method);
	lua_pushlightuserdata(m_state, sender);
	lua_pushcclosure(m_state, global__method, 3);
	return lua_gettop(m_state);
}

int LuaScript::pushPointer(void* param)
{
	if (param != NULL) {
		lua_pushlightuserdata(m_state, param);
	}else{
		lua_pushnil(m_state);
	}
	return lua_gettop(m_state);
}

void LuaScript::createObjectMetatable(lua_State* ls)
{
	pushString("om");

	lua_newtable(ls);
	addMethod("__index",    object__index, -3);
	addMethod("__newindex", object__newindex, -3);
	addMethod("__eq",       object__equal, -3);

	lua_rawset(ls, LUA_REGISTRYINDEX);
}

int LuaScript::createTable()
{
	return createTable(0, 0);
}

int LuaScript::createTable(int narr, int nrec)
{
	lua_createtable(m_state, narr, nrec);
	return lua_gettop(m_state);
}

void LuaScript::releaseTable(int tableIndex)
{
	lua_settop(m_state, tableIndex-1);
}

int LuaScript::setTable(int tableIndex)
{
	int result = 0;
	if ((lua_gettop(m_state) - tableIndex) % 2){
		while (lua_gettop(m_state) > tableIndex){
			lua_rawset(m_state, tableIndex);
			result++;
		}
	}else{
		lua_settop(m_state, tableIndex);
	}
	return result;
}

int LuaScript::setTableArray(int tableIndex)
{
	int result = 0;
	int index = lua_gettop(m_state) - tableIndex;
	while (index > 0){
		lua_rawseti(m_state, tableIndex, index);
		result++;
		index--;
	}
	return result;
}

int LuaScript::setTableSet(int tableIndex)
{
	int result = 0;
	while (lua_gettop(m_state) > tableIndex){
		lua_pushboolean(m_state, 1);
		lua_rawset(m_state, tableIndex);
		result++;
	}
	return result;
}

int LuaScript::getField(const char* fieldName, int tableIndex)
{
	lua_getfield(m_state, tableIndex, fieldName);
	return lua_gettop(m_state);
}

void LuaScript::setField(const char* fieldName, int tableIndex)
{
	lua_setfield(m_state, tableIndex, fieldName);	
}

int LuaScript::getArray(int index, int tableIndex)
{
	lua_rawgeti(m_state, tableIndex, index);
	return lua_gettop(m_state);
}

void LuaScript::setArray(int index, int tableIndex)
{
	lua_rawseti(m_state, tableIndex, index);
}

int LuaScript::getTable(const char* tokens)
{
	int result = lua_gettop(m_state) + 1;
	int prevTable = LUA_GLOBALSINDEX;

	string name = tokens;
	string::size_type p = name.find(".");
	while (p != string::npos){
		name.erase(0, p);
		lua_pushlstring(m_state, name.c_str(), p-1);
		lua_rawget(m_state, prevTable);
		if (lua_type(m_state, -1) != LUA_TTABLE){
			error("Invalid token : ");
			return result;
		}

		p = name.find(".");
		prevTable = -2;
	}

	lua_pushstring(m_state, name.c_str());
	lua_rawget(m_state, prevTable);

	if (lua_gettop(m_state) != result) {
		lua_replace(m_state, result);
		lua_settop(m_state, result);
	}

	return result;
}

const char* LuaScript::getFieldValueStr(const char* fieldName, int tableIndex)
{
	lua_getfield(m_state, tableIndex, fieldName);

	const char* result = lua_tostring(m_state, -1);

	lua_pop(m_state, 1);

	return result;
}

int LuaScript::getFieldValueInt(const char* fieldName, int tableIndex)
{
	lua_getfield(m_state, tableIndex, fieldName);

	int result = (int)(lua_tonumber(m_state, -1));
	lua_pop(m_state, 1);

	return result;
}

void LuaScript::setFieldValue(const char* fieldName, const char* value, int tableIndex)
{
	lua_pushstring(m_state, value);
	lua_setfield(m_state, tableIndex, fieldName);
}

void LuaScript::setFieldValue(const char* fieldName, int value, int tableIndex)
{
	lua_pushinteger(m_state, value);
	lua_setfield(m_state, tableIndex, fieldName);
}

void LuaScript::setFieldValue(const char* fieldName, double value, int tableIndex)
{
	lua_pushnumber(m_state, value);
	lua_setfield(m_state, tableIndex, fieldName);
}

void LuaScript::setFieldValue(const char* fieldName, bool value, int tableIndex)
{
	lua_pushboolean(m_state, value);
	lua_setfield(m_state, tableIndex, fieldName);
}

bool LuaScript::pushFunction(int nHandler)
{
	toluafix_get_function_by_refid(m_state, nHandler);          /* stack: ... func */
	if (!lua_isfunction(m_state, -1))
	{
		this->error("[LUA ERROR] function refid '%d' does not reference a Lua function");
		lua_pop(m_state, 1);
		return false;
	}
	return true;
}
/*
int LuaScript::executeFunctionByHandler(int nHandler, int numArgs)
{
	if (pushFunction(nHandler))                                        
	{
		if (numArgs > 0)
		{
			lua_insert(m_state, -(numArgs + 1));                       
		}

		int traceback = 0;
		lua_getglobal(m_state, "__G__TRACKBACK__");                    
		if (!lua_isfunction(m_state, -1))
		{
			lua_pop(m_state, 1);                                       
		}
		else
		{
			traceback = -(numArgs + 2);
			lua_insert(m_state, traceback);                            
		}

		int error = 0;
		error = lua_pcall(m_state, numArgs, 1, traceback);             
		if (error)
		{
			if (traceback == 0)
			{
				this->error(lua_tostring(m_state, - 1));   
				lua_pop(m_state, 1); // remove error message from stack
			}
			return 0;
		}

		// get return value
		int ret = 0;
		if (lua_isnumber(m_state, -1))
		{
			ret = lua_tointeger(m_state, -1);
		}
		else if (lua_isboolean(m_state, -1))
		{
			ret = lua_toboolean(m_state, -1);
		}

		lua_pop(m_state, 1); // remove return value from stack
		return ret;
	}
	else
	{
		lua_pop(m_state, numArgs); // remove args from stack
		return 0;
	}
}
*/
///////////test
/*
 int LuaScript::reg_type(lua_State *ls, const char *name)
 {
	  //创建metatable，如果成功返回1，失败返回0
	 int result = luaL_newmetatable(ls, name);
	 if (result){
		 //把该metatable注册为全局的
		 lua_pushstring(ls, name);
		 lua_pushvalue(ls, 1);
		 lua_settable(ls, LUA_GLOBALSINDEX);   
	 }

	 if (result){
		 classevent(ls);
	 }

	 lua_pop(ls, 1);

	 return result;
 }

 void LuaScript::classevent(lua_State *ls)
 {
	 //classFunc cf = & LuaScript::class_index_event;

	 lua_pushstring(ls, "__index");
	 lua_pushcfunction(ls, class_index_event);
	 lua_rawset(ls,-3);
 }

 int LuaScript::reg_class(lua_State *ls, const char *name, const char *base)
 {
	 luaL_getmetatable(ls, name);          //stack: mt
	  //如果有基类，我们就把基类作为该类metatable的metatable
	 if(base && *base){
		  luaL_getmetatable(ls, base);     //stack: mt basemt
		  lua_setmetatable(ls, -2);        //stack: mt
	 }

	 lua_pop(ls, 1);

	 return 0;
 }

 int LuaScript::reg_function(lua_State* ls, const char * type, const char* name, lua_CFunction func)
 {
	 luaL_getmetatable(ls, type);
	 lua_pushstring(ls, name);
	 lua_pushcfunction(ls, func);
	 lua_rawset(ls, -3);
	 lua_pop(ls, 1);
	 return 0;
 }

 int LuaScript::reg_userdata(lua_State* ls, void* value, const char* type)
 {
	 luaL_getmetatable(ls, type);
	 lua_pushstring(ls, "script_ubox");
	 lua_rawget(ls, LUA_REGISTRYINDEX);
	 lua_pushlightuserdata(ls,value);

	 lua_rawget(ls, -2);                       // stack: mt ubox ubox[u] 
	 if (lua_isnil(ls, -1)){
		 lua_pop(ls, 1);                          // stack: mt ubox 
		 lua_pushlightuserdata(ls, value);
		 *(void**)lua_newuserdata(ls, sizeof(void *)) = value;   // stack: mt ubox u newud 
		 lua_pushvalue(ls, -1);                   // stack: mt ubox u newud newud 
		 lua_insert(ls, -4);                      // stack: mt newud ubox u newud
		 lua_rawset(ls, -3);                      // stack: mt newud ubox 
		 lua_pop(ls, 1);                          // stack: mt newud 
		 lua_pushvalue(ls, -2);            // stack: mt newud mt
		 lua_setmetatable(ls, -2);          // stack: mt newud 
	 }
	 lua_remove(ls, -2);
	 return 1;
 }
 
 int LuaScript::script_open(lua_State *ls)
 {
	 lua_pushstring(ls, "script_ubox");
	 lua_newtable(ls);  
	 lua_rawset(ls, LUA_REGISTRYINDEX);
	 return 0;
 }
*/
int LuaScript::excuteFunction(const char* funcName, ScriptObject* self, ScriptObject* target)
{
    lua_getglobal(m_state, funcName);
    pushObject(self);
    pushObject(target);
    return lua_pcall(m_state, 2, 0, NULL);
}

lua_State* LuaScript::getState()
{
	return m_state;
}

ScriptMethodTable::ScriptMethodTable(ScriptObject* owner)
{
	m_owner = owner;
	m_list = new map<const char*, ScriptMethod*>;
}

ScriptMethodTable::~ScriptMethodTable()
{
	clear();
	delete m_list;
}

void ScriptMethodTable::clear()
{

}

void ScriptMethodTable::addMethod(const char* name, ScriptMethod* method)
{
	m_list->insert(pair<const char*, ScriptMethod*>(name, method));
}

ScriptMethod* ScriptMethodTable::findMethod(ScriptObject* object, const char* name)
{
	map<const char*, ScriptMethod*>::iterator it_data = m_list->find(name);
	if (it_data != m_list->end()){
		return it_data->second;
	}
	return NULL;
}

bool ScriptMethodTable::doScriptQuery(ScriptObject* object, ScriptMessage* message)
{
	ScriptMethod* method = findMethod(object, message->query.c_str());
	if (method != NULL){
		message->script->pushMethod(m_owner, *method);
		return true;
	}
	return false;
}