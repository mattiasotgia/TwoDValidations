#include "sbnanaobj/StandardRecord/Proxy/SRProxy.h"
#include "sbnana/CAFAna/Core/Var.h"
#include "sbnana/CAFAna/Core/Cut.h"

#include <fstream>
#include <vector>
#include <math.h>

#include "TVector3.h"
#include "TFile.h"

using namespace ana;

/*
 *  Functions.
 */

bool kIsInFV(double x, double y, double z) {  // vertex-based fiducial volume cut

  // check vertex integrity
  if ( std::isnan(x) || std::isnan(y) || std::isnan(z) ) 
    return false;

  // request vertex in FV (25 cm padding in x, y; 30/50 cm in z)
  return (( ( x < -61.94 - 25 && x > -358.49 + 25 ) ||
      ( x >  61.94 + 25 && x <  358.49 - 25 )) &&
      ( ( y > -181.86 + 25 && y < 134.96 - 25 ) &&
      ( z > -894.95 + 30 && z < 894.95 - 50 ) ));
}

bool kIsInAV(double x, double y, double z) {  // vertex-based active volume cut

  // check vertex integrity
  if ( std::isnan(x) || std::isnan(y) || std::isnan(z) ) 
    return false;

  // request vertex in AV 
  return (( ( x < -61.94 && x > -358.49 ) ||
      ( x >  61.94 && x <  358.49 )) &&
      ( ( y > -181.86 && y < 134.96 ) &&
      ( z > -894.95 && z < 894.95 ) ));
}

bool kIsInContained(double ex, double ey, double ez) { 

  // request end point in contained volume (5 cm padding)
  return (( ( ex < -61.94 - 5 && ex > -358.49 + 5 ) ||
      ( ex >  61.94 + 5 && ex <  358.49 - 5 )) &&
      ( ( ey > -181.86 + 5 && ey < 134.96 - 5 ) &&
      ( ez > -894.95 + 5 && ez < 894.95 - 5 ) ));
}


/*
 *  Slice-level generic variables.
 */

// for counting events that passing cuts
const Var kCounting([](const caf::SRSliceProxy *slc)
    {
        return 1;
    });

// vertex coordinates
const Var kVertex_X([](const caf::SRSliceProxy *slc)
    {
        return slc->vertex.x;
    });

const Var kVertex_Y([](const caf::SRSliceProxy *slc)
    {
        return slc->vertex.y;
    });

const Var kVertex_Z([](const caf::SRSliceProxy *slc)
    {
        return slc->vertex.z;
    });

const Var kVertex_vsTruth([](const caf::SRSliceProxy *slc)
    {

        // reconstructed slice vertex
        TVector3 VertexReco(slc->vertex.x, slc->vertex.y, slc->vertex.z);

        // true neutrino vertex in the slice
        TVector3 VertexTrue(slc->truth.position.x, slc->truth.position.y, slc->truth.position.z);

        return (VertexReco-VertexTrue).Mag();
    });

const Var kNPFPs([](const caf::SRSliceProxy *slc)
    {
        // there is always one null PFP!
        return slc->reco.npfp;
    });

// get number of PFPs with > 10 hits associated to the shower
const Var kNPFPs_Electron_CollHitCut([](const caf::SRSliceProxy *slc)
    {
        int nPFPs_CollHitCut = 0;

        for (unsigned int i = 0; i < slc->reco.npfp; i++) 
            if ((slc->reco.pfp[i].shw.plane[2].nHits > 10) &&
                (abs(slc->reco.pfp[i].shw.truth.p.pdg) == 11))
                nPFPs_CollHitCut += 1;

        return nPFPs_CollHitCut;
    });


// number of hits of reconstructed slice
const Var kSlice_NHits([](const caf::SRSliceProxy *slc)
    {
        int kNHits = 0.;

        // loop over PFPs
        for (unsigned int i = 0; i < slc->reco.npfp; i++) 
            kNHits += slc->reco.pfp[i].shw.plane[2].nHits;
        

        return kNHits;
    });

const Var kIsClearCosmic([](const caf::SRSliceProxy* slc)
    { 
        return slc->is_clear_cosmic;
    });


/*
 *  Primary shower identification.
 *  For electron reconstruction studies, you already know that the PDG will be 11.
 *  Just take the biggest shower (in terms of number of hits in Collection) associated with the true electron.
 */ 

