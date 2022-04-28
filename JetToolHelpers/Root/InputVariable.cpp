#include "JetToolHelpers/InputVariable.h"

std::unique_ptr<InputVariable> InputVariable::createVariable(const std::string& name, const std::string& type, const bool isJetVar)
{
    if(name == "" || type == "")
        return nullptr; // we should throw exceptions instead of returning nullptrs no ? 

    if (isJetVar) {
        // Variables stored on the xAOD::Jet
        
        // First, check for pre-defined attributes (not stored as generic auxdata)
        if (name == "e")
            return std::make_unique<InputVariableE>(name);
        if (name == "et")
            return std::make_unique<InputVariableET>(name);
        if (name == "pt")
            return std::make_unique<InputVariablePT>(name);
        if (name == "eta")
            return std::make_unique<InputVariableEta>(name);
        if (name == "abseta" || name == "|eta|")
            return std::make_unique<InputVariableAbsEta>(name);
        if (name == "rapidity" || name == "y")
            return std::make_unique<InputVariableRapidity>(name);
        if (name == "absrapidity" || name == "|rapidity|" || name == "absy" || name == "|y|")
            return std::make_unique<InputVariableAbsRapidity>(name);

        // Not a pre-defined attribute, assume it is a generic attribute
        #ifdef NO_MOCK
        if (type == "float")
            return std::make_unique< InputVariableAttribute<float> >(name);
        if (type == "int")
            return std::make_unique< InputVariableAttribute<int> >(name);
        #endif

        // Unsupported type for a generic attribute
        return nullptr;
    } else {
        // Variables not stored on the xAOD::Jet
        // Here, we need only to check the type of the variable
        // The variables are then stored in string-indexed maps
        if (type == "float")
            return std::make_unique<InputVariableJetContext<float>>(name);
        if (type == "int")
            return std::make_unique<InputVariableJetContext<int>>(name);
        
        // Unsupported type for a non-jet-level variable
        return nullptr;
    }
}

InputVariable::InputVariable(const std::string& name)
    : m_name{name}, m_scale{1.}
{ }



