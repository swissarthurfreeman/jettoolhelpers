/**
 * @file HistoInputUnitTest.cpp
 * @author A. Freeman (github.com/swissarthurfreena)
 * @brief Unit test to test HistoInput class with various scenarios. 
 * These scenarios depend on the dimensionality of the histogram
 * and the axis interpretation, every function is a variation of
 * those parameters. 
 * @date 2022-06-01
 * 
 * @copyright Copyright (c) 2022 for make benefit great nation of Kazakhstan. 
 */
#include <random>
#include <memory>
#include "test/Test.h"
#include "JetToolHelpers/make_histo.h"
#include "TH2.h"
#include "TH3.h"
#include <cmath>

/**
 * @brief Test reading out of 1D H=X Histogram where X axis is a jet variable. 
 * @param jets a vector of jets. 
 */
void test1DHistogramReadingOnXH(std::vector<xAOD::Jet>& jets) {
    TEST_BEGIN_CASE("HistoInput 1D Histogram Reading from Jet Variable");

    // X histogram
    auto histo = new TH1F("1D", "boring histo", 500, 0, 1000);
    
    for(int i=1; i <= histo->GetNbinsX(); i++)
        histo->SetBinContent(i, i);
    
    auto histoInput = MakeHistoInput("Test Histo", "randomFile", "hello", "e", "float", true);
    
    histoInput.setHist(histo); // copies raw ptr, memory is freed once histoInput steps out of frame. 
    JetContext jc;
    
    double val{0};
    for(auto jet: jets) {
        histoInput.getValue(jet, jc, val);
        // interpolation might on i yield a larger value than i which when rounded could
        // be in bin i, i-1 or i + 1. We have to try them all.
        try {
            ASSERT_EQUAL((int) round(val), histo->FindBin(jet.e()));
        } catch(std::runtime_error& e) {
            try {
                ASSERT_EQUAL((int) round(val), histo->FindBin(jet.e()) + 1);    // if underflow
            } catch(std::runtime_error& e) {
                ASSERT_EQUAL((int) round(val), histo->FindBin(jet.e()) - 1);    // if overflow
            }
        }
    }

    TEST_END_CASE("HistoInput 1D Histogram Reading from Jet Variable");
}

/**
 * @brief Test reading out of 1D H=X Histogram where X axis is a JetContext variable. 
 * @param events a vector of JetContext with two keys : (float) saspidity and (int) durphi. 
 */
void test1DHistogramReadingOnXH(std::vector<JetContext>& events) {
    TEST_BEGIN_CASE("HistoInput 1D Histogram Reading from JetContext Variable");

    // X histogram
    auto sas_histo = new TH1F("1D", "boring histo", 50, 0, 1000);
    auto durph_histo = new TH1F("1D", "boring histo", 50, 0, 1000);
    
    for(int i=1; i <= sas_histo->GetNbinsX(); i++) {
        durph_histo->SetBinContent(i, i);
        sas_histo->SetBinContent(i, i);
    }
    
    auto durpHistoInput = MakeHistoInput("Test Histo", "randomFile", "hello", "durphi", "int", false); 
    auto sasHistoInput = MakeHistoInput("Another Histo", "someRandomFile", "bork", "saspidity", "float", false);
    
    durpHistoInput.setHist(sas_histo);
    sasHistoInput.setHist(durph_histo); // copies raw ptr, memory is freed once histoInput steps out of frame. 
    
    xAOD::Jet jet{0.5, 0.5, 0.5, 0.5};
    double sas_val{0};
    double durp_val{0};
    for(auto jc: events) {
        sasHistoInput.getValue(jet, jc, sas_val);
        durpHistoInput.getValue(jet, jc, durp_val);
        
        // interpolation might on i yield a larger value than i which when rounded could
        // be in bin i, i-1 or i + 1. We have to try them all.
        float saspidity_val{0}; 
        jc.getValue("saspidity", saspidity_val); 
        try {
            ASSERT_EQUAL((int) round(sas_val), sas_histo->FindBin(saspidity_val));
        } catch(std::runtime_error& e) {
            try {
                ASSERT_EQUAL((int) round(sas_val), sas_histo->FindBin(saspidity_val) + 1);    // if underflow
            } catch(std::runtime_error& e) {
                ASSERT_EQUAL((int) round(sas_val), sas_histo->FindBin(saspidity_val) - 1);    // if overflow
            }
        }

        int durphi_val{0}; 
        jc.getValue("durphi", durphi_val);
        try {
            ASSERT_EQUAL((int) round(durp_val), durph_histo->FindBin(durphi_val));
        } catch(std::runtime_error& e) {
            try {
                ASSERT_EQUAL((int) round(durp_val), durph_histo->FindBin(durphi_val) + 1);    // if underflow
            } catch(std::runtime_error& e) {
                ASSERT_EQUAL((int) round(durp_val), durph_histo->FindBin(durphi_val) - 1);    // if overflow
            }
        }
    }

    TEST_END_CASE("HistoInput 1D Histogram Reading from Jet Context");
}

