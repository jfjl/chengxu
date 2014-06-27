//
//  gameScene.h
//  ppball
//
//  Created by carlor on 13-3-14.
//
//

#ifndef __ppball__gameScene__
#define __ppball__gameScene__

#include "SceneData.h"
#include "touchMap.h"
#include "DialogManager.h"

class gameScene : public SceneData
{
private:
	CC_SYNTHESIZE(CCLabelTTF, *m_Score, Score);
	CC_SYNTHESIZE(CCLabelTTF, *m_Top, Top)
    CC_SYNTHESIZE(ballMap*, m_BallMap, BallMap);
    CC_SYNTHESIZE(touchMap*, m_TouchMap, TouchMap);
    
    CC_SYNTHESIZE(bool, m_bStepEnabled, StepEnabled);
    CC_SYNTHESIZE(vector<int>, m_vStepPosition, StepPosition);
    CC_SYNTHESIZE(int, m_stepAffected, StepAffected);
    
    
	void onSceneNext(CCObject *ptouchMap);
	void onGameOver(CCObject *ptouchMap);

    int getXByPosKey(int posKey, int width);
    int getYByPosKey(int posKey, int width);
    int getRandomType();
    void getRandomPosition(touchMap *ballMap, std::vector<int> *pos, int count=NEXTCOUNT);

	void showBall(int classID, ball *pball);
	
    void randomShowBall();
    void randomShowMask(std::vector<int>* pos);
    void randomShowBall(ballVector *balls);
    
    int decStepAffected();
private:
	int script_disableBallMap(lua_State* L);
    int script_showNextPosition(lua_State* L);
public:
	int callFunction(lua_State* L);

public:
    gameScene();
    ~gameScene(void);

    static gameScene * scene();

    CC_SYNTHESIZE(int, m_Level, Level);
    
    bool init(ballMap *pBallMap, touchMap *pTouchMap);
    
    void onUpdate(float dt);
    void onActivate(CCNode* pNode, void* param);
    void onDeactivate();
    
    void start(int level);
	void next();
};

#endif /* defined(__ppball__gameScene__) */
