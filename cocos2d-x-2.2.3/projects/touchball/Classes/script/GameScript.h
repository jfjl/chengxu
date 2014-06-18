#pragma once
#ifndef __GAMESCRIPT_H__
#define __GAMESCRIPT_H__

#include "cocos2d.h"
#include "LuaScript.h"

class GameScript: public ScriptObject
{
private:
	LuaScript* m_luaScript;

public:
	GameScript();
	~GameScript(void);

	LuaScript* getScript();

    void loadInitScript();
	int loadScript(const char* name, const char* strScript);
	int excuteFunction(const char* funcName, ScriptObject* self, ScriptObject* target);
};

#endif