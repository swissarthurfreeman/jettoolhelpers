#include <memory>
#include "JetToolHelpers/InputVariable.h"
#include "test/Test.h"

void testClassicalSupportedAttributes() {
    TEST_BEGIN("InputVariable Classical Attributes Support");

    std::unique_ptr<InputVariable> c = InputVariable::createVariable("e", "float", true);
    ASSERT_THROW(c->getName() == "e");
    
    c = InputVariable::createVariable("et", "float", true);
    ASSERT_THROW(c->getName() == "et");
    
    c = InputVariable::createVariable("pt", "float", true);
    ASSERT_THROW(c->getName() == "pt");

    c = InputVariable::createVariable("eta", "float", true);
    ASSERT_THROW(c->getName() == "eta");

    c = InputVariable::createVariable("abseta", "double", true);
    ASSERT_THROW(c->getName() == "abseta");

    c = InputVariable::createVariable("|eta|", "double", true);
    ASSERT_THROW(c->getName() == "|eta|");

    c = InputVariable::createVariable("absrapidity", "double", true);
    ASSERT_THROW(c->getName() == "absrapidity");

    c = InputVariable::createVariable("|rapidity|", "double", true);
    ASSERT_THROW(c->getName() == "|rapidity|");

    c = InputVariable::createVariable("rapidity", "double", true);
    ASSERT_THROW(c->getName() == "rapidity");

    c = InputVariable::createVariable("y", "double", true);
    ASSERT_THROW(c->getName() == "y");

    TEST_END("InputVariable Classical Attributes Support");
}

void testSupportedJetContextAttributes() {
    TEST_BEGIN("InputVariable JetContext Attributes Support");
    
    // testing JetContext InputVariables.
    std::unique_ptr<InputVariable> b = InputVariable::createVariable("randomSupportedJetContextVar", "float", false);
    ASSERT_THROW(b->getName() == "randomSupportedJetContextVar");

    b = InputVariable::createVariable("randomSupportedJetContextVar", "int", false);
    ASSERT_THROW(b->getName() == "randomSupportedJetContextVar");

    b = InputVariable::createVariable("randomUnsupportedJetContextVar", "double", false);
    ASSERT_THROW(b == nullptr);

    TEST_END("InputVariable JetContext Attributes Support");
}

void testArbitrarySupportedJetAttributes() {
    // these tests try to access arbitrary jet data, this is done using ATLAS's AuxElement interface
    // we do not test that interface, we test ours.
    // use different names because AuxElement is typesafe
    TEST_BEGIN("InputVariable Attribute Support");
    
    std::unique_ptr<InputVariable> a = InputVariable::createVariable("randomSupportedJetVar", "float", true);
    ASSERT_THROW(a->getName() == "randomSupportedJetVar");

    a = InputVariable::createVariable("anotherRandomSupportedJetVar", "int", true);
    ASSERT_THROW(a->getName() == "anotherRandomSupportedJetVar"); 

    TEST_END("InputVariable Arbitrary Attribute Support");
}

void testJetContextAttributeGetValue() {
    TEST_BEGIN("JetContext Attribute GetValue");

    std::unique_ptr<InputVariable> b = InputVariable::createVariable("floatAttribute", "float", false);
    
    xAOD::Jet jet;
    JetContext jc;

    ASSERT_EQUAL(jc.setValue("someRandomAttribute", 12.04), true);
    ASSERT_EQUAL(jc.setValue("floatAttribute", (float) 12.04), true);

    ASSERT_EQUAL(b->getValue(jet, jc), 12.04f);

    b = InputVariable::createVariable("anotherJetContextAttribute", "int", false);
    
    ASSERT_EQUAL(jc.setValue("anotherJetContextAttribute", 12), true);
    ASSERT_EQUAL(b->getValue(jet, jc), 12);

    b = InputVariable::createVariable("randomAttributeNotInJetContext", "int", false);
    ASSERT_EQUAL(b->getValue(jet, jc), -999);

    b = InputVariable::createVariable("randomAttributeNotInJetContext", "float", false);
    ASSERT_EQUAL(b->getValue(jet, jc), -999);

    TEST_END("JetContext Attribute GetValue");
}

int main() {
    TEST_BEGIN("InputVariable Unit Test");

    testClassicalSupportedAttributes();
    testArbitrarySupportedJetAttributes();
    testSupportedJetContextAttributes();
    testJetContextAttributeGetValue();
    
    TEST_END("InputVariable Unit Test");
    return 0;
}
