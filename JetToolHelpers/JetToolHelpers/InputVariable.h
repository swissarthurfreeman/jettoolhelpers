#ifndef JET_INPUTVARIABLE_H
#define JET_INPUTVARIABLE_H

#include <string>
#include <cmath>
#include <memory>

#include "JetToolHelpers/Jet.h"
#include "JetToolHelpers/JetContext.h"

// #include "AthContainers/AuxElement.h"
class InputVariable {
    public:
        // static creation method (map config strings to a given instance)
        static std::unique_ptr<InputVariable> createVariable(const std::string& name, const std::string& type, const bool isJetVar);

        // Constructor
        InputVariable(const std::string& name);

        // Core method for returning the value generically
        virtual float getValue(const xAOD::Jet& jet, const JetContext& event) const = 0;

        std::string getName()  const            { return m_name;   }
        float       getScale() const            { return m_scale;  }
        void        setScale(const float scale) { m_scale = scale; }
        void        setGeV()                    { m_scale = 1.e-3; }
        void        setMeV()                    { m_scale = 1.;    }

        static constexpr int ERRORVALUE {-999};

    protected:
        std::string m_name;
        float m_scale;
};

// Specifications of the InputVariable

class InputVariableE : public InputVariable
{
    public:
        InputVariableE(const std::string& name) : InputVariable(name) {}
        virtual float getValue(const xAOD::Jet& jet, const JetContext&) const { return jet.e()*m_scale; }
};

class InputVariableET : public InputVariable
{
    public:
        InputVariableET(const std::string& name) : InputVariable(name) {}
        virtual float getValue(const xAOD::Jet& jet, const JetContext&) const { return jet.p4().Et()*m_scale; }
};

class InputVariablePT : public InputVariable
{
    public:
        InputVariablePT(const std::string& name) : InputVariable(name) {}
        virtual float getValue(const xAOD::Jet& jet, const JetContext&) const { return jet.pt()*m_scale; }
};

class InputVariableEta : public InputVariable
{
    public:
        InputVariableEta(const std::string& name) : InputVariable(name) {}
        virtual float getValue(const xAOD::Jet& jet, const JetContext&) const { return jet.eta(); }
};

class InputVariableAbsEta : public InputVariable
{
    public:
        InputVariableAbsEta(const std::string& name) : InputVariable(name) {}
        virtual float getValue(const xAOD::Jet& jet, const JetContext&) const { return std::abs(jet.eta()); }
};

class InputVariableRapidity : public InputVariable
{
    public:
        InputVariableRapidity(const std::string& name) : InputVariable(name) {}
        virtual float getValue(const xAOD::Jet& jet, const JetContext&) const { return jet.rapidity(); }
};

class InputVariableAbsRapidity : public InputVariable
{
    public:
        InputVariableAbsRapidity(const std::string& name) : InputVariable(name) {}
        virtual float getValue(const xAOD::Jet& jet, const JetContext&) const { return std::abs(jet.rapidity()); }
};

/*simple template to cover case where a variable is a jet variable but not part of the 
predefined attributes (eta, rapidity, E, Et, pt...) we suppose it's a generic one and store
it in ConstAccessor<T>, const accessor means the reference returned is constant therefore unmodifiable,
see https://stackoverflow.com/questions/26917441/accessor-functions-in-class-c*/
/*
template <typename T>
class InputVariableAttribute : public InputVariable
{
    public:
        InputVariableAttribute(const std::string& name) : InputVariable(name), m_acc{name} {}
        virtual float getValue(const xAOD::Jet& jet, const JetContext&) const { return m_acc.isAvailable(jet) ? m_acc(jet)*m_scale : ERRORVALUE; }

    private:
        SG::AuxElement::ConstAccessor<T> m_acc;
};
*/

template <typename T> class InputVariableJetContext : public InputVariable {
    public:
        InputVariableJetContext(const std::string& name) : InputVariable(name) {}
        // should be T
        virtual float getValue(const xAOD::Jet&, const JetContext& event) const { 
            return event.isAvailable(m_name) ? event.getValue<T>(m_name) : ERRORVALUE; 
        }
};

#endif
