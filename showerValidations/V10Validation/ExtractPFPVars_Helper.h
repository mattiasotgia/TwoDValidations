#include "sbnanaobj/StandardRecord/Proxy/SRProxy.h"
#include "sbnana/CAFAna/Core/Var.h"
#include "sbnana/CAFAna/Core/Cut.h"

#include <fstream>
#include <vector>
#include <math.h>

#include "TVector3.h"
#include "TFile.h"

using namespace ana;

// is the 3D point in the fiducial volume?
bool kIsInFV(double x, double y, double z) {  

  // check vertex integrity
  if ( std::isnan(x) || std::isnan(y) || std::isnan(z) ) 
    return false;

  // request vertex in FV (25 cm padding in x, y; 30/50 cm in z)
  return (( ( x < -61.94 - 25 && x > -358.49 + 25 ) ||
      ( x >  61.94 + 25 && x <  358.49 - 25 )) &&
      ( ( y > -181.86 + 25 && y < 134.96 - 25 ) &&
      ( z > -894.95 + 30 && z < 894.95 - 50 ) ));
}

const SpillMultiVar kDummyVar_ExtractBDTTrainingVars([](const caf::SRSpillProxy* sr)-> std::vector<double>
{
    
        std::vector<double> temp;
        int SliceCounter = 0;
        int ShwPDG = -9999;
        int ParentPDG = -9999;
        int IsTrack = -9999;
        double ParticleTrueEnergy = -9999.;
        int TrueNHits = -9999;
        int TrueIdxCryo;
        double DirX, DirY, DirZ, ThetaYZ, ThetaXZ;
        std::string SourceName = sr->hdr.sourceName;

        std::ofstream myOut(Form("202605/%s.txt", OUTPUT_NAME), std::ios::app);

        // loop over slices
        for (auto const &islc : sr->slc) {

            // skip cosmic slices
            if (islc.truth.index >= 0) {
                TrueIdxCryo = islc.truth.position.x < 0 ? 0 : 1;
                
                // skip first PFP (set to default values by construction)
                for (unsigned int iPFP = 1; iPFP < islc.reco.npfp; iPFP += 1) {

                    myOut << SourceName << "\t";
                    myOut << sr->hdr.run << "\t" << sr->hdr.evt << "\t";
                    myOut << SliceCounter << "\t";
                    myOut << islc.reco.pfp[iPFP].pfochar.linfitlen << "\t" << islc.reco.pfp[iPFP].pfochar.linfitdiff << "\t";
                    myOut << islc.reco.pfp[iPFP].pfochar.linfitgaplen << "\t" << islc.reco.pfp[iPFP].pfochar.linfitrms << "\t";
                    myOut << islc.reco.pfp[iPFP].pfochar.vtxdist << "\t" << islc.reco.pfp[iPFP].pfochar.pca2ratio << "\t";
                    myOut << islc.reco.pfp[iPFP].pfochar.pca3ratio << "\t" << islc.reco.pfp[iPFP].pfochar.openanglediff << "\t";
                    myOut << islc.reco.pfp[iPFP].pfochar.chgfracspread << "\t" << islc.reco.pfp[iPFP].pfochar.chgendfrac << "\t";
                    myOut << islc.reco.pfp[iPFP].pfochar.halototratio << "\t";
                    myOut << islc.reco.pfp[iPFP].pfochar.concentration  << "\t" << islc.reco.pfp[iPFP].pfochar.conicalness << "\t";
                    myOut << islc.reco.pfp[iPFP].trackScore << "\t";

                    // decide the reconstruction path and save well-reco conditions
                    // shower if electron or photon
                    ShwPDG = islc.reco.pfp[iPFP].shw.truth.p.pdg;
                    if ((abs(ShwPDG) == 11) | (ShwPDG == 22)) {
                        // parent PDG
                        for (auto const& TrueParticle : sr->true_particles) {

                            // if this is a primary particle at truth level, just give the neutrino PDG
                            if (islc.reco.pfp[iPFP].parent_is_primary) {
                                ParentPDG = islc.truth.pdg;
                            }

                            // if this is not a primary particle
                            else {
                                if (TrueParticle.G4ID == static_cast<int>(islc.reco.pfp[iPFP].shw.truth.p.parent)) {
                                    ParentPDG = TrueParticle.pdg;
                                    break;
                                }
                            }
                        }

                        // start point 
                        TVector3 StartReco(islc.reco.pfp[iPFP].shw.start.x, islc.reco.pfp[iPFP].shw.start.y, islc.reco.pfp[iPFP].shw.start.z);
                        TVector3 StartTrue(islc.reco.pfp[iPFP].shw.truth.p.start.x, islc.reco.pfp[iPFP].shw.truth.p.start.y, islc.reco.pfp[iPFP].shw.truth.p.start.z);
                        TVector3 StartPTrue(islc.reco.pfp[iPFP].shw.truth.p.startp.x, islc.reco.pfp[iPFP].shw.truth.p.startp.y, islc.reco.pfp[iPFP].shw.truth.p.startp.z);
                        DirX = islc.reco.pfp[iPFP].shw.truth.p.startp.x / StartPTrue.Mag();
                        DirY = islc.reco.pfp[iPFP].shw.truth.p.startp.y / StartPTrue.Mag();
                        DirZ = islc.reco.pfp[iPFP].shw.truth.p.startp.z / StartPTrue.Mag();
                        ThetaXZ = (180. / M_PI) * TMath::ATan2(DirX, DirZ);
                        ThetaYZ = (180. / M_PI) * TMath::ATan2(DirY, DirZ);

                        // true energy
                        ParticleTrueEnergy = islc.reco.pfp[iPFP].shw.truth.p.startE - islc.reco.pfp[iPFP].shw.truth.p.endE;

                        // other info
                        IsTrack = 0;
                        myOut << IsTrack << "\t" << islc.reco.pfp[iPFP].shw.truth.p.pdg << "\t" << ParentPDG << "\t";
                        myOut << DirX << "\t" << DirY << "\t" << DirZ << "\t" << ThetaXZ << "\t" << ThetaYZ << "\t";
                        myOut << islc.reco.pfp[iPFP].shw.truth.bestmatch.hit_completeness << "\t" << islc.reco.pfp[iPFP].shw.truth.bestmatch.energy_completeness << "\t";      
                        myOut << islc.reco.pfp[iPFP].shw.truth.bestmatch.hit_purity << "\t" << islc.reco.pfp[iPFP].shw.truth.bestmatch.energy_purity << "\t"; 
                        myOut << (StartReco-StartTrue).Mag() << "\t" << ParticleTrueEnergy << "\t";

                        // hit reconstruction metrics
                        myOut << islc.reco.pfp[iPFP].shw.truth.p.plane[TrueIdxCryo][0].nhit << "\t" << islc.reco.pfp[iPFP].shw.truth.p.plane[TrueIdxCryo][1].nhit << "\t" << islc.reco.pfp[iPFP].shw.truth.p.plane[TrueIdxCryo][2].nhit << "\t";
                        myOut << islc.reco.pfp[iPFP].shw.plane[0].nHits << "\t" << islc.reco.pfp[iPFP].shw.plane[1].nHits << "\t" << islc.reco.pfp[iPFP].shw.plane[2].nHits << "\t";
                    }

                    // track path
                    else {
                        // parent PDG
                        for (auto const& TrueParticle : sr->true_particles) {

                            // if this is a primary particle at truth level, just give the neutrino PDG
                            if (islc.reco.pfp[iPFP].parent_is_primary) {
                                ParentPDG = islc.truth.pdg;
                            }

                            // if this is not a primary particle
                            else {
                                if (TrueParticle.G4ID == static_cast<int>(islc.reco.pfp[iPFP].trk.truth.p.parent)) {
                                    ParentPDG = TrueParticle.pdg;
                                    break;
                                }
                            }
                        }

                        // start point 
                        TVector3 StartReco(islc.reco.pfp[iPFP].trk.start.x, islc.reco.pfp[iPFP].trk.start.y, islc.reco.pfp[iPFP].trk.start.z);
                        TVector3 StartTrue(islc.reco.pfp[iPFP].trk.truth.p.start.x, islc.reco.pfp[iPFP].trk.truth.p.start.y, islc.reco.pfp[iPFP].trk.truth.p.start.z);
                        TVector3 StartPTrue(islc.reco.pfp[iPFP].trk.truth.p.startp.x, islc.reco.pfp[iPFP].trk.truth.p.startp.y, islc.reco.pfp[iPFP].trk.truth.p.startp.z);
                        DirX = islc.reco.pfp[iPFP].trk.truth.p.startp.x / StartPTrue.Mag();
                        DirY = islc.reco.pfp[iPFP].trk.truth.p.startp.y / StartPTrue.Mag();
                        DirZ = islc.reco.pfp[iPFP].trk.truth.p.startp.z / StartPTrue.Mag();
                        ThetaXZ = (180. / M_PI) * TMath::ATan2(DirX, DirZ);
                        ThetaYZ = (180. / M_PI) * TMath::ATan2(DirY, DirZ);

                        // true energy
                        ParticleTrueEnergy = islc.reco.pfp[iPFP].trk.truth.p.startE - islc.reco.pfp[iPFP].trk.truth.p.endE;

                        // other info
                        IsTrack = 1;
                        myOut << IsTrack << "\t" << islc.reco.pfp[iPFP].trk.truth.p.pdg << "\t" << ParentPDG << "\t";
                        myOut << DirX << "\t" << DirY << "\t" << DirZ << "\t" << ThetaXZ << "\t" << ThetaYZ << "\t";
                        myOut << islc.reco.pfp[iPFP].trk.truth.bestmatch.hit_completeness << "\t" << islc.reco.pfp[iPFP].trk.truth.bestmatch.energy_completeness << "\t";            
                        myOut << islc.reco.pfp[iPFP].trk.truth.bestmatch.hit_purity << "\t" << islc.reco.pfp[iPFP].trk.truth.bestmatch.energy_purity << "\t";            
                        myOut << (StartReco-StartTrue).Mag() << "\t" << ParticleTrueEnergy << "\t";

                        // hit reconstruction metrics
                        myOut << islc.reco.pfp[iPFP].trk.truth.p.plane[TrueIdxCryo][0].nhit << "\t" << islc.reco.pfp[iPFP].trk.truth.p.plane[TrueIdxCryo][1].nhit << "\t" << islc.reco.pfp[iPFP].trk.truth.p.plane[TrueIdxCryo][2].nhit << "\t";
                        myOut << islc.reco.pfp[iPFP].trk.calo[0].nhit << "\t" << islc.reco.pfp[iPFP].trk.calo[1].nhit << "\t" << islc.reco.pfp[iPFP].trk.calo[2].nhit << "\t";
                    }

                    // some reconstructed variables 
                    myOut << 180. * islc.reco.pfp[iPFP].shw.open_angle / M_PI << "\t" ///< Relevant only for showers, really
                        << islc.reco.pfp[iPFP].shw.conversion_gap << "\t" 
                        << islc.reco.pfp[iPFP].shw.plane[2].dEdx << "\t" 
                        << islc.reco.pfp[iPFP].shw.len << "\t";

                    // vertex reconstruction metric
                    TVector3 VertexReco(islc.vertex.x, islc.vertex.y, islc.vertex.z);
                    TVector3 VertexTrue(islc.truth.position.x, islc.truth.position.y, islc.truth.position.z);
                    myOut << islc.vertex.x << "\t" << islc.vertex.y << "\t" << islc.vertex.z << "\t";
                    myOut << islc.truth.position.x << "\t" << islc.truth.position.y << "\t" << islc.truth.position.z << "\t";
                    myOut << (VertexReco-VertexTrue).Mag() << "\t" << (int) kIsInFV(islc.truth.position.x, islc.truth.position.y, islc.truth.position.z) << "\t";

                    // neutrino interaction information
                    myOut << islc.truth.index << "\t" << islc.truth.iscc << "\t" << islc.truth.pdg << "\t";

                    myOut << std::endl;
                
                } // end of PFP loop

                SliceCounter += 1;
            } // skip cosmic slices
        } // end of slice loop

        myOut.close();

 	return temp;
});