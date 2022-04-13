#ifndef JET_HISTOINPUT1D_H
#define JET_HISTOINPUT1D_H

#include "TH1.h"
#include "AsgTools/AsgTool.h"

#include "JetToolHelpers/HistoInputBase.h"
#include "JetToolHelpers/InputVariable.h"

class HistoInput1D : public HistoInputBase, public asg::AsgTool
{
    ASG_TOOL_CLASS(HistoInput1D,IInputBase)
    
    public:
        HistoInput1D(const std::string& name, const std::string& fileName, const std::string& histName);
        HistoInput1D(const std::string& name, const std::string& fileName, const std::string& histName, const std::string& varName, const std::string& varType, const bool isJetVar);
        virtual ~HistoInput1D() {}

        virtual StatusCode initialize();
        virtual StatusCode finalize();

        virtual bool getValue(const xAOD::Jet& jet, const JetContext& event, double& value) const;
        using IInputBase::getValue;

        virtual bool runUnitTests() const;

    private:
        const std::string m_varName;
        const std::string m_varType;
        const bool m_isJetVar;

        std::unique_ptr<InputVariable> m_inVar;
};

#endif
