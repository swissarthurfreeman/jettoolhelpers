#include <iostream>

#include "JetToolHelpers/InputVariable.h"
#include "JetToolHelpers/make_histo.h"

/**
 * @brief 
 * 
 */
void readValueFrom2DHistogramWithScaledVariable() {
    Config conf = {
        "Test Histogram",           // name of your histogram input
        "R4_AllComponents.root",    // name of file
        "EtaIntercalibration_TotalStat_AntiKt4EMTopo"   // name of histogram within file.
    };

    JetContext jc;                      // empty jet context
    xAOD::Jet jet{0, 0, 1000, 1000};    // mock jet constructor
    
    auto inVarGeVpt = InputVariable::createVariable("pt", "float", true);
    inVarGeVpt->setGeV();

    // eta is the greek letter of the Y axis.
    auto inVarEta = InputVariable::createVariable("eta", "float", true);  // inVarGeVpt, inVarEta are moved, owner is HistoInput
    auto histo = make_histogram_with(conf, inVarGeVpt, inVarEta);         // do not attempt to reuse them or you'll segfault !
    
    histo.initialize();

    double value{0};
    histo.getValue(jet, jc, value);
    std::cout << "HistoGram readout value = " << value << std::endl;
}

void readValueFrom1DHistogram() {

    JetContext jc;                      // empty jet context
    xAOD::Jet jet{256, 5, 5, 5};        // mock jet constructor
    

    auto histo = MakeHistoInput("Test Histogram", "R4_AllComponents.root", "Zjet_MuStat12_AntiKt4EMTopo", "pt", "float", true);
    histo.initialize();

    double value{0};
    histo.getValue(jet, jc, value);
    std::cout << "HistoGram readout value = " << value << std::endl;
}

int main() {
    readValueFrom1DHistogram();
    readValueFrom2DHistogramWithScaledVariable();
    return 0;
}
