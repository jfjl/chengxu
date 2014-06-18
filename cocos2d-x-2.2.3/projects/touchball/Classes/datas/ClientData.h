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
#include "awardBallData.h"

using namespace cocos2d;

enum EventType
{
    ET_ONPICK   = 1,
    ET_ONPUT    = 2,
};

inline int getEventType(const char* eventName)
{
    if (strcmp("OnPick", eventName) == 0)
    {
        return ET_ONPICK;
    }
    else if (strcmp("OnPut", eventName) == 0)
    {
        return ET_ONPUT;
    }
    
    return 0;
}

const char StrFuncHeader[] = "function ";
const char StrFuncHeader2[] = "(self, target, ...)";
const char StrFuncEnd[] = "end;";
const char StrNilFunc[] = "nil;";

inline const char* getEventFunction(const char* funcName, const char* funcContent)
{
	if (funcContent != ""){
    
        return StrFuncHeader;
		//return StrFuncHeader + funcName + StrFuncHeader2 + funcContent + StrFuncEnd;
	}else{
		return StrNilFunc;
	}
    
}

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
struct awardBallCfg : awardBallData{};


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
    const levelCfg*     getLevelCfg(int key);
    const ballCfg*      getBallCfg(int key);
    const propsCfg*     getPropsCfg(int key);
    const mapCfg*       getMapCfg(int key);
    const awardBallCfg* getAwardBallCfg(int key);
    const awardBallCfg* getAwardBallCfgByBallType(int key); 
    
    const map<int, levelCfg>&       getLevelList();
    const map<int, ballCfg>&        getBallList();
    const map<int, propsCfg>&       getPropsList();
    const map<int, mapCfg>&         getMapList();
    const map<int, awardBallCfg>&   getAwardBallList();
private:
    bool initData();
    void releaseData();
    
    bool loadLevelData();
    bool loadBallsData();
    bool loadPropsData();
    bool loadMapData();
    bool loadAwardBallData();
private:
    map<int, levelCfg>      m_mLevelCfg;
    map<int, ballCfg>       m_mBallCfg;
    map<int, propsCfg>      m_mPropsCfg;
    map<int, mapCfg>        m_mMapCfg;
    map<int, awardBallCfg>  m_mAwardBallCfg;
    map<int, int>           m_mBallKeyAwardBallCfg;
};

extern	ClientData* g_clientData;

#endif /* defined(__pball__ClientData__) */
