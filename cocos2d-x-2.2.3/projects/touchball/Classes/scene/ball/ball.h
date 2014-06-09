//
//  ball.h
//  ppball
//
//  Created by carlor on 13-3-13.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#ifndef __BALL_H__
#define __BALL_H__

#include "gameConst.h"
#include "cocos2d.h"
using namespace cocos2d;

enum
{
	BallStateNormal       = 1,
	BallStateSelected	  = 2,
	BallStatePlayDelete	  = 3,
	BallStateHide		  = 4,
	BallStatePlayShow     = 5
};
typedef unsigned int BallStates;

class ball : public CCSprite
{
private:
	void onShow();
	void onHide();
protected:
	CC_SYNTHESIZE(CCSprite*, m_StateSprite, StateSprite);
public:
	ball(void);
	~ball(void);
public:
	CC_SYNTHESIZE(int, m_ID, ID);
	CC_SYNTHESIZE_READONLY(int, m_ClassID, ClassID);
	CC_SYNTHESIZE_READONLY(BallStates, m_BallState, BallState);
	CC_SYNTHESIZE_READONLY(CCPoint, m_pos, Pos);
public:
	bool init(int classID, CCPoint pos);
	static ball* create(int classID, CCPoint pos);
    
	void setVisible(bool bVisible, bool bPlayAction);
	void setBallState(BallStates value);
	void setBallClass(int value);
    void setBallAddition();
    
	void playShow();
	void playHide();
    
private:
    CCString* getImage(int classId);
    CCLabelTTF* m_pLabel;
};

#endif