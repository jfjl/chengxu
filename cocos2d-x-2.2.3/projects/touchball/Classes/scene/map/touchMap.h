//
//  touchMap.h
//  ppball
//
//  Created by sora on 13-3-13.
//
//

#ifndef __ppball__touchMap__
#define __ppball__touchMap__

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../path/pathFinder.h"
#include "../props/PropsManager.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "pathFinder.h"
#include "PropsManager.h"
#endif

enum
{
	MapStateNormal        = 0,
	MapStateShow          = 1,
	MapStateHide    	  = 2,
	MapStateMove 		  = 3
};
typedef unsigned int MapStates;

class touchMap : public ballMap
{
private:
    //event
    void registerWithTouchDispatcher(void);
	CCPoint getLocalPoint(CCPoint value);
    
    CC_SYNTHESIZE(int, m_MapState, MapState);
	CC_SYNTHESIZE(CCDictionary, *m_actionBalls, ActionBalls);
	CC_SYNTHESIZE(int, m_ActionCount, ActionCount);
    
    //move
    CC_SYNTHESIZE(pathFinder, *m_PathFinder, PathFinder);
    CC_SYNTHESIZE(int, m_curMovePos, CurMovePos);
    
    //props
    CC_SYNTHESIZE(PropsManager*, m_pPropsManager, mPropsManager);
    

    bool checkMove(int srcPos, int x, int y);
    bool onUpdateMove(float dt);

	bool checkOver();
	void remove(CCArray *balls);
	bool checkRemove();
	bool onEnterEvent(CCObject *pball);
	void onShowComplete(CCObject *pball);
	void onHideComplete(CCObject *pball);
	void onMoveComplete(ball *pball);
    
    void clear();
    void createMask(int pos, int r, int g, int b);
    void deleteMask(int pos);
protected:
	void addEventLister(CCObject *pball);
private:
	int script_destroyBall(lua_State* L);
    int script_getOldSelect(lua_State* L);
    int script_getSelect(lua_State* L);
    int script_changePosition(lua_State* L);
    int script_disableSpecialBall(lua_State* L);
    int script_clearSelectProps(lua_State* L);
    int script_getBall(lua_State* L);
    int script_moveto(lua_State* L);
public:
	int callFunction(lua_State* L);

public:
	touchMap(void);
	~touchMap(void);
    
	bool init(const char*fileName, int width, int height);
    static touchMap* create(const char*fileName, int width, int height);

    void setLevel(int level);
    int getLevel();
    void getRandomPosition(std::vector<int> *pos, int count=NEXTCOUNT);
    
    void onUpate(float dt);
    
	void playAction(ball *pball);
    void incActionCount();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    //props
    void maskMap(int pos, int maskType);
    void disMaskMap(int pos);
private:
    int m_level;
    std::map<int, int> m_mShapes;
    std::map<int, CCLayerColor*> m_mMaskSprite;
};
#endif /* defined(__ppball__touchMap__) */
