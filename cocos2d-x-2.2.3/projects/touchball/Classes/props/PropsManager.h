//
//  PropsManager.h
//  touchball
//
//  Created by carlor on 14-6-17.
//
//

#ifndef __touchball__PropsManager__
#define __touchball__PropsManager__

#include "BasicProps.h"
#include "ballMap.h"

typedef std::map<int, BasicProps*> PropsMap;

class PropsManager : public Object, public CCLayer
{
private:
	CC_SYNTHESIZE_READONLY(int, m_seed, Seed);
    int newId();
public:
    PropsManager();
    ~PropsManager();

	bool init(ballMap* owner, int width, int height);
	static PropsManager* create(ballMap* owner, int width, int height);

    CC_SYNTHESIZE(int, m_SelectId, SelectId);
    CC_SYNTHESIZE(int, m_SelectPos, SelectPos);
	CC_SYNTHESIZE_READONLY(int, m_Width, Width);
	CC_SYNTHESIZE_READONLY(int, m_Height, Height);
    CC_SYNTHESIZE_READONLY(ballMap*, m_Owner, Owner);
	CC_SYNTHESIZE(PropsMap, m_PropsList, PropsList);
private:
    void clear();
    
    BasicProps* addProps(int classId);
    bool deleteProps(int id);
public:
    void show(int pos, int classId);
    void pick(int value);
    void put(int pos);
    void hide(int pos);
    
    void onRemoveBall(int pos);
    void onSelectProps(CCObject* pEvent);
private:
	int script_getSelectPos(lua_State* L);
public:
	int callFunction(lua_State* L);

};

#endif /* defined(__touchball__PropsManager__) */
