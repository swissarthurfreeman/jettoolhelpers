
#ifndef JET_HISTOINPUTBASE_H
#define JET_HISTOINPUTBASE_H

#include <memory>
#include "TH1.h"

#include "IInputBase.h"

class HistoInputBase : public IInputBase
{
    public:
        HistoInputBase(const std::string& name, const std::string& fileName, const std::string& histName);
        virtual ~HistoInputBase() {}

        std::string getFileName() const { return m_fileName; }
        std::string getHistName() const { return m_histName; }

    private:
        std::string m_fileName;
        std::string m_histName;

    protected:
        std::unique_ptr<TH1> m_hist;
        bool readHistoFromFile();

        double enforceAxisRange(const TAxis& axis, const double inputValue) const;

        double readFromHisto(const double X, const double Y=0, const double Z=0) const;
    
};

#endif
