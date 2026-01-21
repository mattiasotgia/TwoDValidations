
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

void analysis () 
{
    std::unique_ptr<TFile> writer(new TFile("yz_simulation_debug_muondata_addedEnergyHist.root", "RECREATE"));
    
    for (int scPass=0; scPass<360; scPass++)
    {
        
        std::string simChannelLabel{Form("daq:simpleSC%d", scPass)};
        std::cout << "Looking at SimChannel " << scPass 
                  << " with label " << simChannelLabel << std::endl;

        ana::YZHist simChannelHist(simChannelLabel);

        for (gallery::Event event(data::fileMuonHiE); !event.atEnd(); event.next())
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
