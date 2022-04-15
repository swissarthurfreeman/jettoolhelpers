#ifndef JETCONTEXT_H
#define JETCONTEXT_H

#include <unordered_map>
#include <stdexcept>

class JetContext
{
    public:
        JetContext();

        template <typename T>
        bool setValue(const std::string& name, const T value, bool allowOverwrite = false);

        template <typename T>
        bool getValue(const std::string& name, T& value) const;

        template <typename T>
        T getValue(const std::string& name) const;

        template <typename T>
        bool isAvailable(const std::string& name) const;

        static constexpr int ERRORVALUE {-999};

    private:
        std::unordered_map<std::string,int>   m_intVars;
        std::unordered_map<std::string,float> m_floatVars;

};

// Base template for adding values
// - Unsupported types are in this category
// - All supported types have explicit specifications
template <typename T>
bool JetContext::setValue(const std::string&, const T /*value*/, bool /*allowOverwrite*/)
{
    // Unsupported type
    return false;
}

// Base template for getting values (with error checking)
// - Unsupported types are in this category
// - All supported types have explicit specifications
template <typename T>
bool JetContext::getValue(const std::string&, T& /*value*/) const
{
    // Unsupported type
    return false;
}

// Generic template for getting values (without error checking)
// Calls the above error-checked template, so no need for specifications here
template <typename T>
T JetContext::getValue(const std::string& name) const
{
    T toReturn;
    if (getValue<T>(name,toReturn))
        return toReturn;
    return ERRORVALUE;
}

// Base template for checking if a value exists
// - Unsupported types are in this category
// - All supported types have explicit specifications
template <typename T>
bool JetContext::isAvailable(const std::string& /*name*/) const
{
    // Unsupported type
    return false;
}



#endif

