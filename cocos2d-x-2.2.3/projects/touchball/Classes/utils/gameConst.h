//
//  gameConst.h
//  pball
//
//  Created by sora on 13-3-18.
//
//

#ifndef __pball__gameConst__
#define __pball__gameConst__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

#define NEXTMAPSIZE_WIDTH 3
#define NEXTMAPSIZE_HEIGHT 1
#define GAMEMAPSIZE_WIDTH 9
#define GAMEMAPSIZE_HEIGHT 9
#define GAMEMAPMAXSIZE GAMEMAPSIZE_WIDTH * GAMEMAPSIZE_HEIGHT
#define MAPCELL_SIZE 40
#define TEXTURESCALE 0.85
#define NEXTCOUNT 3
#define REMOVECOUNT 5
#define GAMESCALE 2

#define EVENT_ACTION_SHOWCOMPLETE "EVENT_ACTION_SHOWCOMPLETE"
#define EVENT_ACTION_HIDECOMPLETE "EVENT_ACTION_HIDECOMPLETE"

#define EVENT_SCENE_NEXT "EVENT_SCENE_NEXT"
#define EVENT_SCENE_REMOVE "EVENT_SCENE_REMOVE"

#define EVENT_GAME_COMPLETE "EVENT_GAME_COMPLETE"
#define EVENT_SCORE_CHANGE "EVENT_SCORE_CHANGE"
#define EVENT_MAXSCORE_CHANGE "EVENT_MAXSCORE_CHANGE"

#define EVENT_BALL_MOVE "EVENT_BALL_MOVE"

#define EVENT_DIALOG_SHOW "EVENT_DIALOG_SHOW"
#define EVENT_DIALOG_HIDE "EVENT_DIALOG_HIDE"

#define EVENT_ENTER_GAME "EVENT_ENTER_GAME"

#define EVENT_SELECT_PROPS "EVENT_SELECT_PROPS"

#define MAGICBALL_ID 1000

#define GAMELEVEL {6, 6, 6}
inline int BALLCOUNT_LEVEL(int level) 
{
	int gamelevel[3] = GAMELEVEL;	
	return gamelevel[level-1];
}

//奖励ball条件
enum
{
    BALLTYPE_ROW    = 2,//消一行
    BALLTYPE_COL    = 3,//消一列
    BALLTYPE_X      = 4,//消x
    BALLTYPE_AROUND = 5,//消周围8格
    BALLTYPE_SAME   = 6,//消相同的
    
    AWARDBALL_SHAPE_ROW         = 1,
    AWARDBALL_SHAPE_COL         = 2,
    AWARDBALL_SHAPE_X           = 4,
    AWARDBALL_SHAPE_MULTSHAPE   = 8,
    AWARDBALL_SHAPE_MULTTYPE    = 16,
};

static const CCPoint BALLREMOVEDIRS[4] = {CCPointMake(1, 0), CCPointMake(0, 1),  CCPointMake(1, 1),  CCPointMake(1, -1)};
static const CCPoint BALLXREMOVEDIRS[4] = {CCPointMake(-1, -1), CCPointMake(1, 1), CCPointMake(1, -1), CCPointMake(-1, 1)};
static const CCPoint BALLAROUNDS[8] = {CCPointMake(-1, -1), CCPointMake(0, -1), CCPointMake(1, -1), CCPointMake(1, 0), CCPointMake(1, 1), CCPointMake(0, 1), CCPointMake(-1, 1), CCPointMake(-1, 0)};
static const CCPoint PATHDIRS[4] = {CCPointMake(0, 1), CCPointMake(1, 0), CCPointMake(0, -1), CCPointMake(-1, 0)};
    /*static const int NEXTMAPSIZE_WIDTH  = 3;
    static const int NEXTMAPSIZE_HEIGHT = 1;
    static const int GAMEMAPSIZE_WIDTH  = 9;
    static const int GAMEMAPSIZE_HEIGHT = 9;
    static const int MAPCELL_SIZE       = 40;
    static const float TEXTURESCALE     = 0.8;
    static const int NEXTCOUNT          = 3;
    
    static const int GAMELEVEL[3] = {3, 5, 8};
    
    const char *EVENT_ACTION_SHOWCOMPLETE = "EVENT_ACTION_SHOWCOMPLETE";


	*/

#endif /* defined(__pball__gameConst__) */