const Var kLargestRecoShowerIdx([](const caf::SRSliceProxy* slc) -> int 
    {
        int maxNHitsIdx(-1);
        int electronIdx(-1);
        double maxNHits(-1);

        // loop over PFPs
        for (unsigned int i = 0; i < slc->reco.npfp; i++) {

            // get the showers
            auto const& shw = slc->reco.pfp[i].shw;

            // get the highest-nhits shower index 
            // among those associated with a true electron
            if ((abs(shw.truth.p.pdg) == 11) &&
                (shw.plane[2].nHits > maxNHits)) {
                maxNHitsIdx = i;
                maxNHits = shw.plane[2].nHits;
            }
        }

        electronIdx = maxNHitsIdx;

        return electronIdx;
    });

/* 
 *  Overall reconstruction variables.
 */

const Var kLargestRecoShower_HitPurity([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.truth.bestmatch.hit_purity;
    });

// completeness
const Var kLargestRecoShower_HitCompleteness([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.truth.bestmatch.hit_completeness;
    });

// purity
const Var kLargestRecoShower_EnergyPurity([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.truth.bestmatch.energy_purity;
    });

// completeness
const Var kLargestRecoShower_EnergyCompleteness([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.truth.bestmatch.energy_completeness;
    });

// true energy in hits
const Var kLargestRecoShower_TrueEnergyInHits([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        // find cryostat
        int vCryo = slc->truth.position.x < 0 ? 0 : 1;

        // best-match E-completeness
        double EComp = slc->reco.pfp[largestShwIdx].shw.truth.bestmatch.energy_completeness;
        double EMatch = slc->reco.pfp[largestShwIdx].shw.truth.bestmatch.energy;

        return (EMatch / 3.) / EComp;
    });

// true energy at particle-level
const Var kLargestRecoShower_StartEnd_Energy([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.truth.p.startE - slc->reco.pfp[largestShwIdx].shw.truth.p.endE;
    });

// best-plane energy of largest reconstructed shower
const Var kLargestRecoShower_TruePdg([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.truth.p.pdg;
    });

/*
 *  Start coordinates.
 */

const Var kLargestRecoShower_StartX([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -999;

        return slc->reco.pfp[largestShwIdx].shw.start.x;
    });

const Var kLargestRecoShower_StartY([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -999;

        return slc->reco.pfp[largestShwIdx].shw.start.y;
    });

const Var kLargestRecoShower_StartZ([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -999;

        return slc->reco.pfp[largestShwIdx].shw.start.z;
    });

const Var kLargestRecoShower_StartX_Truth([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -999;

        return slc->reco.pfp[largestShwIdx].shw.truth.p.start.x;
    });

const Var kLargestRecoShower_StartY_Truth([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -999;

        return slc->reco.pfp[largestShwIdx].shw.truth.p.start.y;
    });

const Var kLargestRecoShower_StartZ_Truth([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -999;

        return slc->reco.pfp[largestShwIdx].shw.truth.p.start.z;
    });

const Var kLargestRecoShower_Start_VsTruth([](const caf::SRSliceProxy *slc) -> double
    {

        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -999;

        // reconstructed slice vertex
        TVector3 StartReco(slc->reco.pfp[largestShwIdx].shw.start.x, slc->reco.pfp[largestShwIdx].shw.start.y, slc->reco.pfp[largestShwIdx].shw.start.z);

        // true neutrino vertex in the slice
        TVector3 StartTrue(slc->reco.pfp[largestShwIdx].shw.truth.p.start.x, slc->reco.pfp[largestShwIdx].shw.truth.p.start.y, slc->reco.pfp[largestShwIdx].shw.truth.p.start.z);

        return (StartReco-StartTrue).Mag();
    });

/*
 *  Direction variables.
 */

const Var kLargestRecoShower_DirStartX_Truth([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        TVector3 StartPTrue(slc->reco.pfp[largestShwIdx].shw.truth.p.startp.x, 
                            slc->reco.pfp[largestShwIdx].shw.truth.p.startp.y, 
                            slc->reco.pfp[largestShwIdx].shw.truth.p.startp.z);
        

        return slc->reco.pfp[largestShwIdx].shw.truth.p.startp.x / StartPTrue.Mag();
    });

const Var kLargestRecoShower_DirStartY_Truth([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        TVector3 StartPTrue(slc->reco.pfp[largestShwIdx].shw.truth.p.startp.x, 
                            slc->reco.pfp[largestShwIdx].shw.truth.p.startp.y, 
                            slc->reco.pfp[largestShwIdx].shw.truth.p.startp.z);
        

        return slc->reco.pfp[largestShwIdx].shw.truth.p.startp.y / StartPTrue.Mag();
    });

