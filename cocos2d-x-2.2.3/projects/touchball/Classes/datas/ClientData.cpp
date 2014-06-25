//
//  ClientData.cpp
//  pball
//
//  Created by carlor on 14-6-4.
//
//

#include "ClientData.h"
#include "GameScript.h"

ClientData* g_clientData = 0;

ClientData::ClientData()
{
    g_clientData = this;
    init();
}

ClientData::~ClientData()
{
    
}

bool ClientData::init()
{
    initData();
    return true;
}

bool ClientData::initData()
{
    if (! loadLevelData())
    {
        CCLOG("load level data failed");
        return false;
    }

    if (! loadBallsData())
    {
        CCLOG("load ball data failed");
        return false;
    }
    
    if (! loadPropsData())
    {
        CCLOG("load props data failed");
        return false;
    }
    
    if (! loadMapData())
    {
        CCLOG("load map data failed");
        return false;
    }
    
    if (! loadAwardBallData())
    {
        CCLOG("load awardball data failed");
        return false;
    }
    
    CCLOG("load bin complete");
    
    loadScript();
    CCLOG("load script complete");
    
    return true;
}

void ClientData::releaseData()
{
    
}

#pragma mark 分割字符串 默认以分号分割
vector<int> ClientData::splitUchar2Int(unsigned char * data,char C )
{
    vector<string> szStrng=splitUchar2String(data,C);
    vector<int>szInt;
    vector<string>::iterator it=szStrng.begin();
    while (it!=szStrng.end())
    {
        int value = atoi((*it).c_str());
        szInt.push_back(value);
        it++;//自增
    }
    return szInt;
}

vector<string> ClientData::splitUchar2String(unsigned char * data,char C)
{
    vector<string> result;
    
    std::string str((const char *)data);
    string substring;
    string::size_type start=0,index;
    do {
        index=str.find_first_of(C,start);
        if (index!=string::npos) {
            substring=str.substr(start,index-start);
            result.push_back(substring);
            start=str.find_first_not_of(C,index);
            if (start==string::npos) {
                return result;
            }
        }
    } while (index!=string::npos);
    substring=str.substr(start);
    result.push_back(substring);
    return result;
}

vector<float> ClientData::spliteUchar2Float(unsigned char *data,char C )
{
    vector<string> szStrng=splitUchar2String(data,C);
    vector<float>szfloat;
    vector<string>::iterator it=szStrng.begin();
    while (it!=szStrng.end()) {
        szfloat.push_back(atof((*it).c_str()));
        it++;
    }
    return szfloat;
}

KeyValueVector ClientData::spliteUchar2KeyValue(unsigned char* data, char C1, char C2, bool bCalcRate)
{
    KeyValueVector vKeyValue;
    int maxValue = 0;
    
    vector<string> szStrng=splitUchar2String(data,C1);
    for (size_t i = 0; i < szStrng.size(); i++)
    {
        vector<int> szInt = splitUchar2Int((unsigned char*)szStrng[i].c_str(), C2);
        if (szInt.size() != 2) continue;

        maxValue += szInt[1];

        KeyValue mKeyValue;
        mKeyValue.key = szInt[0];
        if (bCalcRate)
            mKeyValue.value = maxValue;
        else
            mKeyValue.value = szInt[1];
        
        vKeyValue.push_back(mKeyValue);
    }
    
    return vKeyValue;
}

bool ClientData::loadLevelData()
{
    HawkBinCfgFile sReader;
    unsigned long len=0;
    unsigned char * buffer =CCFileUtils::sharedFileUtils()->getFileData("levelData.bin", "rb", &len);
    
    bool bOK=false;
    if (sReader.LoadFromMem(buffer, len))
    {
        Int32 iCount = 0;
        if(sReader.Read(&iCount,sizeof(iCount)))
        {
            bOK=true;
            for (Int32 i=0;i<iCount;i++)
            {
                levelCfg sCfg;
                if (sReader.Read(&sCfg,sizeof(levelData)))
                {
                    sCfg.vBallIds = splitUchar2Int(sCfg.BallIds);
                    sCfg.vRewardProps = splitUchar2Int(sCfg.RewardsProps);
                    sCfg.vCanUseProps = splitUchar2Int(sCfg.CanUseProps);
                    sCfg.vSpecialBallIds = spliteUchar2KeyValue(sCfg.SpecialBallIds);
                    sCfg.vPropsPos = spliteUchar2KeyValue(sCfg.PropsPos, ';', ',', false);
                    
                    m_mLevelCfg[sCfg.Level]=sCfg;
                }
                else
                {
                    bOK=false;
                    break;
                }
            }
            
        }
    }
    delete [] buffer;
    return bOK;
}

