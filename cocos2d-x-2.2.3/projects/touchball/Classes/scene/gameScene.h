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
    
	void onSceneRemove(CCObject *ptouchMap);
	void onSceneNext(CCObject *ptouchMap);
	void onGameOver(CCObject *ptouchMap);

    int getXByPosKey(int posKey, int width);
    int getYByPosKey(int posKey, int width);
    int getRandomType();
    void getRandomPosition(touchMap *ballMap, std::vector<int> *pos, int count=NEXTCOUNT);

	void showBall(int classID, ball *pball);
	
    void randomShowBall();
    void randomShowBall(ballVector *balls);
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
