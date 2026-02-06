#pragma once

#include <string>
#include <memory>

#include <TDirectory.h>
#include <TH1D.h>

namespace ana
{
    class hitHist
    {
        public:
        hitHist(const hitHist&) = delete;
        hitHist(std::string plane);
        void fill(const recob::Hit& hit);
        void writeTo(TDirectory* directory);
        void writeTo(TFile* file);


        private:
        std::unique_ptr<TH1D> m_rms, m_multiplicity, m_goodnessOfFit, m_numberOfHits;
        std::string m_name;
    };
}

ana::hitHist::hitHist(std::string name): m_name(name)
{
    m_rms = std::make_unique<TH1D>("rms", "", 50, 0, 15); 
    m_multiplicity = std::make_unique<TH1D>("multiplicity", "", 15, -.5, 14.5);
    m_goodnessOfFit = std::make_unique<TH1D>("goodnessOfFit", "", 35, 0, 35);
    m_numberOfHits = std::make_unique<TH1D>("numberOfHits", "", 100, 0, 400);
}

void ana::hitHist::fill(const recob::Hit& hit)
{
    m_rms->Fill(hit.RMS());
    m_multiplicity->Fill(hit.Multiplicity());
    m_goodnessOfFit->Fill(hit.GoodnessOfFit());
    // m_numberOfHits->Fill(hit->NumberOfHits());
}

void ana::hitHist::writeTo(TDirectory* directory)
{
    auto where = directory->mkdir(m_name.c_str());
    where->cd();
    m_rms->Write();
    m_multiplicity->Write();
    m_goodnessOfFit->Write();
    m_numberOfHits->Write();
}

void ana::hitHist::writeTo(TFile* file)
{
    
    auto directory = file->GetDirectory(m_name.c_str());
    if (!directory)
    {
        file->mkdir(m_name.c_str());
        directory = file->GetDirectory(m_name.c_str());
    }
    directory->cd();
    m_rms->Write();
    m_multiplicity->Write();
    m_goodnessOfFit->Write();
    // m_numberOfHits->Write();
}