const Var kLargestRecoShower_DirStartZ_Truth([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        TVector3 StartPTrue(slc->reco.pfp[largestShwIdx].shw.truth.p.startp.x, 
                            slc->reco.pfp[largestShwIdx].shw.truth.p.startp.y, 
                            slc->reco.pfp[largestShwIdx].shw.truth.p.startp.z);
        

        return slc->reco.pfp[largestShwIdx].shw.truth.p.startp.z / StartPTrue.Mag();
    });

const Var kLargestRecoShower_DirStartYZ_Truth([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        TVector3 StartPTrue(slc->reco.pfp[largestShwIdx].shw.truth.p.startp.x, 
                            slc->reco.pfp[largestShwIdx].shw.truth.p.startp.y, 
                            slc->reco.pfp[largestShwIdx].shw.truth.p.startp.z);

        return sqrt(1 - pow(slc->reco.pfp[largestShwIdx].shw.truth.p.startp.y / StartPTrue.Mag(), 2));
    });

const Var kLargestRecoShower_DirStartXZ_Truth([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        TVector3 StartPTrue(slc->reco.pfp[largestShwIdx].shw.truth.p.startp.x, 
                            slc->reco.pfp[largestShwIdx].shw.truth.p.startp.y, 
                            slc->reco.pfp[largestShwIdx].shw.truth.p.startp.z);
        

        return (slc->reco.pfp[largestShwIdx].shw.truth.p.startp.z / StartPTrue.Mag()) * 1. / sqrt(1 - pow(slc->reco.pfp[largestShwIdx].shw.truth.p.startp.y / StartPTrue.Mag(), 2));
    });

const Var kLargestRecoShower_DirStartYZ_PolarAngle([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        TVector3 StartPTrue(slc->reco.pfp[largestShwIdx].shw.truth.p.startp.x, 
                            slc->reco.pfp[largestShwIdx].shw.truth.p.startp.y, 
                            slc->reco.pfp[largestShwIdx].shw.truth.p.startp.z);
        

        return 180. * StartPTrue.Theta() / M_PI;
    });

const Var kLargestRecoShower_DirStartXZ_AzimuthAngle([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        TVector3 StartPTrue(slc->reco.pfp[largestShwIdx].shw.truth.p.startp.x, 
                            slc->reco.pfp[largestShwIdx].shw.truth.p.startp.y, 
                            slc->reco.pfp[largestShwIdx].shw.truth.p.startp.z);
        

        return 180. * StartPTrue.Phi() / M_PI;
    });

const Var kLargestRecoShower_DirStartX([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.dir.x;
    });

const Var kLargestRecoShower_DirStartY([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.dir.y;
    });

const Var kLargestRecoShower_DirStartZ([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.dir.z;
    });

// share of hits in the largest PFPs with respect to the others: proxy for shower mis-clustering
const Var kLargestRecoShower_CollHitShare([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        // get number of "shower"-reconstructed hits clustered into PFPs
        int nHitSlcPFPs = 0.;
        for (unsigned int i = 0; i < slc->reco.npfp; i++) 
            nHitSlcPFPs += slc->reco.pfp[i].shw.plane[2].nHits;

        return (double) slc->reco.pfp[largestShwIdx].shw.plane[2].nHits / nHitSlcPFPs;
    });

/*
 *  Energy variables.
 */

// best-plane energy of largest reconstructed shower
const Var kLargestRecoShower_BestEnergy([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.bestplane_energy;
    });

// collection energy of largest reconstructed shower
const Var kLargestRecoShower_Ind1Energy([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.plane[0].energy;
    });

// collection energy of largest reconstructed shower
const Var kLargestRecoShower_Ind2Energy([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.plane[1].energy;
    });

// collection energy of largest reconstructed shower
const Var kLargestRecoShower_CollEnergy([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.plane[2].energy;
    });

// best-plane energy of largest reconstructed shower
const Var kLargestRecoShower_BestEnergy_vsTruth([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        // find cryostat
        int vCryo = slc->truth.position.x < 0 ? 0 : 1;
        
        int bestPlane = slc->reco.pfp[largestShwIdx].shw.truth.p.plane[vCryo][1].nhit > 
                        slc->reco.pfp[largestShwIdx].shw.truth.p.plane[vCryo][2].nhit ? 1 : 2;

        double trueEnergy = slc->reco.pfp[largestShwIdx].shw.truth.p.plane[vCryo][bestPlane].visE;

        if (trueEnergy <= 0) return -5;

        return (slc->reco.pfp[largestShwIdx].shw.bestplane_energy - trueEnergy) / trueEnergy;
    });

