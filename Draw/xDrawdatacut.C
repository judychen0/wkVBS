#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "../Drawlumi/CMS_lumi.C"

using namespace std;
//#define nfile 8
#define njet 2
#define ncolor 10

void xDrawdatacut(Int_t year, Int_t isSM){
  ofstream ftext;
  TString rootname[10];
  TFile *fopen;

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";
  
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/data/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/data1/GMET/ana/230222/fall17/data/output_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/data1/GMET/ana/230222/autumn18/data/output_ggtree.root";
  }
  
  Float_t integral[15] = {};

  // Create histos to import from file
  TH1F *H_phoEB_pt_phopt;
  TH1F *H_phoEB_pt_MphoID;
  TH1F *H_phoEB_pt_lepveto;
  TH1F *H_phoEB_pt_MET;
  TH1F *H_phoEB_pt_dphoMETPhi;
  TH1F *H_phoEB_pt_njet;
  //isSM == 0)
  TH1F *H_phoEB_pt_jetpt;
  TH1F *H_phoEB_pt_jetjetdEta;
  TH1F *H_phoEB_pt_jetjetdPhi;
  TH1F *H_phoEB_pt_phojetdR;
  TH1F *H_phoEB_pt_ptoverMET;
  TH1F *H_phoEB_pt_mindJMETPhi;
  TH1F *H_phoEB_pt_diejtMass;
  
  fopen = new TFile(rootname[0]);

  // import histos from file
  H_phoEB_pt_phopt = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_phopt"));
  H_phoEB_pt_MphoID = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_MphoID"));
  H_phoEB_pt_lepveto = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_lepveto"));
  H_phoEB_pt_MET = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_MET"));
  H_phoEB_pt_dphoMETPhi = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_dphoMETPhi"));
  H_phoEB_pt_njet = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_njet"));
  if(isSM == 0){
    H_phoEB_pt_jetpt = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_jetpt"));
    H_phoEB_pt_jetjetdEta = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_jetjetdEta"));
    H_phoEB_pt_jetjetdPhi = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_jetjetdPhi"));
    H_phoEB_pt_phojetdR = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_phojetdR"));
    H_phoEB_pt_ptoverMET = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_ptoverMET"));
    H_phoEB_pt_mindJMETPhi = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_mindJMETPhi"));
    H_phoEB_pt_diejtMass = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_diejtMass"));
  }

  // calculate remained events after every cut
  integral[0] = H_phoEB_pt_phopt->Integral(1,-1);
  integral[1] = H_phoEB_pt_MphoID->Integral(1,-1);
  integral[2] = H_phoEB_pt_lepveto->Integral(1,-1);
  integral[3] = H_phoEB_pt_MET->Integral(1,-1);
  integral[4] = H_phoEB_pt_dphoMETPhi->Integral(1,-1);
  integral[5] = H_phoEB_pt_njet->Integral(1,-1);
  if(isSM == 0){
    integral[6] = H_phoEB_pt_jetpt->Integral(1,-1);
    integral[7] = H_phoEB_pt_jetjetdEta->Integral(1,-1);
    integral[8] = H_phoEB_pt_jetjetdPhi->Integral(1,-1);
    integral[9] = H_phoEB_pt_phojetdR->Integral(1,-1);
    integral[10] = H_phoEB_pt_ptoverMET->Integral(1,-1);
    integral[11] = H_phoEB_pt_mindJMETPhi->Integral(1,-1);
    integral[12] = H_phoEB_pt_diejtMass->Integral(1,-1);
  }

  TString ltext[20] = {"pho pT", "pho MID", "lep veto", "MET", "dphoMETPhi", "Njet", "jetPt", "jetjetdEta", "jetjetdPhi", "phojetdR", "ptoverMET", "mindJMETPhi", "dijetMass"};
  
  ftext.open("dataEvents.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  ftext << "data yield" << endl;
  for(Int_t j=0; j<13; j++){
    ftext << setw(2) << left << j << setw(5) << left << ")" << setw(12) << left << ltext[j] << setw(8) << left << "yield " << integral[j] << endl;
  }
  ftext.close();
  
}
