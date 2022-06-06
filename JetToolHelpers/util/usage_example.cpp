/**
 * @file usage_example.cpp 
 * @author A. Freeman (swissarthurfreeman@gmail.com)
 * @brief set of usage examples of JetToolHelpers for various
 * use cases. 
 * @date 2022-06-01
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>

#include "JetToolHelpers/InputVariable.h"
#include "JetToolHelpers/make_histo.h"

/**
 * @brief this example shows how to read a value from a 2D 
 * histogram where one axis has a scaled value (GeV).
 * The default scale of variables is MeV. Scaling requires
 * creating the inputvariable directly and calling the config
 * methods on it before passing it to make_histogram_with().
 */
/*
void readValueFrom2DHistogramWithScaledVariable() {
    Config conf = {
        "Test Histogram",                               // name of your histogram input
        "R4_AllComponents.root",                        // name of file
        "EtaIntercalibration_TotalStat_AntiKt4EMTopo"   // name of histogram within file.
    };

    JetContext jc;                      // empty jet context
    xAOD::Jet jet{0, 0, 1000, 1000};    // mock jet constructor
    
    auto inVarGeVpt = InputVariable::createVariable("pt", "float", true);
    inVarGeVpt->setGeV();               // make sure we've got the correct scale. 
        
    // eta is the greek letter of the Y axis.                             // inVarGeVpt, inVarEta are moved, owner is HistoInput
    auto inVarEta = InputVariable::createVariable("eta", "float", true);  // do not attempt to reuse them or you'll segfault !
    auto histo = make_histogram_with(conf, inVarGeVpt, inVarEta);         // make_histogram(conf, x_axis_interpretation, y_axis_interpretation)...
    
    histo.initialize();

    double value{0};
    histo.getValue(jet, jc, value);
    std::cout << "HistoGram readout value = " << value << std::endl;
}
*/

/**
 * @brief Simple classical usage of HistoInput
 * via MakeHistoInput. The parameters pt, float, true
 * mean the first axis is pt (MeV by default), of type float
 * and it's a Jet variable. 
 */
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
    //readValueFrom2DHistogramWithScaledVariable();

    Config conf = {
        "Test Histogram",                               // name of your histogram input
        "R4_AllComponents.root",                        // name of file
        "EtaIntercalibration_TotalStat_AntiKt4EMTopo"   // name of histogram within file.
    };
    
    return 0;
}
