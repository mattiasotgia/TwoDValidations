
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

#include "files.h"
#include "analysis.h"


void runner (data::Events whichEvents, data::hitLabels labels, data::writerDirectory where, bool = false);

std::string resolveTpc (int tpc, int cryo)
{
    std::string stringCryo{cryo == 0 ? "E" : "W"};
    std::string stringTpc{tpc <= 1 ? "E" : "W"};
    return stringCryo+stringTpc;
}

void analysis ()
{
    std::unique_ptr<TFile> writer(new TFile("writeBeamData.root", "RECREATE"));
    
    data::eventMap run1d = {
        {"with_yz_1d", data::with_yz_1d},
        {"without_yz_1d", data::without_yz_1d},
    };

    data::eventMap run2d = {
        {"with_yz_wcdnn", data::with_yz_wcdnn},
        // {"without_yz_wcdnn", data::without_yz_wcdnn},
        {"with_yz_wcdnn_pt10", data::with_yz_wcdnn_pt10},
        // {"without_yz_wcdnn_pt10", data::without_yz_wcdnn_pt10}
    };

    data::eventMap run2dOld = {
        {"without_yz_wcdnn", data::without_yz_wcdnn},
        {"without_yz_wcdnn_pt10", data::without_yz_wcdnn_pt10}
    };


    for (auto const [key, events]: run1d)
    {
        auto where = writer->mkdir(key.c_str());
        runner(events, {"cluster3DCryoE", "cluster3DCryoW"}, where);
    }

    for (auto const [key, events]: run2d)
    {
        auto where = writer->mkdir(key.c_str());
        runner(events, {"combineHitsCryoE", "combineHitsCryoW"}, where);
    }

    for (auto const [key, events]: run2dOld)
    {
        auto where = writer->mkdir(key.c_str());
        runner(events, {"cluster3DCryoE", "cluster3DCryoW"}, where);
    }
}

void runner (data::Events whichEvents, data::hitLabels labels, data::writerDirectory where, bool debug) 
{

    std::cout << "##################################################" << std::endl;
    std::cout << "## Processing runner() with options" << std::endl;
    std::cout << "##  data::Events whichEvents hase size " << whichEvents.size() << std::endl;
    std::cout << "##  data::hitLabels labels = {" << std::flush;
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
    // std::cout << "##  data::writerFileName name = " << name << std::endl;
    std::cout << "##################################################" << std::endl;

    ana::tpcHist EE("EE");
    ana::tpcHist EW("EW");
    ana::tpcHist WE("WE");
    ana::tpcHist WW("WW");
    
    for (gallery::Event event(whichEvents); !event.atEnd(); event.next())
    {

        for (auto const label: labels)
        {
            auto &hits = 
                *event.getValidHandle<std::vector<recob::Hit>>(label);
            
            for (auto const& hit: hits)
            {
                try
                {
                    if (debug)
                    {
                        std::cout << "This hit has WireID: " << hit.WireID().deepestIndex()
                                  << ", PlaneID: " << hit.WireID().asPlaneID().deepestIndex()
                                  << ", TPCID: " << hit.WireID().asTPCID().deepestIndex()
                                  << ", CryostatID: " << hit.WireID().asCryostatID().deepestIndex()
                                  << ", resolves in " << resolveTpc(
                                    hit.WireID().asTPCID().deepestIndex(), 
                                    hit.WireID().asCryostatID().deepestIndex()
                                ) << std::endl;
                    }

                    if (hit.WireID().asCryostatID().deepestIndex() == 0)
                    {
                        // East cryostat
                        if (hit.WireID().asTPCID().deepestIndex() <= 1)
                        {
                            EE.fill(hit);
                        }
                        else
                        {
                            EW.fill(hit);
                        }
                    }
                    else
                    {
                        // West cryostat
                        if (hit.WireID().asTPCID().deepestIndex() <= 1)
                        {
                            WE.fill(hit);
                        }
                        else
                        {
                            WW.fill(hit);
                        }
                    }
                }
                catch (std::exception& e)
                {
                    std::cout << "Error in filling the hit histogram: " << e.what() << std::endl;
                    continue;
                }
            }
        }
        
    }

    where->cd();
    EE.writeTo(where);
    EW.writeTo(where);
    WE.writeTo(where);
    WW.writeTo(where);


    return;
}
