events=(29413 35855 34896 35248 32761 42265)
# files=(
#     msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_1uNp_energyOnly_noPulses_v10_06_00_06p03_stage1_84146801_0 # Event n. 29413
#     msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_1uNp_energyOnly_noPulses_v10_06_00_06p03_stage1_84146803_0 # Event n. 35855
#     msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_1uNp_energyOnly_noPulses_v10_06_00_06p03_stage1_84146807_0 # Event n. 34896
#     msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_1uNp_energyOnly_noPulses_v10_06_00_06p03_stage1_84146809_0 # Event n. 35248
#     msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_1uNp_energyOnly_noPulses_v10_06_00_06p03_stage1_84146826_0 # Event n. 32761
#     msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_1uNp_energyOnly_noPulses_v10_06_00_06p03_stage1_84146835_0 # Event n. 42265
#     msotgia_v10_06_00_06p03_overlay_run9435_B_bnb_nue_withOverlayFixed_1uNp_energyOnly_noPulses_v10_06_00_06p03_stage1_84146848_0 # Event n. 32735
# )

for i in "${!events[@]}"
do
    # echo "I will run ./get_file.sh ${files[i]} /exp/icarus/app/users/msotgia/analysis/twoDRecoStudies/hitTuning/fcl/fixatingPulseTrains/energyOnly_1uNp_parameters.fcl 9435:1:${events[i]} /exp/icarus/app/users/msotgia/event_displays/galleryDisplay/wireDrawings/tmp/V4/e${events[i]}_metric1_1uNp.root"

    # ./get_file.sh ${files[i]} /exp/icarus/app/users/msotgia/analysis/twoDRecoStudies/hitTuning/fcl/fixatingPulseTrains/energyOnly_1uNp_parameters.fcl 9435:1:${events[i]} /exp/icarus/data/users/msotgia/hitTuning/wireDrawingTmp/V4/stage1Redo/e${events[i]}_metric1_1uNp.root

    # ./get_file.sh ${files[i]} /exp/icarus/app/users/msotgia/analysis/twoDRecoStudies/hitTuning/fcl/fixatingPulseTrains/energyOnly_1eNpMpi0_parameters.fcl 9435:1:${events[i]} /exp/icarus/data/users/msotgia/hitTuning/wireDrawingTmp/V4/stage1Redo/e${events[i]}_metric1_1eNpMpi0.root

    # ./get_file.sh ${files[i]} /exp/icarus/app/users/msotgia/analysis/twoDRecoStudies/hitTuning/fcl/fixatingPulseTrains/energyOnly_1eNp0pi0_parameters.fcl 9435:1:${events[i]} /exp/icarus/data/users/msotgia/hitTuning/wireDrawingTmp/V4/stage1Redo/e${events[i]}_metric1_1eNp0pi0.root

    # ./get_file.sh ${files[i]} /exp/icarus/app/users/msotgia/analysis/twoDRecoStudies/hitTuning/fcl/V4/base.fcl 9435:1:${events[i]} /exp/icarus/data/users/msotgia/hitTuning/wireDrawingTmp/V4/stage1Redo/e${events[i]}_default.root

    # running the cafmaker for these files
    lar -c /exp/icarus/app/users/msotgia/analysis/twoDRecoStudies/hitTuning/fcl/cafmakerjob_icarus_detsim2d_systtools_and_fluxwgt_overlay_hits.fcl -n-1 -s e${events[i]}_metric4_1eNp.root
    lar -c /exp/icarus/app/users/msotgia/analysis/twoDRecoStudies/hitTuning/fcl/cafmakerjob_icarus_detsim2d_systtools_and_fluxwgt_overlay_hits.fcl -n-1 -s e${events[i]}_metric4_1uNp.root
done
