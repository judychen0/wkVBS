#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "../Drawlumi/CMS_lumi.C"
using namespace std;
//#define nfile 4
#define nfile 8
#define ncolor 12

void xDrawCutflow_new(Int_t year, Int_t isSM){
  ofstream ftext;
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
    rootname[0] = "/data1/GMET/ana/221116/fall17/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/ana/221116/fall17/mc/VJet/output_merge_VJet.root";
    rootname[2] = "/data1/GMET/ana/221116/fall17/mc/VG/output_merge_VG.root";
    rootname[3] = "/data1/GMET/ana/221116/fall17/mc/monoboson/output_merge_monoBoson.root";
    rootname[4] = "/data1/GMET/ana/221116/fall17/mc/diboson/output_merge_diBoson.root";
    rootname[5] = "/data1/GMET/ana/221116/fall17/mc/triboson/output_merge_triBoson.root";
    rootname[6] = "/data1/GMET/ana/221116/fall17/mc/TT/output_merge_TT.root";
    rootname[7] = "/data1/GMET/ana/221116/fall17/mc/job_UL17_Znunug_VBS/output_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/data1/GMET/ana/221116/autumn18/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/ana/221116/autumn18/mc/VJet/output_merge_VJet.root";
    rootname[2] = "/data1/GMET/ana/221116/autumn18/mc/VG/output_merge_VG.root";
    rootname[3] = "/data1/GMET/ana/221116/autumn18/mc/monoboson/output_merge_monoBoson.root";
    rootname[4] = "/data1/GMET/ana/221116/autumn18/mc/diboson/output_merge_diBoson.root";
    rootname[5] = "/data1/GMET/ana/221116/autumn18/mc/triboson/output_merge_triBoson.root";
    rootname[6] = "/data1/GMET/ana/221116/autumn18/mc/TT/output_merge_TT.root";
    rootname[7] = "/data1/GMET/ana/221116/autumn18/mc/job_UL18_Znunug_VBS/output_ggtree.root";
  }

  //string hexcolor[ncolor] = {"#b23b8c", "#ffea00", "#03a9f4", "#ff9e00", "#4caf50","#DCF563", "#2F81A3", "#ff3360", "#0066ff", "#862D55"};
  string hexcolor[ncolor] = {"#b23b8c", "#ffea00", "#03a9f4", "#ff9e00", "#D9C08D", "#7638A9", "#0066ff", "#4caf50", "#DCF563", "#862D55", "#2F81A3", "#ff3360"};
  TColor *color[ncolor];
  Int_t cnum[ncolor];
  for(Int_t j=0; j<ncolor; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  }

  // Create histos to import from file
  TH1F *H_phoEB_pt_phopt[nfile];
  TH1F *H_phoEB_pt_MphoID[nfile];
  TH1F *H_phoEB_pt_lepveto[nfile];
  TH1F *H_phoEB_pt_MET[nfile];
  TH1F *H_phoEB_pt_dphoMETPhi[nfile];
  TH1F *H_phoEB_pt_njet[nfile];
  //isSM == 0)
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
  //isSM == 0)
  TH1F *H_MET_jetpt[nfile];
  TH1F *H_MET_jetjetdEta[nfile];
  TH1F *H_MET_jetjetdPhi[nfile];
  TH1F *H_MET_phojetdR[nfile];
  TH1F *H_MET_ptoverMET[nfile];
  TH1F *H_MET_mindJMETPhi[nfile];
  TH1F *H_MET_diejtMass[nfile];

  // TGraphAsymmErrors for efficiency plots
  TGraphAsymmErrors *tgAE_phoEB_pt_phopt[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_MphoID[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_lepveto[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_MET[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_dphoMETPhi[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_njet[nfile];
  //isSM == 0)
  TGraphAsymmErrors *tgAE_phoEB_pt_jetpt[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_jetjetdEta[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_jetjetdPhi[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_phojetdR[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_ptoverMET[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_mindJMETPhi[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_diejtMass[nfile];

  TGraphAsymmErrors *tgAE_MET_phopt[nfile];
  TGraphAsymmErrors *tgAE_MET_MphoID[nfile];
  TGraphAsymmErrors *tgAE_MET_lepveto[nfile];
  TGraphAsymmErrors *tgAE_MET_MET[nfile];
  TGraphAsymmErrors *tgAE_MET_dphoMETPhi[nfile];
  TGraphAsymmErrors *tgAE_MET_njet[nfile];
  //isSM == 0)
  TGraphAsymmErrors *tgAE_MET_jetpt[nfile];
  TGraphAsymmErrors *tgAE_MET_jetjetdEta[nfile];
  TGraphAsymmErrors *tgAE_MET_jetjetdPhi[nfile];
  TGraphAsymmErrors *tgAE_MET_phojetdR[nfile];
  TGraphAsymmErrors *tgAE_MET_ptoverMET[nfile];
  TGraphAsymmErrors *tgAE_MET_mindJMETPhi[nfile];
  TGraphAsymmErrors *tgAE_MET_diejtMass[nfile];

  // create legend for efficiency plots
  TLegend *ltgAE = new TLegend(0.65,0.65, 0.75, 0.75);
  ltgAE->SetBorderSize(0);
  ltgAE->SetFillStyle(0);

  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");

  const char *title;
  const char *saveto = ".";

  Float_t integral[20][nfile];

  // phoEB_pt
  // MET
  for(Int_t ii=0; ii<nfile; ii++){
    fopen = new TFile(rootname[ii]);

    // import histos from file
    H_phoEB_pt_phopt[ii] = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_phopt"));
    H_phoEB_pt_MphoID[ii] = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_MphoID"));
    H_phoEB_pt_lepveto[ii] = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_lepveto"));
    H_phoEB_pt_MET[ii] = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_MET"));
    H_phoEB_pt_dphoMETPhi[ii] = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_dphoMETPhi"));
    H_phoEB_pt_njet[ii] = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_njet"));
    if(isSM == 0){
      H_phoEB_pt_jetpt[ii] = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_jetpt"));
      H_phoEB_pt_jetjetdEta[ii] = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_jetjetdEta"));
      H_phoEB_pt_jetjetdPhi[ii] = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_jetjetdPhi"));
      H_phoEB_pt_phojetdR[ii] = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_phojetdR"));
      H_phoEB_pt_ptoverMET[ii] = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_ptoverMET"));
      H_phoEB_pt_mindJMETPhi[ii] = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_mindJMETPhi"));
      H_phoEB_pt_diejtMass[ii] = (TH1F*)fopen->Get(Form("phoEB_pt/h_phoEB_pt_diejtMass"));
    }
    
    H_MET_phopt[ii] = (TH1F*)fopen->Get(Form("MET/h_MET_phopt"));
    H_MET_MphoID[ii] = (TH1F*)fopen->Get(Form("MET/h_MET_MphoID"));
    H_MET_lepveto[ii] = (TH1F*)fopen->Get(Form("MET/h_MET_lepveto"));
    H_MET_MET[ii] = (TH1F*)fopen->Get(Form("MET/h_MET_MET"));
    H_MET_dphoMETPhi[ii] = (TH1F*)fopen->Get(Form("MET/h_MET_dphoMETPhi"));
    H_MET_njet[ii] = (TH1F*)fopen->Get(Form("MET/h_MET_njet"));
    if(isSM == 0){
      H_MET_jetpt[ii] = (TH1F*)fopen->Get(Form("MET/h_MET_jetpt"));
      H_MET_jetjetdEta[ii] = (TH1F*)fopen->Get(Form("MET/h_MET_jetjetdEta"));
      H_MET_jetjetdPhi[ii] = (TH1F*)fopen->Get(Form("MET/h_MET_jetjetdPhi"));
      H_MET_phojetdR[ii] = (TH1F*)fopen->Get(Form("MET/h_MET_phojetdR"));
      H_MET_ptoverMET[ii] = (TH1F*)fopen->Get(Form("MET/h_MET_ptoverMET"));
      H_MET_mindJMETPhi[ii] = (TH1F*)fopen->Get(Form("MET/h_MET_mindJMETPhi"));
      H_MET_diejtMass[ii] = (TH1F*)fopen->Get(Form("MET/h_MET_diejtMass"));
    }
    
    // initialize tgAE plot
    tgAE_phoEB_pt_phopt[ii] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_MphoID[ii] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_lepveto[ii] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_MET[ii] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_dphoMETPhi[ii] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_njet[ii] = new TGraphAsymmErrors();
    //isSM == 0)
    tgAE_phoEB_pt_jetpt[ii] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_jetjetdEta[ii] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_jetjetdPhi[ii] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_phojetdR[ii] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_ptoverMET[ii] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_mindJMETPhi[ii] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_diejtMass[ii] = new TGraphAsymmErrors();

    tgAE_MET_phopt[ii] = new TGraphAsymmErrors();
    tgAE_MET_MphoID[ii] = new TGraphAsymmErrors();
    tgAE_MET_lepveto[ii] = new TGraphAsymmErrors();
    tgAE_MET_MET[ii] = new TGraphAsymmErrors();
    tgAE_MET_dphoMETPhi[ii] = new TGraphAsymmErrors();
    tgAE_MET_njet[ii] = new TGraphAsymmErrors();
    //isSM == 0)
    tgAE_MET_jetpt[ii] = new TGraphAsymmErrors();
    tgAE_MET_jetjetdEta[ii] = new TGraphAsymmErrors();
    tgAE_MET_jetjetdPhi[ii] = new TGraphAsymmErrors();
    tgAE_MET_phojetdR[ii] = new TGraphAsymmErrors();
    tgAE_MET_ptoverMET[ii] = new TGraphAsymmErrors();
    tgAE_MET_mindJMETPhi[ii] = new TGraphAsymmErrors();
    tgAE_MET_diejtMass[ii] = new TGraphAsymmErrors();

    // clear under/overflow to avoid -nan when dividing histos
    H_phoEB_pt_phopt[ii]->ClearUnderflowAndOverflow();
    H_phoEB_pt_MphoID[ii]->ClearUnderflowAndOverflow();
    H_phoEB_pt_lepveto[ii]->ClearUnderflowAndOverflow();
    H_phoEB_pt_MET[ii]->ClearUnderflowAndOverflow();
    H_phoEB_pt_dphoMETPhi[ii]->ClearUnderflowAndOverflow();
    H_phoEB_pt_njet[ii]->ClearUnderflowAndOverflow();
    if(isSM == 0){
      H_phoEB_pt_jetpt[ii]->ClearUnderflowAndOverflow();
      H_phoEB_pt_jetjetdEta[ii]->ClearUnderflowAndOverflow();
      H_phoEB_pt_jetjetdPhi[ii]->ClearUnderflowAndOverflow();
      H_phoEB_pt_phojetdR[ii]->ClearUnderflowAndOverflow();
      H_phoEB_pt_ptoverMET[ii]->ClearUnderflowAndOverflow();
      H_phoEB_pt_mindJMETPhi[ii]->ClearUnderflowAndOverflow();
      H_phoEB_pt_diejtMass[ii]->ClearUnderflowAndOverflow();
    }
    
    H_MET_phopt[ii]->ClearUnderflowAndOverflow();
    H_MET_MphoID[ii]->ClearUnderflowAndOverflow();
    H_MET_lepveto[ii]->ClearUnderflowAndOverflow();
    H_MET_MET[ii]->ClearUnderflowAndOverflow();
    H_MET_dphoMETPhi[ii]->ClearUnderflowAndOverflow();
    H_MET_njet[ii]->ClearUnderflowAndOverflow();
    if(isSM == 0){
      H_MET_jetpt[ii]->ClearUnderflowAndOverflow();
      H_MET_jetjetdEta[ii]->ClearUnderflowAndOverflow();
      H_MET_jetjetdPhi[ii]->ClearUnderflowAndOverflow();
      H_MET_phojetdR[ii]->ClearUnderflowAndOverflow();
      H_MET_ptoverMET[ii]->ClearUnderflowAndOverflow();
      H_MET_mindJMETPhi[ii]->ClearUnderflowAndOverflow();
      H_MET_diejtMass[ii]->ClearUnderflowAndOverflow();
    }

    // calculate remained events after every cut
    integral[0][ii] = H_phoEB_pt_phopt[ii]->Integral(1,-1);
    integral[1][ii] = H_phoEB_pt_MphoID[ii]->Integral(1,-1);
    integral[2][ii] = H_phoEB_pt_lepveto[ii]->Integral(1,-1);
    integral[3][ii] = H_phoEB_pt_MET[ii]->Integral(1,-1);
    integral[4][ii] = H_phoEB_pt_dphoMETPhi[ii]->Integral(1,-1);
    integral[5][ii] = H_phoEB_pt_njet[ii]->Integral(1,-1);
    if(isSM == 0){
      integral[6][ii] = H_phoEB_pt_jetpt[ii]->Integral(1,-1);
      integral[7][ii] = H_phoEB_pt_jetjetdEta[ii]->Integral(1,-1);
      integral[8][ii] = H_phoEB_pt_jetjetdPhi[ii]->Integral(1,-1);
      integral[9][ii] = H_phoEB_pt_phojetdR[ii]->Integral(1,-1);
      integral[10][ii] = H_phoEB_pt_ptoverMET[ii]->Integral(1,-1);
      integral[11][ii] = H_phoEB_pt_mindJMETPhi[ii]->Integral(1,-1);
      integral[12][ii] = H_phoEB_pt_diejtMass[ii]->Integral(1,-1);
    }
    
    // division for efficiency plot
    //tgAE_phoEB_pt_phopt[ii]->Divide(H_phoEB_pt_phopt[ii], H_phoEB_pt_phopt[ii]);
    tgAE_phoEB_pt_MphoID[ii]->Divide(H_phoEB_pt_MphoID[ii], H_phoEB_pt_phopt[ii]);
    tgAE_phoEB_pt_lepveto[ii]->Divide(H_phoEB_pt_lepveto[ii], H_phoEB_pt_phopt[ii]);
    tgAE_phoEB_pt_MET[ii]->Divide(H_phoEB_pt_MET[ii], H_phoEB_pt_phopt[ii]);
    tgAE_phoEB_pt_dphoMETPhi[ii]->Divide(H_phoEB_pt_dphoMETPhi[ii], H_phoEB_pt_phopt[ii]);
    tgAE_phoEB_pt_njet[ii]->Divide(H_phoEB_pt_njet[ii], H_phoEB_pt_phopt[ii]);
    if(isSM == 0){
      tgAE_phoEB_pt_jetpt[ii]->Divide(H_phoEB_pt_jetpt[ii], H_phoEB_pt_phopt[ii]);
      tgAE_phoEB_pt_jetjetdEta[ii]->Divide(H_phoEB_pt_jetjetdEta[ii], H_phoEB_pt_phopt[ii]);
      tgAE_phoEB_pt_jetjetdPhi[ii]->Divide(H_phoEB_pt_jetjetdPhi[ii], H_phoEB_pt_phopt[ii]);
      tgAE_phoEB_pt_phojetdR[ii]->Divide(H_phoEB_pt_phojetdR[ii], H_phoEB_pt_phopt[ii]);
      tgAE_phoEB_pt_ptoverMET[ii]->Divide(H_phoEB_pt_ptoverMET[ii], H_phoEB_pt_phopt[ii]);
      tgAE_phoEB_pt_mindJMETPhi[ii]->Divide(H_phoEB_pt_mindJMETPhi[ii], H_phoEB_pt_phopt[ii]);
      tgAE_phoEB_pt_diejtMass[ii]->Divide(H_phoEB_pt_diejtMass[ii], H_phoEB_pt_phopt[ii]);
    }

    //tgAE_MET_phopt[ii]->Divide(H_MET_phopt[ii], H_MET_phopt[ii]);
    tgAE_MET_MphoID[ii]->Divide(H_MET_MphoID[ii], H_MET_phopt[ii]);
    tgAE_MET_lepveto[ii]->Divide(H_MET_lepveto[ii], H_MET_phopt[ii]);
    tgAE_MET_MET[ii]->Divide(H_MET_MET[ii], H_MET_phopt[ii]);
    tgAE_MET_dphoMETPhi[ii]->Divide(H_MET_dphoMETPhi[ii], H_MET_phopt[ii]);
    tgAE_MET_njet[ii]->Divide(H_MET_njet[ii], H_MET_phopt[ii]);
    if(isSM == 0){
      tgAE_MET_jetpt[ii]->Divide(H_MET_jetpt[ii], H_MET_phopt[ii]);
      tgAE_MET_jetjetdEta[ii]->Divide(H_MET_jetjetdEta[ii], H_MET_phopt[ii]);
      tgAE_MET_jetjetdPhi[ii]->Divide(H_MET_jetjetdPhi[ii], H_MET_phopt[ii]);
      tgAE_MET_phojetdR[ii]->Divide(H_MET_phojetdR[ii], H_MET_phopt[ii]);
      tgAE_MET_ptoverMET[ii]->Divide(H_MET_ptoverMET[ii], H_MET_phopt[ii]);
      tgAE_MET_mindJMETPhi[ii]->Divide(H_MET_mindJMETPhi[ii], H_MET_phopt[ii]);
      tgAE_MET_diejtMass[ii]->Divide(H_MET_diejtMass[ii], H_MET_phopt[ii]);
    }

    // set tgAE Name and title
    tgAE_phoEB_pt_MphoID[ii]->SetNameTitle(Form("tgAE_phoEB_pteff_MphoID_%i", ii), "");
    tgAE_phoEB_pt_lepveto[ii]->SetNameTitle(Form("tgAE_phoEB_pteff_lepveto_%i", ii), "");
    tgAE_phoEB_pt_MET[ii]->SetNameTitle(Form("tgAE_phoEB_pteff_MET_%i", ii), "");
    tgAE_phoEB_pt_dphoMETPhi[ii]->SetNameTitle(Form("tgAE_phoEB_pteff_dphoMETPhi_%i", ii), "");
    tgAE_phoEB_pt_njet[ii]->SetNameTitle(Form("tgAE_phoEB_pteff_njet_%i", ii), "");
    if(isSM == 0){
      tgAE_phoEB_pt_jetpt[ii]->SetNameTitle(Form("tgAE_phoEB_pteff_jetpt_%i", ii), "");
      tgAE_phoEB_pt_jetjetdEta[ii]->SetNameTitle(Form("tgAE_phoEB_pteff_jetjetdEta_%i", ii), "");
      tgAE_phoEB_pt_jetjetdPhi[ii]->SetNameTitle(Form("tgAE_phoEB_pteff_jetjetdPhi_%i", ii), "");
      tgAE_phoEB_pt_phojetdR[ii]->SetNameTitle(Form("tgAE_phoEB_pteff_phojetdR_%i", ii), "");
      tgAE_phoEB_pt_ptoverMET[ii]->SetNameTitle(Form("tgAE_phoEB_pteff_ptoverMET_%i", ii), "");
      tgAE_phoEB_pt_mindJMETPhi[ii]->SetNameTitle(Form("tgAE_phoEB_pteff_mindJMETPhi_%i", ii), "");
      tgAE_phoEB_pt_diejtMass[ii]->SetNameTitle(Form("tgAE_phoEB_pteff_diejtMass_%i", ii), "");
    }

    tgAE_MET_MphoID[ii]->SetNameTitle(Form("tgAE_METeff_MphoID_%i", ii), "");
    tgAE_MET_lepveto[ii]->SetNameTitle(Form("tgAE_METeff_lepveto_%i", ii), "");
    tgAE_MET_MET[ii]->SetNameTitle(Form("tgAE_METeff_MET_%i", ii), "");
    tgAE_MET_dphoMETPhi[ii]->SetNameTitle(Form("tgAE_METeff_dphoMETPhi_%i", ii), "");
    tgAE_MET_njet[ii]->SetNameTitle(Form("tgAE_METeff_njet_%i", ii), "");
    if(isSM == 0){
      tgAE_MET_jetpt[ii]->SetNameTitle(Form("tgAE_METeff_jetpt_%i", ii), "");
      tgAE_MET_jetjetdEta[ii]->SetNameTitle(Form("tgAE_METeff_jetjetdEta_%i", ii), "");
      tgAE_MET_jetjetdPhi[ii]->SetNameTitle(Form("tgAE_METeff_jetjetdPhi_%i", ii), "");
      tgAE_MET_phojetdR[ii]->SetNameTitle(Form("tgAE_METeff_phojetdR_%i", ii), "");
      tgAE_MET_ptoverMET[ii]->SetNameTitle(Form("tgAE_METeff_ptoverMET_%i", ii), "");
      tgAE_MET_mindJMETPhi[ii]->SetNameTitle(Form("tgAE_METeff_mindJMETPhi_%i", ii), "");
      tgAE_MET_diejtMass[ii]->SetNameTitle(Form("tgAE_METeff_diejtMass_%i", ii), "");
    }

    // tgAE detail settings
    ///
    tgAE_phoEB_pt_MphoID[ii]->SetMarkerStyle(24);
    tgAE_phoEB_pt_lepveto[ii]->SetMarkerStyle(24);
    tgAE_phoEB_pt_MET[ii]->SetMarkerStyle(24);
    tgAE_phoEB_pt_dphoMETPhi[ii]->SetMarkerStyle(24);
    tgAE_phoEB_pt_njet[ii]->SetMarkerStyle(24);
    if(isSM == 0){
      tgAE_phoEB_pt_jetpt[ii]->SetMarkerStyle(24);
      tgAE_phoEB_pt_jetjetdEta[ii]->SetMarkerStyle(24);
      tgAE_phoEB_pt_jetjetdPhi[ii]->SetMarkerStyle(24);
      tgAE_phoEB_pt_phojetdR[ii]->SetMarkerStyle(24);
      tgAE_phoEB_pt_ptoverMET[ii]->SetMarkerStyle(24);
      tgAE_phoEB_pt_mindJMETPhi[ii]->SetMarkerStyle(24);
      tgAE_phoEB_pt_diejtMass[ii]->SetMarkerStyle(24);
    }

    tgAE_MET_MphoID[ii]->SetMarkerStyle(24);
    tgAE_MET_lepveto[ii]->SetMarkerStyle(24);
    tgAE_MET_MET[ii]->SetMarkerStyle(24);
    tgAE_MET_dphoMETPhi[ii]->SetMarkerStyle(24);
    tgAE_MET_njet[ii]->SetMarkerStyle(24);
    if(isSM == 0){
      tgAE_MET_jetpt[ii]->SetMarkerStyle(24);
      tgAE_MET_jetjetdEta[ii]->SetMarkerStyle(24);
      tgAE_MET_jetjetdPhi[ii]->SetMarkerStyle(24);
      tgAE_MET_phojetdR[ii]->SetMarkerStyle(24);
      tgAE_MET_ptoverMET[ii]->SetMarkerStyle(24);
      tgAE_MET_mindJMETPhi[ii]->SetMarkerStyle(24);
      tgAE_MET_diejtMass[ii]->SetMarkerStyle(24);
    }

    ///
    tgAE_phoEB_pt_MphoID[ii]->SetMarkerSize(0.9);
    tgAE_phoEB_pt_lepveto[ii]->SetMarkerSize(0.9);
    tgAE_phoEB_pt_MET[ii]->SetMarkerSize(0.9);
    tgAE_phoEB_pt_dphoMETPhi[ii]->SetMarkerSize(0.9);
    tgAE_phoEB_pt_njet[ii]->SetMarkerSize(0.9);
    if(isSM == 0){
      tgAE_phoEB_pt_jetpt[ii]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_jetjetdEta[ii]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_jetjetdPhi[ii]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_phojetdR[ii]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_ptoverMET[ii]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_mindJMETPhi[ii]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_diejtMass[ii]->SetMarkerSize(0.9);
    }

    tgAE_MET_MphoID[ii]->SetMarkerSize(0.9);
    tgAE_MET_lepveto[ii]->SetMarkerSize(0.9);
    tgAE_MET_MET[ii]->SetMarkerSize(0.9);
    tgAE_MET_dphoMETPhi[ii]->SetMarkerSize(0.9);
    tgAE_MET_njet[ii]->SetMarkerSize(0.9);
    if(isSM == 0){
      tgAE_MET_jetpt[ii]->SetMarkerSize(0.9);
      tgAE_MET_jetjetdEta[ii]->SetMarkerSize(0.9);
      tgAE_MET_jetjetdPhi[ii]->SetMarkerSize(0.9);
      tgAE_MET_phojetdR[ii]->SetMarkerSize(0.9);
      tgAE_MET_ptoverMET[ii]->SetMarkerSize(0.9);
      tgAE_MET_mindJMETPhi[ii]->SetMarkerSize(0.9);
      tgAE_MET_diejtMass[ii]->SetMarkerSize(0.9);
    }

    ///
    tgAE_phoEB_pt_MphoID[ii]->SetMarkerColor(cnum[0]);
    tgAE_phoEB_pt_lepveto[ii]->SetMarkerColor(cnum[1]);
    tgAE_phoEB_pt_MET[ii]->SetMarkerColor(cnum[2]);
    tgAE_phoEB_pt_dphoMETPhi[ii]->SetMarkerColor(cnum[3]);
    tgAE_phoEB_pt_njet[ii]->SetMarkerColor(cnum[4]);
    if(isSM == 0){
      tgAE_phoEB_pt_jetpt[ii]->SetMarkerColor(cnum[5]);
      tgAE_phoEB_pt_jetjetdEta[ii]->SetMarkerColor(cnum[6]);
      tgAE_phoEB_pt_jetjetdPhi[ii]->SetMarkerColor(cnum[7]);
      tgAE_phoEB_pt_phojetdR[ii]->SetMarkerColor(cnum[8]);
      tgAE_phoEB_pt_ptoverMET[ii]->SetMarkerColor(cnum[9]);
      tgAE_phoEB_pt_mindJMETPhi[ii]->SetMarkerColor(cnum[10]);
      tgAE_phoEB_pt_diejtMass[ii]->SetMarkerColor(cnum[11]);
    }

    tgAE_MET_MphoID[ii]->SetMarkerColor(cnum[0]);
    tgAE_MET_lepveto[ii]->SetMarkerColor(cnum[1]);
    tgAE_MET_MET[ii]->SetMarkerColor(cnum[2]);
    tgAE_MET_dphoMETPhi[ii]->SetMarkerColor(cnum[3]);
    tgAE_MET_njet[ii]->SetMarkerColor(cnum[4]);
    if(isSM == 0){
      tgAE_MET_jetpt[ii]->SetMarkerColor(cnum[5]);
      tgAE_MET_jetjetdEta[ii]->SetMarkerColor(cnum[6]);
      tgAE_MET_jetjetdPhi[ii]->SetMarkerColor(cnum[7]);
      tgAE_MET_phojetdR[ii]->SetMarkerColor(cnum[8]);
      tgAE_MET_ptoverMET[ii]->SetMarkerColor(cnum[9]);
      tgAE_MET_mindJMETPhi[ii]->SetMarkerColor(cnum[10]);
      tgAE_MET_diejtMass[ii]->SetMarkerColor(cnum[11]);
    }  
  }

  // create multigraph to paste all the tgAE
  TMultiGraph *mg[nfile];
  TMultiGraph *mg_MET[nfile];
  TLegend *lmg = new TLegend(0.18,0.30, 0.28, 0.75);
  lmg->SetTextSize(0.028);

  TF1 *fitresult = new TF1("fitresult", "pol0", 0, 500); //Et75
  Double_t yinter[15][nfile], err[15][nfile];

  writeExtraText = true;
  Int_t period;
  if(year==2016){
    period = 1;
  }
  else if(year==2017){
    period = 2;
  }
  else if(year==2018){
    period = 3;
  }

  for(Int_t ii=0; ii<nfile; ii++){
    mg[ii] = new TMultiGraph();
    mg_MET[ii] = new TMultiGraph();

    mg[ii]->Add(tgAE_phoEB_pt_MphoID[ii]);
    mg[ii]->Add(tgAE_phoEB_pt_lepveto[ii]);
    mg[ii]->Add(tgAE_phoEB_pt_MET[ii]);
    if(isSM == 1)mg[ii]->Add(tgAE_phoEB_pt_dphoMETPhi[ii]);
    mg[ii]->Add(tgAE_phoEB_pt_njet[ii]);
    if(isSM == 0){
      mg[ii]->Add(tgAE_phoEB_pt_jetpt[ii]);
      mg[ii]->Add(tgAE_phoEB_pt_jetjetdEta[ii]);
      mg[ii]->Add(tgAE_phoEB_pt_jetjetdPhi[ii]);
      mg[ii]->Add(tgAE_phoEB_pt_phojetdR[ii]);
      mg[ii]->Add(tgAE_phoEB_pt_ptoverMET[ii]);
      mg[ii]->Add(tgAE_phoEB_pt_mindJMETPhi[ii]);
      mg[ii]->Add(tgAE_phoEB_pt_diejtMass[ii]);
    }

    mg[ii]->Draw("APE");
    mg[ii]->GetXaxis()->SetTitle("p_{T}^{#gamma}#[]{GeV}");
    mg[ii]->GetYaxis()->SetTitle("Efficiency");
    //mg[ii]->GetXaxis()->SetLimits(-100, 500); // pho
    mg[ii]->GetXaxis()->SetLimits(-200, 500); // MET
    mg[ii]->SetMaximum(1.05);
    mg[ii]->SetMinimum(0);
    //if(i==0) mg[i]->SetMaximum(0.055);
    mg[ii]->SetTitle(Form("cutflow_%i", ii));    

    lmg->Clear();
    lmg->AddEntry(tgAE_phoEB_pt_MphoID[ii], "Medium ID", "PE");
    lmg->AddEntry(tgAE_phoEB_pt_lepveto[ii], "leptonveto", "PE");
    lmg->AddEntry(tgAE_phoEB_pt_MET[ii], "MET > 180GeV", "PE");
    if(isSM == 1) lmg->AddEntry(tgAE_phoEB_pt_dphoMETPhi[ii], "|#Delta#phi(#gamma, MET)| > 1.2", "PE");
    lmg->AddEntry(tgAE_phoEB_pt_njet[ii], "Njet", "PE");
    if(isSM == 0){
      lmg->AddEntry(tgAE_phoEB_pt_jetpt[ii], "jet pT", "PE");
      lmg->AddEntry(tgAE_phoEB_pt_jetjetdEta[ii], "#Delta#eta(j_{1},j_{2}) > 2.5", "PE");
      lmg->AddEntry(tgAE_phoEB_pt_jetjetdPhi[ii], "|#Delta#phi(j_{1},j_{2})| < 2.7", "PE");
      lmg->AddEntry(tgAE_phoEB_pt_phojetdR[ii], "#DeltaR(#gamma,jet)>0.5", "PE");
      lmg->AddEntry(tgAE_phoEB_pt_ptoverMET[ii], "P_{T}^{#gamma}/MET < 2.4", "PE");
      lmg->AddEntry(tgAE_phoEB_pt_mindJMETPhi[ii], "|#Delta#phi_{min}(jet, MET)| > 0.5", "PE");
      lmg->AddEntry(tgAE_phoEB_pt_diejtMass[ii], "Mass_{jj} > 250GeV", "PE");
    }
    lmg->Draw("SAME");
    title = mg[ii]->GetTitle();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    
    mg_MET[ii]->Add(tgAE_MET_MphoID[ii]);
    mg_MET[ii]->Add(tgAE_MET_lepveto[ii]);
    mg_MET[ii]->Add(tgAE_MET_MET[ii]);
    if(isSM == 1)mg_MET[ii]->Add(tgAE_MET_dphoMETPhi[ii]);
    mg_MET[ii]->Add(tgAE_MET_njet[ii]);
    if(isSM == 0){
      mg_MET[ii]->Add(tgAE_MET_jetpt[ii]);
      mg_MET[ii]->Add(tgAE_MET_jetjetdEta[ii]);
      mg_MET[ii]->Add(tgAE_MET_jetjetdPhi[ii]);
      mg_MET[ii]->Add(tgAE_MET_phojetdR[ii]);
      mg_MET[ii]->Add(tgAE_MET_ptoverMET[ii]);
      mg_MET[ii]->Add(tgAE_MET_mindJMETPhi[ii]);
      mg_MET[ii]->Add(tgAE_MET_diejtMass[ii]);
    }

    mg_MET[ii]->Draw("APE");
    mg_MET[ii]->GetXaxis()->SetTitle("MET#[]{GeV}");
    mg_MET[ii]->GetYaxis()->SetTitle("Efficiency");
    //mg_MET[ii]->GetXaxis()->SetLimits(-1, 500); // pho
    mg_MET[ii]->GetXaxis()->SetLimits(-200, 500); // MET
    mg_MET[ii]->SetMaximum(1.05);
    mg_MET[ii]->SetMinimum(0);
    //if(i==0) mg[i]->SetMaximum(0.055);
    mg_MET[ii]->SetTitle(Form("cutflowMET_%i", ii));    

    lmg->Clear();
    lmg->AddEntry(tgAE_MET_MphoID[ii], "Medium ID", "PE");
    lmg->AddEntry(tgAE_MET_lepveto[ii], "leptonveto", "PE");
    lmg->AddEntry(tgAE_MET_MET[ii], "MET > 180GeV", "PE");
    if(isSM == 1) lmg->AddEntry(tgAE_MET_dphoMETPhi[ii], "|#Delta#phi(#gamma, MET)| > 1.2", "PE");
    lmg->AddEntry(tgAE_MET_njet[ii], "Njet", "PE");
    if(isSM == 0){
      lmg->AddEntry(tgAE_MET_jetpt[ii], "jet pT", "PE");
      lmg->AddEntry(tgAE_MET_jetjetdEta[ii], "#Delta#eta(j_{1},j_{2}) > 2.5", "PE");
      lmg->AddEntry(tgAE_MET_jetjetdPhi[ii], "|#Delta#phi(j_{1},j_{2})| < 2.7", "PE");
      lmg->AddEntry(tgAE_MET_phojetdR[ii], "#DeltaR(#gamma,jet)>0.5", "PE");
      lmg->AddEntry(tgAE_MET_ptoverMET[ii], "P_{T}^{#gamma}/MET < 2.4", "PE");
      lmg->AddEntry(tgAE_MET_mindJMETPhi[ii], "|#Delta#phi_{min}(jet, MET)| > 0.5", "PE");
      lmg->AddEntry(tgAE_MET_diejtMass[ii], "Mass_{jj} > 250GeV", "PE");
    }
    lmg->Draw("SAME");
    title = mg_MET[ii]->GetTitle();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    // fit with horizontal line to get efficiency
    tgAE_phoEB_pt_MphoID[ii]->Fit(fitresult, "", "", 0, 500);
    yinter[0][ii] = fitresult->GetParameter(0);
    tgAE_phoEB_pt_lepveto[ii]->Fit(fitresult, "", "", 0, 500);
    yinter[1][ii] = fitresult->GetParameter(0);
    tgAE_phoEB_pt_MET[ii]->Fit(fitresult, "", "", 0, 500);
    yinter[2][ii] = fitresult->GetParameter(0);
    tgAE_phoEB_pt_dphoMETPhi[ii]->Fit(fitresult, "", "", 0, 500);
    yinter[3][ii] = fitresult->GetParameter(0);
    tgAE_phoEB_pt_njet[ii]->Fit(fitresult, "", "", 0, 500);
    yinter[4][ii] = fitresult->GetParameter(0);
    if(isSM == 0){
      tgAE_phoEB_pt_jetpt[ii]->Fit(fitresult, "", "", 0, 500);
      yinter[5][ii] = fitresult->GetParameter(0);
      tgAE_phoEB_pt_jetjetdEta[ii]->Fit(fitresult, "", "", 0, 500);
      yinter[6][ii] = fitresult->GetParameter(0);
      tgAE_phoEB_pt_jetjetdPhi[ii]->Fit(fitresult, "", "", 0, 500);
      yinter[7][ii] = fitresult->GetParameter(0);
      tgAE_phoEB_pt_phojetdR[ii]->Fit(fitresult, "", "", 0, 500);
      yinter[8][ii] = fitresult->GetParameter(0);
      tgAE_phoEB_pt_ptoverMET[ii]->Fit(fitresult, "", "", 0, 500);
      yinter[9][ii] = fitresult->GetParameter(0);
      tgAE_phoEB_pt_mindJMETPhi[ii]->Fit(fitresult, "", "", 0, 500);
      yinter[10][ii] = fitresult->GetParameter(0);
      tgAE_phoEB_pt_diejtMass[ii]->Fit(fitresult, "", "", 0, 500);
      yinter[11][ii] = fitresult->GetParameter(0);
    }  
  }

  TString ltext[20] = {"pho MID", "lep veto", "MET", "dphoMETPhi", "Njet", "jetPt", "jetjetdEta", "jetjetdPhi", "phojetdR", "ptoverMET", "mindJMETPhi", "dijetMass"};

  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  for(Int_t ii=0; ii<nfile; ii++){
    ftext << "file" << ii << endl;
    for(Int_t j=0; j<12; j++){
      ftext << setw(2) << left << j << setw(5) << left << ")" << setw(12) << left << ltext[j] << setw(8) << left <<  "cut eff" << setw(12) << left << yinter[j][ii] << setw(8) << left << " error " << err[j][ii] << endl;
    }
  }
  ftext.close();


  ftext.open("events.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  for(Int_t ii=0; ii<nfile; ii++){
    ftext << "file" << ii << endl;
    for(Int_t j=0; j<12; j++){
      ftext << setw(2) << left << j << setw(5) << left<< ")" << setw(12) << left << ltext[j] << setw(8) << left << "yield " << integral[j][ii] << endl;
    }
  }
  ftext.close();
}
