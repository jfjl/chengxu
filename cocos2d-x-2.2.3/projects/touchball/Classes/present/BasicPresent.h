//
//  BasicPresent.h
//  touchball
//
//  Created by carlor on 14-7-2.
//
//

#ifndef __touchball__BasicPresent__
#define __touchball__BasicPresent__

#include "cocos2d.h"

using namespace cocos2d;

enum
{
	PresentType_FloatNumber = 1
};
typedef unsigned int PresentType;


class BasicPresent : public CCLayer
{
public:
    BasicPresent();
    ~BasicPresent();

public:
	CC_SYNTHESIZE_READONLY(int, m_ID, ID);
    
    virtual bool init(int id, std::string fileName, int imgWidth, int imgHeight);

    virtual void onShow(void* param);
    virtual void onHide(void* param);
    virtual bool onUpdate(float dt);
};


#endif /* defined(__touchball__BasicPresent__) */
