#include <random>
#include <memory>
#include "test/Test.h"
#include "JetToolHelpers/make_histo.h"
#include "TH2.h"
#include "TH3.h"
#include <cmath>

void test1DHistogramReadingOn(std::vector<xAOD::Jet>& jets) {
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
        // if jet value was overflow, round will always round to the value
        // of the bucket val is in. 
        ASSERT_EQUAL((int) round(val), histo->FindBin(jet.e()));
    }

    TEST_END_CASE("HistoInput 1D Histogram Reading from Jet Variable");
}

void test2DHistogramReadingOn(std::vector<xAOD::Jet>& jets) {
    TEST_BEGIN_CASE("HistoInput 1D Histogram Reading from Jet Variable");

    // X histogram
    auto histo = new TH2F("1D", "boring histo", 100, 0, 1000, 100, 0, 1000);
    
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
        std::cout << "(e = " << jet.e() << ", eta = " << jet.eta() << ")\n";
        std::cout << "val = " << (int) round(val) << std::endl;
        std::cout << "xbin = " << histo->GetXaxis()->FindBin(jet.e()) << std::endl;
        std::cout << "ybin = " << histo->GetYaxis()->FindBin(jet.eta()) << std::endl;
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

    TEST_END_CASE("HistoInput 1D Histogram Reading from Jet Variable");
}

void SetUp(const int& N_JETS, std::vector<xAOD::Jet>& jets) {
    std::mt19937 gen( 43294 );
    std::uniform_real_distribution< double > dist( 0, 1000 );
    for(int i=0; i < N_JETS; i++) {
        auto jet = xAOD::Jet{dist(gen), dist(gen), dist(gen), dist(gen)};
        jets.push_back( jet );
    }
}

int main() {
    TEST_BEGIN("InputVariable Unit Test");

    std::vector<xAOD::Jet> jets;
    SetUp(1000, jets);
    test1DHistogramReadingOn(jets);
    test2DHistogramReadingOn(jets);

    TEST_END("InputVariable Unit Test");
    return 0;
}