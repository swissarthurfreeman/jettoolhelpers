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
#include "TH1.h"

#include "JetContext.h"
#include "InputVariable.h"
#include "IInputBase.h"

class HistoInput : public IInputBase {
    public:         
        static bool readHistoFromFile(std::unique_ptr<TH1>& m_hist, const std::string m_filename, const std::string m_histName);
        static double enforceAxisRange(const TAxis& axis, const double inputValue);
        static double readFromHisto(const TH1& m_hist, const std::vector<double>& values);

        /**
         * @brief Construct a new 1D Histogram Input Object.
         * 
         * @param name the name of the histogram.
         * @param filename the .root filename. 
         * @param histName the histogram name contained in the .root file.
         * @param varName the input variable name. This is the interpretation
         * of the histogram. 
         * @param varType 
         * @param isJetVar declare within variable is an attribute of JetContext or an
         * attribute of xAOD::Jet. 
         */
        HistoInput(const std::string& name, const std::string& filename, const std::string histName, const std::initializer_list<InputVariable>& lst): 
            IInputBase(name), 
            m_fileName{filename},
            m_histName{histName},
            in_vars_{lst}, 
            nDims{lst.size()} {};

        virtual ~HistoInput() {}
        virtual bool getValue(const xAOD::Jet& jet, const JetContext& event, double& value) const;

        virtual bool initialize();
        virtual bool finalize();

        std::string getFileName() const { return m_fileName; }
        std::string getHistName() const { return m_histName; }
    private:
        const std::string name; 
        const int nDims;
        
        const std::string m_fileName;
        const std::string m_histName;

        std::unique_ptr<TH1> m_hist;    // actual histogram from the which getValue() is done.

        std::vector<InputVariable> in_vars_;
};

#endif