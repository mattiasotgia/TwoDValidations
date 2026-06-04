/* trackValidations MACRO 
 * Author: Mattia Sotgia (mattia.sotgia@ge.infn.it)
 * Date: May 4, 2026
*/

#pragma once

#include "track.h"
#include "slice.h"
#include "analysis.h"

void nuMuValidationHitFinderStudies ()     
{
    std::unique_ptr<TFile> writer(new TFile("writerTwoDValidationsHitFinderStudies.root", "RECREATE"));

    std::map<track::particle_t, std::unique_ptr<analysis::Analysis<ana::SpillMultiVar>>> analyses;
    analyses.emplace(MUON,   std::make_unique<analysis::Analysis<ana::SpillMultiVar>>("Muons")   );
    analyses.emplace(PION,   std::make_unique<analysis::Analysis<ana::SpillMultiVar>>("Pions")   );
    analyses.emplace(PROTON, std::make_unique<analysis::Analysis<ana::SpillMultiVar>>("Protons") );

    for (auto const& [particle, singleAnalysis]: analyses)
    {
        // doubles first-level
        singleAnalysis->AddVariable("purity",                       track::get<double>("purity",                        particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("completeness",                 track::get<double>("completeness",                  particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("purityBestmatch",              track::get<double>("purityBestmatch",               particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("completenessBestmatch",        track::get<double>("completenessBestmatch",         particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("energyCompleteness",           track::get<double>("energyCompleteness",            particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("energyCompletenessAllMatches", track::get<double>("energyCompletenessAllMatches",  particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("energyCompletenessDefault",    track::get<double>("energyCompletenessDefault",     particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("energyMatch",                  track::get<double>("energyMatch",                   particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("energyAllMatches",             track::get<double>("energyAllMatches",              particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("trueEnergy",                   track::get<double>("trueEnergy",                    particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("recoLength",                   track::get<double>("recoLength",                    particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("trueLength",                   track::get<double>("trueLength",                    particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("ratioLength",                  track::get<double>("ratioLength",                   particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("pullLength",                   track::get<double>("pullLength",                    particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("recoTrueDiffEndpoint3d",       track::get<double>("recoTrueDiffEndpoint3d",        particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("trueVisEnergyCollection",      track::get<double>("trueVisEnergyCollection",       particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("trueVisEnergyInduction1",      track::get<double>("trueVisEnergyInduction1",       particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("trueVisEnergyInduction2",      track::get<double>("trueVisEnergyInduction2",       particle, track::contained_t::true_p));

        // points (second-level)
        singleAnalysis->AddVariable("trueEndpoint.x",               track::get<double>("trueEndpoint.x",                particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("trueEndpoint.y",               track::get<double>("trueEndpoint.y",                particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("trueEndpoint.z",               track::get<double>("trueEndpoint.z",                particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("recoEndpoint.x",               track::get<double>("recoEndpoint.x",                particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("recoEndpoint.y",               track::get<double>("recoEndpoint.y",                particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("recoEndpoint.z",               track::get<double>("recoEndpoint.z",                particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("recoTrueDiffEndpoint.x",       track::get<double>("recoTrueDiffEndpoint.x",        particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("recoTrueDiffEndpoint.y",       track::get<double>("recoTrueDiffEndpoint.y",        particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("recoTrueDiffEndpoint.z",       track::get<double>("recoTrueDiffEndpoint.z",        particle, track::contained_t::true_p));

        // calo (second-level)
        singleAnalysis->AddVariable("calo.muonChiSquare",           track::get<double>("calo.muonChiSquare",            particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("calo.protonChiSquare",         track::get<double>("calo.protonChiSquare",          particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("calo.trackScore",              track::get<double>("calo.trackScore",               particle, track::contained_t::true_p));

        // longs first-level
        singleAnalysis->AddVariable("nMatches",                     track::get<double>("nMatches",                      particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("nBestMatches",                 track::get<double>("nBestMatches",                  particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("nHitsTrue",                    track::get<double>("nHitsTrue",                     particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("nHitsReco",                    track::get<double>("nHitsReco",                     particle, track::contained_t::true_p));

        singleAnalysis->AddVariable("recoCosThetaX",                track::get<double>("recoCosThetaX",                 particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("recoCosThetaY",                track::get<double>("recoCosThetaY",                 particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("recoCosThetaZ",                track::get<double>("recoCosThetaZ",                 particle, track::contained_t::true_p));

        singleAnalysis->AddVariable("minHitWidth",                  track::get<double>("minHitWidth",                   particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("meanHitWidth",                 track::get<double>("meanHitWidth",                  particle, track::contained_t::true_p));
        singleAnalysis->AddVariable("maxHitWidth",                  track::get<double>("maxHitWidth",                   particle, track::contained_t::true_p));

        singleAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_flatcaf", "nominalParameters");
        singleAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_spine_flatcaf", "spineParameters");
        singleAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1uNp_energyOnly_respun_caf", "1uNp");
        singleAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1eNpMpi0_energyOnly_respun_caf", "1eNpMpi0");
        singleAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1eNp0pi0_energyOnly_respun_caf", "1eNp0pi0");

        singleAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1uNp_energyOnly_respun2_caf", "1uNp_2");
        singleAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1eNp0pi0_energyOnly_respun2_caf", "1eNpMpi0_2");
        singleAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1eNpMpi0_energyOnly_respun2_caf", "1eNp0pi0_2");

        singleAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1uNp_energyOnly_noPulses_caf", "1uNp_noPulse");
        singleAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1eNp0pi0_energyOnly_noPulses_caf", "1eNpMpi0_noPulse");
        singleAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1eNpMpi0_energyOnly_noPulses_caf", "1eNp0pi0_noPulse");

        // singleAnalysis->AddDataset("/exp/icarus/data/users/msotgia/hitTuning/caf/respunWithHitWidth/msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1uNp_energyOnly_noPulses_v10_06_00_06p03_stage1_91299944_0-05e696de-cb1f-40b4-893c-45401ddb44d8.flat.caf.root", "2DHits");

        singleAnalysis->AddDataset("/exp/icarus/data/users/msotgia/hitTuning/caf/respunWithHitWidth/fullProd/msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1uNp_energyOnly_noPulses_v10_06_00_06p03_stage1_91299944_0-05e696de-cb1f-40b4-893c-45401ddb44d8.flat.caf.root", 
            "2DHits1uNp_noPulse");

        singleAnalysis->Go();
        singleAnalysis->WriteTo(writer.get());
    }

    std::unique_ptr<analysis::Analysis<ana::Var>> sliceAnalysis(new analysis::Analysis<ana::Var>("GeneralSlice"));

    sliceAnalysis->AddVariable("nPfpsPerSlice", slices::get("nPfpsPerSlice"));
    sliceAnalysis->AddVariable("trueRecoDiffVertex3d", slices::get("trueRecoDiffVertex3d"));

    sliceAnalysis->AddVariable("recoVertex.x", slices::get("recoVertex.x"));
    sliceAnalysis->AddVariable("recoVertex.y", slices::get("recoVertex.y"));
    sliceAnalysis->AddVariable("recoVertex.z", slices::get("recoVertex.z"));

    sliceAnalysis->AddVariable("trueVertex.x", slices::get("trueVertex.x"));
    sliceAnalysis->AddVariable("trueVertex.y", slices::get("trueVertex.y"));
    sliceAnalysis->AddVariable("trueVertex.z", slices::get("trueVertex.z"));

    sliceAnalysis->AddVariable("trueRecoDiffVertex.x", slices::get("trueRecoDiffVertex.x"));
    sliceAnalysis->AddVariable("trueRecoDiffVertex.y", slices::get("trueRecoDiffVertex.y"));
    sliceAnalysis->AddVariable("trueRecoDiffVertex.z", slices::get("trueRecoDiffVertex.z"));

    sliceAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_flatcaf", "nominalParameters");
    sliceAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_spine_flatcaf", "spineParameters");
    sliceAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1uNp_energyOnly_respun_caf", "1uNp");
    sliceAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1eNpMpi0_energyOnly_respun_caf", "1eNpMpi0");
    sliceAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1eNp0pi0_energyOnly_respun_caf", "1eNp0pi0");

    sliceAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1uNp_energyOnly_respun2_caf", "1uNp_2");
    sliceAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1eNp0pi0_energyOnly_respun2_caf", "1eNpMpi0_2");
    sliceAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1eNpMpi0_energyOnly_respun2_caf", "1eNp0pi0_2");

    sliceAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1uNp_energyOnly_noPulses_caf", "1uNp_noPulse");
    sliceAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1eNp0pi0_energyOnly_noPulses_caf", "1eNpMpi0_noPulse");
    sliceAnalysis->AddDataset("msotgia_v10_06_00_06p03_overlay_run9435_A_bnb_withOverlayFixed_1eNpMpi0_energyOnly_noPulses_caf", "1eNp0pi0_noPulse");

    // sliceAnalysis->Go();
    // sliceAnalysis->WriteTo(writer.get());
}