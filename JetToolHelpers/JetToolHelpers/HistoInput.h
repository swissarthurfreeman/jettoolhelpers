/**
 * @file HistoInput.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
 * 
 */
#ifndef JET_HISTOINPUT_H
#define JET_HISTOINPUT_H

#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include "TH1.h"
#include "JetToolHelpers/Mock.h"

#include "JetToolHelpers/JetContext.h"
#include "JetToolHelpers/InputVariable.h"
#include "JetToolHelpers/IInputBase.h"

struct Config {
    const std::string name;
    const std::string m_fileName;
    const std::string m_histName;
};

template <typename T>
class HistoInput : public IInputBase {
    public:
        static double enforceAxisRange(const TAxis& axis, const double inputValue);
        static bool readHistoFromFile(
            std::unique_ptr<TH1>& m_hist, 
            const std::string m_filename, 
            const std::string m_histName
        );
        static const double constexpr Dim = std::tuple_size_v<T>;
        
        std::string m_fileName;
        std::string m_histName;

        T in_vars_;
        HistoInput(T var_tuple): IInputBase{"test"}, in_vars_{var_tuple} {}

        template<typename... A>
        double readFromHisto(const A... vals) {
            return 10.0;
            //return m_hist->Interpolate(vals...);
        }
        
        virtual ~HistoInput() {}
        virtual bool getValue(const xAOD::Jet& jet, const JetContext& event, double& value) const {
            // TODO: extend this to have different reading strategies
            /*if constexpr (Dim == 1)
                return readFromHisto(
                    std::get<0>(in_vars_).getValue(jet, event)
                );
            else if constexpr (Dim == 2)
                return readFromHisto(
                    std::get<0>(in_vars_).getValue(jet, event),
                    std::get<1>(in_vars_).getValue(jet, event)    
                );
            throw std::runtime_error("Unexpected number of dimensions of histogram.");*/
            return 0;
        };

        std::unique_ptr<TH1> m_hist;

        virtual bool initialize() {
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

            if (m_hist->GetDimension() != Dim) {
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

        virtual bool finalize() {
            if (m_hist)
                m_hist.reset();
            return true;
        }
};

#endif
