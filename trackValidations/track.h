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
    template<class T>
    const ana::SpillMultiVar get(std::string what, particle_t particle)
    {
        return ana::SpillMultiVar ([=](const caf::SRSpillProxy* spill) -> std::vector<T> 
        {
            base::trackData td;

            for (auto const& true_particle: spill->true_particles)
            {
                bool isParticle{particle == std::abs(true_particle.pdg)};

                if (not isParticle) continue;
                if (not true_particle.contained) continue;
                if (true_particle.cryostat == -1) continue;

                int trueG4ID = true_particle.G4ID;

                // std::cerr << "I'm here... G4ID = " << trueG4ID << std::endl;

                for (auto const& slice: spill->slc)
                {

                    long nMatches{0}, nBestMatches{0};
                    for (auto const& pfp: slice.reco.pfp)
                    {
                        // nMatches += std::count_if(
                        //     pfp.trk.truth.matches.begin(),
                        //     pfp.trk.truth.matches.end(),
                        //     [&] (auto const& match) { return match.G4ID == trueG4ID; }
                        // );

                        for (auto const& match: pfp.trk.truth.matches)
                        {
                            if (match.G4ID == trueG4ID) nMatches++;
                        }

                        if (pfp.trk.truth.bestmatch.G4ID == trueG4ID) nBestMatches++;
                    }

                    for (auto const& pfp: slice.reco.pfp)
                    {

                        // std::cout << "This is a PFP, with G4ID: " << pfp.trk.truth.bestmatch.G4ID << std::endl;
                        // std::cout << "trk.truth.p.startE: " << pfp.trk.truth.p.startE << std::endl
                        //           << "trk.truth.p.pdg:    " << pfp.trk.truth.p.pdg << std::endl;

                        if (pfp.trk.truth.bestmatch.G4ID == trueG4ID)
                        {
                            // Here do stuff
                            // Selecting the pfp from slice.reco.pfp which
                            // has the bestmatch with the true particle selected (from pdg)

                            // std::cout << "Should've filled with some data" << std::endl;

                            td.purity.push_back(pfp.trk.truth.bestmatch.hit_purity);
                            td.completeness.push_back(pfp.trk.truth.bestmatch.hit_completeness);
                            td.energyCompleteness.push_back(pfp.trk.truth.bestmatch.energy_completeness); 

                            td.recoLength.push_back(pfp.trk.len);
                            td.trueLength.push_back(true_particle.length);
                            td.ratioLength.push_back(pfp.trk.len/true_particle.length);
                            td.pullLength.push_back((pfp.trk.len - true_particle.length)/true_particle.length);

                            td.trueEndpoint.x.push_back(true_particle.end.x);
                            td.trueEndpoint.y.push_back(true_particle.end.y);
                            td.trueEndpoint.z.push_back(true_particle.end.z);

                            td.recoEndpoint.x.push_back(pfp.trk.end.x);
                            td.recoEndpoint.y.push_back(pfp.trk.end.y);
                            td.recoEndpoint.z.push_back(pfp.trk.end.z);

                            td.recoTrueDiffEndpoint.x.push_back(pfp.trk.end.x - true_particle.end.x);
                            td.recoTrueDiffEndpoint.y.push_back(pfp.trk.end.y - true_particle.end.y);
                            td.recoTrueDiffEndpoint.z.push_back(pfp.trk.end.z - true_particle.end.z);

                            td.recoTrueDiffEndpoint3d.push_back(
                                std::sqrt(
                                    std::pow(pfp.trk.end.x - true_particle.end.x, 2) +
                                    std::pow(pfp.trk.end.y - true_particle.end.y, 2) +
                                    std::pow(pfp.trk.end.z - true_particle.end.z, 2)
                                )
                            );

                            // Calorimetric informations
                            td.calo.trackScore.push_back(pfp.trackScore);
                            
                            // chiSquare computation
                            std::vector<double> dedx;
                            std::vector<double> rr;

                            for (auto const& calopoint: pfp.trk.calo[PLANE].points)
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
                            td.nHitsReco.push_back(pfp.trk.calo[PLANE].nhit);
                            td.nHitsTrue.push_back(true_particle.plane[true_particle.cryostat][PLANE].nhit);

                            // Vis. Energy (GeV) on PLANE
                            td.trueVisEnergy.push_back(true_particle.plane[true_particle.cryostat][PLANE].visE);
                            // std::cout << "trueVisEnergy: " << true_particle.plane[true_particle.cryostat][PLANE].visE << std::endl;
                            // std::cout << "true_particle.startE:   " << true_particle.startE << std::endl;
                            // std::cout << "pfp.trk.truth.p.startE: " << pfp.trk.truth.p.startE << std::endl;
                            // std::cout << "true_particle.endE:   " << true_particle.endE << std::endl;
                            // std::cout << "pfp.trk.truth.p.endE: " << pfp.trk.truth.p.endE << std::endl;
                            // break;
                        }
                    }
                }
            }

            return base::getTrackValue<T>(td, what);

        });
    } // const ana::SpillMultiVar get
} // namespace track

#endif