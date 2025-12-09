#ifndef SLICE_H
#define SLICE_H

#include "sbnana/CAFAna/Core/SpectrumLoader.h"
#include "sbnana/CAFAna/Core/Spectrum.h"
#include "sbnana/CAFAna/Core/Binning.h"
#include "sbnana/CAFAna/Core/Cut.h"
#include "sbnana/CAFAna/Core/Utilities.h"
#include "sbnana/CAFAna/Core/LoadFromFile.h"
#include "sbnana/CAFAna/Core/Var.h"

#include "sbnanaobj/StandardRecord/Proxy/SRProxy.h"
#include "sbnanaobj/StandardRecord/StandardRecord.h"

#include "baseSlice.h"

namespace slices
{
    const ana::Var get(std::string what)
    {
        return ana::Var ([=](const caf::SRSliceProxy* slice) -> double {
            baseSlice::sliceData sd;

            sd.nPfpsPerSlice = slice->reco.npfp;
            sd.trueVertex = {
                slice->truth.position.x,
                slice->truth.position.y,
                slice->truth.position.z
            };

            sd.recoVertex = {
                slice->vertex.x,
                slice->vertex.y,
                slice->vertex.z
            };

            sd.trueRecoDiffVertex = {
                slice->truth.position.x - slice->vertex.x,
                slice->truth.position.y - slice->vertex.y,
                slice->truth.position.z - slice->vertex.z
            };

            sd.trueRecoDiffVertex3d = std::sqrt(
                std::pow(slice->truth.position.x - slice->vertex.x, 2) + 
                std::pow(slice->truth.position.y - slice->vertex.y, 2) + 
                std::pow(slice->truth.position.z - slice->vertex.z, 2)
            );

            return getSliceDataValue(sd, what);
        });
    }
}

#endif