//
//  BasicDialog.cpp
//  touchball
//
//  Created by carlor on 14-6-7.
//
//

#include "BasicDialog.h"


void BasicDialog::release(void)
{
    CCObject::release();
}

bool BasicDialog::init()
{
    if (! CCLayer::init())
        return false;
    
    return true;
}

void BasicDialog::onShow(CCNode* pParent)
{
    pParent->addChild(this, 1000);
}

void BasicDialog::onHide(bool bRemove)
{
    if (bRemove)
        this->removeFromParentAndCleanup(true);
    else
        this->setVisible(false);
}

void BasicDialog::setName(const char* name)
{
    m_name = name;
}
