//
//  PresentManager.h
//  touchball
//
//  Created by carlor on 14-7-2.
//
//

#ifndef __touchball__PresentManager__
#define __touchball__PresentManager__

#include "BasicPresent.h"

class PresentManager : public CCLayer
{
public:
    PresentManager();
    ~PresentManager();
    
public:
    bool init(CCLayer* owner);
	static PresentManager* create(CCLayer* owner);

    virtual void onEnter();
    virtual void onExit();

    void onUpdate(float dt);
    
    BasicPresent* getPresent(int id);
    BasicPresent* addPresent(PresentType type, std::string sFileName, int imgWidth, int imgHeight, void* param);
    bool removePresent(int id);
private:
    int getSeed();
    void clear();
    
    void onHidePresent(CCObject* object);
    void onChangeScore(CCObject* data);
    
private:
    CCDictionary* m_mPresentList;
    int m_nSeed;
    CCLayer* m_pOwner;
};

extern PresentManager* g_presentManager;

#endif /* defined(__touchball__PresentManager__) */
