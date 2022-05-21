#include <iostream>

#include "JetToolHelpers/HistoInput.h"

bool HistoInput::initialize() {
    // Now deal with the histogram
    // Make sure we haven't already retrieved the histogram
    if (m_hist != nullptr) {
        std::cout << "The histogram already exists" << std::endl;
        return false;
    }

    if (!HistoInput::readHistoFromFile(m_hist, m_fileName, m_histName)) {
        std::cout << "Failed while reading histogram from file" << std::endl;
        throw std::runtime_error("Error reading histogram from file");
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

    std::vector<double> values{0, 0, 0};
    
    for(int i=0; i < (int) in_vars_.size(); i++)
        values.at(i) = in_vars_.at(i).getValue(jet, event);

    // *a if a is unique_ptr returns a reference to the managed object
    value = HistoInput::readFromHisto(*m_hist, values);
    return true;
}