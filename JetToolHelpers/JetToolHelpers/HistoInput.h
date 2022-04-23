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
#include "TH1.h"
#include "Jet.h"
#include "JetContext.h"
#include "InputVariable.h"
#include "IInputBase.h"

class HistoInput : public IInputBase {
    public:         
        static bool readHistoFromFile(std::unique_ptr<TH1>& m_hist, const std::string m_filename, const std::string m_histName);
        static double enforceAxisRange(const TAxis& axis, const double inputValue);
        static double readFromHisto(const TH1& m_hist, const double X, const double Y=0, const double Z=0);

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
        HistoInput(
            const std::string& name, 
            const std::string& fileName, 
            const std::string& histName,
            const std::string& varName, const std::string& varType,
            bool isJetVar
        );

        /**
         * @brief Construct a new 2D Histo Input object
         */
        HistoInput(
            const std::string& name, 
            const std::string& fileName, 
            const std::string& histName,
            const std::string& varName1, const std::string& varType1, const bool isJetVar1,
            const std::string& varName2, const std::string& varType2, const bool isJetVar2
        );

        /**
         * @brief Construct a new 3D Histo Input object
         */
        HistoInput(
            const std::string& name, 
            const std::string& fileName, 
            const std::string& histName,
            const std::string& varName1, const std::string& varType1, const bool isJetVar1,
            const std::string& varName2, const std::string& varType2, const bool isJetVar2,
            const std::string& varName3, const std::string& varType3, const bool isJetVar3
        );
        virtual ~HistoInput() {}

        virtual bool getValue(const xAOD::Jet& jet, const JetContext& event, double& value) const;
        virtual bool initialize();
        virtual bool finalize();

        std::string getFileName() const { return m_fileName; }
        std::string getHistName() const { return m_histName; }
    private:
        const Int_t nDims;

        const std::string name; 
        const std::string m_fileName;
        const std::string m_histName;

        std::unique_ptr<TH1> m_hist;    // actual histogram from the which getValue() is done.

        const std::string m_varName1;
        const std::string m_varType1;
        const bool m_isJetVar1;
        std::unique_ptr<InputVariable> m_inVar1;
        
        const std::string m_varName2;
        const std::string m_varType2;
        const bool m_isJetVar2;
        std::unique_ptr<InputVariable> m_inVar2;
};

#endif