/**
 * @file Mock.h
 * @brief Contains all mock implementations of ROOT and 
 * Athena required to run JetToolHelpers.
 */
#include <string>

#ifndef XAOD_JET_H
#define XAOD_JET_H

class LocalP4 {
    public:
        LocalP4(double pt, double eta, double phi, double m)
            : m_pt{pt}, m_eta{eta}, m_phi{phi}, m_mass{m} {}
        
        double E() const { return m_pt; } 
        double Et() const { return m_pt; } 
        double Rapidity() const { return m_eta; }

    private:
        double m_pt;
        double m_eta;
        double m_phi;
        double m_mass;
};


namespace xAOD {
    class Jet {
        public:
            Jet(double pt, double eta, double phi, double m): m_pt{pt}, m_eta{eta}, m_phi{phi}, m_mass{m} {};

            double pt()  const { return m_pt;   }
            double eta() const { return m_eta;  }
            double phi() const { return m_phi;  }
            double m()   const { return m_mass; }
            double e()   const { return p4().E(); }
            double rapidity() const { return p4().Rapidity(); }
            LocalP4 p4() const { return LocalP4(m_pt,m_eta,m_phi,m_mass); }
        // a jet depends on 4 properties : it's a 4D vector
        private:
            double m_pt;
            double m_eta;
            double m_phi;
            double m_mass;
    };
}

#endif

#ifndef ROOT_TH1
#define ROOT_TH1

class TAxis {
    public:
        int GetNbins() const { return 10; }
        int GetBinWidth(double i) const { return i; }
        int GetBinLowEdge(int i) const { return i; }
        int GetBinWidth(int i) const { return i; }
        int FindFixBin(int i) const { return i; }
};

class TObject {
    public:
        TObject() {};
        virtual void doNothing() {}
};

class TH1: public TObject {
    public:
        TH1(TObject*): TObject() {}
        double Interpolate(double X) const { return 0; }
        double Interpolate(double X, double Y) const { return 0; }
        double Interpolate(double X, double Y, double Z) const { return 0; }
        void SetDirectory(double i) {}
        int GetDimension() const { return 2; }
        virtual void doNothing() {}
        TObject * get(const char * str) { return this; }
};

class TFile {
    public:
        TFile(std::string, std::string) {}
        bool IsZombie() { return false; }
        void Close() {}
        TObject * Get(const char * str) { return new TH1(new TObject); }
};

#endif