#include <iostream>

#include "JetToolHelpers/HistoInput.h"
#include "JetToolHelpers/InputVariable.h"
#include "TH1.h"

int main() {
    TH1 * a;
    std::unique_ptr<InputVariable> i1 = InputVariable::createVariable("pt", "float", true);
    std::unique_ptr<InputVariable> i2 = InputVariable::createVariable("et", "float", true);
    
    std::string fileName("R4_AllComponents.root");
    std::string histName2D("RelativeNonClosure_AFII_AntiKt4EMTopo");

    HistoInput histogram = HistoInput("Test histogram", fileName, histName2D, {*i1, *i2});
    histogram.initialize();
    return 0;
}