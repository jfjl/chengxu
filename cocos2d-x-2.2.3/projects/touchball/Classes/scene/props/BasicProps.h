//
//  BasicProps.h
//  touchball
//
//  Created by carlor on 14-6-17.
//
//

#ifndef __touchball__BasicProps__
#define __touchball__BasicProps__

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../../utils/gameConst.h"
#include "../../script/ScriptObject.h"
#include "../map/ballMap.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "gameConst.h"
#include "ScriptObject.h"
#include "ballMap.h"
#endif

class BasicProps : public Object, public CCSprite
{
public:
    BasicProps();
    ~BasicProps();

	CC_SYNTHESIZE(int, m_ID, ID);
	CC_SYNTHESIZE_READONLY(int, m_ClassID, ClassID);
	CC_SYNTHESIZE_READONLY(int, m_pos, Pos);
    CC_SYNTHESIZE_READONLY(ballMap*, m_Owner, Owner);
    
    
    bool isAround(int pos);    
private:
	int script_maskMap(lua_State* L);
    int script_dismaskMap(lua_State* L);
public:
	int callFunction(lua_State* L);

public:
	bool init(ballMap* owner, int id, int classId);
	static BasicProps* create(ballMap* owner, int id, int classId);
public:
    virtual int show(int pos);
    virtual bool onRemoveBall(int pos);
};

#endif /* defined(__touchball__BasicProps__) */