bool ClientData::loadBallsData()
{
    HawkBinCfgFile sReader;
    unsigned long len=0;
    unsigned char * buffer =CCFileUtils::sharedFileUtils()->getFileData("ballData.bin", "rb", &len);
    
    bool bOK=false;
    if (sReader.LoadFromMem(buffer, len))
    {
        Int32 iCount = 0;
        if(sReader.Read(&iCount,sizeof(iCount)))
        {
            bOK=true;
            for (Int32 i=0;i<iCount;i++)
            {
                ballCfg sCfg;
                if (sReader.Read(&sCfg,sizeof(ballData)))
                {
                    sCfg.nRemoveCount = 1;
                    sCfg.nBasicBall = (sCfg.BallId / 1000) * 1000;
                    m_mBallCfg[sCfg.BallId]=sCfg;
                }
                else
                {
                    bOK=false;
                    break;
                }
            }
            
        }
    }
    
    for (map<int, ballCfg>::iterator it = m_mBallCfg.begin(); it != m_mBallCfg.end(); it++) {
        const ballCfg* temp = getBallCfg(it->second.PreBall);
        
        while (temp) {
            it->second.nRemoveCount++;
            temp = getBallCfg(temp->PreBall);
        }
    }
    delete [] buffer;
    return bOK;
}

bool ClientData::loadPropsData()
{
    HawkBinCfgFile sReader;
    unsigned long len=0;
    unsigned char * buffer =CCFileUtils::sharedFileUtils()->getFileData("propsData.bin", "rb", &len);
    
    bool bOK=false;
    if (sReader.LoadFromMem(buffer, len))
    {
        Int32 iCount = 0;
        if(sReader.Read(&iCount,sizeof(iCount)))
        {
            bOK=true;
            for (Int32 i=0;i<iCount;i++)
            {
                propsCfg sCfg;
                if (sReader.Read(&sCfg,sizeof(propsData)))
                {
                    if (sCfg.OnPick) {
                        sCfg.pEventList->addEventFunction("PropsConfig", "OnPick", sCfg.PropsId, (char*)sCfg.OnPick);
                    }
                    if (sCfg.OnPut)
                    {
//                        if (sCfg.PropsId == 1005)
//                        {
//                            char buf[1024] = "local selectPos = callFunction(self, 'getSelectPos');local ball = callFunction(touchMap, 'getBall', selectPos);if (ball) then print('ball ball');callFunction(touchMap, 'clearSelectProps'); else print('nonono'); callFunction(touchMap, 'moveto', selectPos, 1); end;";
//                            sCfg.pEventList->addEventFunction("PropsConfig", "OnPut", sCfg.PropsId, (char*)buf);
//                            
//                        }else
                        sCfg.pEventList->addEventFunction("PropsConfig", "OnPut", sCfg.PropsId, (char*)sCfg.OnPut);
                    }
                    if (sCfg.OnShow)
                    {
                        sCfg.pEventList->addEventFunction("PropsConfig", "OnShow", sCfg.PropsId, (char*)sCfg.OnShow);
                    }
                    if (sCfg.OnAroundRemove) {
                        sCfg.pEventList->addEventFunction("PropsConfig", "OnAroundRemove", sCfg.PropsId, (char*)sCfg.OnAroundRemove);
                    }
                    m_mPropsCfg[sCfg.PropsId]=sCfg;
                }
                else
                {
                    bOK=false;
                    break;
                }
            }
            
        }
    }
    delete [] buffer;
    return bOK;
}

