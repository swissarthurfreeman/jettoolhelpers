#include <iostream>
#include <random>
#include <limits>

#include "JetToolHelpers/InputVariable.h"
#include "JetToolHelpers/HistoInput.h"

#include "test/Test.h"

#define MAX_DOUBLE std::numeric_limits<double>::max()

int main() {
    TEST_BEGIN("R4ComponentsTest"); // prints to cerr in order to be visible in ctest --verbose

    std::string fileName("R4_AllComponents.root");
    std::string histName1D("EffectiveNP_1_AntiKt4EMTopo");
    std::string histName2D("RelativeNonClosure_AFII_AntiKt4EMTopo");

    HistoInput myH1D = HistoInput("Test HistoGram", fileName, histName1D, "pt", "float", true);
    
    JetContext jc;
    double value{0};

    ASSERT_THROW(myH1D.initialize() == true);

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-MAX_DOUBLE, MAX_DOUBLE);
    
    for(int i=0; i < 1000; i++) {
        xAOD::Jet jet{distribution(generator), distribution(generator), distribution(generator), distribution(generator)};
        ASSERT_THROW(myH1D.getValue(jet, jc, value) == true);
    }
    
    

    TEST_END("R4ComponentsTest");
    return 0;
}
