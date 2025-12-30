/* muonTestingYZSim MACRO 
 * Author: Mattia Sotgia (mattia.sotgia@ge.infn.it)
 * Date: December 23, 2025
*/

#pragma once

#include "track.h"
#include "analysis.h"

void muonTestingYZSim ()     
{
    std::unique_ptr<TFile> writer(new TFile("writerMuonTestingYZSim.root", "RECREATE"));

    std::map<track::particle_t, std::unique_ptr<analysis::Analysis<ana::SpillMultiVar>>> analyses;
    analyses.emplace(MUON,   std::make_unique<analysis::Analysis<ana::SpillMultiVar>>("Muons")   );
    // analyses.emplace(PION,   std::make_unique<analysis::Analysis<ana::SpillMultiVar>>("Pions")   );
    // analyses.emplace(PROTON, std::make_unique<analysis::Analysis<ana::SpillMultiVar>>("Protons") );

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
        singleAnalysis->AddVariable("trueVisEnergyInduction1",      track::get<double>("trueVisEnergyInduction1",       particle));
        singleAnalysis->AddVariable("trueVisEnergyInduction2",      track::get<double>("trueVisEnergyInduction2",       particle));
        singleAnalysis->AddVariable("trueVisEnergyCollection",      track::get<double>("trueVisEnergyCollection",       particle));

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
        
        singleAnalysis->AddDataset("/exp/icarus/data/users/msotgia/twoDValidations/singleProd/muons1.5GeVOnly/prod_muon_1.5GeV_isotropic_icarus_active_gen_G4_DetSimNoYZ_MCStage0_MCStage1.flat.caf.root",   "testNotYZ");
        singleAnalysis->AddDataset("/exp/icarus/data/users/msotgia/twoDValidations/singleProd/muons1.5GeVOnly/prod_muon_1.5GeV_isotropic_icarus_active_gen_G4_DetSimWithYZ_MCStage0_MCStage1.flat.caf.root",   "testWithYZ");

        singleAnalysis->AddDataset("/exp/icarus/data/users/msotgia/twoDValidations/singleProd/muons1.5GeVOnly/1dDeconv/prod_muon_1.5GeV_isotropic_icarus_active_gen_G4_DetSimNoYZ_MCStage0_MCStage1.flat.caf.root",   "testOneDDeconvNotYZ");
        singleAnalysis->AddDataset("/exp/icarus/data/users/msotgia/twoDValidations/singleProd/muons1.5GeVOnly/1dDeconv/prod_muon_1.5GeV_isotropic_icarus_active_gen_G4_DetSimWithYZ_MCStage0_MCStage1.flat.caf.root",   "testOneDDeconvWithYZ");

        // singleAnalysis->RunOnly({"testNotYZ"});
        singleAnalysis->Go();
        singleAnalysis->WriteTo(writer.get());
    }
}    
