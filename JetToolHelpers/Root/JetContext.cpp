
#include "JetToolHelpers/JetContext.h"

JetContext::JetContext()
    : m_intVars{}, m_floatVars{}
{ }






template <>
bool JetContext::setValue<int>(const std::string& name, const int value, bool allowOverwrite)
{
    if (m_intVars.find(name) != m_intVars.end() && !allowOverwrite)
        return false;

    m_intVars.insert(std::make_pair(name,value));
    return true;
}

template <>
bool JetContext::setValue<float>(const std::string& name, const float value, bool allowOverwrite)
{
    if (m_floatVars.find(name) != m_floatVars.end() && !allowOverwrite)
        return false;

    m_floatVars.insert(std::make_pair(name,value));
    return true;
}







template <>
bool JetContext::getValue<int>(const std::string& name, int& value) const
{
    try
    {
        value = m_intVars.at(name);
        return true;
    }
    catch (const std::out_of_range&)
    {
        return false;
    }
}

template <>
bool JetContext::getValue<float>(const std::string& name, float& value) const
{
    try
    {
        value = m_floatVars.at(name);
        return true;
    }
    catch (const std::out_of_range&)
    {
        return false;
    }
}




template <>
bool JetContext::isAvailable<int>(const std::string& name) const
{
    return m_intVars.find(name) != m_intVars.end();
}

template <>
bool JetContext::isAvailable<float>(const std::string& name) const
{
    return m_floatVars.find(name) != m_floatVars.end();
}

