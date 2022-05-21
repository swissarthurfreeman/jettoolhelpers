#ifndef JET_INPUTVARIABLE_H
#define JET_INPUTVARIABLE_H

#include <string>
#include <cmath>
#include <memory>
#include <functional>

#include "JetToolHelpers/JetContext.h"
#include "JetToolHelpers/Mock.h"

/**
 * @brief 
 * 
 */
class InputVariable {
    public:
        /**
         * @brief Create a Variable object by mapping config strings to a given instance
         * 
         * @param name 
         * @param type 
         * @param isJetVar 
         * @return std::unique_ptr<InputVariable> 
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
        void setGeV() { m_scale = 1.e-3; }
        void setMeV() { m_scale = 1.;    }

        static constexpr int ERRORVALUE {-999};

    protected:
        const std::string m_name;
        float m_scale;
};

/**
 * @brief Template covering the case where input variable is jet but the attribute
 * is not predefined variation of et, pt, eta, phi... But an arbitrary one. 
 * We suppose that the user is using jets which contain this metadata or else we return
 * an ERRORVALUE.  
 * 
 * @tparam T 
 */
#ifdef USE_ATHENA
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

template <typename T> class InputVariableJetContext : public InputVariable {
    public:
        InputVariableJetContext(const std::string& name) : InputVariable(name) {}
        // should be T
        virtual float getValue(const xAOD::Jet&, const JetContext& event) const { 
            return event.isAvailable(m_name) ? event.getValue<T>(m_name) : ERRORVALUE; 
        }
};

#endif
