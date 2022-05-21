#include <iostream>
#include <random>
#include <limits>
#include <benchmark/benchmark.h>

#include "JetToolHelpers/HistoInput.h"
#include "JetToolHelpers/InputVariable.h"
#include "JetToolHelpers/Mock.h"

class JetFixture : public benchmark::Fixture {
    protected:
        std::vector<xAOD::Jet> jets;

        void SetUp(const ::benchmark::State& state) {
            
            std::mt19937 gen( 43294 );
            std::uniform_real_distribution< double > dist( -10000, 10000 );
            const int N_JETS = state.range(0);
            for(int i=0; i < N_JETS; i++) {
                auto jet = xAOD::Jet{dist(gen), dist(gen), dist(gen), dist(gen)};
                jets.push_back( jet );
            }
        }

        void TearDown(const ::benchmark::State& state) {
            jets.clear();
        }
};

static void BM_getJetValueOver2DHistogram(benchmark::State& state) {
    std::mt19937 gen( 43294 );
    std::uniform_real_distribution< double > dist( -10000, 10000 );

    std::vector<xAOD::Jet> jets; 
    const int N_JETS = state.range(0);

    for(int i=0; i < N_JETS; i++) {
        auto jet = xAOD::Jet{dist(gen), dist(gen), dist(gen), dist(gen)};
        jets.push_back( jet );
    }

    // benchmarking with 2D histogram.
    std::unique_ptr<InputVariable> i1 = InputVariable::createVariable("pt", "float", true);
    std::unique_ptr<InputVariable> i2 = InputVariable::createVariable("abseta", "float", true);
    std::string fileName("./R4_AllComponents.root");
    std::string histName2D("EtaIntercalibration_Modelling_AntiKt4EMPFlow");

    HistoInput histogram = HistoInput("Test histogram", fileName, histName2D, {*i1, *i2});
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

BENCHMARK_DEFINE_F(JetFixture, BM_getJetValueOver1DHistogram)(benchmark::State& state) {
    std::unique_ptr<InputVariable> i1 = InputVariable::createVariable("pt", "float", true);
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
};

BENCHMARK_REGISTER_F(JetFixture, BM_getJetValueOver1DHistogram)->RangeMultiplier(2)->Range(100, 10<<13);
BENCHMARK_MAIN();