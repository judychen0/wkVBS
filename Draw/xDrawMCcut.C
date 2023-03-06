#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "../Drawlumi/CMS_lumi.C"
using namespace std;
#define nfile 11

void xDrawMCcut(Int_t year, Int_t isSM){
  ofstream ftext;
  TFile *fopen;
  TString rootname[20];
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Wg_aMCatNLO/output_ggtree.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Zg_aMCatNLO/output_ggtree.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_TT_aMCatNLO/output_ggtree.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Znunug_pt130/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/data1/GMET/ana/230222/fall17/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/ana/230222/fall17/mc/VJet/output_merge_VJet_withTT.root";
    rootname[2] = "/data1/GMET/ana/230222/fall17/mc/VG/output_merge_VG.root";
    rootname[3] = "/data1/GMET/ana/230222/fall17/mc/monoboson/output_merge_monoBoson.root";
    rootname[4] = "/data1/GMET/ana/230222/fall17/mc/diboson/output_merge_diBoson.root";
    rootname[5] = "/data1/GMET/ana/230222/fall17/mc/triboson/output_merge_triBoson.root";
    rootname[6] = "/data1/GMET/ana/230222/fall17/mc/TT/output_merge_TT.root";
    rootname[7] = "/data1/GMET/ana/230222/fall17/mc/job_UL17_Znunug_VBS/output_ggtree.root";
    rootname[8] = "/data1/GMET/ana/230222/fall17/mc/VG/output_merge_Wg.root";
    rootname[9] = "/data1/GMET/ana/230222/fall17/mc/VG/output_merge_Zg.root";
    rootname[10] = "/data1/GMET/ana/230222/fall17/mc/output_merge_SMTrue.root";
  }
  else if(year==2018){
    rootname[0] = "/data1/GMET/ana/230222/autumn18/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/ana/230222/autumn18/mc/VJet/output_merge_VJet_withTT.root";
    rootname[2] = "/data1/GMET/ana/230222/autumn18/mc/VG/output_merge_VG.root";
    rootname[3] = "/data1/GMET/ana/230222/autumn18/mc/monoboson/output_merge_monoBoson.root";
    rootname[4] = "/data1/GMET/ana/230222/autumn18/mc/diboson/output_merge_diBoson.root";
    rootname[5] = "/data1/GMET/ana/230222/autumn18/mc/triboson/output_merge_triBoson.root";
    rootname[6] = "/data1/GMET/ana/230222/autumn18/mc/TT/output_merge_TT.root";
    rootname[7] = "/data1/GMET/ana/230222/autumn18/mc/job_UL18_Znunug_VBS/output_ggtree.root";
    rootname[8] = "/data1/GMET/ana/230222/autumn18/mc/VG/output_merge_Wg.root";
    rootname[9] = "/data1/GMET/ana/230222/autumn18/mc/VG/output_merge_Zg.root";
    rootname[10] = "/data1/GMET/ana/230222/autumn18/mc/output_merge_SMTrue.root";
  }

  // Plots for cutflow efficiency
  
  TH1F *H_phoEB_pt_phopt[nfile];
  TH1F *H_phoEB_pt_MphoID[nfile];
  TH1F *H_phoEB_pt_lepveto[nfile];
  TH1F *H_phoEB_pt_MET[nfile];
  TH1F *H_phoEB_pt_dphoMETPhi[nfile];
  TH1F *H_phoEB_pt_njet[nfile];
  // if isSM == 0
  TH1F *H_phoEB_pt_jetpt[nfile];
  TH1F *H_phoEB_pt_jetjetdEta[nfile];
  TH1F *H_phoEB_pt_jetjetdPhi[nfile];
  TH1F *H_phoEB_pt_phojetdR[nfile];
  TH1F *H_phoEB_pt_ptoverMET[nfile];
  TH1F *H_phoEB_pt_mindJMETPhi[nfile];
  TH1F *H_phoEB_pt_diejtMass[nfile];
 
  TH1F *H_MET_phopt[nfile];
  TH1F *H_MET_MphoID[nfile];
  TH1F *H_MET_lepveto[nfile];
  TH1F *H_MET_MET[nfile];
  TH1F *H_MET_dphoMETPhi[nfile];
  TH1F *H_MET_njet[nfile];
  // if isSM == 0
  TH1F *H_MET_jetpt[nfile];
  TH1F *H_MET_jetjetdEta[nfile];
  TH1F *H_MET_jetjetdPhi[nfile];
  TH1F *H_MET_phojetdR[nfile];
  TH1F *H_MET_ptoverMET[nfile];
  TH1F *H_MET_mindJMETPhi[nfile];
  TH1F *H_MET_diejtMass[nfile];

  Float_t integral[20][nfile];

  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    
    H_phoEB_pt_phopt[i] = (TH1F*)fopen->Get("phoEB_pt/h_phoEB_pt_phopt");
    H_phoEB_pt_MphoID[i] = (TH1F*)fopen->Get("phoEB_pt/h_phoEB_pt_MphoID");
    H_phoEB_pt_lepveto[i] = (TH1F*)fopen->Get("phoEB_pt/h_phoEB_pt_lepveto");
    H_phoEB_pt_MET[i] = (TH1F*)fopen->Get("phoEB_pt/h_phoEB_pt_MET");
    H_phoEB_pt_dphoMETPhi[i] = (TH1F*)fopen->Get("phoEB_pt/h_phoEB_pt_dphoMETPhi");
    H_phoEB_pt_njet[i] = (TH1F*)fopen->Get("phoEB_pt/h_phoEB_pt_njet");
    // if isSM == 0
    if(isSM == 0){
      H_phoEB_pt_jetpt[i] = (TH1F*)fopen->Get("phoEB_pt/h_phoEB_pt_jetpt");
      H_phoEB_pt_jetjetdEta[i] = (TH1F*)fopen->Get("phoEB_pt/h_phoEB_pt_jetjetdEta");
      H_phoEB_pt_jetjetdPhi[i] = (TH1F*)fopen->Get("phoEB_pt/h_phoEB_pt_jetjetdPhi");
      H_phoEB_pt_phojetdR[i] = (TH1F*)fopen->Get("phoEB_pt/h_phoEB_pt_phojetdR");
      H_phoEB_pt_ptoverMET[i] = (TH1F*)fopen->Get("phoEB_pt/h_phoEB_pt_ptoverMET");
      H_phoEB_pt_mindJMETPhi[i] = (TH1F*)fopen->Get("phoEB_pt/h_phoEB_pt_mindJMETPhi");
      H_phoEB_pt_diejtMass[i] = (TH1F*)fopen->Get("phoEB_pt/h_phoEB_pt_diejtMass");
    }

    // get integral of each cut from each sample file
    integral[0][i] = H_phoEB_pt_phopt[i]->Integral(1, -1);
    integral[1][i] = H_phoEB_pt_MphoID[i]->Integral(1, -1);
    integral[2][i] = H_phoEB_pt_lepveto[i]->Integral(1, -1);
    integral[3][i] = H_phoEB_pt_MET[i]->Integral(1, -1);
    integral[4][i] = H_phoEB_pt_dphoMETPhi[i]->Integral(1, -1);
    integral[5][i] = H_phoEB_pt_njet[i]->Integral(1, -1);
    // if isSM == 0
    if(isSM == 0){
      integral[6][i] = H_phoEB_pt_jetpt[i]->Integral(1, -1);
      integral[7][i] = H_phoEB_pt_jetjetdEta[i]->Integral(1, -1);
      integral[8][i] = H_phoEB_pt_jetjetdPhi[i]->Integral(1, -1);
      integral[9][i] = H_phoEB_pt_phojetdR[i]->Integral(1, -1);
      integral[10][i] = H_phoEB_pt_ptoverMET[i]->Integral(1, -1);
      integral[11][i] = H_phoEB_pt_mindJMETPhi[i]->Integral(1, -1);
      integral[12][i] = H_phoEB_pt_diejtMass[i]->Integral(1, -1);
    }
  }

  TString ltext[20] = {"pho pT", "pho MID", "lep veto", "MET", "dphoMETPhi", "Njet", "jetPt", "jetjetdEta", "jetjetdPhi", "phojetdR", "ptoverMET", "mindJMETPhi", "dijetMass"};
  TString filetext[20] = {"GJet", "VJet", "VG", "monoboson", "diboson", "triboson", "TTgamma", "ZgVBS", "Wg", "Zg", "Truegamma"};
  // output to .txt
  ftext.open("MC_events.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }

  for(Int_t i=0; i<nfile; i++){
    ftext << "file " << filetext[i] << endl;
    for(Int_t j=0; j<13; j++){
      ftext << setw(2) << left << j << setw(5) << left << ")" << setw(12) << left << ltext[j] << setw(8) << left << integral[j][i] << endl;
    }
  }

  ftext.close();

}
