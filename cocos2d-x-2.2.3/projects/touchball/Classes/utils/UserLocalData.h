//
//  UserLocalData.h
//  touchball
//
//  Created by carlor on 14-6-27.
//
//

#ifndef __touchball__UserLocalData__
#define __touchball__UserLocalData__

#include "ClientData.h"

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
