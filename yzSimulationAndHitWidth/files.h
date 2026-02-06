#pragma once

#include <string>
#include <vector>

namespace data
{
    using Events = std::vector<std::string>;
    using eventMap = std::map<std::string, Events>;
    using hitLabels = std::vector<std::string>;
    using writerFileName = std::string;
    using writerDirectory = TDirectory*;

    Events with_yz_1d = {
        "/exp/icarus/data/users/msotgia/twoDValidations/singleProd/muons1.5GeVOnly/test_yz_toCaf/stage1/with_yz_1d_stage1.root"
    };

    Events with_yz_wcdnn = {
        "/exp/icarus/data/users/msotgia/twoDValidations/singleProd/muons1.5GeVOnly/test_yz_toCaf/stage1/with_yz_2ddnn_stage1.root"
    };

    Events with_yz_wcdnn_pt10 = {
        "/exp/icarus/data/users/msotgia/twoDValidations/singleProd/muons1.5GeVOnly/test_yz_toCaf/stage1/with_yz_2ddnn_pt10_stage1.root"
    };

    Events without_yz_1d = {
        "/exp/icarus/data/users/msotgia/twoDValidations/singleProd/muons1.5GeVOnly/test_yz_toCaf/stage1/without_yz_1d_stage1.root"
    };

    Events without_yz_wcdnn = {
        "/exp/icarus/data/users/msotgia/twoDValidations/singleProd/muons1.5GeVOnly/test_yz_toCaf/stage1/without_yz_2ddnn_stage1.root"
    };

    Events without_yz_wcdnn_pt10 = {
        "/exp/icarus/data/users/msotgia/twoDValidations/singleProd/muons1.5GeVOnly/test_yz_toCaf/stage1/without_yz_2ddnn_pt10_stage1.root"
    };
}