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
auto make_histogram_with(Config config, InVar invar, InVars... invars) {
    auto t = std::make_tuple(invar, invars...);
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

    auto iv1 = *InputVariable::createVariable(varName, varType, isJetVar);
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

    auto iv1 = *InputVariable::createVariable(varName1, varType1, isJetVar1);
    auto iv2 = *InputVariable::createVariable(varName2, varType2, isJetVar2);
    
    return make_histogram_with(conf, iv1, iv2);
}

#endif