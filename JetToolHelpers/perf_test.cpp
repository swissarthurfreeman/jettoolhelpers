/**
 * @file perf_test.cpp
 * @author A. Freeman (github.com/swissarthurfreeman/)
 * @brief Set of JetToolHelpers benchmark to test reading values
 * from Histogram speeds between branches. 
 * @date 2022-05-31
 * 
 * @copyright Copyright (c) 2022 for the benefit of the ATLAS collaboration.
 * 
 */
#include <iostream>
#include <random>
#include <limits>
#include <benchmark/benchmark.h>

#include "JetToolHelpers/make_histo.h"
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

class JetContextFixture: public benchmark::Fixture {
    protected:
        std::vector<JetContext> events;

        void SetUp(const ::benchmark::State& state) {    
            std::mt19937 gen( 43294 );
            std::uniform_real_distribution< float > dist( -10000, 10000 );
            const int N_JETS = state.range(0);
            for(int i=0; i < N_JETS; i++) {
                auto jc = JetContext();
                jc.setValue("saspidity", dist(gen));
                events.push_back( jc );
            }
        }

        void TearDown(const ::benchmark::State& state) {
            events.clear();
        }
};

BENCHMARK_DEFINE_F(JetFixture, BM_getJetValueOver2DHistogram)(benchmark::State& state) {
    // benchmarking with 2D histogram.
    auto histogram = MakeHistoInput(
        "Test Histogram",
        "./R4_AllComponents.root",
        "EtaIntercalibration_Modelling_AntiKt4EMPFlow", 
        "pt", "float", true, "abseta", "float", true);
    histogram.initialize();
    JetContext jc;
    
    for(auto _: state) {
        for(auto& jet: jets) {
            double value{0};
            histogram.getValue(jet, jc, value);
        }
    }
}

BENCHMARK_DEFINE_F(JetFixture, BM_getJetValueOver1DHistogram)(benchmark::State& state) {
    std::string fileName("./R4_AllComponents.root");
    std::string histName1D("EffectiveNP_1_AntiKt4EMTopo");

    HistoInput histogram = MakeHistoInput("Test histogram", fileName, histName1D, "pt", "float", true);
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

BENCHMARK_DEFINE_F(JetContextFixture, BM_getJetContextValueOver1DHistogram)(benchmark::State& state) {
    std::unique_ptr<InputVariable> i1 = InputVariable::createVariable("saspidity", "float", false);
    std::string fileName("./R4_AllComponents.root");
    std::string histName1D("EffectiveNP_1_AntiKt4EMTopo");

    HistoInput histogram = MakeHistoInput("Test histogram", fileName, histName1D, "saspidity", "float", false);
    histogram.initialize();

    xAOD::Jet jet{5, 5, 5, 5};
    // anything before the loop is not counted.
    for(auto _: state) {
        for(auto& jc: events) {
            double value{0};
            histogram.getValue(jet, jc, value);
        }
    }
};

BENCHMARK_DEFINE_F(JetContextFixture, BM_getJetContextValueOver2DHistogram)(benchmark::State& state) {
    // benchmarking with 2D histogram.
    std::string fileName("./R4_AllComponents.root");
    std::string histName2D("EtaIntercalibration_Modelling_AntiKt4EMPFlow");

    HistoInput histogram = MakeHistoInput("Test histogram", fileName, histName2D, "pt", "float", true, "abseta", "float", true);
    histogram.initialize();

    xAOD::Jet jet{5, 5, 5, 5};
    // anything before the loop is not counted.
    for(auto _: state) {
        for(auto& jc: events) {
            double value{0};
            histogram.getValue(jet, jc, value);
        }
    }
}

BENCHMARK_REGISTER_F(JetFixture, BM_getJetValueOver1DHistogram)->RangeMultiplier(2)->Range(1000, 10<<10);
BENCHMARK_REGISTER_F(JetFixture, BM_getJetValueOver2DHistogram)->RangeMultiplier(2)->Range(1000, 10<<10);

BENCHMARK_REGISTER_F(JetContextFixture, BM_getJetContextValueOver1DHistogram)->RangeMultiplier(2)->Range(1000, 10<<10);
BENCHMARK_REGISTER_F(JetContextFixture, BM_getJetContextValueOver2DHistogram)->RangeMultiplier(2)->Range(1000, 10<<10);

BENCHMARK_MAIN();


