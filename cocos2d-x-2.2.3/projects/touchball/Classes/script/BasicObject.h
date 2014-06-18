//
//  BasicObject.h
//  touchball
//
//  Created by carlor on 14-6-17.
//
//

#ifndef __touchball__BasicObject__
#define __touchball__BasicObject__

#include "ScriptObject.h"

typedef void (*eventCallBack) (void* param);

struct EventData
{
	int eventId;
	void* param;
	eventCallBack callBack;
};

class BasicObject;
typedef int (BasicObject::*DispatcherScriptMethod)(void* param);


class BasicObject : public ScriptObject
{
public:
    BasicObject();
    ~BasicObject();
private:
	int script_testScript(void* param);
protected:
	virtual void scriptQuery(void* msg);
	virtual void scriptIndex(void* index);
	virtual void scriptSet(void* msg);
public:
    virtual void update(float curTime);
private:
    int m_ID;
    int m_ClassID;

};

#endif /* defined(__touchball__BasicObject__) */
