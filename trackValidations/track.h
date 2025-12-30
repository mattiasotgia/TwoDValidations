#ifndef TRACK_H
#define TRACK_H

#include "sbnana/CAFAna/Core/SpectrumLoader.h"
#include "sbnana/CAFAna/Core/Spectrum.h"
#include "sbnana/CAFAna/Core/Binning.h"
#include "sbnana/CAFAna/Core/Cut.h"
#include "sbnana/CAFAna/Core/Utilities.h"
#include "sbnana/CAFAna/Core/LoadFromFile.h"
#include "sbnana/CAFAna/Core/Var.h"

#include "sbnanaobj/StandardRecord/Proxy/SRProxy.h"
#include "sbnanaobj/StandardRecord/StandardRecord.h"

// LOCAL
#include "base.h"
#include "chiSquare.h"

#define ELECTRON 11
#define MUON 13
#define PION 211
#define PROTON 2212

namespace track 
{
    using particle_t = const int;
    using PfpPtr = const caf::Proxy<caf::SRPFP> *;
    using SlicePtr = const caf::Proxy<caf::SRSlice> *;
    using MatchPtr = const caf::Proxy<caf::SRParticleMatch> *;

    enum contained_t {
        true_p,
        reconstructed_p,
        none_p
    };

    void fillSomeVars(long& nMatches, long& nBestMatches, double& allEnergyInMatches, SlicePtr slice, const int trueG4ID, const int trueCryostat)
    {
        for (auto const& pfp: slice->reco.pfp)
        {
            // nMatches += std::count_if(
            //     pfp.trk.truth.matches.begin(),
            //     pfp.trk.truth.matches.end(),
            //     [&] (auto const& match) { return match.G4ID == trueG4ID; }
            // );

            const int pfpCryostat{pfp.trk.start.x < 0 ? 0 : 1};

            for (auto const& match: pfp.trk.truth.matches)
            {
                if (match.G4ID == trueG4ID and pfpCryostat == trueCryostat) 
                {
                    nMatches++;
                    allEnergyInMatches += match.energy / 3.;
                }
            }

            if (pfp.trk.truth.bestmatch.G4ID == trueG4ID and pfpCryostat == trueCryostat) nBestMatches++;
        }
    }

