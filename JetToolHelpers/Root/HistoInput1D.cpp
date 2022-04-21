
#include <iostream>

#include "JetToolHelpers/HistoInput1D.h"

HistoInput1D::HistoInput1D(const std::string&name, const std::string& fileName, const std::string& histName)
    : HistoInputBase{name,fileName,histName}//, asg::AsgTool{name}
    , m_varName{""}, m_varType{""}, m_isJetVar{true}
    , m_inVar{nullptr}
{
    //declareProperty("VariableName",m_varName);
    //declareProperty("VariableType",m_varType);
    //declareProperty("VarIsFromJet",m_isJetVar);
}

HistoInput1D::HistoInput1D(const std::string& name, const std::string& fileName, const std::string& histName, const std::string& varName, const std::string& varType, const bool isJetVar)
    : HistoInputBase{name,fileName,histName}//, asg::AsgTool{name}
    , m_varName{varName}, m_varType{varType}, m_isJetVar{isJetVar}
    , m_inVar{nullptr}
{ }

bool HistoInput1D::initialize()
{
    // First deal with the input variable
    // Make sure we haven't already configured the input variable
    if (m_inVar != nullptr)
    {
        std::cout << "The input variable was already configured\n";
        return false;
    }

    m_inVar = InputVariable::createVariable(m_varName,m_varType,m_isJetVar);
    if (!m_inVar)
    {
        std::cout << "Failed to create the input variable\n";
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

    if (m_hist->GetDimension() != 1)
    {
        std::cout << "Read the specified histogram, but it has a dimension of " << m_hist->GetDimension() << " instead of the expected 1\n";
        return false;
    }

    // TODO
    // We have both, set the dynamic range of the input variable according to histogram range
    // Low edge of first bin (index 1, as index 0 is underflow)
    // High edge of last bin (index N, as index N+1 is overflow)
    //m_inVar.SetDynamicRange(m_hist->GetXaxis()->GetBinLowEdge(1),m_hist->GetXaxis()->GetBinUpEdge(m_hist->GetNbinsX()));

    return true;
}

bool HistoInput1D::finalize()
{
    if (m_hist)
        m_hist.reset();
    return true;
}


bool HistoInput1D::getValue(const xAOD::Jet& jet, const JetContext& event, double& value) const
{
    float varValue {m_inVar->getValue(jet,event)};
    
    // TODO make this configurable as error vs frozen to edges
    varValue = enforceAxisRange(*m_hist->GetXaxis(),varValue);
    
    // TODO Handle interpolation vs projected+cached interpolate vs bin content
    value = readFromHisto(varValue);
    return true;
}


bool HistoInput1D::runUnitTests() const
{
    // TODO
    return false;
}

