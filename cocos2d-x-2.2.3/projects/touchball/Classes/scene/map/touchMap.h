//
//  touchMap.h
//  ppball
//
//  Created by sora on 13-3-13.
//
//

#ifndef __ppball__touchMap__
#define __ppball__touchMap__

#include "pathFinder.h"

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

    bool checkMove(int srcPos, int x, int y);
    bool onUpdateMove(float dt);

	bool checkOver();
	void remove(CCArray *balls);
	bool checkRemove();
	bool onEnterEvent(CCObject *pball);
	void onShowComplete(CCObject *pball);
	void onHideComplete(CCObject *pball);
	void onMoveComplete(ball *pball);
protected:
	void addEventLister(CCObject *pball);
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

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    int m_level;
    std::map<int, int> m_mShapes;
};
#endif /* defined(__ppball__touchMap__) */
