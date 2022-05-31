#ifndef JET_IINPUTBASE_H
#define JET_IINPUTBASE_H

#include <string>

#ifdef USING_XAOD
    #include "AsgTools/IAsgTool.h" // For definition of StatusCode
    #include "xAODJet/Jet.h"        
#else
    #include "JetToolHelpers/JetContext.h"
    #include "JetToolHelpers/Mock.h"
    #include "JetToolHelpers/JetContext.h"
#endif

/**
 * @brief basic interface for an input. Contains core 
 * bool getValue(jet, jetContext, value) method. The
 * underlying structure being read from is left to the 
 * implementations. 
 */
class IInputBase {
    public:
        IInputBase(const std::string& name) : m_name{name} {}
        virtual ~IInputBase() {}

        virtual StatusCode initialize() = 0;
        virtual StatusCode finalize() = 0;

        virtual bool getValue(const xAOD::Jet& jet, const JetContext& event, double& value) const = 0;
        // default implementation calls bool getValue
        virtual double getValue(const xAOD::Jet& jet, const JetContext& event) const {
            double returnVal {0};
            if (!getValue(jet, event, returnVal))
                throw std::runtime_error("An unchecked problem occurred in IInputBase::getValue()");
            return returnVal;
        }
        std::string getName() { return m_name; }
    private:
        std::string m_name;
};

#endif
