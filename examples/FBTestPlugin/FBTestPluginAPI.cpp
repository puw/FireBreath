/**********************************************************\
Original Author: Richard Bateman and Georg Fritzsche

Created:    December 3, 2009
License:    Dual license model; choose one of two:
            New BSD License
            http://www.opensource.org/licenses/bsd-license.php
            - or -
            GNU Lesser General Public License, version 2.1
            http://www.gnu.org/licenses/lgpl-2.1.html

Copyright 2009 PacketPass Inc, Georg Fritzsche,
               Firebreath development team
\**********************************************************/

#include "BrowserObjectAPI.h"
#include "DOM/Document.h"
#include "DOM/Window.h"
#include "variant_list.h"
#include "SimpleMathAPI.h"

#include "FBTestPluginAPI.h"

FBTestPluginAPI::FBTestPluginAPI(FB::BrowserHost host) : m_host(host)
{
    registerMethod("add",  make_method(this, &FBTestPluginAPI::add));
    registerMethod(L"echo",  make_method(this, &FBTestPluginAPI::echo));
    registerMethod(L"asString",  make_method(this, &FBTestPluginAPI::asString));
    registerMethod(L"asBool",  make_method(this, &FBTestPluginAPI::asBool));
    registerMethod(L"asInt",  make_method(this, &FBTestPluginAPI::asInt));
    registerMethod("asDouble",  make_method(this, &FBTestPluginAPI::asDouble));
    registerMethod("listArray",  make_method(this, &FBTestPluginAPI::listArray));
    registerMethod("reverseArray",  make_method(this, &FBTestPluginAPI::reverseArray));
    registerMethod("getUserData",  make_method(this, &FBTestPluginAPI::getUserData));
    registerMethod("getUserArray",  make_method(this, &FBTestPluginAPI::getUserArray));
    registerMethod("getObjectKeys",  make_method(this, &FBTestPluginAPI::getObjectKeys));
    registerMethod("getObjectValues",  make_method(this, &FBTestPluginAPI::getObjectValues));
    registerMethod("testEvent",  make_method(this, &FBTestPluginAPI::testEvent));
    registerMethod("testStreams",  make_method(this, &FBTestPluginAPI::testStreams));
    registerMethod("getTagAttribute", make_method(this, &FBTestPluginAPI::getTagAttribute));
    registerMethod("getPageLocation", make_method(this, &FBTestPluginAPI::getPageLocation));
     
    registerMethod(L"скажи",  make_method(this, &FBTestPluginAPI::say));

    // Read-write property
    registerProperty("testString",
                     make_property(this,
                        &FBTestPluginAPI::get_testString,
                        &FBTestPluginAPI::set_testString));

    registerProperty("simpleMath",
                     make_property(this,
                        &FBTestPluginAPI::get_simpleMath));
    // Read-only property
    registerProperty("someInt",
                     make_property(this,
                        &FBTestPluginAPI::get_someInt));

    registerEvent("onfired");

    m_simpleMath = FB::JSAPIPtr(new SimpleMathAPI(m_host));
}

FBTestPluginAPI::~FBTestPluginAPI()
{
    //std::map<int,int>::capacity()
}

std::wstring FBTestPluginAPI::say(const std::wstring& val)
{
    return L"Ð²Ð¾Ñ, Ñ Ð³Ð¾Ð²Ð¾ÑÑ \"" + val + L"\"";
}

// Read/Write property someInt
std::string FBTestPluginAPI::get_testString()
{
    return m_testString;
}
void FBTestPluginAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property someInt
long FBTestPluginAPI::get_someInt()
{
    m_host->htmlLog("Returning some int");
    return 12;
}

// add Method
long FBTestPluginAPI::add(long a, long b)
{
    m_host->htmlLog("Adding two numbers");
    return a+b;
}

// test firing an event
void FBTestPluginAPI::testEvent(const std::string& param)
{
    this->FireEvent("onfired", FB::variant_list_of(param));
}

FB::variant FBTestPluginAPI::echo(const FB::variant& a)
{
    m_host->htmlLog("Echoing: " + a.convert_cast<std::string>());
    return a;
}

