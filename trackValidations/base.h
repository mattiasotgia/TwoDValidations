
#ifndef BASE_H
#define BASE_H

#include <algorithm>
#include <map>
#include <numeric>
#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>     


namespace base
{
    struct trackData 
    {
        std::vector<double> purity, completeness;                               //!< Hit purity and hit completeness 
        std::vector<double> energyCompleteness;                                 //!< Energy deposited over true energy ratio
        std::vector<double> recoLength, trueLength, ratioLength, pullLength;    //!< ratio: reco/true; pull: (reco-true)/true
        std::vector<double> recoTrueDiffEndpoint3d;                             //!< 3d difference reco/true endpoint
        std::vector<double> trueVisEnergy;                                      //!< Vis. Energy (GeV) on PLANE
        struct Point
        {
            std::vector<double> x, y, z;
        } trueEndpoint, recoEndpoint, recoTrueDiffEndpoint;                     //!< x, y, z for true/reco endpoint and the reco/true difference
        struct Calo
        {
            std::vector<double> muonChiSquare, protonChiSquare;                 //!< chi2 computed under the assumption of muon/proton
            std::vector<double> trackScore;                                     //!< Pandora's trackScore for the best matchedpfp
        } calo;                                                                 //!< calorimetric informations
        std::vector<double> nMatches;                                             //!< Number of matches with this true particle
        std::vector<double> nHitsTrue, nHitsReco;                                 //!< Number of hits in the bestmatch pfp to the true particle
    };

    using VecDoubleMember = std::vector<double> trackData::*;
    using VecLongMember   = std::vector<double>   trackData::*;
    using VecDoubleCaloMember = std::vector<double> trackData::Calo::*;
    using VecDoublePointMember = std::vector<double> trackData::Point::*;

    inline const std::unordered_map<std::string, VecDoubleMember> topLevelDoubles = {
        {"purity",                  &trackData::purity                  },
        {"completeness",            &trackData::completeness            },
        {"energyCompleteness",      &trackData::energyCompleteness      },
        {"recoLength",              &trackData::recoLength              },
        {"trueLength",              &trackData::trueLength              },
        {"ratioLength",             &trackData::ratioLength             },
        {"pullLength",              &trackData::pullLength              },
        {"recoTrueDiffEndpoint3d",  &trackData::recoTrueDiffEndpoint3d  },
        {"trueVisEnergy",           &trackData::trueVisEnergy           },
    };

    inline const std::unordered_map<std::string, VecLongMember> topLevelLongs = {
        {"nMatches", &trackData::nMatches   },
        {"nHitsTrue", &trackData::nHitsTrue },
        {"nHitsReco", &trackData::nHitsReco },
    };

    inline const std::unordered_map<std::string, VecDoubleCaloMember> caloDoubleMembers = {
        {"muonChiSquare",   &trackData::Calo::muonChiSquare     },
        {"protonChiSquare", &trackData::Calo::protonChiSquare   },
        {"trackScore",      &trackData::Calo::trackScore        },
    };

    inline const std::unordered_map<std::string, VecDoublePointMember> pointDoubleMembers = {
        {"x", &trackData::Point::x}, 
        {"y", &trackData::Point::y}, 
        {"z", &trackData::Point::z},
    };

    template <typename T>
    inline std::vector<T> getTrackValue(const trackData& t, const std::string& key)
    {
        // --- double fields ---
        if constexpr (std::is_same_v<T, double>)
        {
            if (auto it = topLevelDoubles.find(key); it != topLevelDoubles.end())
                return t.*(it->second);

            if (key.rfind("calo.", 0) == 0)
            {
                std::string sub = key.substr(5);
                if (auto it = caloDoubleMembers.find(sub); it != caloDoubleMembers.end())
                    return t.calo.*(it->second);
            }

            if (key.rfind("trueEndpoint.", 0) == 0)
            {
                std::string sub = key.substr(13);
                if (auto it = pointDoubleMembers.find(sub); it != pointDoubleMembers.end())
                    return t.trueEndpoint.*(it->second);
            }

            if (key.rfind("recoEndpoint.", 0) == 0)
            {
                std::string sub = key.substr(13);
                if (auto it = pointDoubleMembers.find(sub); it != pointDoubleMembers.end())
                    return t.recoEndpoint.*(it->second);
            }

            if (key.rfind("recoTrueDiffEndpoint.", 0) == 0)
            {
                std::string sub = key.substr(21);
                if (auto it = pointDoubleMembers.find(sub); it != pointDoubleMembers.end())
                    return t.recoTrueDiffEndpoint.*(it->second);
            }
            if (auto it = topLevelLongs.find(key); it != topLevelLongs.end())
                return t.*(it->second);
        }

        // --- long fields ---
        if constexpr (std::is_same_v<T, double>)
        {
            std::runtime_error("Not implemented...");
        }

        throw std::runtime_error("Unknown or type-mismatched field: " + key);
    }

} // namespace base

#endif