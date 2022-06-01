#include <iostream>

#include "JetToolHelpers/InputVariable.h"
#include "JetToolHelpers/make_histo.h"

int main() {
    std::string histName2D{"EtaIntercalibration_TotalStat_AntiKt4EMTopo"};
    JetContext jc;
    xAOD::Jet jet{0, 0, 1000, 1000};
    
    Config conf = {
        "Test Histogram",
        "R4_AllComponents.root",
        histName2D
    };

    auto inVarGeVpt = InputVariable::createVariable("pt", "float", true);
    inVarGeVpt->setGeV();

    // eta is the greek letter of the Y axis.
    auto inVarEta = InputVariable::createVariable("eta", "float", true);  // inVarGeVpt, inVarEta are moved, owner is HistoInput
    auto histo = make_histogram_with(conf, inVarGeVpt, inVarEta);         // do not attempt to reuse them or you'll segfault !
    
    histo.initialize();

    double value{0};
    histo.getValue(jet, jc, value);
    std::cout << "HistoGram readout value = " << value << " where histname is " << histName2D << std::endl;

    
    return 0;
}
