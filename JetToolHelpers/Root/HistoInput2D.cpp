
#include <iostream>

#include "JetToolHelpers/HistoInput2D.h"

HistoInput2D::HistoInput2D(const std::string& name, const std::string& fileName, const std::string& histName)
    : HistoInputBase{name,fileName,histName}, asg::AsgTool{name}
    , m_varName1{""}, m_varType1{""}, m_isJetVar1{true}
    , m_varName2{""}, m_varType2{""}, m_isJetVar2{true}
    , m_inVar1{nullptr}, m_inVar2{nullptr}
{
    declareProperty("VariableName1",m_varName1);
    declareProperty("VariableType1",m_varType1);
    declareProperty("VarIsFromJet1",m_isJetVar1);
    declareProperty("VariableName2",m_varName2);
    declareProperty("VariableType2",m_varType2);
    declareProperty("VarIsFromJet2",m_isJetVar2);
}

HistoInput2D::HistoInput2D(const std::string& name, const std::string& fileName, const std::string& histName, const std::string& varName1, const std::string& varType1, const bool isJetVar1, const std::string& varName2, const std::string& varType2, const bool isJetVar2)
    : HistoInputBase{name,fileName,histName}, asg::AsgTool{name}
    , m_varName1{varName1}, m_varType1{varType1}, m_isJetVar1{isJetVar1}
    , m_varName2{varName2}, m_varType2{varType2}, m_isJetVar2{isJetVar2}
    , m_inVar1{nullptr}, m_inVar2{nullptr}
{ }

StatusCode HistoInput2D::initialize()
{
    // First deal with the input variables
    // Make sure we haven't already configured the input variables
    if (m_inVar1 != nullptr || m_inVar2 != nullptr)
    {
        std::cout << "Input variable(s) were already configured\n";
        return false;
    }

    m_inVar1 = InputVariable::createVariable(m_varName1,m_varType1,m_isJetVar1);
    m_inVar2 = InputVariable::createVariable(m_varName2,m_varType2,m_isJetVar2);
    if (!m_inVar1 || !m_inVar2)
    {
        std::cout << "Failed to create input variable(s)\n";
        return false;
    }

    // Now deal with the histogram
    // Make sure we haven't already retrieved the histogram
    if (m_hist != nullptr)
    {
        std::cout << "The histogram already exists\n";
        return false;
    }

    if (!readHistoFromFile())
    {
        std::cout << "Failed while reading histogram from file\n";
        return false;
    }

    if (!m_hist)
    {
        std::cout << "Histogram pointer is empty after reading from file\n";
        return false;
    }

    if (m_hist->GetDimension() != 2)
    {
        std::cout << "Read the specified histogram, but it has a dimension of " << m_hist->GetDimension() << " instead of the expected 2\n";
        return false;
    }

    // TODO
    // We have both, set the dynamic range of the input variable according to histogram range
    // Low edge of first bin (index 1, as index 0 is underflow)
    // High edge of last bin (index N, as index N+1 is overflow)
    //m_inVar1.SetDynamicRange(m_hist->GetXaxis()->GetBinLowEdge(1),m_hist->GetXaxis()->GetBinUpEdge(m_hist->GetNbinsX()));
    //m_inVar2.SetDynamicRange(m_hist->GetYaxis()->GetBinLowEdge(1),m_hist->GetYaxis()->GetBinUpEdge(m_hist->GetNbinsY()));

    return true;
}

StatusCode HistoInput2D::finalize()
{
    if (m_hist)
        m_hist.reset();
    return true;
}


bool HistoInput2D::getValue(const xAOD::Jet& jet, const JetContext& event, double& value) const
{
    float varValue1 {m_inVar1->getValue(jet,event)};
    varValue1 = enforceAxisRange(*m_hist->GetXaxis(),varValue1);
    
    float varValue2 {m_inVar2->getValue(jet,event)};
    varValue2 = enforceAxisRange(*m_hist->GetYaxis(),varValue2);
    
    value = readFromHisto(varValue1,varValue2);
    return true;
}


bool HistoInput2D::runUnitTests() const
{
    // TODO
    return false;
}


