
#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <memory>

#include <CLI11.hpp>
#include <TFile.h>
#include <TTReeReader.h>
#include <TTreeReaderValue.h>

class DeltaThreeP {
    DeltaThreeP(); 
    void runOnFile(std::string whichFile);

private:
    TH1F* 
};

void DeltaThreeP::runOnFile(std::string whichFile)
{
    auto readerFile = TFile::Open(whichFile.c_str());
    if (!readerFile || readerFile->IsZombie())
    {
        throw std::exception("No file provided or unable to read file");
    }

    TTReeReader tpcReader("caloskimW/TrackCaloSkim", readerFile);

    TTreeReaderValue<int> 
}

int main(int argc, char **argv)
{
    CLI11::App cli{"deltaThreeP: Track studies"};

    int DNp = 4;
    std::string fileList;

    cli.add_option("-f,--file", fileList, "List of files");

    cli.add_option("-p", DNp, "DNp value");

    CLI11_PARSE(cli, argc, argv);

    std::unique_ptr<TH1F> hTime(new TH1F("Time", "", 2000, 0, 0));
    std::unique_ptr<TH1F> hEventNumber(new TH1F("EventNumber","",1000000, 0, 1000000));

    std::unique_ptr<TH1F> hDeltaThreePInd1(new TH1F("DeltaThreePInd1","", 200, -20.0, 20.0));
    std::unique_ptr<TH1F> hDeltaThreePInd2(new TH1F("DeltaThreePInd2","", 200, -20.0, 20.0));
    std::unique_ptr<TH1F> hDeltaThreePColl(new TH1F("DeltaThreePColl","", 200, -20.0, 20.0));

    std::unique_ptr<TH2F> hDeltaThreePInd1VsPitch(new TH2F("DeltaThreePInd1VsPitch", ";Pitch (cm) ; #delta3p (ticks)", 60, 0, 3, 200, -20.0, 20.0));
    std::unique_ptr<TH2F> hDeltaThreePInd2VsPitch(new TH2F("DeltaThreePInd2VsPitch", ";Pitch (cm) ; #delta3p (ticks)", 60, 0, 3, 200, -20.0, 20.0));
    std::unique_ptr<TH2F> hDeltaThreePCollVsPitch(new TH2F("DeltaThreePCollVsPitch", ";Pitch (cm) ; #delta3p (ticks)", 60, 0, 3, 200, -20.0, 20.0));


    
    std::unique_ptr<TH1F> hTimeDiff(new TH1F("TimeDiff", "", 2000, 0, 0));
    std::unique_ptr<TH1F> hIntegral1(new TH1F("Integral1","",3000, 0.,3000.0));
    std::unique_ptr<TH1F> hIntegral2(new TH1F("Integral2","",3000, 0.,3000.0));


    std::unique_ptr<TH2F> h_integral_m2(new TH2F("h_integral_m2"," ; Integral 1 [ADC]; Integral 2 [ADC] ",3000,0.,3000,3000, 0.,3000.0));
    std::unique_ptr<TH2F> h_integral_m2_not(new TH2F("h_integral_m2_not"," ; Integral 1 [ADC]; Integral 2 [ADC] ",3000,0.,3000,3000, 0.,3000.0));
    std::unique_ptr<TH2F> h_dqdx_mult2(new TH2F("h_dqdx_mult2","dqdx for mult 2 - Coll; dqdx bigger hit [MeV/cm] ; dqdx smaller hit [MeV/cm]",2000,0,2000,2000, 0,2000.0));


    TH1F *h_delta3p_m1=new TH1F("h_delta3p_m1","",200, -20.0,20.0);
    TH1F *h_delta3p_largep=new TH1F("h_delta3p_largp","",200, -20.0,20.0);
    TH1F *h_delta3p_smallp=new TH1F("h_delta3p_smallp","",200, -20.0,20.0);
    //Doing some slice on pitch 
    TH1F *h_delta3p_30_35=new TH1F("h_delta3p_30_35","",200, -20.0,20.0);
    TH1F *h_delta3p_35_40=new TH1F("h_delta3p_35_40","",200, -20.0,20.0);
    TH1F *h_delta3p_40_45=new TH1F("h_delta3p_40_45","",200, -20.0,20.0);
    TH1F *h_delta3p_45_50=new TH1F("h_delta3p_45_50","",200, -20.0,20.0);
    TH1F *h_delta3p_50_55=new TH1F("h_delta3p_50_55","",200, -20.0,20.0);
    TH1F *h_delta3p_55_60=new TH1F("h_delta3p_55_60","",200, -20.0,20.0);
    TH1F *h_delta3p_60_65=new TH1F("h_delta3p_60_65","",200, -20.0,20.0);
    TH1F *h_delta3p_65_70=new TH1F("h_delta3p_65_70","",200, -20.0,20.0);
    TH1F *h_delta3p_70_75=new TH1F("h_delta3p_70_75","",200, -20.0,20.0);
    TH1F *h_delta3p_75_80=new TH1F("h_delta3p_75_80","",200, -20.0,20.0);
    TH1F *h_delta3p_1_05=new TH1F("h_delta3p_1_05","",200, -20.0,20.0);
    TH1F *h_delta3p_15_55=new TH1F("h_delta3p_15_55","",200, -20.0,20.0);
    TH1F *h_delta3p_2_05=new TH1F("h_delta3p_2_05","",200, -20.0,20.0);



    TH1F *h_pitch=new TH1F("h_pitch","",2000, 0,0);
    TH1F *h_dqdx=new TH1F("h_dqdx","",2000, 0,0);
    TH1F *h_dqdx_m1=new TH1F("h_dqdx_m1","",2000, 0,0);
    TH1F *h_dqdx_m2=new TH1F("h_dqdx_m2","",2000, 0,0);
    TH1F *h_dqdx_m3=new TH1F("h_dqdx_m3","",2000, 0,0);
    TH1F *h_t0v=new TH1F("h_t0v","",2000, 0,0);
    TH1F *h_t0=new TH1F("h_t0","",10, -5,5);
    TH1F *h_not0=new TH1F("h_not0","",10, -5,5);
    TH1F *hmultt=new TH1F("hmultt","",21, -10.5,10.5);
    TH1F *h_z=new TH1F("h_z","",4400, -1100,1100);
    TH1F *h_z_nopitch=new TH1F("h_z_nopitch","",4400,-1100,1100);
    
    DeltaThreeP delta3p;



}