// best-plane energy of largest reconstructed shower
const Var kLargestRecoShower_CollEnergy_vsTruth([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        // find cryostat
        int vCryo = slc->truth.position.x < 0 ? 0 : 1;
        double trueEnergy = slc->reco.pfp[largestShwIdx].shw.truth.p.plane[vCryo][2].visE;

        if (trueEnergy <= 0) return -5;

        return (slc->reco.pfp[largestShwIdx].shw.plane[2].energy - trueEnergy) / trueEnergy;
    });

const Var kLargestRecoShower_Ind2Energy_vsTruth([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        // find cryostat
        int vCryo = slc->truth.position.x < 0 ? 0 : 1;
        double trueEnergy = slc->reco.pfp[largestShwIdx].shw.truth.p.plane[vCryo][1].visE;

        if (trueEnergy <= 0) return -5;

        return (slc->reco.pfp[largestShwIdx].shw.plane[1].energy - trueEnergy) / trueEnergy;
    });

const Var kLargestRecoShower_Ind1Energy_vsTruth([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        // find cryostat
        int vCryo = slc->truth.position.x < 0 ? 0 : 1;
        double trueEnergy = slc->reco.pfp[largestShwIdx].shw.truth.p.plane[vCryo][0].visE;

        if (trueEnergy <= 0) return -5;

        return (slc->reco.pfp[largestShwIdx].shw.plane[0].energy - trueEnergy) / trueEnergy;
    });

const Var kLargestRecoShower_TrueCollEnergy([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        // find cryostat
        int vCryo = slc->truth.position.x < 0 ? 0 : 1;
        double trueEnergy = slc->reco.pfp[largestShwIdx].shw.truth.p.plane[vCryo][2].visE;

        return trueEnergy;
    });

const Var kLargestRecoShower_TrueInd2Energy([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        // find cryostat
        int vCryo = slc->truth.position.x < 0 ? 0 : 1;
        double trueEnergy = slc->reco.pfp[largestShwIdx].shw.truth.p.plane[vCryo][1].visE;

        return trueEnergy;
    });

const Var kLargestRecoShower_TrueInd1Energy([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        // find cryostat
        int vCryo = slc->truth.position.x < 0 ? 0 : 1;
        double trueEnergy = slc->reco.pfp[largestShwIdx].shw.truth.p.plane[vCryo][0].visE;

        return trueEnergy;
    });

/*
 *  dE/dx variables
 */

// best-plane dE/dx at the start of largest reconstructed shower
const Var kLargestRecoShower_dEdx([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        // get best-plane
        int bestPlaneIdx = slc->reco.pfp[largestShwIdx].shw.plane[2].nHits > slc->reco.pfp[largestShwIdx].shw.plane[1].nHits ? 2 : 1;

        return slc->reco.pfp[largestShwIdx].shw.plane[bestPlaneIdx].dEdx;
    });

// best-plane dE/dx at the start of largest reconstructed shower
const Var kLargestRecoShower_dEdx_CollectionFirst([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if (largestShwIdx == -1) return -5;

        if (slc->reco.pfp[largestShwIdx].shw.plane[2].dEdx > 0) {
            return slc->reco.pfp[largestShwIdx].shw.plane[2].dEdx;
        }
        else {
            return slc->reco.pfp[largestShwIdx].shw.plane[1].dEdx;
        }

    });

// best-plane dE/dx at the start of largest reconstructed shower
const Var kLargestRecoShower_dEdx_CollectionFirst_withInd1([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if (largestShwIdx == -1) return -5;

        if (slc->reco.pfp[largestShwIdx].shw.plane[2].dEdx > 0) {
            return slc->reco.pfp[largestShwIdx].shw.plane[2].dEdx;
        }
        else if (slc->reco.pfp[largestShwIdx].shw.plane[1].dEdx > 0) {
            return slc->reco.pfp[largestShwIdx].shw.plane[1].dEdx;
        }
        else {
            return slc->reco.pfp[largestShwIdx].shw.plane[0].dEdx;
        }
    });

// best-plane dE/dx at the start of largest reconstructed shower
const Var kLargestRecoShower_Ind1dEdx([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.plane[0].dEdx;
    });

// best-plane dE/dx at the start of largest reconstructed shower
const Var kLargestRecoShower_Ind2dEdx([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.plane[1].dEdx;
    });

// best-plane dE/dx at the start of largest reconstructed shower
const Var kLargestRecoShower_ColldEdx([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        // if(largestShwIdx == -1 || slc->reco.pfp[largestShwIdx].shw.plane[2].dEdx < 0) return -5;
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.plane[2].dEdx;
    });

/*
 *  Track score variables.
 */

// best-plane dE/dx at the start of largest reconstructed shower
const Var kLargestRecoShower_TrackScore([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].trackScore;
    });

