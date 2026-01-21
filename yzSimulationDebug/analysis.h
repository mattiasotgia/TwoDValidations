
#pragma once

#include <string>
#include <memory>

#include <TH2D.h>

#include "geometry.h"

#define PRESAMPLING 846
#define TICKS 4096
#define WIRES  55295

namespace ana
{
    class YZHist
    {
    public:
        YZHist(std::string simChannel);
        YZHist(const YZHist&) = delete;

        void fill(double __y, double __z, double __charge, double __energy, int __channel);
        void writeTo(TDirectory* directory);

    private:
        std::unique_ptr<TH2D> m_EE_ind1, m_EE_ind2, m_EE_coll;
        std::unique_ptr<TH2D> m_EW_ind1, m_EW_ind2, m_EW_coll;
        std::unique_ptr<TH2D> m_WE_ind1, m_WE_ind2, m_WE_coll;
        std::unique_ptr<TH2D> m_WW_ind1, m_WW_ind2, m_WW_coll;

        std::unique_ptr<TH1D> m_EE_ind1_energy, m_EE_ind2_energy, m_EE_coll_energy;
        std::unique_ptr<TH1D> m_EW_ind1_energy, m_EW_ind2_energy, m_EW_coll_energy;
        std::unique_ptr<TH1D> m_WE_ind1_energy, m_WE_ind2_energy, m_WE_coll_energy;
        std::unique_ptr<TH1D> m_WW_ind1_energy, m_WW_ind2_energy, m_WW_coll_energy;

        std::unique_ptr<TH2D> m_yzChannelReading;



        const geo::Detector m_icarusChannelMap = geo::icarus;
    };
}

ana::YZHist::YZHist(std::string simChannel)
{

    int yBins{105}; // One bin is ~3 cm 
    int zBins{600}; // One bin is ~3 cm 

    double yMin{-181.86}, yMax{134.96}; // TPC size
    double zMin{-894.951}, zMax{894.951};  // TPC size

    double nBins{10}, minEnergy{0}, maxEnergy{0}; // Energy are in MeV

    m_EE_ind1 = std::make_unique<TH2D>("EE_I1", "", zBins, zMin, zMax, yBins, yMin, yMax);
    m_EE_ind2 = std::make_unique<TH2D>("EE_I2", "", zBins, zMin, zMax, yBins, yMin, yMax);
    m_EE_coll = std::make_unique<TH2D>("EE_C",  "", zBins, zMin, zMax, yBins, yMin, yMax);

    m_EW_ind1 = std::make_unique<TH2D>("EW_I1", "", zBins, zMin, zMax, yBins, yMin, yMax);
    m_EW_ind2 = std::make_unique<TH2D>("EW_I2", "", zBins, zMin, zMax, yBins, yMin, yMax);
    m_EW_coll = std::make_unique<TH2D>("EW_C",  "", zBins, zMin, zMax, yBins, yMin, yMax);
    
    m_WE_ind1 = std::make_unique<TH2D>("WE_I1", "", zBins, zMin, zMax, yBins, yMin, yMax);
    m_WE_ind2 = std::make_unique<TH2D>("WE_I2", "", zBins, zMin, zMax, yBins, yMin, yMax);
    m_WE_coll = std::make_unique<TH2D>("WE_C",  "", zBins, zMin, zMax, yBins, yMin, yMax);
    
    m_WW_ind1 = std::make_unique<TH2D>("WW_I1", "", zBins, zMin, zMax, yBins, yMin, yMax);
    m_WW_ind2 = std::make_unique<TH2D>("WW_I2", "", zBins, zMin, zMax, yBins, yMin, yMax);
    m_WW_coll = std::make_unique<TH2D>("WW_C",  "", zBins, zMin, zMax, yBins, yMin, yMax);

    m_EE_ind1_energy = std::make_unique<TH1D>("EE_I1_Energy", "", nBins, minEnergy, maxEnergy);
    m_EE_ind2_energy = std::make_unique<TH1D>("EE_I2_Energy", "", nBins, minEnergy, maxEnergy);
    m_EE_coll_energy = std::make_unique<TH1D>("EE_C_Energy",  "", nBins, minEnergy, maxEnergy);

    m_EW_ind1_energy = std::make_unique<TH1D>("EW_I1_Energy", "", nBins, minEnergy, maxEnergy);
    m_EW_ind2_energy = std::make_unique<TH1D>("EW_I2_Energy", "", nBins, minEnergy, maxEnergy);
    m_EW_coll_energy = std::make_unique<TH1D>("EW_C_Energy",  "", nBins, minEnergy, maxEnergy);
    
    m_WE_ind1_energy = std::make_unique<TH1D>("WE_I1_Energy", "", nBins, minEnergy, maxEnergy);
    m_WE_ind2_energy = std::make_unique<TH1D>("WE_I2_Energy", "", nBins, minEnergy, maxEnergy);
    m_WE_coll_energy = std::make_unique<TH1D>("WE_C_Energy",  "", nBins, minEnergy, maxEnergy);
    
    m_WW_ind1_energy = std::make_unique<TH1D>("WW_I1_Energy", "", nBins, minEnergy, maxEnergy);
    m_WW_ind2_energy = std::make_unique<TH1D>("WW_I2_Energy", "", nBins, minEnergy, maxEnergy);
    m_WW_coll_energy = std::make_unique<TH1D>("WW_C_Energy",  "", nBins, minEnergy, maxEnergy);

    m_yzChannelReading = std::make_unique<TH2D>(
        "yzChannelReading", "", 
        TICKS/64, 0, TICKS,
        WIRES/128, 0, WIRES 
    );
}

