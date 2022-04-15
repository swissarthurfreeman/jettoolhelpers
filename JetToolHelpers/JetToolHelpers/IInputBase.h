#ifndef JET_IINPUTBASE_H
#define JET_IINPUTBASE_H

#include <string>

#include "AsgTools/IAsgTool.h" // For definition of StatusCode

#include "xAODJet/Jet.h"
#include "JetToolHelpers/JetContext.h"

class IInputBase
{
    public:
        IInputBase(const std::string& name) : m_name{name} {}
        virtual ~IInputBase() {}

        virtual StatusCode initialize() = 0;
        virtual StatusCode finalize() = 0;

        virtual bool   getValue(const xAOD::Jet& jet, const JetContext& event, double& value) const = 0;
        virtual double getValue(const xAOD::Jet& jet, const JetContext& event) const
        {
            double returnVal {0};
            if (!getValue(jet,event,returnVal))
                throw std::runtime_error("An unchecked problem occurred in IInputBase::getValue()");
            return returnVal;
        }

        virtual bool runUnitTests() const = 0;

    private:
        std::string m_name;

};



#endif