const Var kLargestRecoShower_ChgEndFrac([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].pfochar.chgendfrac;
    });

const Var kLargestRecoShower_ChgFracSpread([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].pfochar.chgfracspread;
    });

const Var kLargestRecoShower_Concentration([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].pfochar.concentration;
    });

const Var kLargestRecoShower_Concicalness([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].pfochar.conicalness;
    });

const Var kLargestRecoShower_HaloTotRatio([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].pfochar.halototratio;
    });

const Var kLargestRecoShower_LinFitDiff([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].pfochar.linfitdiff;
    });

const Var kLargestRecoShower_LinFitGapLen([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].pfochar.linfitgaplen;
    });

const Var kLargestRecoShower_LinFitLen([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].pfochar.linfitlen;
    });

const Var kLargestRecoShower_LinFitRMS([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].pfochar.linfitrms;
    });

const Var kLargestRecoShower_OpenAngleDiff([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].pfochar.openanglediff;
    });

const Var kLargestRecoShower_PCA2_Ratio([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].pfochar.pca2ratio;
    });

const Var kLargestRecoShower_PCA3_Ratio([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].pfochar.pca3ratio;
    });

const Var kLargestRecoShower_VtxDist([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].pfochar.vtxdist;
    });

// conversion gap of largest reconstructed shower
const Var kLargestRecoShower_ConversionGap([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.conversion_gap;
    });

// density of largest reconstructed shower
const Var kLargestRecoShower_Density([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.density;
    });

// hits in the best-plane of largest reconstructed shower
const Var kLargestRecoShower_CollectionHits([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.plane[2].nHits;
    });

// hits in the best-plane of largest reconstructed shower
const Var kLargestRecoShower_BestPlaneHits([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        // get best plane for calorimetry
        int bestPlaneIdx = slc->reco.pfp[largestShwIdx].shw.plane[1].nHits > slc->reco.pfp[largestShwIdx].shw.plane[2].nHits ? 1 : 2;

        return slc->reco.pfp[largestShwIdx].shw.plane[bestPlaneIdx].nHits;
    });

// hits in the best-plane of largest reconstructed shower
const Var kLargestRecoShower_HitsOverLength([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        // get best plane for calorimetry
        int bestPlaneIdx = slc->reco.pfp[largestShwIdx].shw.plane[1].nHits > slc->reco.pfp[largestShwIdx].shw.plane[2].nHits ? 1 : 2;

        return slc->reco.pfp[largestShwIdx].shw.plane[bestPlaneIdx].nHits / slc->reco.pfp[largestShwIdx].shw.len;
    });

// hits in the best-plane of largest reconstructed shower
const Var kLargestRecoShower_CollHitsOverLength([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.plane[2].nHits / slc->reco.pfp[largestShwIdx].shw.len;
    });

// hits in the best-plane of largest reconstructed shower
const Var kLargestRecoShower_CollHitsOverEnergy([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.plane[2].nHits / (slc->reco.pfp[largestShwIdx].shw.bestplane_energy * 1.e3);
    });

// hits in the best-plane of largest reconstructed shower
const Var kLargestRecoShower_LengthOverEnergy([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.len / (slc->reco.pfp[largestShwIdx].shw.bestplane_energy * 1.e3);
    });

//  opening angle of largest reconstructed shower
const Var kLargestRecoShower_OpenAngle([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return 180. * slc->reco.pfp[largestShwIdx].shw.open_angle / M_PI;
    });

// length of largest reconstructed shower
const Var kLargestRecoShower_Length([](const caf::SRSliceProxy* slc) -> double 
    {
        // get largest reco shower
        const int largestShwIdx = kLargestRecoShowerIdx(slc);
        if(largestShwIdx == -1) return -5;

        return slc->reco.pfp[largestShwIdx].shw.len;
    });

/*
 *  Neutrino variables.
 */

const Var kSelected_TrueNeutrinoEnergy([](const caf::SRSliceProxy* slc) -> float
    { 

        if (slc->truth.index < 0)
            return -5;

        return slc->truth.E;
    });

/*
 *  Signal definition.
 */

const double VISIBILTY_THRESHOLD_P = 0.05;
const double VISIBILTY_THRESHOLD_PI = 0.025;

