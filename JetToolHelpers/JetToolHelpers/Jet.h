#ifndef XAOD_JET_H
#define XAOD_JET_H

// purely a mock class mimicking what there is in the xAOD namespace
// xAOD stuff is part of the atlas software framework. I just wonder
// where's the documentation...
class LocalP4 {
    public:
        LocalP4(double pt, double eta, double phi, double m)
            : m_pt{pt}, m_eta{eta}, m_phi{phi}, m_mass{m} {}
        
        double E() const { return m_pt; } // Not correct, for mocking
        double Et() const { return m_pt; } // Not correct, for mocking
        double Rapidity() const { return m_eta; } // Not correct, for mocking

    private:
        double m_pt;
        double m_eta;
        double m_phi;
        double m_mass;
};

namespace xAOD {
    class Jet {
        public:
            Jet(double pt, double eta, double phi, double m);

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