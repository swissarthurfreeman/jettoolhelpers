/**
 * @file HistoInput.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration 
 */
#ifndef JET_HISTOINPUT_H
#define JET_HISTOINPUT_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include "TH1.h"
#include "TFile.h"

#ifdef USING_XAOD
    #include "AsgTools/AsgTool.h"
#else
    #include "JetToolHelpers/Mock.h"
#endif

#include "JetToolHelpers/JetContext.h"
#include "JetToolHelpers/InputVariable.h"
#include "JetToolHelpers/IInputBase.h"

template <typename T>
class HistoInput : public IInputBase {
    public:         
        static constexpr double Dim = std::tuple_size_v<T>;
        ~HistoInput() {}
        HistoInput(const std::string& name, const std::string& filename, const std::string& histName, const T& vars): 
            IInputBase{name}, m_fileName{filename}, m_histName{histName}, in_vars_{vars} {};
        
        virtual bool getValue(const xAOD::Jet& jet, const JetContext& event, double& value) const {
            if constexpr (Dim == 1) {
                value = m_hist->Interpolate(
                    enforceAxisRange(*m_hist->GetXaxis(), std::get<0>(in_vars_).getValue(jet, event))
                );
            } else if constexpr(Dim == 2) { 
                value = m_hist->Interpolate(
                    enforceAxisRange(*m_hist->GetXaxis(), std::get<0>(in_vars_).getValue(jet, event)),
                    enforceAxisRange(*m_hist->GetYaxis(), std::get<1>(in_vars_).getValue(jet, event))
                );
            } else if constexpr(Dim == 3) {
                std::cout << "3D interpolate on \n";
                double x = enforceAxisRange(*(m_hist->GetXaxis()), std::get<0>(in_vars_).getValue(jet, event));
                double y = enforceAxisRange(*(m_hist->GetYaxis()), std::get<1>(in_vars_).getValue(jet, event));
                double z = enforceAxisRange(*(m_hist->GetZaxis()), std::get<2>(in_vars_).getValue(jet, event));
                std::cout << "x = " << x << " y = " << y << " z = " << z << std::endl;
                std::cout << "value = " << m_hist->Interpolate(x, y, z) << "\n";
                std::cout << "bins = " << m_hist->GetXaxis()->FindBin(x) << " " << m_hist->GetYaxis()->FindBin(y) << " " << m_hist->GetZaxis()->FindBin(z) << "\n"; 
                
                value = m_hist->Interpolate(
                    enforceAxisRange(*(m_hist->GetXaxis()), std::get<0>(in_vars_).getValue(jet, event)),
                    enforceAxisRange(*(m_hist->GetYaxis()), std::get<1>(in_vars_).getValue(jet, event)),
                    enforceAxisRange(*(m_hist->GetZaxis()), std::get<2>(in_vars_).getValue(jet, event))
                );
            } else {
                throw std::runtime_error("Unsupported number of dimensions of histogram.");
                return false;
            }
            return true;
        }

        virtual StatusCode initialize() {
            if (m_hist != nullptr) {
                std::cerr << "The histogram already exists" << std::endl;
                return false;
            }

            if (!readHistoFromFile(m_hist, m_fileName, m_histName)) {
                std::cerr << "Failed while reading histogram from file" << std::endl;
                throw std::runtime_error("Error reading histogram from file");
                return false;
            }

            if (!m_hist) {
                std::cerr << "Histogram pointer is empty after reading from file" << std::endl;
                return false;
            }

            if (m_hist->GetDimension() != Dim) {
                std::cout << "Read the specified histogram, but it has a dimension of " 
                << m_hist->GetDimension() << " instead of the expected 1" << std::endl;
                return false;
            }
            return true;
        }
        
        virtual StatusCode finalize() {
            if (m_hist)
                m_hist.reset();     // releases and deletes the owned histogram.
            return true;
        }

        void setHist(TH1* hist) { m_hist.reset(hist); }

        std::string getFileName() const { return m_fileName; }
        std::string getHistName() const { return m_histName; }
        
    private:
        const std::string name; 
        const std::string m_fileName;
        const std::string m_histName;

        std::unique_ptr<TH1> m_hist;
        T in_vars_;

        static bool readHistoFromFile(std::unique_ptr<TH1>& m_hist, const std::string fileName, const std::string histName);
        static double enforceAxisRange(const TAxis& axis, const double inputValue);
};

template<typename T>
bool HistoInput<T>::readHistoFromFile(std::unique_ptr<TH1>& m_hist, const std::string fileName, const std::string histName) {
    TFile inputFile(fileName.c_str(), "READ");
    if (inputFile.IsZombie()) {
        std::cout << "Failed to open the file to read: " << fileName << "\n";
        inputFile.Close();
        return false;
    }

    // Get the input object
    TObject* inputObject = inputFile.Get(histName.c_str());
    if (!inputObject) {
        std::cout << "Failed to retreive the requested histogram \"" 
        << histName << "\" from the file: " << fileName << "\n";
        inputFile.Close();
        return false;
    }

    // Confirm that the input object is a histogram
    m_hist = std::unique_ptr<TH1>(dynamic_cast<TH1*>(inputObject));
    if (!m_hist) {
        std::cout << "Failed to convert the retrieved input to a histogram \"" 
        << histName << "\" from the file: " << fileName << "\n";
        inputFile.Close();
        return false;
    }

    // Successfully retrieved the histogram
    m_hist->SetDirectory(0);
    inputFile.Close();
    return true;
}

/**
 * @brief Returns clipped inputValue to the axis range.
 * Returns a copy of returnValue if no clipping is done. 
 * @param axis a root TAxis.
 * @param inputValue 
 * @return double the clipped inputValue.
 */
template<typename T>
double HistoInput<T>::enforceAxisRange(const TAxis& axis, const double inputValue) {
    static constexpr double edgeOffset {1.e-4};
    // Root histogram binning: bin 0 = underflow bin, 
    // bin 1 = first actual bin, bin N = last actual bin, bin N+1 = overflow bin
    const int numBins {axis.GetNbins()};
    const int binIndex {axis.FindFixBin(inputValue)};
    
    // Return just inside the range of the first real bin
    if (binIndex < 1)
        return axis.GetBinLowEdge(1) + edgeOffset*axis.GetBinWidth(1);
        
    // Don't use the upper edge as floating point can make it roll into the next bin (which is overflow)
    // Instead, use the last bin width to go slightly within the boundary, return just inside the range of the last real bin
    if (binIndex > numBins)
        return axis.GetBinLowEdge(numBins) + (1-edgeOffset)*axis.GetBinWidth(numBins);

    return inputValue;
}

#endif
