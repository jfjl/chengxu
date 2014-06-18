//
//  ScriptObject.cpp
//  touchball
//
//  Created by carlor on 14-6-17.
//
//

#include <map>
#include "ScriptObject.h"

using namespace std;

static map<string, ClassInfo*>* classInfoMap = NULL;

IMPLEMENT_CLASS(ScriptObject)

bool ScriptObject::Register(ClassInfo* ci)
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
ScriptObject* ScriptObject::CreateObject(std::string name)
{
	std::map< string,ClassInfo*>::const_iterator iter = classInfoMap->find(name);
	if(classInfoMap->end() != iter)  {
		return iter->second->CreateObject();
	}
	return NULL;
}

bool Register(ClassInfo* ci)
{
	return ScriptObject::Register(ci);
}

void ScriptObject::scriptQuery(void* msg)
{
    
}

void ScriptObject::scriptIndex(void* index)
{
    
}

void ScriptObject::scriptSet(void* msg)
{
    
}