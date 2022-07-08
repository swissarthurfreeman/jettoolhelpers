/**
 * @file perf_test.cpp
 * @author A. Freeman (github.com/swissarthurfreeman/)
 * @brief Set of JetToolHelpers benchmark to test reading values
 * from Histogram speeds between branches. 
 * @date 2022-05-31
 * @note This requires the google benchmark library. Will not be compiled
 * in production. 
 * 
 * The general workflow to register a benchmark is this : Create a fixture
 * class which inherits publicly from benchmark::Fixture. (a fixture is a set of preconditions
 * this is where we generate benchmark data, it's not counted in the measured times)
 * 
 * Register and declare a new benchmark by using the following macro, 
 * @code {.cpp}
 * BENCHMARK_DEFINE_F(fixture_class, name_of_method)(benchmark::State& state) {
 *      //specialized config... (not counted in time)     
 *      // method declaration
 *      for(auto _: state) {
 *          // actual benchmark code (this is where what you're measuring goes)
 *          // access whatever you defined as protected in your fixture by the
 *          // same name than what you declared in the fixture
 *          doComputationallyExpensiveThing();
 *      }
 * }
 * @endcode
 * 
 * @copyright Copyright (c) 2022 for the benefit of the ATLAS collaboration.
 */
#include <iostream>
#include <random>
#include <limits>
#include <benchmark/benchmark.h>

#include "JetToolHelpers/make_histo.h"
#include "JetToolHelpers/Mock.h"

/**
 * @brief Simple test setup for benchmarking
 * reading from jet vector. Jets are distributed for every
 * variable between -10k, 10k following a uniform distribution. 
 */
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

/**
 * @brief Simple test setup for benchmarking
 * reading from jet context. Context value is "saspidity" and is 
 * distributed uniformly for every between -10k, 10k. 
 */
class JetContextFixture: public benchmark::Fixture {
    protected:
        std::vector<JetContext> events;
        
        void SetUp(const ::benchmark::State& state) {    
            std::mt19937 gen( 43294 );
            std::uniform_real_distribution< float > dist( -10000, 10000 );
            std::uniform_int_distribution< int > int_dist( -10000, 10000 );
            const int N_JETS = state.range(0);
            for(int i=0; i < N_JETS; i++) {
                auto jc = JetContext();
                jc.setValue("saspidity", dist(gen));
                jc.setValue("acidity", int_dist(gen));
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

    HistoInput histogram = MakeHistoInput("Test histogram", fileName, histName2D, "saspidity", "float", false, "acidity", "int", false);
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

//BENCHMARK_REGISTER_F(JetFixture, BM_getJetValueOver1DHistogram)->RangeMultiplier(2)->Range(1000, 10<<10);
//BENCHMARK_REGISTER_F(JetFixture, BM_getJetValueOver2DHistogram)->RangeMultiplier(2)->Range(1000, 10<<10);

//BENCHMARK_REGISTER_F(JetContextFixture, BM_getJetContextValueOver1DHistogram)->RangeMultiplier(2)->Range(1000, 10<<10);
BENCHMARK_REGISTER_F(JetContextFixture, BM_getJetContextValueOver2DHistogram)->RangeMultiplier(2)->Range(1000, 10<<10);

BENCHMARK_MAIN();


