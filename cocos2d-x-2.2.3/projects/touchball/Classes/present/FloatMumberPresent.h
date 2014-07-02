//
//  FloatMumberPresent.h
//  touchball
//
//  Created by carlor on 14-7-2.
//
//

#ifndef __touchball__FloatMumberPresent__
#define __touchball__FloatMumberPresent__

#include "BasicPresent.h"
#include "cocos-ext.h"

class FloatNumberPresent : public BasicPresent
{
public:
    FloatNumberPresent();
    ~FloatNumberPresent();
private:
    void onMoveBy();
public:
    virtual bool init(int id, std::string fileName, int imgWidth, int imgHeight, void* param);
	static FloatNumberPresent* create(int id, std::string fileName, int imgWidth, int imgHeight, void* param);
    
    virtual void onShow(void* param);
    virtual void onHide(void* param);
private:
    ui::LabelAtlas* m_lblNumber;
};
#endif /* defined(__touchball__FloatMumberPresent__) */
