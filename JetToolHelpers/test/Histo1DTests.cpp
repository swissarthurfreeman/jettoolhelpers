#include <iostream>
#include <stdexcept>

#include "TH1.h"
#include "JetToolHelpers/InputVariable.h"
// after making run ctest --output-on-failure

int main() {
    //throw std::runtime_error(std::string("This is an exception"));
    TH1F h1("h1", "Histo grom a Gaussian", 100, -3, 3);
    h1.FillRandom("gaus", 10000);
    std::cout << "Good lord, we're writing code" << std::endl;
    return 0;
}