# Initial tests

In `v10_06_00_04p04` at least there is something strange with the `reco` path

For the 2D deconvolution stage the `combineHitsCryo` module it used, as shown in 
```json
reco: [
      "gaushit2dTPCEW",
      "gaushit2dTPCEE",
      "combineHitsCryoE",
      "gaushitPT2dTPCEW",
      "gaushitPT2dTPCEE",
      "cluster3DCryoE",
      "pandoraGausCryoE",
      "pandoraTrackGausCryoE",
      "pandoraKalmanTrackGausCryoE",
      "SBNShowerGausCryoE",
      "fmatchCryoE",
      "fmatchopCryoE",
      "tpcpmtbarycentermatchCryoE",
      "crttrack",
      "CRTT0Tagging",
      "caloskimCalorimetryCryoE",
      "mcassociationsGausCryoE",
      "mcreco"
   ]
```

Both Pandora modules (E/W) are using the `HitFinderModuleLabel: "combineHitsCryoE"` label...

Opposite to this, the same path for the 1D deconvolution chain is only using the `cluster3D` module as shown in 
```json
reco: [
      "gaushit1dTPCEW",
      "gaushit1dTPCEE",
      "cluster3DCryoE",
      "pandoraGausCryoE",
      "pandoraTrackGausCryoE",
      "pandoraKalmanTrackGausCryoE",
      "SBNShowerGausCryoE",
      "fmatchCryoE",
      "fmatchopCryoE",
      "tpcpmtbarycentermatchCryoE",
      "crttrack",
      "CRTT0Tagging",
      "caloskimCalorimetryCryoE",
      "mcassociationsGausCryoE",
      "mcreco"
   ]
```

## 1D deconvolution chain

Aside from all the junk seen before, the 1D chain requires the addition of 
```json
# Tentative fix for mcreco
physics.producers.mcreco.SimChannelLabel: "shifted"

# Tentative fix for caloskim
physics.analyzers.caloskimW.SimChannelproducer: "daq:simpleSC"
physics.analyzers.caloskimE.SimChannelproducer: "daq:simpleSC"
```

in the stage1 reconstruction. `mcreco` is used for SPINE, whereas the `caloskim` analyzer is used for _what_?

Identical setting to be added to the 2D+DNNROI chain
```json
# Tentative fix for mcreco
physics.producers.mcreco.SimChannelLabel: "shifted"
`
# Tentative fix for caloskim`
physics.analyzers.caloskimW.SimChannelproducer: "daq:simpleSC"
physics.analyzers.caloskimE.SimChannelproducer: "daq:simpleSC"
```