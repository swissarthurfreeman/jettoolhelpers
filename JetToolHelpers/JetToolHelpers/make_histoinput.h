#include "JetToolHelpers/HistoInput.h"

template<class InVar, class... InVars>
auto make_histo_input(InVar&& invar, InVars&&... invars) {
    auto var_tuple = std::make_tuple(invar, invars...);
    return HistoInput<decltype(var_tuple)>(var_tuple);
}

auto make_histo( 
    const std::string& varName,
    const std::string& varType,
    const bool isJetVar) {
    auto iv1 = *InputVariable::createVariable(varName, varType, isJetVar);
    return make_histo_input(iv1);
}

/*
auto make_histo( 
    const std::string& varName1,
    const std::string& varType1,
    const bool isJetVar1,
    const std::string& varName2,
    const std::string& varType2,
    const bool isJetVar2
) {
    auto iv1 = InputVariable::createVariable(varName1, varType1, isJetVar1);
    auto iv2 = InputVariable::createVariable(varName2, varType2, isJetVar2);
    return make_histo_input(iv1, iv2);
}*/