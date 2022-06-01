/**
 * @file InputVariable.h
 * @author A. Freeman (swissarthurfreeman@gmail.com)
 * @brief Contains core InputVariable class and it's specializations
 * for arbitrary attributes and jet context attributes.
 * @date 2022-06-01
 * 
 * @copyright Copyright (c) 2022 for the benefit of the ATLAS collaboration.
 */
#ifndef JET_INPUTVARIABLE_H
#define JET_INPUTVARIABLE_H

#include <cmath>
#include <memory>
#include <functional>
#include <string>

#include "JetToolHelpers/JetContext.h"

#ifdef USING_XAOD
    #include "xAODJet/Jet.h"
    #include "AthContainers/AuxElement.h"
#else
    #include "JetToolHelpers/Mock.h"
#endif

/**
 * @brief specificator of axis interpretation. 
 * Contains a getValue(const xAOD::Jet&, const JetContext&) method which yields
 * the corresponding axis interpretation. 
 * @note For example, if a histogram has a |eta| axis, creating an
 * input variable like so,
 * @code {.cpp}
 *  auto inVarAbsEta = InputVariable::createVariable("abseta", "float", true);
 *  xAOD::Jet jet{0.5, -100, 12, 54.5};
 *  std::cout << std::abs(jet.eta()) << "=" << inVarAbsEta << std::endl; // yields 100 = 100 
 * @endcode
 */
class InputVariable {
    public:
        /**
         * @brief factory method to create InputVariables. 
         * 
         * @param name the name of the variable, supported names are "e", "et", "phi", "eta", "abseta", 
         * "|eta|", "rapidity", "y", "absrapidity", "|rapidity|", "absy", "|y|" for Jet variables. 
         * If it's a jet variable who'se name isn't supported, it is assumed to be auxiliary jet data. 
         * int and float variables are supported for JetContext variables. 
         * @param type the type of the input variable, supported types are int or float. If a supported non auxiliary jet variable
         * is given the type is silently ignored. 
         * @param isJetVar wether it's part of xAOD::Jet or JetContext. 
         * @return std::unique_ptr<InputVariable>
         * 
         * @note Atlas doesn't allow exceptions in Config time, this is a config time function so a nullptrs
         * is returned if a variable is not supported. 
         */
        static std::unique_ptr<InputVariable> createVariable(
            const std::string& name, 
            const std::string& type, 
            const bool isJetVar
        );

        // Constructors
        InputVariable(const std::string& name): m_name{name}, m_scale{1.} {}
        
        /**
         * @brief Construct a new Input Variable with a custom function for returning
         * a value of Jet correctly. 
         * @param name 
         * @param func 
         */
        InputVariable(
            const std::string& name, 
            std::function<float(const xAOD::Jet& jet, const JetContext& jc)> func
        );
        std::function<float(const xAOD::Jet& jet, const JetContext& jc)> customFunction;

        // Core method for returning the value generically
        virtual float getValue(const xAOD::Jet& jet, const JetContext& jc) const {
            return m_scale * (customFunction(jet, jc));
        }

        std::string getName() const { return m_name;   }
        float getScale() const { return m_scale;  }
        void setScale(const float scale) { m_scale = scale; }
        void setGeV() { m_scale = 1.e3; }   // this is not -3, if we give 500, by default it's MeV, so if it's GeV we must multiply by 1e3.
        void setMeV() { m_scale = 1.;    }

        InputVariable(const InputVariable&) = delete;
        static constexpr int ERRORVALUE {-999};

    protected:
        const std::string m_name;           // name of the InputVariable, if it's a JetContext, name must appear as attribute in the map !
        float m_scale;                      // scale of the InputVariable, by default 1. e.g. MeV.
};

/**
 * @brief Class template for when the axis interpretation is that of a JetContext.
 * @tparam T the type of the variable readout (int or float).
 */
template <typename T> class InputVariableJetContext : public InputVariable {
    public:
        InputVariableJetContext(const std::string& name) : InputVariable(name) {}
        // should be T
        virtual float getValue(const xAOD::Jet&, const JetContext& event) const { 
            return event.isAvailable(m_name) ? event.getValue<T>(m_name) : ERRORVALUE; 
        }
};

#ifdef USING_XAOD
/**
 * @brief Template covering the case where input variable is jet but the attribute
 * is not predefined variation of et, pt, eta, phi... But an arbitrary one. 
 * We suppose that the user is using jets which contain this metadata or else we return
 * an ERRORVALUE.  
 * TODO : Update to throwing exception, remove ERRORVALUE
 * @tparam T 
 */
template <typename T> class InputVariableAttribute : public InputVariable {
    public:
        InputVariableAttribute(const std::string& name) : InputVariable(name), m_acc{name} {}
        virtual float getValue(const xAOD::Jet& jet, const JetContext&) const { 
            return m_acc.isAvailable(jet) ? m_acc(jet)*m_scale : ERRORVALUE; 
        }
    private:
        SG::AuxElement::ConstAccessor<T> m_acc;
};
#endif

#endif
