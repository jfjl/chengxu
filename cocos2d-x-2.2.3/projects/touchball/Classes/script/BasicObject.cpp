//
//  BasicObject.cpp
//  touchball
//
//  Created by carlor on 14-6-17.
//
//

#include "BasicObject.h"
#include "LuaScript.h"

BasicObject::BasicObject()
{
    
}

BasicObject::~BasicObject()
{
    
}

int BasicObject::script_testScript(void* param)
{
	LuaScript* luaScript = (LuaScript*) param;
	int result = 0;
	
	int paramCount = luaScript->getStackSize() - 1;
	if (paramCount < 0) return result;
    
	int paramData[255];
	int msgId = luaScript->getInteger(1);
	for (int i = 0; i < paramCount; i++){
		paramData[i] = luaScript->getInteger(i+2);
	}
	//postMessage(msgId, paramData);
    
	return result;
    
}

void BasicObject::scriptQuery(void* msg)
{
	//int (Object::*ScriptMethod)(LuaScript* script);
	ScriptMessage* sm = (ScriptMessage*) msg;
	if (sm->query == "testScript"){
		DispatcherScriptMethod edsm = & BasicObject::script_testScript;
		sm->script->pushMethod(this, static_cast<ScriptMethod>(edsm));
	}
}

void BasicObject::scriptIndex(void* index)
{
//    ScriptIndex* si = (ScriptIndex*) index;
//    
//    if (si->index >= 0 && si->index < list.count) {
//        si->script->pushObject(list[si->index]);
//    }
}

void BasicObject::scriptSet(void* msg)
{
//	ScriptMessage* sm = (ScriptMessage*) msg;
//    if (sm->script->par) {
//
//    }
}

void BasicObject::update(float curTime)
{
    
}



