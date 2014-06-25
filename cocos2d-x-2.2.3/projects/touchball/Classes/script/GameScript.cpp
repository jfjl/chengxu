#include "GameScript.h"

GameScript* g_gameScript;

GameScript::GameScript()
{
    g_gameScript = this;
	LuaInit();
}


GameScript::~GameScript(void)
{
	LuaClear();
}

int GameScript::loadScript(const char* name, const char* strScript)
{
	return g_luaReg->Run(strScript, name);
}

int GameScript::excuteFunction(const char* funcName, Object* self, Object* gameScene, Object* touchMap)
{
	return g_luaReg->excuteFunction(funcName, self, gameScene, touchMap);
}