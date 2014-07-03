//
//  DialogEvent.cpp
//  pball
//
//  Created by sora on 13-5-7.
//
//

#include "DialogEvent.h"

DialogEvent::DialogEvent()
{
    m_nValueList.clear();
    m_sValueList.clear();
}

DialogEvent::~DialogEvent()
{
    m_nValueList.clear();
    m_sValueList.clear();
}

void DialogEvent::addKeyValue(std::string key, int value)
{
    std::map<std::string, int>::iterator it = m_nValueList.find(key);
    if (it == m_nValueList.end()) {
        m_nValueList[key] = value;
    }
}

void DialogEvent::addKeyValue(std::string key, std::string value)
{
    std::map<std::string, std::string>::iterator it = m_sValueList.find(key);
    if (it == m_sValueList.end()) {
        m_sValueList[key] = value;
    }
    
}

int DialogEvent::getIntValue(std::string key)
{
    std::map<std::string, int>::iterator it = m_nValueList.find(key);
    if (it == m_nValueList.end()) {
        return 0;
    }
    return it->second;
}

std::string DialogEvent::getStrValue(std::string key)
{
    std::map<std::string, std::string>::iterator it = m_sValueList.find(key);
    if (it == m_sValueList.end()) {
        return "";
    }
    return it->second;
    
}
