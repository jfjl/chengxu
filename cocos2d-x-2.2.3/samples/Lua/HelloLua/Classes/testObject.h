//
//  testObject.h
//  HelloLua
//
//  Created by carlor on 14-6-21.
//
//

#ifndef __HelloLua__testObject__
#define __HelloLua__testObject__

#include "cocos2d.h"

class testObject : public cocos2d::CCObject
{
public:
    testObject();
    ~testObject();
    
    int add(int a, int b);
};
#endif /* defined(__HelloLua__testObject__) */
