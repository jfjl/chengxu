//
//  UserLocalData.cpp
//  touchball
//
//  Created by carlor on 14-6-27.
//
//

#include "gameConst.h"
#include "UserLocalData.h"

const char* UserLocalData::getKey(string key, int subkey)
{
    string susername = "carlor";
    string result = susername + "_" + key;

    char buf[10] = {0};
    sprintf(buf, "_%d", subkey);
    result = result + buf;
    
    return result.c_str();
}

const char* UserLocalData::getKey(string key)
{
    string susername = "carlor";
    string result = susername + "_" + key;
    return result.c_str();
}


void UserLocalData::setLevelInfo(int level, int score, int star)
{
    setMaxLevel(level);

    int oldScore = 0;
    int oldStar = 0;
    if (getLevelInfo(level, oldScore, oldStar)) {
        if (oldScore > score && oldStar > star) {
            return;
        }
        if (oldScore > score) {
            score = oldScore;
        }
        if (oldStar > star) {
            star = oldStar;
        }
    }
    
    
    char buf[10] = {0};
    sprintf(buf, "%d;%d", score, star);
    string value = buf;
    
    CCUserDefault::sharedUserDefault()->setStringForKey(getKey("level", level), value);
}

bool UserLocalData::getLevelInfo(int level, int& score, int& star)
{
    string slevelInfo = CCUserDefault::sharedUserDefault()->getStringForKey(getKey("level", level));

    vector<int> values = ClientData::splitUchar2Int((unsigned char*)slevelInfo.c_str());
    if (values.size() != 2) return false;
    
    score = values[0];
    star = values[1];
    
    return true;
}

void UserLocalData::setMaxLevel(int level)
{
    int maxLevel = getMaxLevel();
    if (level <= maxLevel) return;

    CCUserDefault::sharedUserDefault()->setIntegerForKey(getKey("maxLevel"), level);
}

int UserLocalData::getMaxLevel()
{
    return CCUserDefault::sharedUserDefault()->getIntegerForKey(getKey("maxLevel"));
}