    template<class T>
    const ana::SpillMultiVar get(std::string what, particle_t particle, contained_t contained = contained_t::none_p)
    {
        return ana::SpillMultiVar ([=](const caf::SRSpillProxy* spill) -> std::vector<T> 
        {
            base::trackData td;

            for (auto const& true_particle: spill->true_particles)
            {
                bool isParticle{particle == std::abs(true_particle.pdg)};

                if (not isParticle) continue;
                if (not true_particle.contained and contained == contained_t::true_p) continue;
                if (true_particle.cryostat == -1) continue;

                PfpPtr selectedPfp = nullptr;
                MatchPtr selectedParticleMatch = nullptr;

                const int trueG4ID{true_particle.G4ID};
                const int trueCryostat{true_particle.cryostat};
                long nMatches{0}, nBestMatches{0};
                
                double maxMatchEnergy{-std::numeric_limits<double>::max()};
                double allEnergyInMatches{0.};

                // std::cerr << "I'm here... G4ID = " << trueG4ID 
                //           << " and there are " << spill->nslc << " slices in this reco" << std::endl;

                for (auto const& slice: spill->slc)
                {
                    fillSomeVars(nMatches, nBestMatches, allEnergyInMatches, &slice, trueG4ID, trueCryostat);

                    for (auto const& pfp: slice.reco.pfp)
                    {
                        // std::cout << "This is a PFP, with G4ID: " << pfp.trk.truth.bestmatch.G4ID << std::endl;
                        // std::cout << "trk.truth.p.startE: " << pfp.trk.truth.p.startE << std::endl
                        //           << "trk.truth.p.pdg:    " << pfp.trk.truth.p.pdg << std::endl;

                        const int pfpCryostat{pfp.trk.start.x < 0 ? 0 : 1};

                        for (auto const& match: pfp.trk.truth.matches)
                        {
                            if (match.G4ID == trueG4ID and pfpCryostat == trueCryostat and (match.energy / 3.) > maxMatchEnergy)
                            {
                                // Selected matched pfp to truth with highes energy match
                                maxMatchEnergy       = (match.energy / 3.);
                                selectedPfp          = &pfp;
                                selectedParticleMatch = &match;

                                // std::cout << "    --> Looking for energy match, now at E = " << maxMatchEnergy << std::endl;
                            } // Selection scope
                        } // Looping through matches

                        // if (pfp.trk.truth.bestmatch.G4ID == trueG4ID and (pfp.trk.truth.bestmatch.energy / 3.) > maxMatchEnergy)
                        // {
                        //     maxMatchEnergy       = (pfp.trk.truth.bestmatch.energy / 3.);
                        //     selectedPfp          = &pfp;
                        //     selectedParticleMatch = &pfp.trk.truth.bestmatch;
                        // }
                    } // Looping through pfps
                } // Looping through slices

                if ((selectedPfp != nullptr) and (selectedParticleMatch != nullptr))
                {
                    // Here do stuff
                    // Selecting the pfp from slice.reco.pfp which
                    // has the bestmatch with the true particle selected (from pdg)

                    // std::cout << "Should've filled with some data" << std::endl;
                    // std::cout << "The most energetic pfp matched has energy " << maxMatchEnergy 
                    //           << " and trackScore " << (*selectedPfp).trackScore << std::endl;

                    // break;

                    // std::cout << "Did find the most energy match E = " << (*selectedParticleMatch).energy 
                    //           << " (this is " << ((((*selectedParticleMatch).energy / 3.) == maxMatchEnergy) ? "" : "NOT") 
                    //           << " equal to the maxMatchEnergy = " 
                    //           << maxMatchEnergy << ")" << std::endl;

                    // Double check
                    if ((*selectedPfp).trk.truth.p.pdg != true_particle.pdg)
                    {
                        // std::cerr << "WARNING: This PFP is: " << (*selectedPfp).trk.truth.p.pdg << "; MC is: " << true_particle.pdg << std::endl;
                        // throw std::logic_error("The selected PFP's truth matched true particle's pdg is NOT right");
                    }
    
                    td.purity.push_back((*selectedParticleMatch).hit_purity);
                    td.completeness.push_back((*selectedParticleMatch).hit_completeness);
                    td.purityBestmatch.push_back((*selectedPfp).trk.truth.bestmatch.hit_purity);
                    td.completenessBestmatch.push_back((*selectedPfp).trk.truth.bestmatch.hit_completeness);
                    // td.energyCompleteness.push_back((*selectedParticleMatch).energy_completeness); 
                    td.energyCompleteness.push_back(maxMatchEnergy/(true_particle.startE - true_particle.endE));
                    td.energyCompletenessAllMatches.push_back(allEnergyInMatches/(true_particle.startE - true_particle.endE));

                    td.energyMatch.push_back(maxMatchEnergy);
                    td.energyAllMatches.push_back(allEnergyInMatches);
                    td.trueEnergy.push_back(true_particle.startE - true_particle.endE);
    
                    td.recoLength.push_back((*selectedPfp).trk.len);
                    td.trueLength.push_back(true_particle.length);
                    td.ratioLength.push_back((*selectedPfp).trk.len/true_particle.length);
                    td.pullLength.push_back(((*selectedPfp).trk.len - true_particle.length)/true_particle.length);
    
                    td.trueEndpoint.x.push_back(true_particle.end.x);
                    td.trueEndpoint.y.push_back(true_particle.end.y);
                    td.trueEndpoint.z.push_back(true_particle.end.z);
    
                    td.recoEndpoint.x.push_back((*selectedPfp).trk.end.x);
                    td.recoEndpoint.y.push_back((*selectedPfp).trk.end.y);
                    td.recoEndpoint.z.push_back((*selectedPfp).trk.end.z);
    
                    td.recoTrueDiffEndpoint.x.push_back((*selectedPfp).trk.end.x - true_particle.end.x);
                    td.recoTrueDiffEndpoint.y.push_back((*selectedPfp).trk.end.y - true_particle.end.y);
                    td.recoTrueDiffEndpoint.z.push_back((*selectedPfp).trk.end.z - true_particle.end.z);
    
                    td.recoTrueDiffEndpoint3d.push_back(
                        std::sqrt(
                            std::pow((*selectedPfp).trk.end.x - true_particle.end.x, 2) +
                            std::pow((*selectedPfp).trk.end.y - true_particle.end.y, 2) +
                            std::pow((*selectedPfp).trk.end.z - true_particle.end.z, 2)
                        )
                    );
    
                    // Calorimetric informations
                    td.calo.trackScore.push_back((*selectedPfp).trackScore);
                    
                    // chiSquare computation
                    std::vector<double> dedx;
                    std::vector<double> rr;
    
                    for (auto const& calopoint: (*selectedPfp).trk.calo[PLANE].points)
                    {
                        dedx.push_back(calopoint.dedx);
                        rr.push_back(calopoint.rr);
                    }
    
                    auto chi = chiSquare::compute(dedx, rr, 0.0, 25.0);
                    td.calo.muonChiSquare.push_back(chi.muon);
                    td.calo.protonChiSquare.push_back(chi.proton);
    
                    // Add nMatches
                    td.nMatches.push_back(nMatches);
                    td.nBestMatches.push_back(nBestMatches);
    
                    // Add nHits
                    td.nHitsReco.push_back((*selectedPfp).trk.calo[PLANE].nhit);
                    td.nHitsTrue.push_back(true_particle.plane[true_particle.cryostat][PLANE].nhit);
    
                    // Vis. Energy (GeV) on PLANE
                    td.trueVisEnergy.push_back(true_particle.plane[true_particle.cryostat][PLANE].visE);

                    td.trueVisEnergyCollection.push_back(true_particle.plane[true_particle.cryostat][COLLECTION].visE);
                    td.trueVisEnergyInduction2.push_back(true_particle.plane[true_particle.cryostat][INDUCTION2].visE);
                    td.trueVisEnergyInduction1.push_back(true_particle.plane[true_particle.cryostat][INDUCTION1].visE);

                    // std::cout << "trueVisEnergy: " << true_particle.plane[true_particle.cryostat][PLANE].visE << std::endl;
                    // std::cout << "true_particle.startE:   " << true_particle.startE << std::endl;
                    // std::cout << "pfp.trk.truth.p.startE: " << pfp.trk.truth.p.startE << std::endl;
                    // std::cout << "true_particle.endE:   " << true_particle.endE << std::endl;
                    // std::cout << "pfp.trk.truth.p.endE: " << pfp.trk.truth.p.endE << std::endl;
                    // break;
                }
            }

            return base::getTrackValue<T>(td, what);

        });
    } // const ana::SpillMultiVar get
} // namespace track

#endif