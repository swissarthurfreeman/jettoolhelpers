#include <iostream>

#include "JetToolHelpers/HistoInput.h"

bool HistoInput::initialize()
{
    // First deal with the input variable
    // Make sure we haven't already configured the input variable
    if (m_inVar1 != nullptr) {
        std::cout << "The input variable(s) were already configured" << std::endl;
        return false;
    }

    m_inVar1 = InputVariable::createVariable(m_varName1, m_varType1, m_isJetVar1);
    m_inVar2 = InputVariable::createVariable(m_varName2, m_varType2, m_isJetVar2);
    
    // m_varName2 will be "" (default string ctr) if 1D Histo constructor was called.
    if ( !m_inVar1 || (!m_inVar2 && m_varName2 != "") ) {
        std::cout << "Failed to create an input variable" << std::endl;
        return false;
    }

    // Now deal with the histogram
    // Make sure we haven't already retrieved the histogram
    if (m_hist != nullptr) {
        std::cout << "The histogram already exists" << std::endl;
        return false;
    }

    if (!HistoInput::readHistoFromFile(m_hist, m_fileName, m_histName)) {
        std::cout << "Failed while reading histogram from file" << std::endl;
        return false;
    }

    if (!m_hist) {
        std::cout << "Histogram pointer is empty after reading from file" << std::endl;
        return false;
    }

    if (m_hist->GetDimension() != nDims) {
        std::cout << "Read the specified histogram, but it has a dimension of " 
        << m_hist->GetDimension() << " instead of the expected 1" << std::endl;
        return false;
    }

    // TODO
    // We have both, set the dynamic range of the input variable according to histogram range
    // Low edge of first bin (index 1, as index 0 is underflow)
    // High edge of last bin (index N, as index N+1 is overflow)
    //m_inVar.SetDynamicRange(m_hist->GetXaxis()->GetBinLowEdge(1),m_hist->GetXaxis()->GetBinUpEdge(m_hist->GetNbinsX()));   
    return true;
}

bool HistoInput::finalize() {
    if (m_hist)
        m_hist.reset();
    return true;
}

bool HistoInput::getValue(const xAOD::Jet& jet, const JetContext& event, double& value) const {
    float varValue1 {m_inVar1->getValue(jet, event)};
    varValue1 = HistoInput::enforceAxisRange(*m_hist->GetXaxis(), varValue1);
    
    float varValue2{0};
    if (nDims > 1) {
        varValue2 = m_inVar2->getValue(jet,event);
        varValue2 = HistoInput::enforceAxisRange(*m_hist->GetYaxis(), varValue2);
    }
    // *a if a is unique_ptr returns a reference to the managed object
    value = HistoInput::readFromHisto(*m_hist, varValue1, varValue2);
    return true;
}