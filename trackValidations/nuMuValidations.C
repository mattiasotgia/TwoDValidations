/* trackValidations MACRO 
 * Author: Mattia Sotgia (mattia.sotgia@ge.infn.it)
 * Date: December 1, 2025
*/

#pragma once

#include "track.h"
#include "analysis.h"

void nuMuValidations ()     
{
    std::map<track::particle_t, std::unique_ptr<analysis::Analysis<ana::SpillMultiVar>>> analyses;
    analyses.emplace(MUON,   std::make_unique<analysis::Analysis<ana::SpillMultiVar>>("Muons")   );
    // analyses.emplace(PION,   std::make_unique<analysis::Analysis<ana::SpillMultiVar>>("Pions")   );
    // analyses.emplace(PROTON, std::make_unique<analysis::Analysis<ana::SpillMultiVar>>("Protons") );

    std::unique_ptr<TFile> writer(new TFile("writerTwoDValidations.root", "RECREATE"));

    for (auto const& [particle, singleAnalysis]: analyses)
    {
        // doubles first-level
        singleAnalysis->AddVariable("purity",                 track::get<double>("purity",                   particle));
        singleAnalysis->AddVariable("completeness",           track::get<double>("completeness",             particle));
        singleAnalysis->AddVariable("energyCompleteness",     track::get<double>("energyCompleteness",       particle));
        singleAnalysis->AddVariable("recoLength",             track::get<double>("recoLength",               particle));
        singleAnalysis->AddVariable("trueLength",             track::get<double>("trueLength",               particle));
        singleAnalysis->AddVariable("ratioLength",            track::get<double>("ratioLength",              particle));
        singleAnalysis->AddVariable("pullLength",             track::get<double>("pullLength",               particle));
        singleAnalysis->AddVariable("recoTrueDiffEndpoint3d", track::get<double>("recoTrueDiffEndpoint3d",   particle));
        singleAnalysis->AddVariable("trueVisEnergy",          track::get<double>("trueVisEnergy",            particle));

        // points (second-level)
        singleAnalysis->AddVariable("trueEndpoint.x",         track::get<double>("trueEndpoint.x",           particle));
        singleAnalysis->AddVariable("trueEndpoint.y",         track::get<double>("trueEndpoint.y",           particle));
        singleAnalysis->AddVariable("trueEndpoint.z",         track::get<double>("trueEndpoint.z",           particle));
        singleAnalysis->AddVariable("recoEndpoint.x",         track::get<double>("recoEndpoint.x",           particle));
        singleAnalysis->AddVariable("recoEndpoint.y",         track::get<double>("recoEndpoint.y",           particle));
        singleAnalysis->AddVariable("recoEndpoint.z",         track::get<double>("recoEndpoint.z",           particle));
        singleAnalysis->AddVariable("recoTrueDiffEndpoint.x", track::get<double>("recoTrueDiffEndpoint.x",   particle));
        singleAnalysis->AddVariable("recoTrueDiffEndpoint.y", track::get<double>("recoTrueDiffEndpoint.y",   particle));
        singleAnalysis->AddVariable("recoTrueDiffEndpoint.z", track::get<double>("recoTrueDiffEndpoint.z",   particle));

        // calo (second-level)
        singleAnalysis->AddVariable("calo.muonChiSquare",    track::get<double>("calo.muonChiSquare",        particle));
        singleAnalysis->AddVariable("calo.protonChiSquare",  track::get<double>("calo.protonChiSquare",      particle));
        singleAnalysis->AddVariable("calo.trackScore",       track::get<double>("calo.trackScore",           particle));

        // longs first-level
        singleAnalysis->AddVariable("nMatches",              track::get<double>("nMatches",                  particle));
        singleAnalysis->AddVariable("nHitsTrue",             track::get<double>("nHitsTrue",                 particle));
        singleAnalysis->AddVariable("nHitsReco",             track::get<double>("nHitsReco",                 particle));

        singleAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_1d_caf_numu", "configA");
        singleAnalysis->AddDataset("msotgia_v10_06_00_07_BNB_2d_caf_numu", "configB");
        singleAnalysis->RunOnly({"configA", "configB"});

        // singleAnalysis->AddDataset("/exp/icarus/app/users/msotgia/analysis/twoDRecoStudies/tmp/testCAFBacktrackerNew/stage0/1d_numu_baseline_redo.flat.caf.root",   "testA");
        // singleAnalysis->AddDataset("/exp/icarus/app/users/msotgia/analysis/twoDRecoStudies/tmp/testCAFBacktrackerNew/stage0/2d_numu_nodnn_redo.flat.caf.root",      "testB");
        // singleAnalysis->AddDataset("/exp/icarus/app/users/msotgia/analysis/twoDRecoStudies/tmp/testCAFBacktrackerNew/test_standard/stage1_dnn_yzsim.flat.caf.root", "testC");
        // singleAnalysis->AddDataset("/exp/icarus/app/users/msotgia/analysis/twoDRecoStudies/tmp/testCAFBacktrackerNew/test_standard/stage1_yzsim.flat.caf.root",     "testD");
        // singleAnalysis->RunOnly({"testA", "testB", "testC", "testD"});

        singleAnalysis->Go();
        singleAnalysis->WriteTo(writer.get());
    }
}

