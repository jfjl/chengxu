//
//  ScriptObject.h
//  touchball
//
//  Created by carlor on 14-6-21.
//
//

#ifndef __touchball__ScriptObject__
#define __touchball__ScriptObject__

#include "LuaRegDef.h"

using namespace std;

class Object;
class ClassInfo;

typedef Object* (*ObjectConstructorFn)(void);
bool Register(ClassInfo* ci);

class ClassInfo
{
public:
	ClassInfo(const std::string className,ObjectConstructorFn ctor)
    :m_className(className) ,m_objectConstructor(ctor)
	{
		Register(this);
	}
	virtual ~ClassInfo(){}
	Object* CreateObject()const { return m_objectConstructor ? (*m_objectConstructor)() : 0;    }
	bool IsDynamic()const { return NULL != m_objectConstructor;}
	const std::string GetClassName()const { return m_className;}
	ObjectConstructorFn GetConstructor()const{ return m_objectConstructor;}
public:
	string m_className;
	ObjectConstructorFn m_objectConstructor;
};

#define DECLARE_CLASS(name) \
protected: \
static ClassInfo ms_classinfo; \
public:  \
virtual ClassInfo* GetClassInfo() const; \
static Object* CreateObject();

#define IMPLEMENT_CLASS_COMMON(name,func) \
ClassInfo name::ms_classinfo((#name), \
                             (ObjectConstructorFn) func); \
\
ClassInfo *name::GetClassInfo() const \
{return &name::ms_classinfo;}

#define IMPLEMENT_CLASS(name)            \
IMPLEMENT_CLASS_COMMON(name,name::CreateObject) \
Object* name::CreateObject()                   \
{ return new name;}

class Object;
typedef int (Object::*ScriptMethod)(void* param);

class Object
{
	DECLARE_CLASS(Object)
public:
	Object(){}
	virtual ~Object(){}
	static bool Register(ClassInfo* ci);
	static Object* CreateObject(string name);

////////script
private:
	int script_postMessage(lua_State* L);
public:
	virtual int callFunction(lua_State* L);
};

#endif /* defined(__touchball__ScriptObject__) */
