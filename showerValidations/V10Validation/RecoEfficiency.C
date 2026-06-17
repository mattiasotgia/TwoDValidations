#pragma once 

// spectrum stuff
#include "sbnana/CAFAna/Core/SpectrumLoader.h"
#include "sbnana/CAFAna/Core/Spectrum.h"

// #define OUTPUT_NAME "2D_DNN_1uNpMpi0_noPulses"
// #define OUTPUT_NAME "2D_DNN_1eNp0pi0_noPulses"
// #define OUTPUT_NAME "2D_DNN_1eNpMpi0_noPulses"
// #define OUTPUT_NAME "2D_DNN_baseline"
// #define OUTPUT_NAME "TESTING_1uNp_flats"
#define OUTPUT_NAME "TESTING_1eNp_flats"

// include my variables and cuts
#include "RecoEfficiency_Helper.h"

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
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/remc-nue/wdnn-wpulsetrain-wcluster3d/*/*.flat.caf.root";   ///< with DNN-ROI and Cluster3D and w/ pulse trains (all standard)
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/remc-nue/wdnn-nopulsetrain-wcluster3d/*/*.flat.caf.root";   ///< with DNN-ROI and Cluster3D (standard) and no pulse trains (new)
    // const std::string TargetFile = "/pnfs/icarus/scratch/users/gputnam/icarusv10D/remc-nue/nodnn-wpulsetrain-wcluster3d/*/*.flat.caf.root";   ///< no DNN-ROI (old) and Cluster3D (standard) and w/ pulse trains (standard)
    // 1D
    // const std::string TargetFile = "/pnfs/sbn/data/sbn_fd/poms_production/2025A_ICARUS_NuMI_NueCos_CV_DetSys/2025A_MC_Nue_Cos_CV/mc/reconstructed/icaruscode_v09_89_01_02p02/flatcaf/*/*/*.root";

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
    // const std::string TargetFile = "msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_flatcaf";
    // const std::string TargetFile = "/exp/icarus/data/users/msotgia/hitTuning/wireDrawingTmp/V4/stage1Redo/CAFs/*1uNp.flat.caf.root";
    const std::string TargetFile = "/exp/icarus/data/users/msotgia/hitTuning/wireDrawingTmp/V4/stage1Redo/CAFs/*1eNp.flat.caf.root";

    // load CAFs and create "Spectrum"
    SpectrumLoader NuLoader(TargetFile);

    // dummy var
    Spectrum *sDummy = new Spectrum("", 
                                    Binning::Simple(3, 0, 3),
                                    NuLoader, 
                                    kDummyVar_ExtractShowerVars_NueCC,
                                    kNoSpillCut);

    // fill spectra
    NuLoader.Go();

    double TargetPOT = sDummy->POT();
    std::cout << TargetPOT << std::endl;

    return;
}