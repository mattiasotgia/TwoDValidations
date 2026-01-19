#pragma once
#include <string>

namespace geo
{
    struct Detector
    {
        struct Tpc 
        {
            struct Plane 
            {
                double minChannel, maxChannel;
            } I1, I2, C;
        } EE, EW, WE, WW;
    };

    const Detector icarus{
        {
            {   0,  2239},
            {2304,  8063},
            {8064, 13823}
        },
        {
            {13824, 16063},
            {16128, 21887},
            {21888, 27647}
        },
        {
            {27648, 29887},
            {29952, 35711},
            {35712, 41471}
        },
        {
            {41472, 43711},
            {43476, 49535},
            {49536, 55295}
        }
    };
}