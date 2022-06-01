\mainpage 
# JetToolHelpers

## Goal

This project is an Athena interface to ROOT Histograms whose goal is to allow the user to provide compile time axis interpretations. 

## Usage

```c++
#include "make_histo.h"

HistoInput histogram = make_histogram(
    "MachuPichu",                   
    "./R4_AllComponents.root",                            // the .root file containing 
    "EtaIntercalibration_Modelling_AntiKt4EMPFlow",       // this Histogram
    "absrapidity", "float", true, "abseta", "float", true // the axes interpretations
);

histogram.initialize();     // reads the .root file, checks dimensions.

xAOD::Jet jet{5, 5, 5, 5};
JetContext jc;
double x{0};

// getValue will know how to read abs(jet.rapidity) and abs(jet.eta)
// when interpolating in the histogram. 
std::cout << histogram.getValue(jet, jc, x) std << cout;
```

## Dependencies

ATHENA includes are mocked and only requires a local C++17 ROOT installation.

Since April 13th 2022 ROOT has prebuilt [binary releases available](https://root.cern/install/all_releases/).

This project has been built with [Release 6.26/02](https://root.cern/releases/release-62602/).

## Configuring CMake

The root CMakeLists.txt file defines where all sources are. CMake has a find package method that makes ```ROOT_INCLUDE_DIRS``` and ```ROOT_LIBRARIES``` variables available. 
Your local root installation location must be defined in this file. The CMake variable ```ROOT_DIR``` must be set for ```find_package(ROOT)``` to work. 

```cmake
set(ROOT_DIR /your/path/to/where/you/extracted/root/)
```

If your configuration is correct, please go to the build directory and run cmake.
```bash
JetToolHelpers/build$ mkdir build && cd build
JetToolHelpers/build$ cmake ../
JetToolHelpers/build$ make
```

## Testing

After having successfully built the project, you can run the tests with
```bash
JetToolHelpers/build$ ctest
``` 

If you would like more output, or if a test doesn't pass, you can use,
```bash
JetToolHelpers/build$ ctest --verbose
```

## Authors

S. Schramm Université de Genève, A. Freeman Université de Genève.