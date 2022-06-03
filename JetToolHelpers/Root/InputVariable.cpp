/**
 * @file InputVariable.cpp
 * @author A. Freeman, S. Schramm (swissarthurfreeman@gmail.com, steven.schramm@cern.ch)
 * @date 2022-06-01
 * 
 * @copyright Copyright (c) 2022 for the benefit of the ATLAS collaboration. 
 * 
 */
#include <iostream>
#include "JetToolHelpers/InputVariable.h"

InputVariable::InputVariable(
    const std::string& name, 
    double (*func)(const xAOD::Jet& jet, const JetContext& jc)
/*std::function<float(const xAOD::Jet& jet, const JetContext& jc)> func*/): InputVariable(name) {
    customFunction = func;
}

std::unique_ptr<InputVariable> InputVariable::createVariable(const std::string& name, const std::string& type, const bool isJetVar) {
    if (isJetVar) {
        // Variables stored on the xAOD::Jet
        // First, check for pre-defined attributes (not stored as generic auxdata)
        // TODO : What about phi ? 
        if (name == "e")
            return std::make_unique<InputVariable>(name,
                +[](const xAOD::Jet& jet, const JetContext&) {
                    return jet.e();
                });

        if (name == "et")
            return std::make_unique<InputVariable>(name,
                +[](const xAOD::Jet& jet, const JetContext&) {
                    return jet.p4().Et();
                });

        if(name == "pt")
            return std::make_unique<InputVariable>(name,
                +[](const xAOD::Jet& jet, const JetContext&) {
                    return jet.pt();
                });

        if(name == "phi")
            return std::make_unique<InputVariable>(name,
                +[](const xAOD::Jet& jet, const JetContext&) {
                    return jet.phi();
                });

        if (name == "eta")
            return std::make_unique<InputVariable>(name,
                +[](const xAOD::Jet& jet, const JetContext&) {
                    return jet.eta();
                });

        if (name == "abseta" || name == "|eta|")
            return std::make_unique<InputVariable>(name,
                +[](const xAOD::Jet& jet, const JetContext&) {
                    return std::abs(jet.eta());
                });

        if (name == "rapidity" || name == "y")
            return std::make_unique<InputVariable>(name,
                +[](const xAOD::Jet& jet, const JetContext&) {
                    return jet.rapidity();
                });

        if (name == "absrapidity" || name == "|rapidity|" || name == "absy" || name == "|y|")
            return std::make_unique<InputVariable>(name,
                +[](const xAOD::Jet& jet, const JetContext&) {
                    return std::abs(jet.rapidity());
                }); 

        // Not a pre-defined attribute, assume it is a generic attribute
        #ifdef IN_PRODUCTION
            if (type == "float")
                return std::make_unique<InputVariableAttribute<float>>(name);
            
            if (type == "int")
                return std::make_unique<InputVariableAttribute<int>>(name);
        #endif
        // Unsupported type for a generic attribute
        std::cerr << "\nWARNING : user requested Jet InputVariable " << name << " is unsupported\n"; 
        return nullptr;
    } else {
        // Variables not stored on the xAOD::Jet
        // Here, we need only to check the type of the variable
        // The variables are then stored in string-indexed maps
        
        if(type == "int")
            return std::make_unique<InputVariableJetContext<int>>(name);

        if(type == "float")
            return std::make_unique<InputVariableJetContext<float>>(name);

        // Unsupported type for a non-jet-level variable
        std::cerr << "\nWARNING : user requested JetContext InputVariable type" << name << " is unsupported\n"; 
        return nullptr;
    }
}
