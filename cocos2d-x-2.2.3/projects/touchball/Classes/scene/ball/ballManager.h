//
//  ballManager.h
//  ppball
//
//  Created by carlor on 13-3-13.
//
//

#ifndef __ppball__ballManager__
#define __ppball__ballManager__

#include <iostream>
#include <vector>
#include "ball.h"

typedef std::vector<ball*> ballVector;

class ballManager
{
private:
	CC_SYNTHESIZE_READONLY(int, m_seed, Seed);
    
	int getKey(int x, int y);
	void clear();

	CCArray* collectRemoveBalls(int classID, int x, int y, int ox, int oy);
	bool compare(ball *pball, int classID);
public:
	ballManager(void);
	~ballManager(void);
    
	bool init(int width, int height);
	static ballManager* create(int width, int height);
    
	int add(ball* value);
    
	CC_SYNTHESIZE(ballVector*, m_BallList, BallList);
    CC_SYNTHESIZE(int, m_SelectId, SelectId);
	CC_SYNTHESIZE_READONLY(int, m_Width, Width);
	CC_SYNTHESIZE_READONLY(int, m_Height, Height);
    
	//move
	int select(int x, int y);
	int show(int classId, int x, int y);
	int hide(int x, int y);
    int show(int classId, int posKey);
    int hide(int posKey);
    
    bool checkMove(int x, int y);
    
	//
	int playShow(int classId, int x, int y);
	int playHide(int x, int y);
	int playHide(ball *pball);
    
	//remove
	CCArray* checkRemove(int x, int y, int removeCount);
    
    //
	ball* getBall(int x, int y);
    ball* getBall(int pos);
    void getVisibleBall(bool vis, CCArray *balls);
};

#endif /* defined(__ppball__ballManager__) */
