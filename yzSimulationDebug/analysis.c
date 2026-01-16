
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

#define PRESAMPLING 846
#define TICKS 4096
#define WIRES  53248

void analysis () 
{
    std::unique_ptr<TFile> writer(new TFile("yzSimulationDebugWriterBeamData.root", "RECREATE"));
    
    for (int scPass=0; scPass<360; scPass++)
    {
        
        std::string simChannelLabel{Form("daq:simpleSC%d", scPass)};
        std::cout << "Looking at SimChannel " << scPass 
                  << " with label " << simChannelLabel << std::endl;
        
        std::unique_ptr<TH2D> simChannelHist(
            new TH2D(simChannelLabel.c_str(), "", TICKS/16, 0, TICKS, WIRES/256, 0, WIRES));

        for (gallery::Event event(data::filesBeam); !event.atEnd(); event.next())
        {

            auto const& eventAuxiliary = event.eventAuxiliary();
            std::cout << "\tLooking at event " << eventAuxiliary.event() << std::endl;

            auto &simChannel = 
                *event.getValidHandle<std::vector<sim::SimChannel>>(simChannelLabel);

            for (auto const& simWire: simChannel)
            {
                std::cout << "\t\tLooking at wire" << simWire.Channel() << "\r";

                for (int j=0; j<TICKS; j++)
                {
                    if (simWire.Charge(j) > 0)
                    {
                        simChannelHist->Fill(j, simWire.Channel(), simWire.Charge(j));
                    }
                }

            }
            if (simChannel.size() > 0)
            {
                std::cout << std::endl; 
            }

        }

        if (simChannelHist->GetEntries() > 0)
        {
            std::cout << "\t\tSaving simChannelHist with " << simChannelHist->GetEntries() 
                      << " entries" << std::endl;

            writer->WriteObject(simChannelHist.get(), simChannelLabel.c_str());
        }
        else
        {
            std::cout << "\t\tNo entries with label " << simChannelLabel << std::endl;
        }
    }

    writer->Close();

    return;
}
