#pragma once
#ifndef __GAMESCRIPT_H__
#define __GAMESCRIPT_H__

#include "LuaReg.h"
#include "ScriptObject.h"

class GameScript: public Object
{
public:
	GameScript();
	~GameScript(void);

	int loadScript(const char* name, const char* strScript);
	int excuteFunction(const char* funcName, Object* self, Object* gameScene, Object* touchMap);
};

extern GameScript* g_gameScript;

#endif