#pragma once

#include <string>
#include <memory>

#include <TDirectory.h>
#include <TH1D.h>

namespace ana
{

    class metaHist
    {
        public:
        metaHist(const metaHist&) = delete;
        virtual ~metaHist() = default;

        virtual void fill(const recob::Hit& hit) = 0;
        virtual void writeTo(TDirectory* directory) = 0;

        virtual void writeTo(TFile* file)
        {
            writeTo(static_cast<TDirectory*>(file));
        }

        protected:
        explicit metaHist(std::string name)
          : m_name(std::move(name)) {}
        std::string m_name;
    };

    class hitHist: public metaHist
    {
        public: 
        explicit hitHist(std::string name)
          : metaHist(std::move(name))
        {
            m_rms = std::make_unique<TH1D>("rms", "", 50, 0, 15); 
            m_multiplicity = std::make_unique<TH1D>("multiplicity", "", 15, -.5, 14.5);
            m_goodnessOfFit = std::make_unique<TH1D>("goodnessOfFit", "", 35, 0, 35);

            m_rms->SetDirectory(nullptr);
            m_multiplicity->SetDirectory(nullptr);
            m_goodnessOfFit->SetDirectory(nullptr);
        }

        void fill(const recob::Hit& hit) override
        {
            m_rms->Fill(hit.RMS());
            m_multiplicity->Fill(hit.Multiplicity());
            m_goodnessOfFit->Fill(hit.GoodnessOfFit());
        }

        void writeTo(TDirectory* directory) override
        {
            auto where = directory->GetDirectory(m_name.c_str());
            if (!where)
                where = directory->mkdir(m_name.c_str());

            where->cd();
            m_rms->Write();
            m_multiplicity->Write();
            m_goodnessOfFit->Write();
        }

        private:
        std::unique_ptr<TH1D> m_rms, m_multiplicity, m_goodnessOfFit;
    };

    class tpcHist: public metaHist
    {
        public: 
        explicit tpcHist(std::string name)
          : metaHist(std::move(name)),
            m_ind1(std::make_unique<hitHist>("ind1")),
            m_ind2(std::make_unique<hitHist>("ind2")),
            m_coll(std::make_unique<hitHist>("coll"))
        {
        }

        void fill(const recob::Hit& hit) override
        {
            if (hit.WireID().planeID().deepestIndex() == 0)
            {
                m_ind1->fill(hit);
            }
            else if (hit.WireID().planeID().deepestIndex() == 1)
            {
                m_ind2->fill(hit);
            }
            else if (hit.WireID().planeID().deepestIndex() == 2)
            {
                m_coll->fill(hit);
            }
            else
            {
                throw std::logic_error("This recob::Hit has a CryostatID unknown");
            }
        }

        void writeTo(TDirectory* directory) override
        {
            TDirectory* where = directory->GetDirectory(m_name.c_str());
            if (!where)
                where = directory->mkdir(m_name.c_str());

            m_ind1->writeTo(where);
            m_ind2->writeTo(where);
            m_coll->writeTo(where);
        }

        private:
        std::unique_ptr<hitHist> m_ind1, m_ind2, m_coll;
    };
}