// signal definitions at the MCNeutrino level
bool kIsTrueCC1e0pi(const caf::Proxy<caf::SRTrueInteraction>& nu) {

    bool kTrueNueCC = (nu.iscc) && 
                      (abs(nu.pdg) == 12) &&
                      kIsInFV(nu.position.x, nu.position.y, nu.position.z);

    int nPrimElectron = 0, nVisProtons = 0, nVisOther = 0;
    int vCryo = nu.position.x < 0 ? 0 : 1;
    int bestPlaneIdx = 2; ///< for now, just stick to Collection for calorimetry

    for (int ip(0); ip < nu.nprim ; ++ip) {

        // int bestPlaneIdx = nu.prim[ip].plane[vCryo][1].nhit > nu.prim[ip].plane[vCryo][2].nhit ? 1 : 2;

        // electron
        if (abs(nu.prim[ip].pdg) == 11) {
            if (nu.prim[ip].plane[vCryo][bestPlaneIdx].visE > 0.2) {
                ++nPrimElectron;
            }
        }

        // protons
        if (nu.prim[ip].pdg == 2212) {
            if ((nu.prim[ip].plane[vCryo][bestPlaneIdx].visE > VISIBILTY_THRESHOLD_P) &&
                kIsInContained(nu.prim[ip].end.x, nu.prim[ip].end.y, nu.prim[ip].end.z)) {
                    ++nVisProtons;
                }
        }

        // pi0
        if (nu.prim[ip].pdg == 111) {
            ++nVisOther;
        }

        // neutrons (any number)
        if (nu.prim[ip].pdg == 2112) {
            continue;
        }

        // other particles (e.g., pions)
        if ((nu.prim[ip].pdg != 2212) && 
            (abs(nu.prim[ip].pdg) != 11) & 
            (nu.prim[ip].pdg != 2112)  && 
            (nu.prim[ip].pdg != 111)) {
            if (nu.prim[ip].plane[vCryo][bestPlaneIdx].visE >= VISIBILTY_THRESHOLD_PI) {
                ++nVisOther;
            }
        }

    }

    return kTrueNueCC && (nPrimElectron == 1) && (nVisProtons > 0) && (nVisOther == 0);
}

// signal definitions at the slice level
const Cut kTrueCC1e0pi([](const caf::SRSliceProxy* slc) { 

    if (std::isnan(slc->truth.position.x) || std::isnan(slc->truth.position.y) || std::isnan(slc->truth.position.z)) return false;

    bool kTrueNueCC = (slc->truth.index >= 0) && 
                        (slc->truth.iscc) && 
                        (abs(slc->truth.pdg) == 12) &&
                        kIsInFV(slc->truth.position.x, slc->truth.position.y, slc->truth.position.z);

    int nPrimElectron = 0, nVisProtons = 0, nVisOther = 0;
    int vCryo = slc->truth.position.x < 0 ? 0 : 1;
    int bestPlaneIdx = 2; ///< for now, just stick to Collection for calorimetry

    for (int ip(0); ip < slc->truth.nprim ; ++ip) {

        // int bestPlaneIdx = slc->truth.prim[ip].plane[vCryo][1].nhit > slc->truth.prim[ip].plane[vCryo][2].nhit ? 1 : 2;

        // electron
        if (abs(slc->truth.prim[ip].pdg) == 11) {
            if (slc->truth.prim[ip].plane[vCryo][bestPlaneIdx].visE > 0.200) {
                ++nPrimElectron;
            }
        }

        // proton
        if (slc->truth.prim[ip].pdg == 2212) {
            if ((slc->truth.prim[ip].plane[vCryo][bestPlaneIdx].visE > VISIBILTY_THRESHOLD_P) &&
                kIsInContained(slc->truth.prim[ip].end.x, slc->truth.prim[ip].end.y, slc->truth.prim[ip].end.z)) {
                    ++nVisProtons; 
                }
        }

        // pi0
        if (slc->truth.prim[ip].pdg == 111) {
            ++nVisOther;
        }
        
        // neutrons (any number)
        if (slc->truth.prim[ip].pdg == 2112) {
            continue;
        }

        // other particles (e.g., pions)
        if ((slc->truth.prim[ip].pdg != 2212) && 
            (abs(slc->truth.prim[ip].pdg) != 11) && 
            (slc->truth.prim[ip].pdg != 2112) && 
            (slc->truth.prim[ip].pdg != 111)) {

            if (slc->truth.prim[ip].plane[vCryo][bestPlaneIdx].visE >= VISIBILTY_THRESHOLD_PI)
                ++nVisOther;
        }

    }

    return kTrueNueCC && (nPrimElectron == 1) && (nVisProtons > 0) && (nVisOther == 0);
});

const Cut kIsNuOFV([](const caf::SRSliceProxy* slc)
    { 
        return (slc->truth.index >= 0) &&
               !kIsInFV(slc->truth.position.x, slc->truth.position.y, slc->truth.position.z);
    });

const Cut kIsCosmic([](const caf::SRSliceProxy* slc)
    { 
        return (slc->truth.index < 0);
    });

