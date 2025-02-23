#ifndef JET_IINPUTBASE_H
#define JET_IINPUTBASE_H

#include <string>
#include "JetToolHelpers/JetContext.h"
#include "Mock.h"

class IInputBase
{
    public:
        IInputBase(const std::string& name) : m_name{name} {}
        virtual ~IInputBase() {}

        virtual bool initialize() = 0;
        virtual bool finalize() = 0;

        virtual bool   getValue(const xAOD::Jet& jet, const JetContext& event, double& value) const = 0;
        double getValue(const xAOD::Jet& jet, const JetContext& event) const
        {
            double returnVal {0};
            if (!getValue(jet,event,returnVal))
                throw std::runtime_error("An unchecked problem occurred in IInputBase::getValue()");
            return returnVal;
        }

    private:
        std::string m_name;

};

#endif
