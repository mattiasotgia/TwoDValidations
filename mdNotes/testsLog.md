# Initial tests

In `v10_06_00_04p04` at least there is something strange with the `reco` path

For the 2D deconvolution stage the `combineHitsCryo` module it used, as shown in 
```
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
```
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
