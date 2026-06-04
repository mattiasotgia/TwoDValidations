#pragma once 

// spectrum stuff
#include "sbnana/CAFAna/Core/SpectrumLoader.h"
#include "sbnana/CAFAna/Core/Spectrum.h"

// include my variables and cuts
#include "SelectionNp_Helper.h"

// root stuff
#include "TCanvas.h"
#include "TFile.h"
#include "TTreeReader.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TStyle.h"
#include "THStack.h"

using namespace ana;

void RecoEfficiency() {

    // NuE samples
    const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/remc-nue/wdnn-wpulsetrain-wcluster3d/*/*.flat.caf.root";   ///< with DNN-ROI and Cluster3D and w/ pulse trains (all standard)
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/remc-nue/wdnn-nopulsetrain-wcluster3d/*/*.flat.caf.root";   ///< with DNN-ROI and Cluster3D (standard) and no pulse trains (new)
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/remc-nue/nodnn-wpulsetrain-wcluster3d/*/*.flat.caf.root";   ///< no DNN-ROI (old) and Cluster3D (standard) and w/ pulse trains (standard)

    // load CAFs and create "Spectrum"
    SpectrumLoader NuLoader(TargetFile);

    // dummy var
    Spectrum *sDummy = new Spectrum("", 
                                    Binning::Simple(3, 0, 3),
                                    NuLoader, 
                                    kDummyVar_ExtractVars_nueCC_Standard,
                                    kNoSpillCut);

    // fill spectra
    NuLoader.Go();

    double TargetPOT = sDummy->POT();
    std::cout << TargetPOT << std::endl;

    return;
}