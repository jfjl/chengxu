//
//  BasicDialog.cpp
//  touchball
//
//  Created by carlor on 14-6-7.
//
//

#include "BasicDialog.h"

bool BasicDialog::init()
{
    if (! CCLayer::init())
        return false;
    
    return true;
}

void BasicDialog::onShow(CCNode* pParent)
{
    if (! pParent) return;
    
    pParent->addChild(this, 1000);
}

void BasicDialog::onHide(bool bRemove)
{
    if (bRemove)
        this->removeFromParentAndCleanup(true);
    else
        this->setVisible(false);
}