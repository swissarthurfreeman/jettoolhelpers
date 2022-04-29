#include <iostream>

#include "JetToolHelpers/InputVariable.h"
#include "JetToolHelpers/HistoInput.h"

int main(int argc, char* argv[])
{
    std::string fileName{"/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/JetUncertainties/CalibArea-08/rel21/Summer2019/R4_AllComponents.root"};
    std::string histName1D{"EffectiveNP_1_AntiKt4EMPFlow"};
    std::string histName2D{"EtaIntercalibration_Modelling_AntiKt4EMPFlow"};
    std::unique_ptr<InputVariable> a = InputVariable::createVariable("pt", "float", true);
    JetContext jc;
    xAOD::Jet jet{30,3.5,0,0};
    double value{0};
    std::cout << jet.pt() << std::endl;
    IInputBase* myH1D = new HistoInput("Test HistoGram", fileName, histName1D, "pt", "float", true);
    
    // should return 30
    // std::cout << a->getValue(jet, jc) << std::endl;
    /*
    IInputBase* myH1D = new HistoInput("Test HistoGram", fileName, histName1D, "pt", "float", true);
    if (!myH1D->initialize()) {
        std::cout << "Failed to initialise 1D HistoInput\n";
        return 1;
    }
    else
        std::cout << "Initialised HistoInput with 1D histogram\n";
    
    if (!myH1D->getValue(jet, jc, value))
        std::cout << "Failed to get 1D value\n";
    else 
        std::cout << "1D HistoInput value = " << value << std::endl;
    
    IInputBase* myH2D = new HistoInput("2D Test HistoGram", fileName, histName2D, "pt", "float", true, "abseta", "float", true);
    if (!myH2D->initialize()) {
        std::cout << "Failed to initialise 2D HistoInput\n";
        return 1;
    }
    else
        std::cout << "Initialised HistoInput with 2D histogram\n";
    
    if (!myH2D->getValue(jet, jc, value))
        std::cout << "Failed to get 2D value\n";
    else 
        std::cout << "2D HistoInput value = " << value << std::endl;
    
    IInputBase* myJetContextH2D = new HistoInput("2D Test HistoGram", fileName, histName2D, "someProperty", "int", false, "anotherOne", "float", false);
    if (!myJetContextH2D->initialize()) {
        std::cout << "Failed to initialise 2D HistoInput\n";
        return 1;
    }
    else
        std::cout << "Initialised HistoInput with 2D histogram\n";
    
    jc.setValue("someProperty", 12);            // X axis input variable
    jc.setValue("anotherOne", (float) 4.5);     // Y axis input variable

    if (!myJetContextH2D->getValue(jet, jc, value))
        std::cout << "Failed to get 2D jetContext value\n";
    else 
        std::cout << "2D HistoInput value = " << value << std::endl;
*/
    return 0;
}
