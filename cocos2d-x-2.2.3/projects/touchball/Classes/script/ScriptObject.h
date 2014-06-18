//
//  ScriptObject.h
//  touchball
//
//  Created by carlor on 14-6-17.
//
//

#ifndef __touchball__ScriptObject__
#define __touchball__ScriptObject__

#include <string>

class ScriptObject;
class ClassInfo;

typedef ScriptObject* (*ObjectConstructorFn)(void);
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
	ScriptObject* CreateObject()const { return m_objectConstructor ? (*m_objectConstructor)() : 0;    }
	bool IsDynamic()const { return NULL != m_objectConstructor;}
	const std::string GetClassName()const { return m_className;}
	ObjectConstructorFn GetConstructor()const{ return m_objectConstructor;}
public:
    std::string m_className;
	ObjectConstructorFn m_objectConstructor;
};

#define DECLARE_CLASS(name) \
protected: \
static ClassInfo ms_classinfo; \
public:  \
virtual ClassInfo* GetClassInfo() const; \
static ScriptObject* CreateObject();

#define IMPLEMENT_CLASS_COMMON(name,func) \
ClassInfo name::ms_classinfo((#name), \
                             (ObjectConstructorFn) func); \
\
ClassInfo *name::GetClassInfo() const \
{return &name::ms_classinfo;}

#define IMPLEMENT_CLASS(name)            \
IMPLEMENT_CLASS_COMMON(name,name::CreateObject) \
ScriptObject* name::CreateObject()                   \
{ return new name;}

class ScriptObject;
typedef int (ScriptObject::*ScriptMethod)(void* param);

class ScriptObject
{
	DECLARE_CLASS(ScriptObject)
public:
	ScriptObject(){};
	virtual ~ScriptObject(){};
    
	static bool Register(ClassInfo* ci);
	static ScriptObject* CreateObject(std::string name);
public:
	virtual void scriptQuery(void* msg);
	virtual void scriptIndex(void* index);
	virtual void scriptSet(void* msg);
};
#endif /* defined(__touchball__ScriptObject__) */
