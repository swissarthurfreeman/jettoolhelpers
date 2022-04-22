#include <iostream>

#include "JetToolHelpers/InputVariable.h"
#include "JetToolHelpers/HistoInput.h"

#include "test/Test.h"

int main() {
    TEST_BEGIN("R4ComponentsTest");
    std::string fileName("R4_AllComponents.root");
    std::string histName1D("EffectiveNP_1_AntiKt4EMTopo");
    std::string histName2D("RelativeNonClosure_AFII_AntiKt4EMTopo");

    IInputBase* myH1D = new HistoInput("Test HistoGram", fileName, histName1D, "pt", "float", true);
    if (!myH1D->initialize()) {
        std::cout << "Failed to initialise 1D HistoInput\n";
        return 1;
    }
    else
        std::cout << "Initialised HistoInput with 1D histogram\n";
    
    JetContext jc;
    xAOD::Jet jet{30,3.5,0,0};
    double value{0};

    TEST_ASSERT_THROW(myH1D->getValue(jet, jc, value) == true);
    /*

    if (!myH1D->getValue(jet, jc, value))
        std::cout << "Failed to get 1D value\n";

    IInputBase* myH2Djc = new HistoInput("testInput", fileName, histName2D, "pt", "float", false, "abseta", "float", false);
    IInputBase* myH2Djj = new HistoInput("testInput", fileName, histName2D, "pt", "float",true, "myVar","float", true);
    
    if (!myH2Djj->initialize() || !myH2Djc->initialize()) {
        std::cout << "Failed to initialise HistoInput2D\n";
        return 1;
    }
    else
        std::cout << "Initialised HistoInput with 2D histogram\n";

    jc.setValue("myVar", (float) 3.5);

    if (!myH1D->getValue(jet,jc,value))
        std::cout << "Failed to get 1D value\n";
    else
        std::cout << "1D value is " << value << "\n";

    if (!myH2Djj->getValue(jet,jc,value))
        std::cout << "Failed to get 2D value\n";
    else
        std::cout << "2D value is " << value << " via the jet\n";
    */
    /*
    if (!myH2Djc->getValue(jet,jc,value))
        std::cout << "Failed to get 2D value\n";
    else
        std::cout << "2D value is " << value << " via the jet context\n";
    */
    TEST_END();
    return 0;
}
