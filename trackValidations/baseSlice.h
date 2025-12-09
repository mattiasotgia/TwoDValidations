#ifndef BASE_SLICE_H
#define BASE_SLICE_H

namespace baseSlice
{
    struct sliceData
    {
        struct Point 
        {
            double x, y, z;
        } recoVertex, trueVertex, trueRecoDiffVertex;

        double trueRecoDiffVertex3d;
        double nPfpsPerSlice;
    };

    using DoubleMember = double sliceData::*;
    using DoublePointMember = double sliceData::Point::*;

    inline const std::unordered_map<std::string, DoubleMember> topLevelDoubles = {
        {"trueRecoDiffVertex3d", &sliceData::trueRecoDiffVertex3d},
        // {"nSlices", &sliceData::nSlices},
        {"nPfpsPerSlice", &sliceData::nPfpsPerSlice},
    };

    inline const std::unordered_map<std::string, DoublePointMember> pointDoubleMembers = {
        {"x", &sliceData::Point::x},
        {"y", &sliceData::Point::y},
        {"z", &sliceData::Point::z},
    };


    inline double getSliceDataValue (const sliceData & s, const std::string & key)
    {
        if (auto it = topLevelDoubles.find(key); it != topLevelDoubles.end())
                return s.*(it->second);
        
        if (key.rfind("recoVertex.", 0) == 0)
        {
            std::string sub = key.substr(11);
            if (auto it = pointDoubleMembers.find(sub); it != pointDoubleMembers.end())
                return s.recoVertex.*(it->second);
        }

        if (key.rfind("trueVertex.", 0) == 0)
        {
            std::string sub = key.substr(11);
            if (auto it = pointDoubleMembers.find(sub); it != pointDoubleMembers.end())
                return s.trueVertex.*(it->second);
        }

        if (key.rfind("trueRecoDiffVertex.", 0) == 0)
        {
            std::string sub = key.substr(19);
            if (auto it = pointDoubleMembers.find(sub); it != pointDoubleMembers.end())
                return s.trueRecoDiffVertex.*(it->second);
        }

        throw std::runtime_error("Unknown or type-mismatched field: " + key);
    }
}

#endif