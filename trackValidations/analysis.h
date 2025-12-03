#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "sbnana/CAFAna/Core/SpectrumLoader.h"
#include "sbnana/CAFAna/Core/Spectrum.h"
#include "sbnana/CAFAna/Core/Binning.h"
#include "sbnana/CAFAna/Core/Cut.h"
#include "sbnana/CAFAna/Core/Utilities.h"
#include "sbnana/CAFAna/Core/LoadFromFile.h"
#include "sbnana/CAFAna/Core/Var.h"
#include "sbnana/CAFAna/Core/Tree.h"

#include "sbnanaobj/StandardRecord/Proxy/SRProxy.h"
#include "sbnanaobj/StandardRecord/StandardRecord.h"

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

namespace analysis
{
    template <class T>
    class Analysis 
    {
    private:

        std::vector<std::string> __loadedDefinitions;
        std::vector<std::string> __loadedDefNames;
        std::map<std::string, std::string> __keys;
        std::vector<std::string> __variablesName;
        std::vector<T>  __variables;
        ana::Cut        __defSliceCut = ana::kNoCut;
        ana::SpillCut   __defSpillCut = ana::kNoSpillCut;

        std::map<std::string, std::unique_ptr<ana::Tree>> __trees;

        bool __runAll = true;
        std::string __analysisName;

    public:
        Analysis() = delete;
        Analysis(std::string);

        void AddDataset(const std::string &, const std::string & name = "none");
        void AddVariable(const std::string &, const T &);
        void AddCut(const ana::SpillCut & spillCut = ana::kNoSpillCut, const ana::Cut & cut = ana::kNoCut);
        void Go();
        void RunOnly(const std::vector<std::string>);
        void WriteTo(TFile* file);
    };

    template<class T>
    inline Analysis<T>::Analysis (std::string name): 
        __analysisName{name}
    {
        std::cout << "--------------------------" << std::endl;
        std::cout << " Starting Analysis job..." << std::endl;
        std::cout << " Analysis: " << name << std::endl;
        std::cout << "--------------------------" << std::endl << std::endl;
    }

    template<class T>
    void Analysis<T>::AddDataset (const std::string & dataset, const std::string & name)
    {
        std::string tmpName = (name == "none") ? dataset : name;

        __loadedDefNames.push_back(tmpName);
        __loadedDefinitions.push_back(dataset);
        __keys.emplace(tmpName, dataset);
    }

    template <class T>
    void Analysis<T>::AddVariable (const std::string & name, const T & variable) {
        __variables.push_back(variable);
        __variablesName.push_back(name);
    }

    template <class T>
    void Analysis<T>::RunOnly (const std::vector<std::string> which) 
    {
        bool __runAll = false;
        __loadedDefNames = std::move(which);
    }

    template <class T>
    void Analysis<T>::AddCut (const ana::SpillCut & spillCut, const ana::Cut & cut)
    {
        __defSliceCut = __defSliceCut && cut;
        __defSpillCut = __defSpillCut && spillCut;
    }

    template <class T>
    void Analysis<T>::Go()
    {
        for (auto const& availableDataset: __loadedDefNames)
        {
            try
            {
                ana::SpectrumLoader loader(__keys.at(availableDataset).c_str());
                
                if constexpr (std::is_same_v<T, ana::SpillMultiVar> or std::is_same_v<T, ana::SpillVar>)
                {
                    // Filling Spill-level variables
                    __trees.emplace(availableDataset, std::make_unique<ana::Tree>(
                        availableDataset.c_str(),
                        __variablesName,    //!> vector/ variables
                        loader,             //!> loader
                        __variables,        //!> variable names
                        __defSpillCut,      //!> takes only spill-level cuts (any others are implemented in the variables)
                        true                //!> save event, subrun, run columns
                    ));
                }
    
                if constexpr (std::is_same_v<T, ana::MultiVar> or std::is_same_v<T, ana::Var>)
                {
                    // Filling Slice-level variables
                    __trees.emplace(availableDataset, std::make_unique<ana::Tree>(
                        availableDataset.c_str(),
                        __variablesName,    //!> vector/ variables
                        loader,             //!> loader
                        __variables,        //!> variable names
                        __defSpillCut,      //!> takes both
                        __defSliceCut,      //!> takes both
                        ana::kNoShift,      //!> not running any syst-shifts
                        true,               //!> save event, subrun, run columns
                        true                //!> save slice #
                    ));
                }

                loader.Go();
            }
            catch (const std::exception & e)
            {
                std::cerr << "Exception while loading dataset " << availableDataset << std::endl;
                std::cerr << "Exception: " << e.what() << std::endl;
                continue;
            }
        }
    }

    template <class T>
    void Analysis<T>::WriteTo (TFile* file)
    {
        file->mkdir(__analysisName.c_str());
        for (auto const& availableDataset: __loadedDefNames)
        {
            __trees.at(availableDataset)->SaveTo(file->mkdir((__analysisName + "/" + availableDataset).c_str()));
        }
    }

}

#endif