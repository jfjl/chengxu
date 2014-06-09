//
//  ClientData.cpp
//  pball
//
//  Created by carlor on 14-6-4.
//
//

#include "ClientData.h"

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
    
    CCLOG("load bin complete");
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

        KeyValue mKeyValue;
        mKeyValue.key = szInt[0];
        mKeyValue.value = szInt[1];
        maxValue += szInt[1];
        
        vKeyValue.push_back(mKeyValue);
    }
    
    if (bCalcRate)
    {
        for (size_t i = 0; i < vKeyValue.size(); i++)
        {
            vKeyValue[i].value = vKeyValue[i].value * 100 / maxValue;
        }
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
                    sCfg.nBasicBall = sCfg.BallId;
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
    
    for (map<int, ballCfg>::iterator it = m_mBallCfg.begin(); it != m_mBallCfg.end(); it++)
    {
        if (! it->second.PreBall) continue;
        
        int ballid = it->second.PreBall;
        while (ballid) {
            map<int, ballCfg>::iterator ittemp = m_mBallCfg.find(ballid);
            if (ittemp != m_mBallCfg.end())
            {
                it->second.nRemoveCount++;
                ballid = ittemp->second.PreBall;
                if (! ballid)
                    it->second.nBasicBall = ittemp->second.BallId;
            }
            else
            {
                ballid = 0;
            }
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

