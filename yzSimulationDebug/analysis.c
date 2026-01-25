
/*
 * Analysis of the SimChannel maps
 * Author: M Sotgia <msotgia(at)cern(dot)ch>
 * Date: Jan 14th, 2026
 */
#pragma once

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"
#include "gallery/ValidHandle.h"
#include "gallery/Handle.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindOne.h"
#include "canvas/Persistency/Common/FindManyP.h"
#include "canvas/Persistency/Common/fwd.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Persistency/Provenance/Timestamp.h"

#include "lardataobj/Simulation/SimChannel.h"

#include "files.h"
#include "analysis.h"


void runner (data::Events, data::simChannelLabels, data::writerFileName, ana::Binning bins);

void analysis ()
{
    data::simChannelLabels simChnlPasses;

    for (int i=0; i<360; i++)
    {
        simChnlPasses.push_back(Form("daq:simpleSC%d", i));
    }

    runner (data::fileMuonHiENoYZProdV,   {"daq:simpleSC"}, 
        "plots_muondata_v10_06_00_04p04_production/yz_simulation_debug_beamdata_addedEnergyHist_no_yz_productionVersion.root", ana::Binning(250, 0, 2.5));
    runner (data::fileMuonHiEWithYZProdV, {"merge"}, 
        "plots_muondata_v10_06_00_04p04_production/yz_simulation_debug_beamdata_addedEnergyHist_with_yz_productionVersion.root", ana::Binning(250, 0, 2.5));
    
    runner (data::fileMuonHiE, simChnlPasses, 
        "plots_muondata_v10_07_xx_develop/yz_simulation_debug_muondata_addedEnergyHist.root", ana::Binning(2000, 0, 20));
    runner (data::fileMuonHiE, {"merge"}, 
        "plots_muondata_v10_07_xx_develop/yz_simulation_debug_muondata_addedEnergyHist_merge.root", ana::Binning(2000, 0, 20));
    runner (data::fileMuonHiENoYZ, {"daq:simpleSC"}, 
        "plots_muondata_v10_07_xx_develop/yz_simulation_debug_muondata_addedEnergyHist_noYZ.root", ana::Binning(250, 0, 2.5));
}

void runner (data::Events whichEvents, data::simChannelLabels labels, data::writerFileName name, ana::Binning bins) 
{

    std::cout << "##################################################" << std::endl;
    std::cout << "## Processing runner() with options" << std::endl;
    std::cout << "##  data::Events whichEvents hase size " << whichEvents.size() << std::endl;
    std::cout << "##  data::simChannelLabels labels = {" << std::flush;
    for (int d = 0; d < std::min(2, (int)labels.size()); d++) 
    {
        std::cout << labels[d] << ", " << std::flush;
    }
    if (labels.size() > 1)
    {
        std::cout << "... }" << std::endl;
    }
    else
    {
        std::cout << "}" << std::endl; 
    }
    std::cout << "##  data::writerFileName name = " << name << std::endl;
    std::cout << "##################################################" << std::endl;



    std::unique_ptr<TFile> writer(new TFile(name.c_str(), "RECREATE"));
    
    for (auto const& simChannelLabel: labels)
    {
    
        std::cout << "Looking at SimChannel with label "
                  << simChannelLabel << std::endl;

        ana::YZHist simChannelHist(simChannelLabel, bins);

        for (gallery::Event event(whichEvents); !event.atEnd(); event.next())
        {
            auto const& eventAuxiliary = event.eventAuxiliary();
            std::cout << "\tLooking at event " << eventAuxiliary.event() << "\r" << std::flush;

            auto &simChannel = 
                *event.getValidHandle<std::vector<sim::SimChannel>>(simChannelLabel);

            for (auto const& simWire: simChannel)
            {   
                for (int j=0; j<TICKS; j++)
                {
                    if (simWire.Charge(j) > 0)
                    {
                        simChannelHist.fill(j, simWire.Channel(), simWire.Charge(j));
                    }

                    auto const& tdcIdeMap = simWire.TDCIDEMap();
                    for (auto const& [tdc, ides]: tdcIdeMap)
                    {
                        for (auto const& ide: ides)
                        {
                            simChannelHist.fill(ide.y, ide.z, ide.numElectrons, ide.energy, simWire.Channel());
                        }
                    }
                }
            }
        }
        auto const directory = writer->mkdir(simChannelLabel.c_str());
        simChannelHist.writeTo(directory);
    }

    writer->Close();

    return;
}