bool ClientData::loadMapData()
{
    HawkBinCfgFile sReader;
    unsigned long len=0;
    unsigned char * buffer =CCFileUtils::sharedFileUtils()->getFileData("mapData.bin", "rb", &len);
    
    bool bOK=false;
    if (sReader.LoadFromMem(buffer, len))
    {
        Int32 iCount = 0;
        if(sReader.Read(&iCount,sizeof(iCount)))
        {
            bOK=true;
            for (Int32 i=0;i<iCount;i++)
            {
                mapCfg sCfg;
                if (sReader.Read(&sCfg,sizeof(mapData)))
                {
                    sCfg.vMapCell = splitUchar2Int(sCfg.MapCell);
                    m_mMapCfg[sCfg.MapId]=sCfg;
                }
                else
                {
                    bOK=false;
                    break;
                }
            }
            
        }
    }
    delete [] buffer;
    return bOK;
}

bool ClientData::loadAwardBallData()
{
    HawkBinCfgFile sReader;
    unsigned long len=0;
    unsigned char * buffer =CCFileUtils::sharedFileUtils()->getFileData("awardBallData.bin", "rb", &len);
    
    bool bOK=false;
    if (sReader.LoadFromMem(buffer, len))
    {
        Int32 iCount = 0;
        if(sReader.Read(&iCount,sizeof(iCount)))
        {
            bOK=true;
            for (Int32 i=0;i<iCount;i++)
            {
                awardBallCfg sCfg;
                if (sReader.Read(&sCfg,sizeof(awardBallData)))
                {
                    if (sCfg.ShapeCondition == 3)
                        sCfg.ShapeCondition = 4;
                    m_mAwardBallCfg[sCfg.ShapeCondition]=sCfg;
                    m_mBallKeyAwardBallCfg[sCfg.BallType] = sCfg.ShapeCondition;
                }
                else
                {
                    bOK=false;
                    break;
                }
            }
            
        }
    }
    delete [] buffer;
    return bOK;
}

void ClientData::loadScript()
{
    for (map<int, propsCfg>::iterator it = m_mPropsCfg.begin(); it != m_mPropsCfg.end(); it++) {
        for (map<string, StrKeyValue>::iterator it2 = it->second.pEventList->mEventFuncs.begin(); it2 != it->second.pEventList->mEventFuncs.end(); it2++) {
            g_gameScript->loadScript("config", it2->second.value.c_str());
        }
    }
}


const levelCfg* ClientData::getLevelCfg(int key)
{
    map<int, levelCfg>::iterator iter = m_mLevelCfg.find(key);
    if (iter != m_mLevelCfg.end())
        return &iter->second;
    
    return NULL;
}

const ballCfg* ClientData::getBallCfg(int key)
{
    map<int, ballCfg>::iterator iter = m_mBallCfg.find(key);
    if (iter != m_mBallCfg.end())
        return &iter->second;
    
    return NULL;
}

const propsCfg* ClientData::getPropsCfg(int key)
{
    map<int, propsCfg>::iterator iter = m_mPropsCfg.find(key);
    if (iter != m_mPropsCfg.end())
        return &iter->second;
    
    return NULL;
}

const mapCfg* ClientData::getMapCfg(int key)
{
    map<int, mapCfg>::iterator iter = m_mMapCfg.find(key);
    if (iter != m_mMapCfg.end())
        return &iter->second;
    
    return NULL;
}

const awardBallCfg* ClientData::getAwardBallCfg(int key)
{
    map<int, awardBallCfg>::iterator iter = m_mAwardBallCfg.find(key);
    if (iter != m_mAwardBallCfg.end())
        return &iter->second;
    
    return NULL;
}

const awardBallCfg* ClientData::getAwardBallCfgByBallType(int key)
{
    map<int, int>::iterator iter = m_mBallKeyAwardBallCfg.find(key);
    if (iter != m_mBallKeyAwardBallCfg.end())
    {
        return getAwardBallCfg(iter->second);
    }
    return NULL;
}

const map<int, levelCfg>& ClientData::getLevelList()
{
    return m_mLevelCfg;
}

const map<int, ballCfg>& ClientData::getBallList()
{
    return m_mBallCfg;
}

const map<int, propsCfg>& ClientData::getPropsList()
{
    return m_mPropsCfg;
}

const map<int, mapCfg>& ClientData::getMapList()
{
    return m_mMapCfg;
}

const map<int, awardBallCfg>& ClientData::getAwardBallList()
{
    return m_mAwardBallCfg;
}

