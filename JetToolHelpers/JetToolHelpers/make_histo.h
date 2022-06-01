#ifndef MAKE_HISTO
#define MAKE_HISTO

#include <tuple>
#include "Mock.h"
#include "JetToolHelpers/HistoInput.h"

struct Config {
    const std::string name;
    const std::string fileName;
    const std::string histName;
};

template <class InVar, class... InVars>
auto make_histogram_with(Config config, std::unique_ptr<InVar>& invar, std::unique_ptr<InVars>&... invars) {
    auto t = std::make_tuple(std::move(invar), std::move(invars)...);
    //return t;
    return HistoInput<decltype(t)>(config.name, config.fileName, config.histName, t);
}

// makes a 1D histogram.
auto MakeHistoInput(
    const std::string& name, const std::string& fileName, 
    const std::string& histName, const std::string& varName, 
    const std::string& varType, const bool isJetVar) {
    
    Config conf = {
        name,
        fileName,
        histName
    };

    auto iv1 = InputVariable::createVariable(varName, varType, isJetVar);
    return make_histogram_with(conf, iv1);
}

auto MakeHistoInput(
    const std::string& name, const std::string& fileName, 
    const std::string& histName, const std::string& varName1, 
    const std::string& varType1, const bool isJetVar1,
    const std::string& varName2, const std::string& varType2,
    const bool isJetVar2) {
    
    Config conf = {
        name,
        fileName,
        histName
    };

    auto iv1 = InputVariable::createVariable(varName1, varType1, isJetVar1);
    auto iv2 = InputVariable::createVariable(varName2, varType2, isJetVar2);
    
    return make_histogram_with(conf, iv1, iv2);
}

auto MakeHistoInput(
    const std::string& name, const std::string& fileName, 
    const std::string& histName, const std::string& varName1, 
    const std::string& varType1, const bool isJetVar1,
    const std::string& varName2, const std::string& varType2,
    bool isJetVar2,
    const std::string& varName3, const std::string& varType3,
    const bool isJetVar3) {
    
    Config conf = {
        name,
        fileName,
        histName
    };

    auto iv1 = InputVariable::createVariable(varName1, varType1, isJetVar1);
    auto iv2 = InputVariable::createVariable(varName2, varType2, isJetVar2);
    auto iv3 = InputVariable::createVariable(varName3, varType3, isJetVar3);
    
    return make_histogram_with(conf, iv1, iv2, iv3);
}

#endif