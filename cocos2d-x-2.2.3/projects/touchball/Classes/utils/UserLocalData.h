//
//  UserLocalData.h
//  touchball
//
//  Created by carlor on 14-6-27.
//
//

#ifndef __touchball__UserLocalData__
#define __touchball__UserLocalData__

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "ClientData.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../datas/ClientData.h"
#endif

class UserLocalData
{
public:
    static void setLevelInfo(int level, int score, int star);
    static bool getLevelInfo(int level, int& score, int& star);
    static void setMaxLevel(int level);
    static int  getMaxLevel();
private:
    static const char* getKey(string key, int subkey);
    static const char* getKey(string key);
};

#endif /* defined(__touchball__UserLocalData__) */
