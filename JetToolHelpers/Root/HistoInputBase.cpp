
#include <iostream>

#include "JetToolHelpers/HistoInputBase.h"

#include "TFile.h"

HistoInputBase::HistoInputBase(const std::string& name, const std::string& fileName, const std::string& histName)
    : IInputBase{name}
    , m_fileName{fileName}, m_histName{histName}, m_hist{nullptr}
{ }

bool HistoInputBase::readHistoFromFile()
{
    // Open the input file
    TFile inputFile(m_fileName.c_str(),"READ");
    if (inputFile.IsZombie())
    {
        std::cout << "Failed to open the file to read: " << m_fileName << "\n";
        inputFile.Close();
        return false;
    }

    // Get the input object
    TObject* inputObject = inputFile.Get(m_histName.c_str());
    if (!inputObject)
    {
        std::cout << "Failed to retreive the requested histogram \"" << m_histName << "\" from the file: " << m_fileName << "\n";
        inputFile.Close();
        return false;
    }

    // Confirm that the input object is a histogram
    m_hist = std::unique_ptr<TH1>(dynamic_cast<TH1*>(inputObject));
    if (!m_hist)
    {
        std::cout << "Failed to convert the retrieved input to a histogram \"" << m_histName << "\" from the file: " << m_fileName << "\n";
        inputFile.Close();
        return false;
    }

    // Successfully retrieved the histogram
    m_hist->SetDirectory(0);
    inputFile.Close();
    return true;
}

double HistoInputBase::enforceAxisRange(const TAxis& axis, const double inputValue) const
{
    // edgeOffset should be chosen to be above floating point precision, but negligible compared to the bin size
    // An offset of edgeOffset*binWidth is therefore irrelevant for physics as the values don't change fast (but avoids edge errors)
    static constexpr double edgeOffset {1.e-4};

    // Root histogram binning:
    //  bin 0 = underflow bin
    //  bin 1 = first actual bin
    //  bin N = last actual bin
    //  bin N+1 = overflow bin
    const int numBins {axis.GetNbins()};
    const int binIndex {axis.FindFixBin(inputValue)};

    if (binIndex < 1)
        // Return just inside the range of the first real bin
        return axis.GetBinLowEdge(1) + edgeOffset*axis.GetBinWidth(1);
    if (binIndex > numBins)
        // Return just inside the range of the last real bin
        // Don't use the upper edge as floating point can make it roll into the next bin (which is overflow)
        // Instead, use the last bin width to go slightly within the boundary
        return axis.GetBinLowEdge(numBins) + (1-edgeOffset)*axis.GetBinWidth(numBins);
    return inputValue;
}

double HistoInputBase::readFromHisto(const double X, const double Y, const double Z) const
{
    // TODO: extend this to have different reading strategies
    const int nDim {m_hist->GetDimension()};
    if (nDim == 1)
        return m_hist->Interpolate(X);
    else if (nDim == 2)
        return m_hist->Interpolate(X,Y);
    else if (nDim == 3)
        return m_hist->Interpolate(X,Y,Z);
    // Shouldn't reach here due to previous checks
    throw std::runtime_error("Unexpected number of dimensions of histogram: " + nDim);
    return 0;
}

