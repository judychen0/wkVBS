#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "../Drawlumi/CMS_lumi.C"
using namespace std;
#define nfile 11
#define nhisto 3

void xDrawWgCRcut(Int_t year, Int_t isSM){
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
    rootname[0] = "/data1/GMET/WgCR_MC/fall17/allmc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/WgCR_MC/fall17/allmc/VJet/output_merge_VJet.root";
    rootname[2] = "/data1/GMET/WgCR_MC/fall17/allmc/VG/output_merge_VG.root";
    rootname[3] = "/data1/GMET/WgCR_MC/fall17/allmc/monoboson/output_merge_monoBoson.root";
    rootname[4] = "/data1/GMET/WgCR_MC/fall17/allmc/diboson/output_merge_diBoson.root";
    rootname[5] = "/data1/GMET/WgCR_MC/fall17/allmc/triboson/output_merge_triBoson.root";
    rootname[6] = "/data1/GMET/WgCR_MC/fall17/allmc/TT/output_merge_TT.root";
    rootname[7] = "/data1/GMET/WgCR_MC/fall17/allmc/job_UL17_Znunug_VBS/output_ggtree.root";
    rootname[8] = "/data1/GMET/WgCR_MC/fall17/allmc/VG/output_merge_Wg.root";
    rootname[9] = "/data1/GMET/WgCR_MC/fall17/allmc/VG/output_merge_Zg.root";
    rootname[10] = "/data1/GMET/WgCR_MC/fall17/SMTrue/output_merge_SMTrue.root";
  }
  else if(year==2018){
    rootname[0] = "/data1/GMET/WgCR_MC/autumn18/allmc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/WgCR_MC/autumn18/allmc/VJet/output_merge_VJet.root";
    rootname[2] = "/data1/GMET/WgCR_MC/autumn18/allmc/VG/output_merge_VG.root";
    rootname[3] = "/data1/GMET/WgCR_MC/autumn18/allmc/monoboson/output_merge_monoBoson.root";
    rootname[4] = "/data1/GMET/WgCR_MC/autumn18/allmc/diboson/output_merge_diBoson.root";
    rootname[5] = "/data1/GMET/WgCR_MC/autumn18/allmc/triboson/output_merge_triBoson.root";
    rootname[6] = "/data1/GMET/WgCR_MC/autumn18/allmc/TT/output_merge_TT.root";
    rootname[7] = "/data1/GMET/WgCR_MC/autumn18/allmc/job_UL18_Znunug_VBS/output_ggtree.root";
    rootname[8] = "/data1/GMET/WgCR_MC/autumn18/allmc/VG/output_merge_Wg.root";
    rootname[9] = "/data1/GMET/WgCR_MC/autumn18/allmc/VG/output_merge_Zg.root";
    rootname[10] = "/data1/GMET/WgCR_MC/autumn18/SMTrue/output_merge_SMTrue.root";
  }

  // plots for each sample
  TH1F *H_dijetMass_WgCR[nfile][nhisto];
  TH1F *Hsum_dijetMass_WgCR[nfile];

  Float_t integral[nfile];

  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    for(Int_t ihis=0; ihis<nhisto; ihis++){

      H_dijetMass_WgCR[i][ihis] = (TH1F*)fopen->Get(Form("hbin_WgCR_dijetMass_cutsieie_%i", ihis+1));
      if(ihis==0) Hsum_dijetMass_WgCR[i] = (TH1F*)fopen->Get(Form("hbin_WgCR_dijetMass_cutsieie_%i", ihis+1));
      
    }
  }

  for(Int_t i=0; i<nfile; i++){
    if(i>0 && i<nfile-1){
      for(Int_t ihis=0; ihis<nhisto; ihis++){ Hsum_dijetMass_WgCR[i]->Add(H_dijetMass_WgCR[i][ihis]);}
    }
    else if(i==0){
      for(Int_t ihis=0; ihis<nhisto; ihis++){ Hsum_dijetMass_WgCR[i]->Add(H_dijetMass_WgCR[i][ihis]);}
    }
    else if(i==nfile-1){
      for(Int_t ihis=0; ihis<nhisto; ihis++){ Hsum_dijetMass_WgCR[i]->Add(H_dijetMass_WgCR[i][ihis]);}
    }
    
    integral[i] = Hsum_dijetMass_WgCR[i]->Integral(1,-1);
  }

  TString filetext[20] = {"GJet", "VJet", "VG", "monoboson", "diboson", "triboson", "TTgamma", "ZgVBS", "Wg", "Zg", "Truegamma"};

  // output to .txt
  ftext.open("WgCR_events.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }

  for(Int_t i=0; i<nfile; i++){
    ftext << "file " << filetext[i] << endl;
    ftext << setw(5) << left << ")" << setw(12) << left << integral[i] << endl;
  }

  ftext.close();
  
}
  
