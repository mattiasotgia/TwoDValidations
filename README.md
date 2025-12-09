
# README TwoDValidations

Git repository with the code/stuff to handle the validations for two-d deconvolution data processing

Useful tools
- TITUS evd for local usage is in [here](#setting-up-titus-evdpy-for-local-usage), instructions are a WIP
- Log of the test between 1D/2D is in [here](mdNotes/testsLog.md)

## Setting up TITUS (`evd.py`) for local usage

### MacOS setup

As a staring point the needed software is `brew` (installation can be [found on the site brew.sh](https://brew.sh/)), required to install a couple of tools. 

- XQuartz, downloaded with `brew install --cask xquartz`. This provides the 

## Running docker-TITUS

1. Open XQuartz with `open -a Xquartz`, then infer the `DISPLAY` port with `ps aux | grep Xquartz`, it should be something like `:0`, `:1` or any `:N`. 
2. Go to the `/path/where/is/your/data/` and start the docker with the command
```bash
docker run -it --rm -e "DISPLAY=host.docker.internal:N" -v /path/where/is/your/data/:/data sfbaylaser/titus /bin/bash
```
3. At this point setup titus and it should work...

> **Warning** :shipit: (updated Nov. 26th)
> The current `docker` image is not working correctly so for now this is NON-functioning code.

## Initial tests

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

### 1D deconvolution chain

Aside from all the junk seen before, the 1D chain requires the addition of 
``` 
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

# Tentative fix for caloskim
physics.analyzers.caloskimW.SimChannelproducer: "daq:simpleSC"
physics.analyzers.caloskimE.SimChannelproducer: "daq:simpleSC"
```

These steps are done to remove the YZ-sim labels (which are now on by default)