/**
 * @file HistoInput.Ctr.cpp
 * @author S. Schramm, A.Freeman
 * @brief Contains all constructor implementations of HistoInput.h 
 * @version 0.1
 * @date 2022-04-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "JetToolHelpers/HistoInput.h"

HistoInput::HistoInput(
            const std::string& name, 
            const std::string& fileName, 
            const std::string& histName, 
            const std::string& varName, 
            const std::string& varType, 
            const bool isJetVar
): IInputBase(name), 
      nDims{1},
      m_fileName{fileName}, m_histName{histName}, 
      m_varName1{varName}, m_varType1{varType}, 
      m_isJetVar1{isJetVar}, m_inVar1{nullptr},
      m_varName2{""}, m_varType2{""}, 
      m_isJetVar2{true}, m_inVar2{nullptr}
{}

/**
 * @brief 2D Histogram constructor.
 */
HistoInput::HistoInput(
            const std::string& name, 
            const std::string& fileName, 
            const std::string& histName,
            const std::string& varName1, const std::string& varType1, const bool isJetVar1,
            const std::string& varName2, const std::string& varType2, const bool isJetVar2
): IInputBase(name), 
      nDims{2},
      m_fileName{fileName}, m_histName{histName}, 
      m_varName1{varName1}, m_varType1{varType1}, 
      m_isJetVar1{isJetVar1}, m_inVar1{nullptr},
      m_varName2{varName2}, m_varType2{varType2}, 
      m_isJetVar2{isJetVar2}, m_inVar2{nullptr}
{
    if(varName2 == "")
        throw std::runtime_error("varName2 cannot be emptystring");
}
