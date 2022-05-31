/**
 * @file Mock.h
 * @brief Contains all mock implementations of  Athena 
 * required to run JetToolHelpers.
 */
#include <string>

#ifndef XAOD_JET_H
#define XAOD_JET_H

typedef bool StatusCode;

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