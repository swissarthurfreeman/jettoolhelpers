#ifndef JET_HISTOINPUT2D_H
#define JET_HISTOINPUT2D_H

#include "TH2.h"
#include "AsgTools/AsgTool.h"

#include "JetToolHelpers/HistoInputBase.h"
#include "JetToolHelpers/InputVariable.h"

class HistoInput2D : public HistoInputBase, public asg::AsgTool
{
    ASG_TOOL_CLASS(HistoInput2D,IInputBase)

    public:
        HistoInput2D(const std::string& name, const std::string& fileName, const std::string& histName);
        HistoInput2D(const std::string& name, const std::string& fileName, const std::string& histName, const std::string& varName1, const std::string& varType1, const bool isJetVar1, const std::string& varName2, const std::string& varType2, const bool isJetVar2);
        virtual ~HistoInput2D() {}

        virtual StatusCode initialize();
        virtual StatusCode finalize();

        virtual bool getValue(const xAOD::Jet& jet, const JetContext& event, double& value) const;
        using IInputBase::getValue;

        virtual bool runUnitTests() const;

    private:
        const std::string m_varName1;
        const std::string m_varType1;
        const bool m_isJetVar1;
        const std::string m_varName2;
        const std::string m_varType2;
        const bool m_isJetVar2;

        std::unique_ptr<InputVariable> m_inVar1;
        std::unique_ptr<InputVariable> m_inVar2;
};

#endif

