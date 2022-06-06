/**
 * @file make_histo.h
 * @author A. Freeman (swissarthurfreeman@gmail.com)
 * @brief Core factory methods to generate histograms.
 * @date 2022-06-01
 * 
 * @copyright Copyright (c) 2022 for the benefit of the ATLAS collaboration. 
 */
#ifndef MAKE_HISTO
#define MAKE_HISTO

#include <tuple>
#include "Mock.h"
#include "JetToolHelpers/HistoInput.h"

/**
 * @brief Config struct with name of IInputBase,
 * .root file name and name of the histogram within
 * said file.
 */
struct Config {
    const std::string name;
    const std::string fileName;
    const std::string histName;
};

/** @brief makes an ND histogram with a configuration and N InVars unique pointers.
 * This code will always compile, it will yield a runtime error if InVar or InVars has
 * no getValue(jet, JetContext) method.
*/
/*
template <class InVar, class... InVars>
auto make_histogram_with(Config config, std::unique_ptr<InVar>& invar, std::unique_ptr<InVars>&... invars) {
    auto t = std::make_tuple(std::move(invar), std::move(invars)...);
    return HistoInput<decltype(t)>(config.name, config.fileName, config.histName, t);
}
*/

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
    std::array<std::unique_ptr<InputVariable>, 1> arr{ std::move(iv1) };    // array takes ownership
    
    return HistoInput<std::unique_ptr<InputVariable>, 1>(conf.name, conf.fileName, conf.histName, arr);
}

/** @brief makes a 2D histogram.*/
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
    
    std::array<std::unique_ptr<InputVariable>, 2> arr{ std::move(iv1), std::move(iv2) };
    return HistoInput<std::unique_ptr<InputVariable>, 2>(conf.name, conf.fileName, conf.histName, arr);
}

/** @brief makes a 3D histogram.*/
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
    
    std::array<std::unique_ptr<InputVariable>, 3> arr{ std::move(iv1), std::move(iv2), std::move(iv3) };
    return HistoInput<std::unique_ptr<InputVariable>, 3>(conf.name, conf.fileName, conf.histName, arr);
}

#endif