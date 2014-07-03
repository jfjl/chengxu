//
//  DialogEvent.h
//  pball
//
//  Created by sora on 13-5-7.
//
//

#ifndef __pball__DialogEvent__
#define __pball__DialogEvent__

#include "cocos2d.h"
using namespace cocos2d;

class DialogEvent : public CCObject
{
public:
    DialogEvent();
    ~DialogEvent();
    
    CC_SYNTHESIZE(std::string, m_key, key);
    CC_SYNTHESIZE(int, m_value, value);
    CC_SYNTHESIZE(std::string, m_strvalue, strvalue);
    
    void addKeyValue(std::string key, int value);
    void addKeyValue(std::string key, std::string value);
    
    int getIntValue(std::string key);
    std::string getStrValue(std::string key);
private:
    std::map<std::string, int> m_nValueList;
    std::map<std::string, std::string> m_sValueList;
};

#endif /* defined(__pball__DialogEvent__) */