/**
 * @brief Test reading out of 2D H=X+Y Histogram where X and Y axis are jet variables. 
 * @param jets a vector of jets. 
 */
void test2DHistogramReadingOnXYH(std::vector<xAOD::Jet>& jets) {
    TEST_BEGIN_CASE("HistoInput 2D Histogram Reading from Jet Variable");

    // X histogram
    auto histo = new TH2F("1D", "boring histo", 50, 0, 1000, 50, 0, 1000);
    
    for(int i=1; i <= histo->GetNbinsX(); i++)
        for(int j=1; j <= histo->GetNbinsY(); j++)
            histo->SetBinContent(i, j, i + j);
    
    auto histoInput = MakeHistoInput("Test Histo", "randomFile", "hello", "e", "float", true, "eta", "float", true);
    
    histoInput.setHist(histo); // copies raw ptr, memory is freed once histoInput steps out of frame. 
    JetContext jc;
    
    double val{0};
    for(auto jet: jets) {
        histoInput.getValue(jet, jc, val);
        // if jet value was overflow, round will always round to the value
        // of the bucket val is in.
        try {
            ASSERT_EQUAL((int) round(val), histo->GetXaxis()->FindBin(jet.e()) + histo->GetYaxis()->FindBin(jet.eta()));
        } catch(std::runtime_error& e) {
            try {
                ASSERT_EQUAL((int) round(val), histo->GetXaxis()->FindBin(jet.e()) + histo->GetYaxis()->FindBin(jet.eta()) - 1);
            } catch(std::runtime_error& e) {
                try {
                    ASSERT_EQUAL((int) round(val), histo->GetXaxis()->FindBin(jet.e()) + histo->GetYaxis()->FindBin(jet.eta()) - 2);
                } catch(std::runtime_error& e) {
                    try {
                        ASSERT_EQUAL((int) round(val), histo->GetXaxis()->FindBin(jet.e()) + histo->GetYaxis()->FindBin(jet.eta()) + 1);
                    } catch(std::runtime_error& e) { 
                        ASSERT_EQUAL((int) round(val), histo->GetXaxis()->FindBin(jet.e()) + histo->GetYaxis()->FindBin(jet.eta()) + 2);
                    }
                }
            }
        }
    }
    TEST_END_CASE("HistoInput 2D Histogram Reading from Jet Variable");
}

/**
 * @brief Test reading out of 2D H=X+Y Histogram where Y axis is a JetContext
 * variable and X axis is a Jet variable. 
 * @param events a vector of JetContext with two keys : (float) saspidity and (int) durphi. 
 */
void test2DHistogramReadingOnXYH(std::vector<xAOD::Jet>& jets, std::vector<JetContext>& events) {
    TEST_BEGIN_CASE("HistoInput 2D Histogram Reading from Jet Variable");

    // X histogram
    auto histo = new TH2F("1D", "boring histo", 100, 0, 1000, 100, 0, 1000);
    
    for(int i=1; i <= histo->GetNbinsX(); i++)
        for(int j=1; j <= histo->GetNbinsY(); j++)
            histo->SetBinContent(i, j, i + j);
    
    auto histoInput = MakeHistoInput("Test Histo", "randomFile", "hello", "e", "float", true, "saspidity", "float", false);
    
    histoInput.setHist(histo); // copies raw ptr, memory is freed once histoInput steps out of frame. 
    JetContext jc;
    
    double val{0};
    for (auto jet: jets) {
        for(auto jc: events) {
            histoInput.getValue(jet, jc, val);
            // if jet value was overflow, round will always round to the value
            // of the bucket val is in.
            float sas_value{0}; // TODO : manage jc.getValue() when value passed by reference is double. 
            jc.getValue("saspidity", sas_value);
            try {
                ASSERT_EQUAL((int) round(val), histo->GetXaxis()->FindBin(jet.e()) + histo->GetYaxis()->FindBin(sas_value));
            } catch(std::runtime_error& e) {
                try {
                    ASSERT_EQUAL((int) round(val), histo->GetXaxis()->FindBin(jet.e()) + histo->GetYaxis()->FindBin(sas_value) - 1);
                } catch(std::runtime_error& e) {
                    try {
                        ASSERT_EQUAL((int) round(val), histo->GetXaxis()->FindBin(jet.e()) + histo->GetYaxis()->FindBin(sas_value) - 2);
                    } catch(std::runtime_error& e) {
                        try {
                            ASSERT_EQUAL((int) round(val), histo->GetXaxis()->FindBin(jet.e()) + histo->GetYaxis()->FindBin(sas_value) + 1);
                        } catch(std::runtime_error& e) { 
                            ASSERT_EQUAL((int) round(val), histo->GetXaxis()->FindBin(jet.e()) + histo->GetYaxis()->FindBin(sas_value) + 2);
                        }
                    }
                }
            }
        }
    }
    TEST_END_CASE("HistoInput 2D Histogram Reading from Jet Variable");
}

/**
 * @brief Test reading out of 3D H=X+Y+Z Histogram where all axis are Jet variables. 
 * @param jets a vector of jets with arbitrary 4 component values returned by SetUp(). 
 */
