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
#include "JetToolHelpers/Mock.h"

#include "JetToolHelpers/JetContext.h"
#include "JetToolHelpers/InputVariable.h"
#include "JetToolHelpers/IInputBase.h"

class HistoInput : public IInputBase {
    public:         
        static bool readHistoFromFile(std::unique_ptr<TH1>& m_hist, const std::string m_filename, const std::string m_histName);
        static double enforceAxisRange(const TAxis& axis, const double inputValue);
        static double readFromHisto(const TH1& m_hist, const std::vector<double>& values);

        HistoInput(const std::string& name, const std::string& filename, const std::string histName, const std::initializer_list<InputVariable>& lst): 
            IInputBase(name),
            m_fileName{filename},
            m_histName{histName},
            in_vars_{lst},
            nDims{(int)lst.size()} {};

        HistoInput(
            const std::string& name, const std::string& filename, const std::string histName, 
            const std::string& name1, const std::string& type1, const bool isJetVar1): IInputBase(name), 
                m_fileName{filename},
                m_histName{histName},
                in_vars_{*InputVariable::createVariable(name1, type1, isJetVar1)},
                nDims{(int)in_vars_.size()} {};
        
        HistoInput(const std::string& name, const std::string& filename, const std::string histName, 
        const std::string& name1, const std::string& type1, const bool isJetVar1,
        const std::string& name2, const std::string& type2, const bool isJetVar2): 
            IInputBase(name),
            m_fileName{filename},
            m_histName{histName},
            in_vars_{*InputVariable::createVariable(name1, type1, isJetVar1), *InputVariable::createVariable(name2, type2, isJetVar2)},
            nDims{(int)in_vars_.size()} {};

        virtual ~HistoInput() {}
        virtual bool getValue(const xAOD::Jet& jet, const JetContext& event, double& value) const;

        virtual bool initialize();
        virtual bool finalize();

        std::string getFileName() const { return m_fileName; }
        std::string getHistName() const { return m_histName; }
            
        const std::string name; 
        const std::string m_fileName;
        const std::string m_histName;

        std::vector<InputVariable> in_vars_;
        const int nDims;

        std::unique_ptr<TH1> m_hist;
};

#endif