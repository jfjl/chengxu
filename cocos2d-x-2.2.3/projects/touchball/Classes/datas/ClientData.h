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
    ET_ONSHOW   = 3,
};

const char StrFuncHeader[] = "function ";
const char StrFuncHeader2[] = "(self, gameScene, touchMap, ...)";
const char StrFuncEnd[] = "end;";
const char StrNilFunc[] = "nil;";

inline string getEventFunction(string funcName, string funcContent)
{
    if (funcContent != ""){
        return StrFuncHeader + funcName + StrFuncHeader2 + funcContent + StrFuncEnd;
	}
    else{
		return StrNilFunc;
	}
}

inline string getFuncName(string configName, string eventName, int key)
{
    stringstream temp;
    temp<<key;
    string skey = temp.str();
    
//    configName.append("_");
    configName.append(skey);
//    configName.append("_");
    configName.append(eventName);
    return configName;
}

struct KeyValue
{
    int key;
    int value;
};

typedef vector<KeyValue> KeyValueVector;
typedef map<int, KeyValue> KeyValueMap;

struct StrKeyValue
{
    string key;
    string value;
};
typedef map<string, StrKeyValue> EventMap;

class EventList
{
public:
    EventMap mEventFuncs;
public:
    EventList()
    {
        mEventFuncs.clear();
    }
    
    void addEventFunction(string configName, string eventName, int key, string funcContent)
    {
        string funcName = getFuncName(configName, eventName, key);
        string func = getEventFunction(funcName, funcContent);
        StrKeyValue keyValue;
        keyValue.key = funcName;
        keyValue.value = func;
        mEventFuncs[eventName] = keyValue;
        CCLog("eventName = %s , \n funcName = %s, \n func = %s \n", eventName.c_str(), funcName.c_str(), func.c_str());
    }
};

struct levelCfg		: levelData
{
    vector<int> vBallIds;
    vector<int> vRewardProps;
    vector<int> vCanUseProps;
    KeyValueVector vSpecialBallIds;
    KeyValueVector vPropsPos;
    
    levelCfg()
    {
        vBallIds.clear();
        vRewardProps.clear();
        vCanUseProps.clear();
        vSpecialBallIds.clear();
        vPropsPos.clear();
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
struct propsCfg     : propsData
{
    EventList* pEventList;
    
    propsCfg()
    {
        pEventList = new EventList();
    }
};
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
    
    void loadScript();
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
