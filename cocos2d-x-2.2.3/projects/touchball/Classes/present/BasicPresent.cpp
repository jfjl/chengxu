//
//  BasicPresent.cpp
//  touchball
//
//  Created by carlor on 14-7-2.
//
//

#include "BasicPresent.h"

BasicPresent::BasicPresent()
{
    
}

BasicPresent::~BasicPresent()
{
    CCLog("///////////////////dfddddd");
}

bool BasicPresent::init(int id, std::string fileName, int imgWidth, int imgHeight)
{
    if (! CCLayer::init()) {
        return false;
    }
    
    m_ID = id;
    
    return true;
}

void BasicPresent::onShow(void *param)
{
    
}

void BasicPresent::onHide(void *param)
{
    this->removeAllChildrenWithCleanup(true);
}

bool BasicPresent::onUpdate(float dt)
{
    return true;
}
