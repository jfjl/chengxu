//
//  ScriptObject.cpp
//  touchball
//
//  Created by carlor on 14-6-21.
//
//

#include "ScriptObject.h"

static map< string, ClassInfo*> *classInfoMap = NULL;
using namespace std;
IMPLEMENT_CLASS(Object)

bool Object::Register(ClassInfo* ci)
{
	if(!classInfoMap)   {
		classInfoMap = new map< string,ClassInfo*>();
	}
	if(ci)  {
		if(classInfoMap->find(ci->m_className) == classInfoMap->end()){
			classInfoMap->insert(map< string,ClassInfo*>::value_type(ci->m_className,ci));
		}
	}
	return true;
}
Object* Object::CreateObject(std::string name)
{
	std::map< string,ClassInfo*>::const_iterator iter = classInfoMap->find(name);
	if(classInfoMap->end() != iter)  {
		return iter->second->CreateObject();
	}
	return NULL;
}

bool Register(ClassInfo* ci)
{
	return Object::Register(ci);
}

int Object::script_postMessage(lua_State* L)
{
	return 0;
	/*
     LuaScript* luaScript = (LuaScript*) param;
     int result = 0;
     
     int paramCount = luaScript->getStackSize() - 1;
     if (paramCount < 0) return result;
     
     int paramData[255];
     int msgId = luaScript->getInteger(1);
     for (int i = 0; i < paramCount; i++){
     paramData[i] = luaScript->getInteger(i+2);
     }
     postMessage(msgId, paramData);
     
     return result;
     */
}


int Object::callFunction(lua_State* L)
{
	const char* funcName = lua_tostring(L, 2);
	if (strcmp(funcName, "postMessage") == 0)
		return script_postMessage(L);
}
