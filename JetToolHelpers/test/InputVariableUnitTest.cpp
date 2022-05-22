#include "JetToolHelpers/InputVariable.h"
#include "JetToolHelpers/HistoInput.h"

#include "test/Test.h"
#include "JetToolHelpers/Mock.h"

/**
 * @brief Tests specification of InputVariable.
 * STEVEN : We should refactor to throw invalid_argument exception
 * in case of non supported arguments no ? 
 * What we test for : 
 * - Creating supported variables (jetVar or not jetVar).
 * - Creating unsupported variables (jetVar or not).
 * - getting var name, getting scale, setting scale.
 * - getting GeV setting GeV...
 */

void testSupportedNames() {
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

    std::unique_ptr<InputVariable> a = InputVariable::createVariable("random", "float", true);
    ASSERT_THROW(a == nullptr);

    std::unique_ptr<InputVariable> b = InputVariable::createVariable("e", "double", false);
    ASSERT_THROW(b == nullptr);
}
/*
void testSupportedFunctions() {
    xAOD::TEvent event;

    // Hard-code the ROOT histogram file for now (for our tool tests)
    const std::string histFile{"./R4_AllComponents.root"};
    const std::string histName1D{"EffectiveNP_1_AntiKt4EMPFlow"};
    const std::string histName2D{"EtaIntercalibration_Modelling_AntiKt4EMPFlow"};
    std::unique_ptr<HistoInput> myHist = std::make_unique<HistoInput>("myH1D", histFile, histName1D, "pt", "float", true);
    myHist->initialize();

    TChain* chain = new TChain("CollectionTree");
    
    chain->AddFile("./testfile.root");

    // Get the event
    event.getEntry(0);
    
    // Get EventInfo and metadata
    const xAOD::EventInfo* eInfo = nullptr;
    if(event.retrieve(eInfo,"EventInfo").isFailure() || !eInfo) {
        printf("Failed to retrieve EventInfo\n");
        exit(1);
    }

    // Get jets
    const xAOD::JetContainer* jets = nullptr;
    if (event.retrieve(jets, "AntiKt4EMPFlowJets").isFailure() || !jets) {
        printf("Failed to retreive jets for event %lld (entry %lld)\n",eInfo->eventNumber(),0);
        exit(1);
    }

    // Make an empty JetContext object
    JetContext jc;

    // Print some info
    printf(", leading jet (pt,eta,phi,m) = (%.1f,%.1f,%.1f,%.1f)\n",jets->at(0)->pt()/1.e3,jets->at(0)->eta(),jets->at(0)->phi(),jets->at(0)->m()/1.e3);
    
    double myVal{0};
    myHist->getValue(*jets->at(0), jc, myVal);
    printf("Histogram values are %f (H1D)\n", myVal);
}
*/
int main() {
    TEST_BEGIN("InputVariable Unit Test");

    testSupportedNames();
    
    TEST_END("InputVariable Unit Test");
    return 0;
}