#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "../Drawlumi/CMS_lumi.C"
using namespace std;
#define nfile 11
#define nhisto 3

void xDrawJFakecut(Int_t year, Int_t isSM){
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
    rootname[0] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/JetFake/output_merge_JetFake.root";
    rootname[2] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/VG/output_merge_VG.root";
    rootname[3] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/monoboson/output_merge_monoBoson.root";
    rootname[4] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/diboson/output_merge_diBoson.root";
    rootname[5] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/triboson/output_merge_triBoson.root";
    rootname[6] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/TT/output_merge_TT.root";
    rootname[7] = "/data1/GMET/Jfake_pseudoData/fall17/job_UL17_Znunug_VBS/output_ggtree.root";
    rootname[8] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/VG/output_merge_Wg.root";
    rootname[9] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/VG/output_merge_Zg.root";
    rootname[10] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/output_merge_SMTrue.root";
  }
  else if(year==2018){
    rootname[0] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/JetFake/output_merge_JetFake.root";
    rootname[2] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/VG/output_merge_VG.root";
    rootname[3] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/monoboson/output_merge_monoBoson.root";
    rootname[4] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/diboson/output_merge_diBoson.root";
    rootname[5] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/triboson/output_merge_triBoson.root";
    rootname[6] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/TT/output_merge_TT.root";
    rootname[7] = "/data1/GMET/Jfake_pseudoData/autumn18/job_UL18_Znunug_VBS/output_ggtree.root";
    rootname[8] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/VG/output_merge_Wg.root";
    rootname[9] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/VG/output_merge_Zg.root";
    rootname[10] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/output_merge_SMTrue.root";
  }

  // plots for each sample
  TH1F *H_Truecount[nfile];
  TH1F *H_Fakecount[nfile];

  Float_t True_integral[nfile];
  Float_t Fake_integral[nfile];

  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    H_Truecount[i] = (TH1F*)fopen->Get("h_TrueCount");
    H_Fakecount[i] = (TH1F*)fopen->Get("h_FakeCount");
  }

  for(Int_t i=0; i<nfile; i++){
    True_integral[i] = H_Truecount[i]->Integral(1,-1);
    Fake_integral[i] = H_Fakecount[i]->Integral(1,-1);
  }

  TString filetext[20] = {"GJet", "VJet", "VG", "monoboson", "diboson", "triboson", "TTgamma", "ZgVBS", "Wg", "Zg", "Truegamma"};

  // output to .txt
  ftext.open("JetFake_events.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }

  for(Int_t i=0; i<nfile; i++){
    ftext << "file " << filetext[i] << endl;
    ftext << setw(5) << left << ")" << setw(12) << left << "True : " << setw(8) << left << True_integral[i] << " Fake : " << setw(8) << left << Fake_integral[i] << endl;
  }

  ftext.close();

}