const Cut kTrueVertexInFV([](const caf::SRSliceProxy* slc)
    { 
        return (slc->truth.index >= 0) &&
               kIsInFV(slc->truth.position.x, slc->truth.position.y, slc->truth.position.z);
    });

const Cut kIsNue([](const caf::SRSliceProxy* slc)
    { 
        return (slc->truth.index >= 0) && (abs(slc->truth.pdg) == 12);
    });

const Cut kIsNuMu([](const caf::SRSliceProxy* slc)
    { 
        return (slc->truth.index >= 0) && (abs(slc->truth.pdg) == 14);
    });

const Cut kIsCC([](const caf::SRSliceProxy* slc)
    { 
        return (slc->truth.index >= 0) && (slc->truth.iscc);
    });

const Cut kIsNC([](const caf::SRSliceProxy* slc)
    { 
        return (slc->truth.index >= 0) && (slc->truth.isnc);
    });

const Cut kTrueQE([](const caf::SRSliceProxy* slc)
    { 
        return (slc->truth.genie_mode == caf::kQE);
    });

const Cut kTrueDIS([](const caf::SRSliceProxy* slc)
    { 
        return (slc->truth.genie_mode == caf::kDIS);
    });

const Cut kTrueRes([](const caf::SRSliceProxy* slc)
    { 
        return (slc->truth.genie_mode == caf::kRes);
    });

const Cut kTrueCoh([](const caf::SRSliceProxy* slc)
    { 
        return (slc->truth.genie_mode == caf::kCoh);
    });

const Cut kTrueMEC([](const caf::SRSliceProxy* slc)
    { 
        return (slc->truth.genie_mode == caf::kMEC);
    });


const Var kTrue_NProtons([](const caf::SRSliceProxy* slc)
    { 
        int nVisProtons = 0;
        int vCryo = slc->truth.position.x < 0 ? 0 : 1;

        // loop over primary particles
        for (int ip(0); ip < slc->truth.nprim ; ++ip) {
            if (slc->truth.prim[ip].pdg == 2212) {
                if (slc->truth.prim[ip].plane[vCryo][2].visE >= VISIBILTY_THRESHOLD_P) {
                    nVisProtons += 1;
                }
            }
        }

        return nVisProtons;
    });

/*
 *  Dummy Var to extract shower-level variables for the largest shower 
 *  associated with a primary electron at the truth level in NuECC interactions.
 *  Gen-level information are also extracted, in order to produce "reco. efficiency"
 *  plots for the particles (e.g., requiring that the corresponding PPF has a decent completeness and purity).
 *  Energy variables are also extracted, in order to compare energy resolution and reconstruction
 *  performance across several signal processing and reconstruction changes.
 *  Output refers to a single PFP per slice.
 */
