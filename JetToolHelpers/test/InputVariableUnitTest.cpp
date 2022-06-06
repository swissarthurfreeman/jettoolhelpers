/**
 * @file InputVariableUnitTest.cpp
 * @author A. Freeman (swissarthurfreeman@gmail.com)
 * @brief Unit test to test intended InputVariable behaviour. 
 * @date 2022-06-01
 * 
 * @copyright Copyright (c) 2022 for the benefit of the atlas collaboration. 
 * 
 */
#include "JetToolHelpers/Mock.h"
#include <memory>
#include "JetToolHelpers/InputVariable.h"
#include "test/Test.h"

void testClassicalSupportedAttributes() {
    TEST_BEGIN_CASE("InputVariable Classical Attributes Support");

    // pt, eta, phi, m
    JetContext jc;
    xAOD::Jet jet{-100, -200, -300, -400};
    std::unique_ptr<InputVariable> c = InputVariable::createVariable("e", "float", true);
    ASSERT_THROW(c->getName() == "e");
    ASSERT_THROW(c->getValue(jet, jc) == jet.e());

    c = InputVariable::createVariable("et", "float", true);
    ASSERT_THROW(c->getName() == "et");
    ASSERT_THROW(c->getValue(jet, jc) == jet.p4().Et());
    
    c = InputVariable::createVariable("pt", "float", true);
    ASSERT_THROW(c->getName() == "pt");
    ASSERT_THROW(c->getValue(jet, jc) == jet.pt());

    c = InputVariable::createVariable("eta", "float", true);
    ASSERT_THROW(c->getName() == "eta");
    ASSERT_THROW(c->getValue(jet, jc) == jet.eta());

    c = InputVariable::createVariable("abseta", "double", true);
    ASSERT_THROW(c->getName() == "abseta");
    ASSERT_THROW(c->getValue(jet, jc) == std::abs(jet.eta()));

    c = InputVariable::createVariable("|eta|", "double", true);
    ASSERT_THROW(c->getName() == "|eta|");
    ASSERT_THROW(c->getValue(jet, jc) == std::abs(jet.eta()));

    c = InputVariable::createVariable("absrapidity", "double", true);
    ASSERT_THROW(c->getName() == "absrapidity");

    c = InputVariable::createVariable("|rapidity|", "double", true);
    ASSERT_THROW(c->getName() == "|rapidity|");
    ASSERT_THROW(c->getValue(jet, jc) == std::abs(jet.rapidity()));

    c = InputVariable::createVariable("rapidity", "double", true);
    ASSERT_THROW(c->getName() == "rapidity");
    ASSERT_THROW(c->getValue(jet, jc) == jet.rapidity());

    c = InputVariable::createVariable("y", "double", true);
    ASSERT_THROW(c->getName() == "y");
    ASSERT_THROW(c->getValue(jet, jc) == jet.rapidity());

    TEST_END_CASE("InputVariable Classical Attributes Support");
}

void testSupportedJetContextAttributes() {
    TEST_BEGIN_CASE("InputVariable JetContext Attributes Support");
    
    // testing JetContext InputVariables.
    std::unique_ptr<InputVariable> b = InputVariable::createVariable("randomSupportedJetContextVar", "float", false);
    ASSERT_THROW(b->getName() == "randomSupportedJetContextVar");

    b = InputVariable::createVariable("randomSupportedJetContextVar", "int", false);
    ASSERT_THROW(b->getName() == "randomSupportedJetContextVar");

    b = InputVariable::createVariable("randomUnsupportedJetContextVar", "double", false);
    ASSERT_THROW(b == nullptr);

    TEST_END_CASE("InputVariable JetContext Attributes Support");
}

void testArbitrarySupportedJetAttributes() {
    // these tests try to access arbitrary jet data, this is done using ATLAS's AuxElement interface
    // we do not test that interface, we test ours.
    // use different names because AuxElement is typesafe
    TEST_BEGIN_CASE("InputVariable Attribute Support");
    
    std::unique_ptr<InputVariable> a = InputVariable::createVariable("randomSupportedJetVar", "float", true);
    ASSERT_THROW(a->getName() == "randomSupportedJetVar");

    a = InputVariable::createVariable("anotherRandomSupportedJetVar", "int", true);
    ASSERT_THROW(a->getName() == "anotherRandomSupportedJetVar"); 

    TEST_END_CASE("InputVariable Arbitrary Attribute Support");
}

void testJetContextAttributeGetValue() {
    TEST_BEGIN_CASE("JetContext Attribute GetValue");

    std::unique_ptr<InputVariable> b = InputVariable::createVariable("floatAttribute", "float", false);
    
    xAOD::Jet jet{0.5, 0.5, 0.5, 0.5};
    JetContext jc;

    ASSERT_EQUAL(jc.setValue("someRandomAttribute", 12.04), true);
    ASSERT_EQUAL(jc.setValue("floatAttribute", (float) 12.04), true);

    ASSERT_EQUAL(b->getValue(jet, jc), 12.04f);

    b = InputVariable::createVariable("anotherJetContextAttribute", "int", false);
    
    ASSERT_EQUAL(jc.setValue("anotherJetContextAttribute", 12), true);
    ASSERT_EQUAL(b->getValue(jet, jc), 12); // catch std::invalid_argument

    b = InputVariable::createVariable("randomAttributeNotInJetContext", "int", false);
    ASSERT_EQUAL(b->getValue(jet, jc), false);

    b = InputVariable::createVariable("randomAttributeNotInJetContext", "float", false);
    ASSERT_EQUAL(b->getValue(jet, jc), false);

    TEST_END_CASE("JetContext Attribute GetValue");
}

int main() {
    TEST_BEGIN("InputVariable Unit Test");

    testClassicalSupportedAttributes();

    #ifdef IN_PRODUCTION
        testArbitrarySupportedJetAttributes();
    #endif
    
    testSupportedJetContextAttributes();
    testJetContextAttributeGetValue();

    TEST_END("InputVariable Unit Test");
    return 0;
}
