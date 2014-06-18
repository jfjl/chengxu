#include "GameScript.h"

GameScript* g_gameScript;

GameScript::GameScript()
{
    g_gameScript = this;
	m_luaScript = new LuaScript();
}


GameScript::~GameScript(void)
{
}

LuaScript* GameScript::getScript()
{
	return m_luaScript;
}

void GameScript::loadInitScript()
{
    std::string path = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("init.lua");
    m_luaScript->open(path.c_str());
}

int GameScript::loadScript(const char* name, const char* strScript)
{
	int result;

	int stackIndex = m_luaScript->getStackSize();
	if (m_luaScript->run(strScript, name) != 0){
		result = -1;
	}else{
		result =  m_luaScript->getStackSize();//m_luaScript->getStackSize() - stackIndex - 1;
	}

	//m_luaScript->setStackSize(stackIndex);
	return result;
}

int GameScript::excuteFunction(const char* funcName, ScriptObject* self, ScriptObject* target)
{
	return m_luaScript->excuteFunction(funcName, self, target);
}

void GameScript::registerObject(const char* name, ScriptObject* scriptObject)
{
    m_luaScript->registerObject(name, scriptObject);
}
