#include <iostream>

#include "JetToolHelpers/InputVariable.h"
#include "JetToolHelpers/HistoInput1D.h"
#include "JetToolHelpers/HistoInput2D.h"

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cout << "USAGE: " << argv[0] << " <ROOT file name> <1D histogram name> <2D histogram name>\n";
        return 1;
    }
    std::string fileName {argv[1]};
    std::string histName1D {argv[2]};
    std::string histName2D {argv[3]};

    IInputBase* myH1D = new HistoInput1D{"testInput",fileName,histName1D,"pt","float",true};
    if (!myH1D->initialize())
    {
        std::cout << "Failed to initialise HistoInput1D\n";
        return 1;
    }
    else
        std::cout << "Initialised HistoInput1D\n";
    
    IInputBase* myH2Djj = new HistoInput2D{"testInput",fileName,histName2D,"pt","float",true,"abseta","float",true};
    IInputBase* myH2Djc = new HistoInput2D{"testInput",fileName,histName2D,"pt","float",true,"myVar","float",false};
    if (!myH2Djj->initialize() || !myH2Djc->initialize())
    {
        std::cout << "Failed to initialise HistoInput2D\n";
        return 1;
    }
    else
        std::cout << "Initialised HistoInput2D\n";

    xAOD::Jet jet{30,3.5,0,0};
    JetContext jc;
    jc.setValue<float>("myVar",3.5);
    double value {0};
    if (!myH1D->getValue(jet,jc,value))
        std::cout << "Failed to get 1D value\n";
    else
        std::cout << "1D value is " << value << "\n";

    if (!myH2Djj->getValue(jet,jc,value))
        std::cout << "Failed to get 2D value\n";
    else
        std::cout << "2D value is " << value << " via the jet\n";

    if (!myH2Djc->getValue(jet,jc,value))
        std::cout << "Failed to get 2D value\n";
    else
        std::cout << "2D value is " << value << " via the jet context\n";

    return 0;
}
