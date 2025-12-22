/* trackValidations MACRO 
 * Author: Mattia Sotgia (mattia.sotgia@ge.infn.it)
 * Date: December 1, 2025
*/

#pragma once

#include "track.h"
#include "slice.h"
#include "analysis.h"

void nuMuValidations ()     
{
    std::unique_ptr<TFile> writer(new TFile("writerTwoDValidationsRedoCorrectBestmatch.root", "RECREATE"));

    std::map<track::particle_t, std::unique_ptr<analysis::Analysis<ana::SpillMultiVar>>> analyses;
    analyses.emplace(MUON,   std::make_unique<analysis::Analysis<ana::SpillMultiVar>>("Muons")   );
    analyses.emplace(PION,   std::make_unique<analysis::Analysis<ana::SpillMultiVar>>("Pions")   );
    analyses.emplace(PROTON, std::make_unique<analysis::Analysis<ana::SpillMultiVar>>("Protons") );

    for (auto const& [particle, singleAnalysis]: analyses)
    {
        // doubles first-level
        singleAnalysis->AddVariable("purity",                       track::get<double>("purity",                        particle));
        singleAnalysis->AddVariable("completeness",                 track::get<double>("completeness",                  particle));
        singleAnalysis->AddVariable("purityBestmatch",              track::get<double>("purityBestmatch",               particle));
        singleAnalysis->AddVariable("completenessBestmatch",        track::get<double>("completenessBestmatch",         particle));
        singleAnalysis->AddVariable("energyCompleteness",           track::get<double>("energyCompleteness",            particle));
        singleAnalysis->AddVariable("energyCompletenessAllMatches", track::get<double>("energyCompletenessAllMatches",  particle));
        singleAnalysis->AddVariable("energyMatch",                  track::get<double>("energyMatch",                   particle));
        singleAnalysis->AddVariable("energyAllMatches",             track::get<double>("energyAllMatches",              particle));
        singleAnalysis->AddVariable("trueEnergy",                   track::get<double>("trueEnergy",                    particle));
        singleAnalysis->AddVariable("recoLength",                   track::get<double>("recoLength",                    particle));
        singleAnalysis->AddVariable("trueLength",                   track::get<double>("trueLength",                    particle));
        singleAnalysis->AddVariable("ratioLength",                  track::get<double>("ratioLength",                   particle));
        singleAnalysis->AddVariable("pullLength",                   track::get<double>("pullLength",                    particle));
        singleAnalysis->AddVariable("recoTrueDiffEndpoint3d",       track::get<double>("recoTrueDiffEndpoint3d",        particle));
        singleAnalysis->AddVariable("trueVisEnergy",                track::get<double>("trueVisEnergy",                 particle));

        // points (second-level)
        singleAnalysis->AddVariable("trueEndpoint.x",               track::get<double>("trueEndpoint.x",                particle));
        singleAnalysis->AddVariable("trueEndpoint.y",               track::get<double>("trueEndpoint.y",                particle));
        singleAnalysis->AddVariable("trueEndpoint.z",               track::get<double>("trueEndpoint.z",                particle));
        singleAnalysis->AddVariable("recoEndpoint.x",               track::get<double>("recoEndpoint.x",                particle));
        singleAnalysis->AddVariable("recoEndpoint.y",               track::get<double>("recoEndpoint.y",                particle));
        singleAnalysis->AddVariable("recoEndpoint.z",               track::get<double>("recoEndpoint.z",                particle));
        singleAnalysis->AddVariable("recoTrueDiffEndpoint.x",       track::get<double>("recoTrueDiffEndpoint.x",        particle));
        singleAnalysis->AddVariable("recoTrueDiffEndpoint.y",       track::get<double>("recoTrueDiffEndpoint.y",        particle));
        singleAnalysis->AddVariable("recoTrueDiffEndpoint.z",       track::get<double>("recoTrueDiffEndpoint.z",        particle));

        // calo (second-level)
        singleAnalysis->AddVariable("calo.muonChiSquare",           track::get<double>("calo.muonChiSquare",            particle));
        singleAnalysis->AddVariable("calo.protonChiSquare",         track::get<double>("calo.protonChiSquare",          particle));
        singleAnalysis->AddVariable("calo.trackScore",              track::get<double>("calo.trackScore",               particle));

        // longs first-level
        singleAnalysis->AddVariable("nMatches",                     track::get<double>("nMatches",                      particle));
        singleAnalysis->AddVariable("nBestMatches",                 track::get<double>("nBestMatches",                  particle));
        singleAnalysis->AddVariable("nHitsTrue",                    track::get<double>("nHitsTrue",                     particle));
        singleAnalysis->AddVariable("nHitsReco",                    track::get<double>("nHitsReco",                     particle));

        singleAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_1d_respun2_caf_numu",              "configA");
        singleAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_2d_respun2_caf_numu",              "configB");
        singleAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_stage1_noPulseTrains_numu_caf",    "configC");
        singleAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_stage1_pulseTrains_numu_caf",      "configD");

        singleAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_numu_caf",                             "configAWithYZSim");
        singleAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_yzsim_wc_numu_caf",                    "configBWithYZSim");
        singleAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_yzsim_wcdnn_noPulseTrains_numu_caf",   "configCWithYZSim");
        singleAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_yzsim_wcdnn_pulseTrains_numu_caf",     "configDWithYZSim");
        
        singleAnalysis->AddDataset("/exp/icarus/app/users/msotgia/analysis/twoDRecoStudies/tmp/testCAFBacktrackerNew/stage0/1d_numu_baseline_redo.flat.caf.root",   "testA");
        singleAnalysis->AddDataset("/exp/icarus/app/users/msotgia/analysis/twoDRecoStudies/tmp/testCAFBacktrackerNew/stage0/2d_numu_nodnn_redo.flat.caf.root",      "testB");
        singleAnalysis->AddDataset("/exp/icarus/app/users/msotgia/analysis/twoDRecoStudies/tmp/testCAFBacktrackerNew/test_standard/stage1_dnn_yzsim.flat.caf.root", "testC");
        singleAnalysis->AddDataset("/exp/icarus/app/users/msotgia/analysis/twoDRecoStudies/tmp/testCAFBacktrackerNew/test_standard/stage1_yzsim.flat.caf.root",     "testD");

        // singleAnalysis->RunOnly({"configA"});
        singleAnalysis->RunOnly({"configA", "configB", "configC", "configD", "configAWithYZSim", "configBWithYZSim", "configCWithYZSim", "configDWithYZSim"});
        // singleAnalysis->RunOnly({"testA", "testB", "testC", "testD"});

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

    sliceAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_1d_respun2_caf_numu", "configA");
    sliceAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_2d_respun2_caf_numu", "configB");
    sliceAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_stage1_noPulseTrains_numu_caf", "configC");
    sliceAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_stage1_pulseTrains_numu_caf", "configD");

    sliceAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_numu_caf",                             "configAWithYZSim");
    sliceAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_yzsim_wc_numu_caf",                    "configBWithYZSim");
    sliceAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_yzsim_wcdnn_noPulseTrains_numu_caf",   "configCWithYZSim");
    sliceAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_yzsim_wcdnn_pulseTrains_numu_caf",     "configDWithYZSim");

    // sliceAnalysis->RunOnly({"configA", "configB"});

    sliceAnalysis->Go();
    sliceAnalysis->WriteTo(writer.get());
}    