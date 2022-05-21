#include <iostream>
#include <random>
#include <limits>
#include <benchmark/benchmark.h>

#include "JetToolHelpers/HistoInput.h"
#include "JetToolHelpers/InputVariable.h"
#include "JetToolHelpers/Mock.h"

static void BM_getValueOver(benchmark::State& state) {
    std::mt19937 gen( 43294 );
    std::uniform_real_distribution< double > dist( -10000, 10000 );

    std::vector<xAOD::Jet> jets; 
    const int N_JETS = state.range(0);

    for(int i=0; i < N_JETS; i++) {
        auto jet = xAOD::Jet{dist(gen), dist(gen), dist(gen), dist(gen)};
        jets.push_back( jet );
    }

    std::unique_ptr<InputVariable> i1 = InputVariable::createVariable("pt", "float", true);
    std::unique_ptr<InputVariable> i2 = InputVariable::createVariable("et", "float", true);
    std::string fileName("./R4_AllComponents.root");
    std::string histName1D("EffectiveNP_1_AntiKt4EMTopo");

    HistoInput histogram = HistoInput("Test histogram", fileName, histName1D, {*i1});
    histogram.initialize();

    JetContext jc;
    // anything before the loop is not counted.
    for(auto _: state) {
        for(auto& jet: jets) {
            double value{0};
            histogram.getValue(jet, jc, value);
        }
    }
}

BENCHMARK(BM_getValueOver)->RangeMultiplier(2)->Range(10, 10<<12);
BENCHMARK_MAIN();

/*int main() {
    std::cout << "kill me please" << std::endl;

    std::unique_ptr<InputVariable> i1 = InputVariable::createVariable("pt", "float", true);
    std::unique_ptr<InputVariable> i2 = InputVariable::createVariable("et", "float", true);
        
    std::string fileName("./R4_AllComponents.root");
    std::string histName1D("EffectiveNP_1_AntiKt4EMTopo");
    //std::string histName2D("RelativeNonClosure_AFII_AntiKt4EMTopo");
    HistoInput histogram = HistoInput("Test histogram", fileName, histName1D, {*i1});
    histogram.initialize();

    std::cout << "Lower bound of X axis = "  << histogram.m_hist->GetXaxis()->GetBinLowEdge(1) << std::endl;
    std::cout << "Higher bound of X axis = " << histogram.m_hist->GetXaxis()->GetBinUpEdge(histogram.m_hist->GetNbinsX()) << std::endl;
    
    // double pt, double eta, double phi, double m
    xAOD::Jet a{292.4, 0.3, 25.4, 20.6};
    JetContext jc;
    double val{0};
    histogram.getValue(a, jc, val);
    std::cout << val << std::endl;
    return 0;
}*/