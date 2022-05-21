#include "JetToolHelpers/InputVariable.h"

InputVariable::InputVariable(
    const std::string& name, 
    std::function<float(const xAOD::Jet& jet, const JetContext& jc)> func
):
    InputVariable(name) 
{
    customFunction = func;
}

// TODO : Confer with steven about throwing exceptions instead of returning nullptrs. 
std::unique_ptr<InputVariable> InputVariable::createVariable(const std::string& name, const std::string& type, const bool isJetVar) {
    if(name == "" || type == "")
        return nullptr; 

    if (isJetVar) {
        // Variables stored on the xAOD::Jet
        // First, check for pre-defined attributes (not stored as generic auxdata)
        if (name == "e")
            return std::make_unique<InputVariable>(name,
                [](const xAOD::Jet& jet, const JetContext&) {
                    return jet.e();
                });

        if (name == "et")
            return std::make_unique<InputVariable>(name,
                [](const xAOD::Jet& jet, const JetContext&) {
                    return jet.p4().Et();
                });

        if(name == "pt")
            return std::make_unique<InputVariable>(name,
                [](const xAOD::Jet& jet, const JetContext&) {
                    return jet.pt();
                });

        if (name == "eta")
            return std::make_unique<InputVariable>(name,
                [](const xAOD::Jet& jet, const JetContext&) {
                    return jet.eta();
                });

        if (name == "abseta" || name == "|eta|")
            return std::make_unique<InputVariable>(name,
                [](const xAOD::Jet& jet, const JetContext&) {
                    return std::abs(jet.eta());
                });

        if (name == "rapidity" || name == "y")
            return std::make_unique<InputVariable>(name,
                [](const xAOD::Jet& jet, const JetContext&) {
                    return std::abs(jet.eta());
                }); 

        if (name == "absrapidity" || name == "|rapidity|" || name == "absy" || name == "|y|")
            return std::make_unique<InputVariable>(name,
                [](const xAOD::Jet& jet, const JetContext&) {
                    return std::abs(jet.rapidity());
                }); 

        // Not a pre-defined attribute, assume it is a generic attribute
        #ifdef USE_ATHENA
        if (type == "float")
            return std::make_unique<InputVariableAttribute<float>>(name);
        
        if (type == "int")
            return std::make_unique<InputVariableAttribute<int>>(name);
        #endif
        // Unsupported type for a generic attribute
        return nullptr;
    } else {
        // Variables not stored on the xAOD::Jet
        // Here, we need only to check the type of the variable
        // The variables are then stored in string-indexed maps
        
        if(type == "int")
            return std::make_unique<InputVariable>(name,
                [name](const xAOD::Jet&, const JetContext& event) {
                    return event.isAvailable(name) ? event.getValue<int>(name) : ERRORVALUE;
                });

        if(type == "float")
            return std::make_unique<InputVariable>(name,
                [name](const xAOD::Jet&, const JetContext& event) {
                    return event.isAvailable(name) ? event.getValue<float>(name) : ERRORVALUE;
                });

        // Unsupported type for a non-jet-level variable
        return nullptr;
    }
}
