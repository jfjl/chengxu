//
//  ClientData.h
//  pball
//
//  Created by carlor on 14-6-4.
//
//

#ifndef __pball__ClientData__
#define __pball__ClientData__

#include "cocos2d.h"
#include "levelData.h"
#include "ballData.h"
#include "propsData.h"
#include "mapData.h"

using namespace cocos2d;

struct KeyValue
{
    int key;
    int value;
};

typedef vector<KeyValue> KeyValueVector;
typedef map<int, KeyValue> KeyValueMap;

struct levelCfg		: levelData
{
    vector<int> vBallIds;
    vector<int> vRewardProps;
    vector<int> vCanUseProps;
    KeyValueVector vSpecialBallIds;
    
    levelCfg()
    {
        vBallIds.clear();
        vRewardProps.clear();
        vCanUseProps.clear();
        vSpecialBallIds.clear();
    }
};
struct ballCfg		: ballData
{
    int nBasicBall;
    int nRemoveCount;
    ballCfg()
    {
        nRemoveCount = 1;
    }
};
struct propsCfg     : propsData {};
struct mapCfg       : mapData
{
    vector<int> vMapCell;
    
    mapCfg()
    {
        vMapCell.clear();
    }
};


class ClientData: public cocos2d::CCObject
{
public:
    ClientData();
    ~ClientData();
    
    bool init();
public:
    vector<int>     splitUchar2Int(unsigned char * data,char C = ';');
    vector<string>  splitUchar2String(unsigned char * data,char C = ';');
    vector<float>   spliteUchar2Float(unsigned char *data,char C = ';');
    KeyValueVector  spliteUchar2KeyValue(unsigned char* data, char C1 = ';', char C2 = ',', bool bCalcRate = true);
public:
    const levelCfg*   getLevelCfg(int key);
    const ballCfg*    getBallCfg(int key);
    const propsCfg*   getPropsCfg(int key);
    const mapCfg*     getMapCfg(int key);
    
    const map<int, levelCfg>&  getLevelList();
    const map<int, ballCfg>&   getBallList();
    const map<int, propsCfg>&  getPropsList();
    const map<int, mapCfg>&    getMapList();
private:
    bool initData();
    void releaseData();
    
    bool loadLevelData();
    bool loadBallsData();
    bool loadPropsData();
    bool loadMapData();
private:
    std::map<int, levelCfg> m_mLevelCfg;
    std::map<int, ballCfg> m_mBallCfg;
    std::map<int, propsCfg> m_mPropsCfg;
    std::map<int, mapCfg> m_mMapCfg;
};

extern	ClientData* g_clientData;

#endif /* defined(__pball__ClientData__) */
