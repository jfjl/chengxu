//
//  DialogEvent.h
//  pball
//
//  Created by sora on 13-5-7.
//
//

#ifndef __pball__DialogEvent__
#define __pball__DialogEvent__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class DialogEvent : public CCObject
{
public:
    CC_SYNTHESIZE(std::string, m_key, key);
    CC_SYNTHESIZE(int, m_value, value);
    CC_SYNTHESIZE(std::string, m_strvalue, strvalue);
};

#endif /* defined(__pball__DialogEvent__) */