std::string FBTestPluginAPI::asString(const FB::variant& a)
{
    return a.convert_cast<std::string>();
}

bool FBTestPluginAPI::asBool(const FB::variant& a)
{
    return a.convert_cast<bool>();
}

long FBTestPluginAPI::asInt(const FB::variant& a)
{
    return a.convert_cast<long>();
}

double FBTestPluginAPI::asDouble(const FB::variant& a)
{
    return a.convert_cast<double>();
}

FB::VariantList FBTestPluginAPI::reverseArray(const std::vector<std::string>& arr)
{
    FB::VariantList outArr;
    for (std::vector<std::string>::const_reverse_iterator it = arr.rbegin(); it != arr.rend(); it++)
    {
        outArr.push_back(*it);
    }
    return outArr;
}

FB::VariantList FBTestPluginAPI::getObjectKeys(const FB::JSObject& arr)
{
    FB::VariantList outArr;
    std::map<std::string, FB::variant> inMap;
    arr->GetObjectValues(arr, inMap);

    for (std::map<std::string, FB::variant>::iterator it = inMap.begin(); it != inMap.end(); it++) {
        outArr.push_back(it->first);
    }
    return outArr;
}

FB::VariantList FBTestPluginAPI::getObjectValues(const FB::JSObject& arr)
{
    FB::VariantList outArr;
    std::map<std::string, FB::variant> inMap;
    arr->GetObjectValues(arr, inMap);

    for (std::map<std::string, FB::variant>::iterator it = inMap.begin(); it != inMap.end(); it++) {
        outArr.push_back(it->second);
    }
    return outArr;
}

std::string FBTestPluginAPI::listArray(const std::vector<std::string>& arr)
{
    std::string outStr;
    bool start(true);
    for (std::vector<std::string>::const_iterator it = arr.begin(); it != arr.end(); it++)
    {
        if (!start) {
            outStr += ", ";
        }
        start = false;
        outStr += *it;
    }
    return outStr;
}

FB::VariantList FBTestPluginAPI::getUserArray()
{
    FB::VariantList map;
    map.push_back("Richard Bateman");
    map.push_back("Somewhere in Utah");
    map.push_back("Hazel");
    map.push_back("Brown");
    FB::VariantList kids;
    kids.push_back("Caleb");
    kids.push_back("Unknown");
    kids.push_back("Ok, I only have one, but I'm filling space");
    FB::VariantMap innerMap;
    innerMap["test"] = 12;
    innerMap["test2"] = true;
    innerMap["test3"] = 12.4;
    innerMap["test4"] = "asdf";
    map.push_back(innerMap);
    kids.push_back(innerMap);
    map.push_back(kids);
    return map;
}

FB::VariantMap FBTestPluginAPI::getUserData()
{
    FB::VariantMap map;
    map["Name"] = "Richard Bateman";
    map["Location"] = "Somewhere in Utah";
    map["EyeColor"] = "Hazel";
    map["HairColor"] = "Brown";
    FB::VariantList kids;
    kids.push_back("Caleb");
    kids.push_back("Unknown");
    kids.push_back("Ok, I only have one, but I'm filling space");
    FB::VariantMap innerMap;
    innerMap["test"] = 12;
    innerMap["test2"] = true;
    innerMap["test3"] = 12.4;
    innerMap["test4"] = "asdf";
    map["inner"] = innerMap;
    kids.push_back(innerMap);
    map["Kids"] = kids;
    return map;
}

FB::JSOutObject FBTestPluginAPI::get_simpleMath()
{
    return m_simpleMath;
}

FB::variant FBTestPluginAPI::getTagAttribute(const std::wstring &tagName, const long idx, const std::wstring &attribute)
{
    std::vector<FB::DOM::ElementPtr> tagList = m_host->getDOMDocument()->getElementsByTagName(tagName);
    if (!tagList.size()) {
        return "No matching tags found";
    }
    return tagList[idx]->getJSObject()->GetProperty(attribute);
}

std::string FBTestPluginAPI::getPageLocation()
{
    return m_host->getDOMWindow()->getLocation();
}

#include "SimpleStreams.h"

// test streams
bool FBTestPluginAPI::testStreams()
{
    StreamsTest test( m_host );
    return test.run();
}
