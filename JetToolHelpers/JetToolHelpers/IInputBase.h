/**
 * @file IInputBase.h
 * @author A. Freeman (swissarthurfreeman@gmail.com)
 * @brief IInputBase interface.
 * @date 2022-06-01
 * 
 * @copyright Copyright (c) 2022 for the benefit of the ATLAS collaboration.
 * 
 */
#ifndef JET_IINPUTBASE_H
#define JET_IINPUTBASE_H

#include <string>

#ifdef IN_PRODUCTION
    #include "AsgTools/IAsgTool.h" // For definition of StatusCode
    #include "xAODJet/Jet.h"        
#else
    #include "JetToolHelpers/JetContext.h"
    #include "JetToolHelpers/Mock.h"
    #include "JetToolHelpers/JetContext.h"
#endif

/**
 * @brief basic interface for an input. Contains core getValue method. 
 * The underlying structure being manipulated is left to the 
 * implementations. 
 */
class IInputBase {
    public:
        IInputBase(const std::string& name) : m_name{name} {}
        
        virtual StatusCode initialize() = 0;
        virtual StatusCode finalize() = 0;

        /**
         * @brief return the readout value depending on a jet and a jetContext. 
         * @param jet an instance of xAOD::Jet contains 4 coordinates and arbitrary 
         * auxiliary attributes.
         * @param event a JetContext, contains arbitrary event information.  
         * @param value a reference the value is written to.
         * @return true if suceeded
         * @return false if not.
         */
        virtual bool getValue(const xAOD::Jet& jet, const JetContext& event, double& value) const = 0;
        
        /** @brief super implementation of bool getValue simply calls bool getValue */
        virtual double getValue(const xAOD::Jet& jet, const JetContext& event) const {
            double returnVal {0};
            if (!getValue(jet, event, returnVal))
                throw std::runtime_error("An unchecked problem occurred in IInputBase::getValue()");
            return returnVal;
        }
        /** @brief returns the name of the input. If the underlying structure is a histogram it would be the name of the histogram set. */
        std::string getName() { return m_name; }
    private:
        std::string m_name;
};

#endif
