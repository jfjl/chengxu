//
//  BasicDialog.cpp
//  touchball
//
//  Created by carlor on 14-6-7.
//
//

#include "BasicDialog.h"
#include "DialogManager.h"

void BasicDialog::release(void)
{
    CCObject::release();
}

bool BasicDialog::init()
{
    if (! CCNode::init())
        return false;

    m_nRefreshTime = 0.1;
    m_nLastRefreshTime = 0;
    
    initDialog();
    if (m_pDialog) {
//        this->retain();
    }
    
    return true;
}

void BasicDialog::initDialog()
{
    m_pDialog = NULL;
}

void BasicDialog::onShow(void* param)
{
    
}

void BasicDialog::onHide(void* param)
{
    
}

bool BasicDialog::onUpdate(float dt)
{
    if (m_nLastRefreshTime == 0 || m_nLastRefreshTime < 0.0001) return true;
    if (m_nLastRefreshTime - dt < m_nRefreshTime) return false;
    
    m_nLastRefreshTime = dt;
    return true;    
}

void BasicDialog::setName(const char* name)
{
    m_name = name;
}
