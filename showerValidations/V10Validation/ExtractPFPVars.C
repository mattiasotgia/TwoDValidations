#pragma once 

// spectrum stuff
#include "sbnana/CAFAna/Core/SpectrumLoader.h"
#include "sbnana/CAFAna/Core/Spectrum.h"

// #define OUTPUT_NAME "2D_DNN_1uNpMpi0_noPulses"
// #define OUTPUT_NAME "2D_DNN_1eNp0pi0_noPulses"
// #define OUTPUT_NAME "2D_DNN_1eNpMpi0_noPulses"
#define OUTPUT_NAME "2D_DNN_baseline"


// include variables and cuts
#include "ExtractPFPVars_Helper.h"

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

void ExtractPFPVars() {

    // Samples based on different generated events
    // BNB CV
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/mcnu2D-wpulsetrain-wcosmics/*/*.flat.caf.root";
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/mcnu2D-nopulsetrain-wcosmics/*/*.flat.caf.root";
    // BNB CV NuE-only
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/mcnu2D-nue-wpulsetrain-wcosmics/*/*.flat.caf.root";
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/mcnu2D-nue-nopulsetrain-wcosmics/*/*.flat.caf.root";

    // Samples based on the *same* generated events
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/remc-wcluster3d/wpulsetrain/*/*.flat.caf.root";  ///< with Cluster3D and pulse trains (both standard)
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/remc-wcluster3d/nopulsetrain/*/*.flat.caf.root"; ///< with Cluster3D (standard) and no pulse trains 
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/remc-nocluster3d/nopulsetrain/*/*.flat.caf.root";   ///< without Cluster3D and no pulse trains (all new)

    // NuE samples
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/remc-nue/wdnn-wpulsetrain-wcluster3d/*/*.flat.caf.root";   ///< with DNN-ROI and Cluster3D and w/ pulse trains (all standard)
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/remc-nue/wdnn-nopulsetrain-wcluster3d/*/*.flat.caf.root";   ///< with DNN-ROI and Cluster3D (standard) and no pulse trains (new)
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/remc-nue/nodnn-wpulsetrain-wcluster3d/*/*.flat.caf.root";   ///< no DNN-ROI (old) and Cluster3D (standard) and w/ pulse trains (standard)
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/remc-nue/nodnn-nopulsetrain-wcluster3d/*/*.flat.caf.root";   ///< no DNN-ROI (old) and Cluster3D (standard) and no pulse trains (new)

    // December 2025 validations
    // const std::string TargetFile = "msotgia_v10_06_00_07_BNB_1d_respun2_caf_nue";   ///< 1D
    // const std::string TargetFile = "msotgia_v10_06_00_07_BNB_2d_respun2_caf_nue";   ///< 2D
    // const std::string TargetFile = "msotgia_v10_06_00_07_BNB_stage1_noPulseTrains_nue_caf";   ///< 2D + DNN-ROI - no pulse train
    // const std::string TargetFile = "msotgia_v10_06_00_07_BNB_stage1_pulseTrains_nue_caf";   ///< 2D + DNN-ROI - LongMaxHits = 10

    // May 2026 validations
    // const std::string TargetFile = "msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_1uNp_energyOnly_respun2_caf";
    // const std::string TargetFile = "msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_1eNp0pi0_energyOnly_respun2_caf";
    // const std::string TargetFile = "msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_1eNpMpi0_energyOnly_respun2_caf";

    // const std::string TargetFile = "msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_1uNp_energyOnly_noPulses_caf";
    // const std::string TargetFile = "msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_1eNp0pi0_energyOnly_noPulses_caf";
    // const std::string TargetFile = "msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_1eNpMpi0_energyOnly_noPulses_caf";
    const std::string TargetFile = "msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_flatcaf";


    // load CAFs and create "Spectrum"
    SpectrumLoader NuLoader(TargetFile);

    // dummy var
    Spectrum *sDummy = new Spectrum("", 
                                    Binning::Simple(3, 0, 3),
                                    NuLoader, 
                                    kDummyVar_ExtractBDTTrainingVars,
                                    kNoSpillCut);
    // fill spectra
    NuLoader.Go();

    double TargetPOT = sDummy->POT();
    std::cout << TargetPOT << std::endl;

    return;
}