const SpillMultiVar kDummyVar_ExtractShowerVars_NueCC([](const caf::SRSpillProxy* sr)-> std::vector<double>
{
    
        std::vector<double> temp;
        int counter = 0;
        std::string SourceName = sr->hdr.sourceName;

        /*
         *  Save gen-level information.
         */

        // std::ofstream myOut("V10Validation_Shower_Generated_mcnu2D-remc-nue-wcluster3D_noDNN_wpulsetrain.txt", std::ios::app);
        std::ofstream myOut(Form("202605/%s_Generated.txt", OUTPUT_NAME), std::ios::app);

        int vCryo;
        int NProtons = 0.;

        // loop over generated "neutrinos"
        for (auto const& nu : sr->mc.nu) { 

            bool kTrueNueCC = (nu.iscc) && 
                              (abs(nu.pdg) == 12) &&
                              kIsInAV(nu.position.x, nu.position.y, nu.position.z);

            if (kTrueNueCC) {

                NProtons = 0;
                vCryo = nu.position.x < 0 ? 0 : 1;

                // separate loop to count for protons 
                // this is needed as I found very rare backtracking issues 
                // leading to having multiple primary electrons at the truth level
                for (int ip(0); ip < nu.nprim ; ++ip) {
                    if(nu.prim[ip].pdg == 2212) {
                        ++NProtons;
                    }
                }

                // loop over primary particles to find primary electron
                for (int ip(0); ip < nu.nprim ; ++ip) {
                    if (abs(nu.prim[ip].pdg) == 11) {

                        TVector3 StartMomentum(nu.prim[ip].startp.x, nu.prim[ip].startp.y, nu.prim[ip].startp.z);

                        myOut << SourceName << "\t";
                        myOut << sr->hdr.run << "\t" << sr->hdr.evt << "\t";
                        myOut << nu.position.x << "\t" << nu.position.y << "\t" << nu.position.z << "\t";
                        myOut << nu.E << "\t";
                        myOut << nu.prim[ip].plane[vCryo][2].visE << "\t" << nu.prim[ip].plane[vCryo][1].visE << "\t" << nu.prim[ip].plane[vCryo][0].visE << "\t";
                        myOut << nu.prim[ip].startp.x / StartMomentum.Mag() << "\t" << nu.prim[ip].startp.y / StartMomentum.Mag() << "\t" << nu.prim[ip].startp.z / StartMomentum.Mag() << "\t";
                        
                        // found it, go home
                        break;
                    }
                } // end of primary loop

            myOut << NProtons << "\t" << kIsTrueCC1e0pi(nu) << "\t";
            myOut << std::endl;

            } // end of true selection

	    } // end of nu loop

        myOut.close();

        /*
         *  Write slice-level reco. stuff to file.
         */

        // myOut.open("V10Validation_Shower_Reconstructed_mcnu2D-remc-nue-wcluster3D_noDNN_wpulsetrain.txt", std::ios::app);
        myOut.open(Form("202605/%s_Reconstructed.txt", OUTPUT_NAME), std::ios::app);

        // loop over slices
        for (auto const &islc : sr->slc) {
        
            // select nues
            if (kIsNue(&islc) && kIsCC(&islc) && kIsInAV(islc.truth.position.x, islc.truth.position.y, islc.truth.position.z)) {

                myOut << SourceName << "\t";
                myOut << sr->hdr.run << "\t" << sr->hdr.evt << "\t";
                // vertex
                myOut << islc.truth.position.x << "\t" << islc.truth.position.y << "\t" << islc.truth.position.z << "\t" 
                      << islc.vertex.x << "\t" << islc.vertex.y << "\t" << islc.vertex.z << "\t" 
                      << kVertex_vsTruth(&islc) << "\t";
                // truth 
                myOut << islc.tmatch.eff << "\t" << islc.truth.E << "\t" 
                      << kLargestRecoShower_StartEnd_Energy(&islc) << "\t" << kLargestRecoShower_TrueEnergyInHits(&islc) << "\t"
                      << kLargestRecoShower_TruePdg(&islc) << "\t" << kTrue_NProtons(&islc) << "\t"
                      << kTrueCC1e0pi(&islc) << "\t";
                // directions
                myOut << kLargestRecoShower_DirStartX(&islc) << "\t" << kLargestRecoShower_DirStartY(&islc) << "\t" 
                      << kLargestRecoShower_DirStartZ(&islc) << "\t" << kLargestRecoShower_DirStartX_Truth(&islc) << "\t" 
                      << kLargestRecoShower_DirStartY_Truth(&islc) << "\t" << kLargestRecoShower_DirStartZ_Truth(&islc) << "\t" 
                      << kLargestRecoShower_DirStartYZ_Truth(&islc) << "\t" << kLargestRecoShower_DirStartXZ_Truth(&islc) << "\t";
                // energy reconstruction
                myOut << kLargestRecoShower_CollEnergy(&islc) << "\t" << kLargestRecoShower_Ind2Energy(&islc) << "\t" << kLargestRecoShower_Ind1Energy(&islc) << "\t"
                      << kLargestRecoShower_TrueCollEnergy(&islc) << "\t" << kLargestRecoShower_TrueInd2Energy(&islc) << "\t" << kLargestRecoShower_TrueInd1Energy(&islc) << "\t"
                      << kLargestRecoShower_CollEnergy_vsTruth(&islc) << "\t" << kLargestRecoShower_Ind2Energy_vsTruth(&islc) << "\t" << kLargestRecoShower_Ind1Energy_vsTruth(&islc) << "\t";
                // dEdx
                myOut << kLargestRecoShower_ColldEdx(&islc) << "\t" << kLargestRecoShower_Ind2dEdx(&islc) << "\t" 
                      << kLargestRecoShower_Ind1dEdx(&islc) << "\t" << kLargestRecoShower_dEdx(&islc) << "\t";
                // PFP metrics
                myOut << kLargestRecoShower_HitCompleteness(&islc) << "\t" << kLargestRecoShower_EnergyCompleteness(&islc) << "\t"
                      << kLargestRecoShower_HitPurity(&islc) << "\t" << kLargestRecoShower_EnergyPurity(&islc) << "\t"
                      << kLargestRecoShower_OpenAngle(&islc) << "\t" << kLargestRecoShower_ConversionGap(&islc) << "\t";
                myOut << std::endl;

            } // end of truth-based selection
        } // end of loop over slices

        myOut.close();

 	return temp;
});