void ana::YZHist::fill(double __y, double __z, double __charge, double __energy = 0., int __channel = -999)
{
    if (__channel < 0)
    {
        // Handle the single "giant" map
        m_yzChannelReading->Fill(__y, __z, __charge);
    }
    else if (__channel >= m_icarusChannelMap.EE.I1.minChannel and __channel <= m_icarusChannelMap.EE.I1.maxChannel) 
    {
        // Channels for EE/Ind 1
        m_EE_ind1->Fill(__z, __y, __charge);
        m_EE_ind1_energy->Fill(__energy);
    }
    else if (__channel >= m_icarusChannelMap.EE.I2.minChannel and __channel <= m_icarusChannelMap.EE.I2.maxChannel) 
    {
        // Channels for EE/Ind 2
        m_EE_ind2->Fill(__z, __y, __charge);
        m_EE_ind2_energy->Fill(__energy);
    }
    else if (__channel >= m_icarusChannelMap.EE.C.minChannel  and __channel <= m_icarusChannelMap.EE.C.maxChannel) 
    {
        // Channels for EE/Coll
        m_EE_coll->Fill(__z, __y, __charge);
        m_EE_coll_energy->Fill(__energy);
    }
    else if (__channel >= m_icarusChannelMap.EW.I1.minChannel and __channel <= m_icarusChannelMap.EW.I1.maxChannel) 
    {
        // Channels for EW/Ind 1
        m_EW_ind1->Fill(__z, __y, __charge);
        m_EW_ind1_energy->Fill(__energy);
    }
    else if (__channel >= m_icarusChannelMap.EW.I2.minChannel and __channel <= m_icarusChannelMap.EW.I2.maxChannel) 
    {
        // Channels for EW/Ind 2
        m_EW_ind2->Fill(__z, __y, __charge);
        m_EW_ind2_energy->Fill(__energy);
    }
    else if (__channel >= m_icarusChannelMap.EW.C.minChannel  and __channel <= m_icarusChannelMap.EW.C.maxChannel) 
    {
        // Channels for EW/Coll
        m_EW_coll->Fill(__z, __y, __charge);
        m_EW_coll_energy->Fill(__energy);
    }
    else if (__channel >= m_icarusChannelMap.WE.I1.minChannel and __channel <= m_icarusChannelMap.WE.I1.maxChannel) 
    {
        // Channels for WE/Ind 1
        m_WE_ind1->Fill(__z, __y, __charge);
        m_WE_ind1_energy->Fill(__energy);
    }
    else if (__channel >= m_icarusChannelMap.WE.I2.minChannel and __channel <= m_icarusChannelMap.WE.I2.maxChannel) 
    {
        // Channels for WE/Ind 2
        m_WE_ind2->Fill(__z, __y, __charge);
        m_WE_ind2_energy->Fill(__energy);
    }
    else if (__channel >= m_icarusChannelMap.WE.C.minChannel  and __channel <= m_icarusChannelMap.WE.C.maxChannel) 
    {
        // Channels for WE/Coll
        m_WE_coll->Fill(__z, __y, __charge);
        m_WE_coll_energy->Fill(__energy);
    }
    else if (__channel >= m_icarusChannelMap.WW.I1.minChannel and __channel <= m_icarusChannelMap.WW.I1.maxChannel) 
    {
        // Channels for WW/Ind 1
        m_WW_ind1->Fill(__z, __y, __charge);
        m_WW_ind1_energy->Fill(__energy);
    }
    else if (__channel >= m_icarusChannelMap.WW.I2.minChannel and __channel <= m_icarusChannelMap.WW.I2.maxChannel) 
    {
        // Channels for WW/Ind 2
        m_WW_ind2->Fill(__z, __y, __charge);
        m_WW_ind2_energy->Fill(__energy);
    }
    else if (__channel >= m_icarusChannelMap.WW.C.minChannel  and __channel <= m_icarusChannelMap.WW.C.maxChannel) 
    {
        // Channels for WW/Coll
        m_WW_coll->Fill(__z, __y, __charge);
        m_WW_coll_energy->Fill(__energy);
    }
    else
    {
        std::cout << "This wire is not filled: " << __channel << std::endl;
    }
}

void ana::YZHist::writeTo(TDirectory* directory)
{
    std::vector<TH2D*> toSave = {
        m_EE_ind1.get(), m_EE_ind2.get(), m_EE_coll.get(),
        m_EW_ind1.get(), m_EW_ind2.get(), m_EW_coll.get(),
        m_WE_ind1.get(), m_WE_ind2.get(), m_WE_coll.get(),
        m_WW_ind1.get(), m_WW_ind2.get(), m_WW_coll.get(),
        m_yzChannelReading.get()
    };

    std::vector<TH1D*> toSave1d = {
        m_EE_ind1_energy.get(), m_EE_ind2_energy.get(), m_EE_coll_energy.get(),
        m_EW_ind1_energy.get(), m_EW_ind2_energy.get(), m_EW_coll_energy.get(),
        m_WE_ind1_energy.get(), m_WE_ind2_energy.get(), m_WE_coll_energy.get(),
        m_WW_ind1_energy.get(), m_WW_ind2_energy.get(), m_WW_coll_energy.get()
    };

    for (auto const& hist: toSave)
    {
        if (hist->GetEntries() > 0)
        {
            directory->cd();
            hist->Write();
        }
        else 
        {
            std::cout << "No entries in " << hist->GetName() 
                      << ", not saving data to file" << std::endl;
        }
    }


    for (auto const& hist: toSave1d)
    {
        if (hist->GetEntries() > 0)
        {
            directory->cd();
            hist->Write();
        }
        else 
        {
            std::cout << "No entries in " << hist->GetName() 
                      << ", not saving data to file" << std::endl;
        }
    }
}