void test3DHistogramReadingOnXYZH(std::vector<xAOD::Jet>& jets) {
    TEST_BEGIN_CASE("HistoInput 3D Histogram Reading from Jet Variable");

    // X histogram
    auto histo = new TH3F("3D", "boring histo", 25, 0, 1000, 25, 0, 1000, 25, 0, 1000);
    
    for(int i=1; i <= histo->GetNbinsX(); i++)
        for(int j=1; j <= histo->GetNbinsY(); j++)
            for(int k=1; k <= histo->GetNbinsZ(); k++)
                histo->SetBinContent(i, j, k, i + j + k);
    
    auto histoInput = MakeHistoInput("Test Histo", "randomFile", "hello", 
                                     "pt", "float", true, 
                                     "eta", "float", true, 
                                     "phi", "float", true);
    
    histoInput.setHist(histo); // copies raw ptr, memory is freed once histoInput steps out of frame. 
    JetContext jc;
    
    TAxis * xaxis = histo->GetXaxis();
    TAxis * yaxis = histo->GetYaxis();
    TAxis * zaxis = histo->GetZaxis();
    
    double val{0};
    for(auto jet: jets) {
        // if jet value was overflow, round will always round to the value
        // of the bucket val is in.
        histoInput.getValue(jet, jc, val);
        
        // 3D, underflow / overflow to manage on every axis... 
        try {
            ASSERT_EQUAL((int) round(val), xaxis->FindBin(jet.pt()) + yaxis->FindBin(jet.eta()) + zaxis->FindBin(jet.phi()));
        } catch(std::runtime_error& e) {
            try {
                ASSERT_EQUAL((int) round(val), xaxis->FindBin(jet.pt()) + yaxis->FindBin(jet.eta()) + zaxis->FindBin(jet.phi()) - 1);
            } catch(std::runtime_error& e) {
                try {
                    ASSERT_EQUAL((int) round(val), xaxis->FindBin(jet.pt()) + yaxis->FindBin(jet.eta()) + zaxis->FindBin(jet.phi()) - 2);
                } catch(std::runtime_error& e) {
                    try {
                        ASSERT_EQUAL((int) round(val), xaxis->FindBin(jet.pt()) + yaxis->FindBin(jet.eta()) + zaxis->FindBin(jet.phi()) - 3);
                    } catch(std::runtime_error& e) { 
                        try {
                            ASSERT_EQUAL((int) round(val), xaxis->FindBin(jet.pt()) + yaxis->FindBin(jet.eta()) + zaxis->FindBin(jet.phi()) + 1);
                        } catch(std::runtime_error& e) { 
                            try {
                                ASSERT_EQUAL((int) round(val), xaxis->FindBin(jet.pt()) + yaxis->FindBin(jet.eta()) + zaxis->FindBin(jet.phi()) + 2);
                            } catch(std::runtime_error& e) { 
                                ASSERT_EQUAL((int) round(val), xaxis->FindBin(jet.pt()) + yaxis->FindBin(jet.eta()) + zaxis->FindBin(jet.phi()) + 3);
                            }
                        }
                    }
                }
            }
        }
    }
    TEST_END_CASE("HistoInput 3D Histogram Reading from Jet Variable");
}

/**
 * @brief fills the jets vector with N_JETS where every coordinate is
 * generated according to a uniform distribution between -2000, 2000.
 * @param N_JETS the number of desired jets.
 * @param jets the jet vector. (modified by reference)
 */
void SetUpJets(const int& N_JETS, std::vector<xAOD::Jet>& jets) {
    std::mt19937 gen( 43294 );
    std::uniform_real_distribution< double > dist( -2000, 2000 );
    for(int i=0; i < N_JETS; i++) {
        auto jet = xAOD::Jet{dist(gen), dist(gen), dist(gen), dist(gen)};
        jets.push_back( jet );
    }
}

/**
 * @brief Set up the Jet Contexts vector. Every jc will have a float "saspidity"
 * ant int "durphi" attribute.
 * @param N_JC the number of JetContexts.
 * @param events the initial vector push_back is called on.
 */
void SetUpJetContexts(const int& N_JC, std::vector<JetContext>& events) {
    std::mt19937 gen( 43294 );
    std::uniform_real_distribution< double > dist( -2000, 2000 );
    for(int i=0; i < N_JC; i++) {
        JetContext jc;
        jc.setValue("saspidity", dist(gen));
        jc.setValue("durphi", (int) dist(gen));
        events.push_back( jc );
    }
}

int main() {
    TEST_BEGIN("InputVariable Unit Test");

    std::vector<xAOD::Jet> jets;
    SetUpJets(500, jets);
    test1DHistogramReadingOnXH(jets);
    test2DHistogramReadingOnXYH(jets);
    test3DHistogramReadingOnXYZH(jets);

    std::vector<JetContext> events;
    SetUpJetContexts(500, events);
    test1DHistogramReadingOnXH(events);
    test2DHistogramReadingOnXYH(jets, events);

    TEST_END("InputVariable Unit Test");
    return 0;
}