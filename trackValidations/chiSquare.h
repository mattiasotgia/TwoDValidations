
#ifndef CHISQUARE_H
#define CHISQUARE_H

#include <string>
#include <vector>

#include <TProfile.h>

#define PLANE 2

namespace chiSquare
{
    struct chiSquare
    {
        double muon;
        double proton;
        double kaon;
        double pi;
    };

    std::string dEdx_temp = 
        "/exp/icarus/app/users/msotgia/analysis/sbnana_v09_93_01_thesis_analysis/analysis/dEdxrestemplates.root";
    TFile* file = TFile::Open(dEdx_temp.c_str());

    auto dEdxRangeProton = (TProfile *)file->Get("dedx_range_pro");
    auto dEdxRangeKaon   = (TProfile *)file->Get("dedx_range_ka");
    auto dEdxRangePion  = (TProfile *)file->Get("dedx_range_pi");
    auto dEdxRangeMuon  = (TProfile *)file->Get("dedx_range_mu");

    chiSquare compute (std::vector<double> &dEdx, std::vector<double> &RR, double rr_min, double rr_max) 
    {
        double threshold = 0.5;
        double max_rr = rr_max; // Max value for the residual range (RR)
        double min_rr = rr_min; // Min value for the residual range (RR)

        std::vector<float> trkdedx;
        std::vector<float> trkres;
        std::vector<double> vpida;

        for (std::size_t i(0); i < dEdx.size(); ++i) 
        {
            if (i == 0 || i == dEdx.size() - 1)
                continue;

            if (RR[i] < max_rr && RR[i] > rr_min) 
            {
                trkdedx.push_back(dEdx[i]);
                trkres.push_back(RR[i]);
            }
        }

        int npt = 0;
        double chi2pro{0}, chi2ka{0}, chi2pi{0}, chi2mu{0};
        double avgdedx = 0;
        double PIDA = 0;

        int used_trkres = 0;
        for (unsigned i = 0; i < trkdedx.size(); ++i) 
        { 
            avgdedx += trkdedx[i];
            if (trkres[i] < 26) 
            {
                PIDA += trkdedx[i] * std::pow(trkres[i], 0.42);
                vpida.push_back(trkdedx[i] * std::pow(trkres[i], 0.42));
                used_trkres++;
            }
            if (trkdedx[i] > 100 || trkdedx[i] < threshold)
                continue; // protect against large pulse height
            
            int bin = dEdxRangeProton->FindBin(trkres[i]);
            if (bin >= 1 && bin <= dEdxRangeProton->GetNbinsX()) 
            {
                
                double bincpro = dEdxRangeProton->GetBinContent(bin);
                if (bincpro < 1e-6) 
                { 
                    // for 0 bin content, using neighboring bins
                    bincpro = 
                        (dEdxRangeProton->GetBinContent(bin - 1) + dEdxRangeProton->GetBinContent(bin + 1)) / 2;
                }
                
                double bincka = dEdxRangeKaon->GetBinContent(bin);
                if (bincka < 1e-6)
                    bincka =
                        (dEdxRangeKaon->GetBinContent(bin - 1) + dEdxRangeKaon->GetBinContent(bin + 1)) / 2;

                double bincpi = dEdxRangePion->GetBinContent(bin);
                if (bincpi < 1e-6)
                    bincpi =
                        (dEdxRangePion->GetBinContent(bin - 1) + dEdxRangePion->GetBinContent(bin + 1)) / 2;
                
                double bincmu = dEdxRangeMuon->GetBinContent(bin);
                if (bincmu < 1e-6)
                    bincmu =
                        (dEdxRangeMuon->GetBinContent(bin - 1) + dEdxRangeMuon->GetBinContent(bin + 1)) / 2;
                
                double binepro = dEdxRangeProton->GetBinError(bin);
                if (binepro < 1e-6)
                    binepro =
                        (dEdxRangeProton->GetBinError(bin - 1) + dEdxRangeProton->GetBinError(bin + 1)) / 2;

                double bineka = dEdxRangeKaon->GetBinError(bin);
                if (bineka < 1e-6)
                    bineka = (dEdxRangeKaon->GetBinError(bin - 1) + dEdxRangeKaon->GetBinError(bin + 1)) / 2;

                double binepi = dEdxRangePion->GetBinError(bin);
                if (binepi < 1e-6)
                    binepi = (dEdxRangePion->GetBinError(bin - 1) + dEdxRangePion->GetBinError(bin + 1)) / 2;

                double binemu = dEdxRangeMuon->GetBinError(bin);
                if (binemu < 1e-6)
                    binemu = (dEdxRangeMuon->GetBinError(bin - 1) + dEdxRangeMuon->GetBinError(bin + 1)) / 2;

                // double errke = 0.05*trkdedx[i];   //5% KE resolution

                double errdedx = 0.04231 + 0.0001783 * trkdedx[i] * trkdedx[i]; // resolution on dE/dx
                
                errdedx *= trkdedx[i];
                
                chi2pro += std::pow((trkdedx[i] - bincpro) / std::sqrt(std::pow(binepro, 2) + std::pow(errdedx, 2)), 2);
                chi2ka  += std::pow((trkdedx[i] - bincka)  / std::sqrt(std::pow(bineka, 2)  + std::pow(errdedx, 2)), 2);
                chi2pi  += std::pow((trkdedx[i] - bincpi)  / std::sqrt(std::pow(binepi, 2)  + std::pow(errdedx, 2)), 2);
                chi2mu  += std::pow((trkdedx[i] - bincmu)  / std::sqrt(std::pow(binemu, 2)  + std::pow(errdedx, 2)), 2);
                // std::cout<<i<<" "<<trkdedx[i]<<" "<<trkres[i]<<" "<<bincpro<<std::endl;
                ++npt;
            }
        } // hits

        return {chi2mu / npt, chi2pro / npt, chi2ka / npt, chi2pi / npt};
    } // chi2 compute

} // namespace chiSquare

#endif
