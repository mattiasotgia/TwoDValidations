
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


void runner (data::Events whichEvents, data::hitLabels labels, data::writerDirectory where);

void analysis ()
{
    std::unique_ptr<TFile> writer(new TFile("write.root", "RECREATE"));
    data::eventMap run1d = {
        {"with_yz_1d", data::with_yz_1d},
        {"without_yz_1d", data::without_yz_1d},
    };

    data::eventMap run2d = {
        {"with_yz_wcdnn", data::with_yz_wcdnn},
        {"without_yz_wcdnn", data::without_yz_wcdnn},
        {"with_yz_wcdnn_pt10", data::with_yz_wcdnn_pt10},
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
}

void runner (data::Events whichEvents, data::hitLabels labels, data::writerDirectory where) 
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

    ana::hitHist ind1("ind1");
    ana::hitHist ind2("ind2");
    ana::hitHist coll("coll");
    
    for (gallery::Event event(whichEvents); !event.atEnd(); event.next())
    {

        // const char* eventWithNo{Form("event_%d", event.eventAuxiliary().event())};
        // auto directory = where->mkdir(eventWithNo);


        for (auto const label: labels)
        {
            auto &hits = 
                *event.getValidHandle<std::vector<recob::Hit>>(label);
            
            for (auto const& hit: hits)
            {
                if (hit.WireID().planeID().deepestIndex() == 0)
                {
                    ind1.fill(hit);
                }
                if (hit.WireID().planeID().deepestIndex() == 1)
                {
                    ind2.fill(hit);
                }
                if (hit.WireID().planeID().deepestIndex() == 2)
                {
                    coll.fill(hit);
                }
            }
        }
        
    }

    where->cd();
    ind1.writeTo(where);
    ind2.writeTo(where);
    coll.writeTo(where);

    return;
}
