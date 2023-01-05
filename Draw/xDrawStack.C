#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "TText.h"
#include "../Drawlumi/CMS_lumi.C"

using namespace std;
#define nfile 9
#define nhisto 2
#define njet 2
#define ncolor 20

TH1F* ratioplot(THStack* st, TH1F* h1){

  if (!st) {
       Warning("TRatioPlot", "Need only a stack");
       //return;
    }
  
  TList *stackHists = st->GetHists();
  
  if (stackHists->GetSize() == 0) {
    Warning("TRatioPlot", "Stack does not have histograms");
    //return;
  }
  
  TH1F* tmpHist = (TH1F*)stackHists->At(0)->Clone();
  tmpHist->Reset();
  
  for (int i=0;i<stackHists->GetSize();++i) {
    tmpHist->Add((TH1*)stackHists->At(i));
  }
  
    
  TH1F *ratio = (TH1F*)h1->Clone("ratio");
  ratio->SetLineColor(kBlack);
  ratio->SetMinimum(-0.4);
  ratio->SetMaximum(3.4);
  ratio->Sumw2();
  ratio->Divide(tmpHist);
  
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerSize(0.8);
  ratio->Draw("E3 P");
  
  ratio->SetTitle("");
  ratio->GetYaxis()->SetTitle("Data/MC");
  ratio->GetYaxis()->SetTitleFont(42);
  ratio->GetYaxis()->SetTitleSize(0.1);
  ratio->GetYaxis()->SetTitleOffset(0.4);
  ratio->GetYaxis()->SetLabelFont(42);
  ratio->GetYaxis()->SetLabelOffset(0.008);
  ratio->GetYaxis()->SetLabelSize(0.1);
  ratio->GetYaxis()->SetTickSize(0.032);
  
  ratio->GetXaxis()->SetTitleFont(42);
  ratio->GetXaxis()->SetTitleSize(0.1);
  ratio->GetXaxis()->SetTitleOffset(1.1);
  ratio->GetXaxis()->SetLabelFont(42);
  ratio->GetXaxis()->SetLabelOffset(0.03);
  ratio->GetXaxis()->SetLabelSize(0.1);
  ratio->GetXaxis()->SetTickSize(0.1);
  ratio->GetXaxis()->SetNdivisions(510);
  ratio->GetXaxis()->ChangeLabel(-1, -1, 0, -1, -1, -1, "");

  return ratio;
}

void xDrawStack_new(Int_t year, Int_t isSM){

  TString rootname[20];
  TFile *fopen;

  if(year==2016){
    rootname[0] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/job_summer16_Wg_aMCatNLO/output_ggtree.root";
     rootname[8] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/job_summer16_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/job_summer16_TT_aMCatNLO/output_ggtree.root";
    //rootname[10] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Znunug_pt130/output_ggtree.root";
    rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/job_UL17_Znunug_VBS/output_ggtree.root";
    //rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/job_summer16_Zg_EWK/output_ggtree.root";
    rootname[11] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/data/output_ggtree.root";
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
    rootname[8] = "/data1/GMET/ana/221116/fall17/data/output_ggtree.root";
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
    rootname[8] = "/data1/GMET/ana/221116/autumn18/data/output_ggtree.root";
  }

  // year label
  Float_t lumi16 = 36.33;
  Float_t lumi17 = 41.48;
  Float_t lumi18 = 59.83;
  Float_t lumi = 1.0;
  if(year==2016){
    lumi = lumi16;
  }
  else if(year==2017){
    lumi = lumi17;
  }
  else if(year==2018){
    lumi = lumi18;
  }

  // Build up colors
  string hexcolor[ncolor] = {"#fff176", "#03a9f4", "#ff9e00", "#b23b8c", "#a983d3", "#96ff5a", "#FFCCCC", "#67ccc1"};
  TColor *color[ncolor];
  Int_t cnum[ncolor];
   
  for(Int_t j=0; j<ncolor; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  } 

  TCanvas *c1 = new TCanvas("c1");

  const char *title;
  const char *saveto = ".";

  // plots for each sample
  TH1F *H_phopt_cut[nfile];
  TH1F *H_phoEta_cut[nfile];
  TH1F *H_phoPhi_cut[nfile];
  TH1F *H_MET_cut[nfile];
  TH1F *H_METPhi_cut[nfile];
  TH1F *H_njet_cut[nfile];
  TH1F *H_nvtx_cut[nfile];
  TH1F *H_dPhi_phoMET[nfile];
  // if isSM == 0
  TH1F *H_jetpt_cut[nfile][2];
  TH1F *H_jetEta_cut[nfile][2];
  TH1F *H_jetPhi_cut[nfile][2];
  TH1F *H_dEta_phojet[nfile];  
  TH1F *H_dPhi_phojet[nfile];
  TH1F *H_dR_phojet[nfile];
  TH1F *H_dEta_jetjet[nfile];  
  TH1F *H_dPhi_jetjet[nfile];
  TH1F *H_dR_jetjet[nfile];
  TH1F *H_dPhi_minjetMET[nfile];  
  TH1F *H_ptoverMET_cut[nfile];
  TH1F *H_dijetMass_cut[nfile];

  // plots of MC sum
  TH1F *HA_phopt_cut;
  TH1F *HA_phoEta_cut;
  TH1F *HA_phoPhi_cut;
  TH1F *HA_MET_cut;
  TH1F *HA_METPhi_cut;
  TH1F *HA_njet_cut;
  TH1F *HA_nvtx_cut;
  TH1F *HA_dPhi_phoMET;
  // if isSM == 0
  TH1F *HA_jetpt_cut[2];
  TH1F *HA_jetEta_cut[2];
  TH1F *HA_jetPhi_cut[2];
  TH1F *HA_dEta_phojet;  
  TH1F *HA_dPhi_phojet;
  TH1F *HA_dR_phojet;
  TH1F *HA_dEta_jetjet;  
  TH1F *HA_dPhi_jetjet;
  TH1F *HA_dR_jetjet;
  TH1F *HA_dPhi_minjetMET;  
  TH1F *HA_ptoverMET_cut;
  TH1F *HA_dijetMass_cut;

  // plots of sig MC
  TH1F *HSig_phopt_cut;
  TH1F *HSig_phoEta_cut;
  TH1F *HSig_phoPhi_cut;
  TH1F *HSig_MET_cut;
  TH1F *HSig_METPhi_cut;
  TH1F *HSig_njet_cut;
  TH1F *HSig_nvtx_cut;
  TH1F *HSig_dPhi_phoMET;
  // if isSM == 0
  TH1F *HSig_jetpt_cut[2];
  TH1F *HSig_jetEta_cut[2];
  TH1F *HSig_jetPhi_cut[2];
  TH1F *HSig_dEta_phojet;  
  TH1F *HSig_dPhi_phojet;
  TH1F *HSig_dR_phojet;
  TH1F *HSig_dEta_jetjet;  
  TH1F *HSig_dPhi_jetjet;
  TH1F *HSig_dR_jetjet;
  TH1F *HSig_dPhi_minjetMET;  
  TH1F *HSig_ptoverMET_cut;
  TH1F *HSig_dijetMass_cut;

  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);

    // Get first plot for MC sum
    if(i==0){
      HA_phopt_cut = (TH1F*)fopen->Get("h_phopt_cut")->Clone();
      HA_phoEta_cut = (TH1F*)fopen->Get("h_phoEta_cut")->Clone();
      HA_phoPhi_cut = (TH1F*)fopen->Get("h_phoPhi_cut")->Clone();
      HA_MET_cut = (TH1F*)fopen->Get("h_MET_cut")->Clone();
      HA_METPhi_cut = (TH1F*)fopen->Get("h_METPhi_cut")->Clone();
      HA_njet_cut = (TH1F*)fopen->Get("h_njet_cut")->Clone();
      HA_nvtx_cut = (TH1F*)fopen->Get("h_nvtx_cut")->Clone();
      HA_dPhi_phoMET = (TH1F*)fopen->Get("h_dPhi_phoMET")->Clone();
      // if isSM == 0
      if(isSM == 0){
	for(Int_t jj=0; jj<2; jj++){
	  HA_jetpt_cut[jj] = (TH1F*)fopen->Get(Form("h_jetpt_cut_%i", jj))->Clone();
 	  HA_jetEta_cut[jj] = (TH1F*)fopen->Get(Form("h_jetEta_cut_%i", jj))->Clone();
	  HA_jetPhi_cut[jj] = (TH1F*)fopen->Get(Form("h_jetPhi_cut_%i", jj))->Clone();
	}
	HA_dEta_phojet = (TH1F*)fopen->Get("h_dEta_phojet")->Clone();  
	HA_dPhi_phojet = (TH1F*)fopen->Get("h_dPhi_phojet")->Clone();
	HA_dR_phojet = (TH1F*)fopen->Get("h_dR_phojet")->Clone();
	HA_dEta_jetjet = (TH1F*)fopen->Get("h_dEta_jetjet")->Clone();  
	HA_dPhi_jetjet = (TH1F*)fopen->Get("h_dPhi_jetjet")->Clone();
	HA_dR_jetjet = (TH1F*)fopen->Get("h_dR_jetjet")->Clone();
	HA_dPhi_minjetMET = (TH1F*)fopen->Get("h_dPhi_minjetMET")->Clone();  
	HA_ptoverMET_cut = (TH1F*)fopen->Get("h_ptoverMET_cut")->Clone();
	HA_dijetMass_cut = (TH1F*)fopen->Get("h_dijetMass_cut")->Clone();
      }

      // fill settings
      HA_phopt_cut->SetFillStyle(3145);
      HA_phoEta_cut->SetFillStyle(3145);
      HA_phoPhi_cut->SetFillStyle(3145);
      HA_MET_cut->SetFillStyle(3145);
      HA_METPhi_cut->SetFillStyle(3145);
      HA_njet_cut->SetFillStyle(3145);
      HA_nvtx_cut->SetFillStyle(3145);
      HA_dPhi_phoMET->SetFillStyle(3145);
      // if isSM == 0
      if(isSM == 0){
	for(Int_t jj=0; jj<2; jj++){
	  HA_jetpt_cut[jj]->SetFillStyle(3145);
	  HA_jetEta_cut[jj]->SetFillStyle(3145);
	  HA_jetPhi_cut[jj]->SetFillStyle(3145);
	}
	HA_dEta_phojet->SetFillStyle(3145);
	HA_dPhi_phojet->SetFillStyle(3145);
	HA_dR_phojet->SetFillStyle(3145);
	HA_dEta_jetjet->SetFillStyle(3145);
	HA_dPhi_jetjet->SetFillStyle(3145);
	HA_dR_jetjet->SetFillStyle(3145);
	HA_dPhi_minjetMET->SetFillStyle(3145);
	HA_ptoverMET_cut->SetFillStyle(3145);
	HA_dijetMass_cut->SetFillStyle(3145);
      }

      // fill color settings
      HA_phopt_cut->SetFillColor(12);
      HA_phoEta_cut->SetFillColor(12);
      HA_phoPhi_cut->SetFillColor(12);
      HA_MET_cut->SetFillColor(12);
      HA_METPhi_cut->SetFillColor(12);
      HA_njet_cut->SetFillColor(12);
      HA_nvtx_cut->SetFillColor(12);
      HA_dPhi_phoMET->SetFillColor(12);
      // if isSM == 0
      if(isSM == 0){
	for(Int_t jj=0; jj<2; jj++){
	  HA_jetpt_cut[jj]->SetFillColor(12);
	  HA_jetEta_cut[jj]->SetFillColor(12);
	  HA_jetPhi_cut[jj]->SetFillColor(12);
	}
	HA_dEta_phojet->SetFillColor(12);
	HA_dPhi_phojet->SetFillColor(12);
	HA_dR_phojet->SetFillColor(12);
	HA_dEta_jetjet->SetFillColor(12);
	HA_dPhi_jetjet->SetFillColor(12);
	HA_dR_jetjet->SetFillColor(12);
	HA_dPhi_minjetMET->SetFillColor(12);
	HA_ptoverMET_cut->SetFillColor(12);
	HA_dijetMass_cut->SetFillColor(12);
      }
    }

    // Get sig MC plot
    if(i==nfile-2){
      HSig_phopt_cut = (TH1F*)fopen->Get("h_phopt_cut")->Clone();
      HSig_phoEta_cut = (TH1F*)fopen->Get("h_phoEta_cut")->Clone();
      HSig_phoPhi_cut = (TH1F*)fopen->Get("h_phoPhi_cut")->Clone();
      HSig_MET_cut = (TH1F*)fopen->Get("h_MET_cut")->Clone();
      HSig_METPhi_cut = (TH1F*)fopen->Get("h_METPhi_cut")->Clone();
      HSig_njet_cut = (TH1F*)fopen->Get("h_njet_cut")->Clone();
      HSig_nvtx_cut = (TH1F*)fopen->Get("h_nvtx_cut")->Clone();
      HSig_dPhi_phoMET = (TH1F*)fopen->Get("h_dPhi_phoMET")->Clone();
      // if isSM == 0
      if(isSM == 0){
	for(Int_t jj=0; jj<2; jj++){
	  HSig_jetpt_cut[jj] = (TH1F*)fopen->Get(Form("h_jetpt_cut_%i", jj))->Clone();
	  HSig_jetEta_cut[jj] = (TH1F*)fopen->Get(Form("h_jetEta_cut_%i", jj))->Clone();
	  HSig_jetPhi_cut[jj] = (TH1F*)fopen->Get(Form("h_jetPhi_cut_%i", jj))->Clone();
	}
	HSig_dEta_phojet = (TH1F*)fopen->Get("h_dEta_phojet")->Clone();  
	HSig_dPhi_phojet = (TH1F*)fopen->Get("h_dPhi_phojet")->Clone();
	HSig_dR_phojet = (TH1F*)fopen->Get("h_dR_phojet")->Clone();
	HSig_dEta_jetjet = (TH1F*)fopen->Get("h_dEta_jetjet")->Clone();  
	HSig_dPhi_jetjet = (TH1F*)fopen->Get("h_dPhi_jetjet")->Clone();
	HSig_dR_jetjet = (TH1F*)fopen->Get("h_dR_jetjet")->Clone();
	HSig_dPhi_minjetMET = (TH1F*)fopen->Get("h_dPhi_minjetMET")->Clone();  
	HSig_ptoverMET_cut = (TH1F*)fopen->Get("h_ptoverMET_cut")->Clone();
	HSig_dijetMass_cut = (TH1F*)fopen->Get("h_dijetMass_cut")->Clone();
      }

      // line color setting
      HSig_phopt_cut->SetLineColor(kRed);
      HSig_phoEta_cut->SetLineColor(kRed);
      HSig_phoPhi_cut->SetLineColor(kRed);
      HSig_MET_cut->SetLineColor(kRed);
      HSig_METPhi_cut->SetLineColor(kRed);
      HSig_njet_cut->SetLineColor(kRed);
      HSig_nvtx_cut->SetLineColor(kRed);
      HSig_dPhi_phoMET->SetLineColor(kRed);
      // if isSM == 0
      if(isSM == 0){
	for(Int_t jj=0; jj<2; jj++){
	  HSig_jetpt_cut[jj]->SetLineColor(kRed);
	  HSig_jetEta_cut[jj]->SetLineColor(kRed);
	  HSig_jetPhi_cut[jj]->SetLineColor(kRed);
	}
	HSig_dEta_phojet->SetLineColor(kRed);
	HSig_dPhi_phojet->SetLineColor(kRed);
	HSig_dR_phojet->SetLineColor(kRed);
	HSig_dEta_jetjet->SetLineColor(kRed);
	HSig_dPhi_jetjet->SetLineColor(kRed);
	HSig_dR_jetjet->SetLineColor(kRed);
	HSig_dPhi_minjetMET->SetLineColor(kRed);
	HSig_ptoverMET_cut->SetLineColor(kRed);
	HSig_dijetMass_cut->SetLineColor(kRed);
      }

      // line width settings
      HSig_phopt_cut->SetLineWidth(2);
      HSig_phoEta_cut->SetLineWidth(2);
      HSig_phoPhi_cut->SetLineWidth(2);
      HSig_MET_cut->SetLineWidth(2);
      HSig_METPhi_cut->SetLineWidth(2);
      HSig_njet_cut->SetLineWidth(2);
      HSig_nvtx_cut->SetLineWidth(2);
      HSig_dPhi_phoMET->SetLineWidth(2);
      // if isSM == 0
      if(isSM == 0){
	for(Int_t jj=0; jj<2; jj++){
	  HSig_jetpt_cut[jj]->SetLineWidth(2);
	  HSig_jetEta_cut[jj]->SetLineWidth(2);
	  HSig_jetPhi_cut[jj]->SetLineWidth(2);
	}
	HSig_dEta_phojet->SetLineWidth(2);
	HSig_dPhi_phojet->SetLineWidth(2);
	HSig_dR_phojet->SetLineWidth(2);
	HSig_dEta_jetjet->SetLineWidth(2);
	HSig_dPhi_jetjet->SetLineWidth(2);
	HSig_dR_jetjet->SetLineWidth(2);
	HSig_dPhi_minjetMET->SetLineWidth(2);
	HSig_ptoverMET_cut->SetLineWidth(2);
	HSig_dijetMass_cut->SetLineWidth(2);
      }

    }

    // Get plots from every file
    H_phopt_cut[i] = (TH1F*)fopen->Get("h_phopt_cut");
    H_phoEta_cut[i] = (TH1F*)fopen->Get("h_phoEta_cut");
    H_phoPhi_cut[i] = (TH1F*)fopen->Get("h_phoPhi_cut");
    H_MET_cut[i] = (TH1F*)fopen->Get("h_MET_cut");
    H_METPhi_cut[i] = (TH1F*)fopen->Get("h_METPhi_cut");
    H_njet_cut[i] = (TH1F*)fopen->Get("h_njet_cut");
    H_nvtx_cut[i] = (TH1F*)fopen->Get("h_nvtx_cut");
    H_dPhi_phoMET[i] = (TH1F*)fopen->Get("h_dPhi_phoMET");
    // if isSM == 0
    if(isSM == 0){
      for(Int_t jj=0; jj<2; jj++){
	H_jetpt_cut[i][jj] = (TH1F*)fopen->Get(Form("h_jetpt_cut_%i", jj));
	H_jetEta_cut[i][jj] = (TH1F*)fopen->Get(Form("h_jetEta_cut_%i", jj));
	H_jetPhi_cut[i][jj] = (TH1F*)fopen->Get(Form("h_jetPhi_cut_%i", jj));
      }
      H_dEta_phojet[i] = (TH1F*)fopen->Get("h_dEta_phojet"); 
      H_dPhi_phojet[i] = (TH1F*)fopen->Get("h_dPhi_phojet");
      H_dR_phojet[i] = (TH1F*)fopen->Get("h_dR_phojet");
      H_dEta_jetjet[i] = (TH1F*)fopen->Get("h_dEta_jetjet"); 
      H_dPhi_jetjet[i] = (TH1F*)fopen->Get("h_dPhi_jetjet");
      H_dR_jetjet[i] = (TH1F*)fopen->Get("h_dR_jetjet");
      H_dPhi_minjetMET[i] = (TH1F*)fopen->Get("h_dPhi_minjetMET"); 
      H_ptoverMET_cut[i] = (TH1F*)fopen->Get("h_ptoverMET_cut");
      H_dijetMass_cut[i] = (TH1F*)fopen->Get("h_dijetMass_cut");
    }

    // Set fill color for each sample (except data)
    if(i<nfile-1){
      H_phopt_cut[i]->SetFillColor(cnum[i]);
      H_phoEta_cut[i]->SetFillColor(cnum[i]);
      H_phoPhi_cut[i]->SetFillColor(cnum[i]);
      H_MET_cut[i]->SetFillColor(cnum[i]);
      H_METPhi_cut[i]->SetFillColor(cnum[i]);
      H_njet_cut[i]->SetFillColor(cnum[i]);
      H_nvtx_cut[i]->SetFillColor(cnum[i]);
      H_dPhi_phoMET[i]->SetFillColor(cnum[i]);
      // if isSM == 0
      if(isSM == 0){
	for(Int_t jj=0; jj<2; jj++){
	  H_jetpt_cut[i][jj]->SetFillColor(cnum[i]);
	  H_jetEta_cut[i][jj]->SetFillColor(cnum[i]);
	  H_jetPhi_cut[i][jj]->SetFillColor(cnum[i]);
	}
	H_dEta_phojet[i]->SetFillColor(cnum[i]);
	H_dPhi_phojet[i]->SetFillColor(cnum[i]);
	H_dR_phojet[i]->SetFillColor(cnum[i]);
	H_dEta_jetjet[i]->SetFillColor(cnum[i]);
	H_dPhi_jetjet[i]->SetFillColor(cnum[i]);
	H_dR_jetjet[i]->SetFillColor(cnum[i]);
	H_dPhi_minjetMET[i]->SetFillColor(cnum[i]);
	H_ptoverMET_cut[i]->SetFillColor(cnum[i]);
	H_dijetMass_cut[i]->SetFillColor(cnum[i]);
      }
    }

    // // scale VJet sample to 32.56 events
    // if(i==1){
    //   H_phopt_cut[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    //   H_phoEta_cut[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    //   H_phoPhi_cut[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    //   H_MET_cut[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    //   H_METPhi_cut[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    //   H_njet_cut[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    //   H_nvtx_cut[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    //   H_dPhi_phoMET[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    //   // if isSM == 0
    //   if(isSM == 0){
    // 	for(Int_t jj=0; jj<2; jj++){
    // 	  H_jetpt_cut[i][jj]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    // 	  H_jetEta_cut[i][jj]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    // 	  H_jetPhi_cut[i][jj]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    // 	}
    // 	H_dEta_phojet[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    // 	H_dPhi_phojet[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    // 	H_dR_phojet[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    // 	H_dEta_jetjet[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    // 	H_dPhi_jetjet[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    // 	H_dR_jetjet[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    // 	H_dPhi_minjetMET[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    // 	H_ptoverMET_cut[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    // 	H_dijetMass_cut[i]->Scale(32.56/H_phopt_cut[i]->Integral(1, -1));
    //   }
    // }

    cout << " file " << i << endl;
    // Set Line Color for each sample
    H_phopt_cut[i]->SetLineColor(1);
    H_phoEta_cut[i]->SetLineColor(1);
    H_phoPhi_cut[i]->SetLineColor(1);
    H_MET_cut[i]->SetLineColor(1);
    H_METPhi_cut[i]->SetLineColor(1);
    H_njet_cut[i]->SetLineColor(1);
    H_nvtx_cut[i]->SetLineColor(1);
    H_dPhi_phoMET[i]->SetLineColor(1);
    // if isSM == 0
    if(isSM == 0){
      for(Int_t jj=0; jj<2; jj++){
	H_jetpt_cut[i][jj]->SetLineColor(1);
	H_jetEta_cut[i][jj]->SetLineColor(1);
	H_jetPhi_cut[i][jj]->SetLineColor(1);
      }
      H_dEta_phojet[i]->SetLineColor(1);
      H_dPhi_phojet[i]->SetLineColor(1);
      H_dR_phojet[i]->SetLineColor(1);
      H_dEta_jetjet[i]->SetLineColor(1);
      H_dPhi_jetjet[i]->SetLineColor(1);
      H_dR_jetjet[i]->SetLineColor(1);
      H_dPhi_minjetMET[i]->SetLineColor(1);
      H_ptoverMET_cut[i]->SetLineColor(1);
      H_dijetMass_cut[i]->SetLineColor(1);
    }

    // Set properties for data plots
    if(i == nfile-1){

      // Set Line width
      H_phopt_cut[i]->SetLineWidth(2);
      H_phoEta_cut[i]->SetLineWidth(2);
      H_phoPhi_cut[i]->SetLineWidth(2);
      H_MET_cut[i]->SetLineWidth(2);
      H_METPhi_cut[i]->SetLineWidth(2);
      H_njet_cut[i]->SetLineWidth(2);
      H_nvtx_cut[i]->SetLineWidth(2);
      H_dPhi_phoMET[i]->SetLineWidth(2);
      // if isSM == 0
      if(isSM == 0){
	for(Int_t jj=0; jj<2; jj++){
	  H_jetpt_cut[i][jj]->SetLineWidth(2);
	  H_jetEta_cut[i][jj]->SetLineWidth(2);
	  H_jetPhi_cut[i][jj]->SetLineWidth(2);
	}
	H_dEta_phojet[i]->SetLineWidth(2);
	H_dPhi_phojet[i]->SetLineWidth(2);
	H_dR_phojet[i]->SetLineWidth(2);
	H_dEta_jetjet[i]->SetLineWidth(2);
	H_dPhi_jetjet[i]->SetLineWidth(2);
	H_dR_jetjet[i]->SetLineWidth(2);
	H_dPhi_minjetMET[i]->SetLineWidth(2);
	H_ptoverMET_cut[i]->SetLineWidth(2);
	H_dijetMass_cut[i]->SetLineWidth(2);
      }

      // Set Marker style
      H_phopt_cut[i]->SetMarkerStyle(8);
      H_phoEta_cut[i]->SetMarkerStyle(8);
      H_phoPhi_cut[i]->SetMarkerStyle(8);
      H_MET_cut[i]->SetMarkerStyle(8);
      H_METPhi_cut[i]->SetMarkerStyle(8);
      H_njet_cut[i]->SetMarkerStyle(8);
      H_nvtx_cut[i]->SetMarkerStyle(8);
      H_dPhi_phoMET[i]->SetMarkerStyle(8);
      // if isSM == 0
      if(isSM == 0){
	for(Int_t jj=0; jj<2; jj++){
	  H_jetpt_cut[i][jj]->SetMarkerStyle(8);
	  H_jetEta_cut[i][jj]->SetMarkerStyle(8);
	  H_jetPhi_cut[i][jj]->SetMarkerStyle(8);
	}
	H_dEta_phojet[i]->SetMarkerStyle(8);
	H_dPhi_phojet[i]->SetMarkerStyle(8);
	H_dR_phojet[i]->SetMarkerStyle(8);
	H_dEta_jetjet[i]->SetMarkerStyle(8);
	H_dPhi_jetjet[i]->SetMarkerStyle(8);
	H_dR_jetjet[i]->SetMarkerStyle(8);
	H_dPhi_minjetMET[i]->SetMarkerStyle(8);
	H_ptoverMET_cut[i]->SetMarkerStyle(8);
	H_dijetMass_cut[i]->SetMarkerStyle(8);
      }

      // Set Marker Size
      H_phopt_cut[i]->SetMarkerSize(0.9);
      H_phoEta_cut[i]->SetMarkerSize(0.9);
      H_phoPhi_cut[i]->SetMarkerSize(0.9);
      H_MET_cut[i]->SetMarkerSize(0.9);
      H_METPhi_cut[i]->SetMarkerSize(0.9);
      H_njet_cut[i]->SetMarkerSize(0.9);
      H_nvtx_cut[i]->SetMarkerSize(0.9);
      H_dPhi_phoMET[i]->SetMarkerSize(0.9);
      // if isSM == 0
      if(isSM == 0){
	for(Int_t jj=0; jj<2; jj++){
	  H_jetpt_cut[i][jj]->SetMarkerSize(0.9);
	  H_jetEta_cut[i][jj]->SetMarkerSize(0.9);
	  H_jetPhi_cut[i][jj]->SetMarkerSize(0.9);
	}
	H_dEta_phojet[i]->SetMarkerSize(0.9);
	H_dPhi_phojet[i]->SetMarkerSize(0.9);
	H_dR_phojet[i]->SetMarkerSize(0.9);
	H_dEta_jetjet[i]->SetMarkerSize(0.9);
	H_dPhi_jetjet[i]->SetMarkerSize(0.9);
	H_dR_jetjet[i]->SetMarkerSize(0.9);
	H_dPhi_minjetMET[i]->SetMarkerSize(0.9);
	H_ptoverMET_cut[i]->SetMarkerSize(0.9);
	H_dijetMass_cut[i]->SetMarkerSize(0.9);
      }
    }
    
  }

  for(Int_t i=0; i<nfile-1; i++){
    if(i>0){
      // add HA plots from other samples
      HA_phopt_cut->Add(H_phopt_cut[i]);
      HA_phoEta_cut->Add(H_phoEta_cut[i]);
      HA_phoPhi_cut->Add(H_phoPhi_cut[i]);
      HA_MET_cut->Add(H_MET_cut[i]);
      HA_METPhi_cut->Add(H_METPhi_cut[i]);
      HA_njet_cut->Add(H_njet_cut[i]);
      HA_nvtx_cut->Add(H_nvtx_cut[i]);
      HA_dPhi_phoMET->Add(H_dPhi_phoMET[i]);
      // if isSM == 0
      if(isSM == 0){
	for(Int_t jj=0; jj<2; jj++){
	  HA_jetpt_cut[jj]->Add(H_jetpt_cut[i][jj]);
	  HA_jetEta_cut[jj]->Add(H_jetEta_cut[i][jj]);
	  HA_jetPhi_cut[jj]->Add(H_jetPhi_cut[i][jj]);
	}
	HA_dEta_phojet->Add(H_dEta_phojet[i]);
	HA_dPhi_phojet->Add(H_dPhi_phojet[i]);
	HA_dR_phojet->Add(H_dR_phojet[i]);
	HA_dEta_jetjet->Add(H_dEta_jetjet[i]);
	HA_dPhi_jetjet->Add(H_dPhi_jetjet[i]);
	HA_dR_jetjet->Add(H_dR_jetjet[i]);
	HA_dPhi_minjetMET->Add(H_dPhi_minjetMET[i]);
	HA_ptoverMET_cut->Add(H_ptoverMET_cut[i]);
	HA_dijetMass_cut->Add(H_dijetMass_cut[i]);
      }
    }
  }

  // create stack plots
  THStack *HS_phopt_cut;
  THStack *HS_phoEta_cut;
  THStack *HS_phoPhi_cut;
  THStack *HS_MET_cut;
  THStack *HS_METPhi_cut;
  THStack *HS_njet_cut;
  THStack *HS_nvtx_cut;
  THStack *HS_dPhi_phoMET;
  // if isSM == 0
  THStack *HS_jetpt_cut[2];
  THStack *HS_jetEta_cut[2];
  THStack *HS_jetPhi_cut[2];
  THStack *HS_dEta_phojet;  
  THStack *HS_dPhi_phojet;
  THStack *HS_dR_phojet;
  THStack *HS_dEta_jetjet;  
  THStack *HS_dPhi_jetjet;
  THStack *HS_dR_jetjet;
  THStack *HS_dPhi_minjetMET;  
  THStack *HS_ptoverMET_cut;
  THStack *HS_dijetMass_cut;

  HS_phopt_cut = new THStack("HS_phopt_cut", "HS_phopt_cut");
  HS_phoEta_cut = new THStack("HS_phoEta_cut", "HS_phoEta_cut");
  HS_phoPhi_cut = new THStack("HS_phoPhi_cut", "HS_phoPhi_cut");
  HS_MET_cut = new THStack("HS_MET_cut", "HS_MET_cut");
  HS_METPhi_cut = new THStack("HS_METPhi_cut", "HS_METPhi_cut");
  HS_njet_cut = new THStack("HS_njet_cut", "HS_njet_cut");
  HS_nvtx_cut = new THStack("HS_nvtx_cut", "HS_nvtx_cut");
  HS_dPhi_phoMET = new THStack("HS_dPhi_phoMET", "HS_dPhi_phoMET");
  // if isSM == 0
  if(isSM == 0){
    for(Int_t jj=0; jj<2; jj++){
      HS_jetpt_cut[jj] = new THStack(Form("HS_jetpt_cut_%i", jj), "");
      HS_jetEta_cut[jj] = new THStack(Form("HS_jetEta_cut_%i", jj), "");
      HS_jetPhi_cut[jj] = new THStack(Form("HS_jetPhi_cut_%i", jj), "");
    }
    HS_dEta_phojet = new THStack("HS_dEta_phojet", "HS_dEta_phojet");  
    HS_dPhi_phojet = new THStack("HS_dPhi_phojet", "HS_dPhi_phojet");
    HS_dR_phojet = new THStack("HS_dR_phojet", "HS_dR_phojet");
    HS_dEta_jetjet = new THStack("HS_dEta_jetjet", "HS_dEta_jetjet");  
    HS_dPhi_jetjet = new THStack("HS_dPhi_jetjet", "HS_dPhi_jetjet");
    HS_dR_jetjet = new THStack("HS_dR_jetjet", "HS_dR_jetjet");
    HS_dPhi_minjetMET = new THStack("HS_dPhi_minjetMET", "HS_dPhi_minjetMET");  
    HS_ptoverMET_cut = new THStack("HS_ptoverMET_cut", "HS_ptoverMET_cut");
    HS_dijetMass_cut = new THStack("HS_dijetMass_cut", "HS_dijetMass_cut");
  }

  for(Int_t i=0; i<nfile-1; i++){
    HS_phopt_cut->Add(H_phopt_cut[i]);
    HS_phoEta_cut->Add(H_phoEta_cut[i]);
    HS_phoPhi_cut->Add(H_phoPhi_cut[i]);
    HS_MET_cut->Add(H_MET_cut[i]);
    HS_METPhi_cut->Add(H_METPhi_cut[i]);
    HS_njet_cut->Add(H_njet_cut[i]);
    HS_nvtx_cut->Add(H_nvtx_cut[i]);
    HS_dPhi_phoMET->Add(H_dPhi_phoMET[i]);
    // if isSM == 0
    if(isSM == 0){
      for(Int_t jj=0; jj<2; jj++){
	HS_jetpt_cut[jj]->Add(H_jetpt_cut[i][jj]);
	HS_jetEta_cut[jj]->Add(H_jetEta_cut[i][jj]);
	HS_jetPhi_cut[jj]->Add(H_jetPhi_cut[i][jj]);
      }
      HS_dEta_phojet->Add(H_dEta_phojet[i]);
      HS_dPhi_phojet->Add(H_dPhi_phojet[i]);
      HS_dR_phojet->Add(H_dR_phojet[i]);
      HS_dEta_jetjet->Add(H_dEta_jetjet[i]);
      HS_dPhi_jetjet->Add(H_dPhi_jetjet[i]);
      HS_dR_jetjet->Add(H_dR_jetjet[i]);
      HS_dPhi_minjetMET->Add(H_dPhi_minjetMET[i]);
      HS_ptoverMET_cut->Add(H_ptoverMET_cut[i]);
      HS_dijetMass_cut->Add(H_dijetMass_cut[i]);
    }
  }

  TLegend *lhs = new TLegend(0.50, 0.66, 0.90, 0.90);//VBS
  lhs->SetTextSize(0.035);
  lhs->SetFillStyle(0);
  TString ltext[20] = {"#gamma+jet", "V+jets", "V#gamma+jets", "Monoboson", "Diboson", "Triboson", "tt#gamma", "Z#gamma#rightarrow#nu#nu#gamma(VBS)", "DATA"};//Znunug VBS

  // Data/MC event yield text
  Int_t dataIntegral =  H_MET_cut[nfile-1]->Integral(1, -1);
  Float_t mcIntegral = HA_MET_cut->Integral(1, -1);
  TLatex *ths = new TLatex(0.51, 0.63, Form("Data/MC = %i/%.2f", dataIntegral, mcIntegral));
  ths->SetNDC();
  ths->SetTextAlign(13);
  ths->SetTextFont(42);
  ths->SetTextSize(0.04);

  TH1F *hratio;
  Double_t x[3] = {-2000, 0.0, 2000};
  Double_t y[3] = {1, 1, 1};
  TGraph *tg = new TGraph(3, x, y);
  tg->SetLineColor(kRed);
  tg->SetLineWidth(1);
  tg->SetLineStyle(8);

  TPad *pad1 = new TPad("pad1", "", 0., 0.3, 1., 1.);
  pad1->SetBottomMargin(0);
  pad1->SetLogy(1);
  pad1->Draw();
  
  TPad *pad2 = new TPad("pad2", "", 0., 0.02, 1., 0.3);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.3);
  pad2->Draw();

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

  //save plots//
  
  pad1->cd();
  HS_phopt_cut->Draw("HIST");
  H_phopt_cut[nfile-1]->Draw("SAME");
  HSig_phopt_cut->Draw("HIST SAME");
  HA_phopt_cut->Draw("E2 SAME");
  HS_phopt_cut->GetXaxis()->SetTitle("");
  HS_phopt_cut->GetYaxis()->SetTitle("Events");
  HS_phopt_cut->GetXaxis()->SetLabelSize(0);
  HS_phopt_cut->GetXaxis()->SetRangeUser(75, 1000);
  HS_phopt_cut->SetMaximum(10000);
  HS_phopt_cut->SetMinimum(0.1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    if(ifile == nfile-1) lhs->AddEntry(H_phopt_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_phopt_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_phopt_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  ths->Draw("SAME");
  title = HS_phopt_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_phopt_cut, H_phopt_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("p_{T}^{#gamma}[GeV]");
  hratio->GetXaxis()->SetRangeUser(75, 1000);
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  pad1->cd();
  HS_phoEta_cut->Draw("HIST");
  H_phoEta_cut[nfile-1]->Draw("SAME");
  HSig_phoEta_cut->Draw("HIST SAME");
  HA_phoEta_cut->Draw("E2 SAME");
  HS_phoEta_cut->GetXaxis()->SetTitle("");
  HS_phoEta_cut->GetYaxis()->SetTitle("Events");
  HS_phoEta_cut->GetXaxis()->SetLabelSize(0);
  HS_phoEta_cut->SetMaximum(100000);
  HS_phoEta_cut->SetMinimum(0.1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    if(ifile == nfile-1) lhs->AddEntry(H_phoEta_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_phoEta_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_phoEta_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  ths->Draw("SAME");
  title = HS_phoEta_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_phoEta_cut, H_phoEta_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#eta_{SC}^{#gamma}");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  pad1->cd();
  HS_phoPhi_cut->Draw("HIST");
  H_phoPhi_cut[nfile-1]->Draw("SAME");
  HSig_phoPhi_cut->Draw("HIST SAME");
  HA_phoPhi_cut->Draw("E2 SAME");
  HS_phoPhi_cut->GetXaxis()->SetTitle("");
  HS_phoPhi_cut->GetYaxis()->SetTitle("Events");
  HS_phoPhi_cut->GetXaxis()->SetLabelSize(0);
  HS_phoPhi_cut->SetMaximum(80000);
  HS_phoPhi_cut->SetMinimum(0.1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    if(ifile == nfile-1) lhs->AddEntry(H_phoPhi_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_phoPhi_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_phoPhi_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  ths->Draw("SAME");
  title = HS_phoPhi_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_phoPhi_cut, H_phoPhi_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#phi^{#gamma}[Rad]");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_MET_cut->Draw("HIST");
  H_MET_cut[nfile-1]->Draw("SAME");
  HSig_MET_cut->Draw("HIST SAME");
  HA_MET_cut->Draw("E2 SAME");
  HS_MET_cut->GetXaxis()->SetTitle("");
  HS_MET_cut->GetYaxis()->SetTitle("Events");
  HS_MET_cut->GetXaxis()->SetLabelSize(0);
  HS_MET_cut->SetMaximum(100000);
  HS_MET_cut->SetMinimum(0.1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    if(ifile == nfile-1) lhs->AddEntry(H_MET_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_MET_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_MET_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  ths->Draw("SAME");
  title = HS_MET_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_MET_cut, H_MET_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#slash{E}_{T}[GeV]");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_METPhi_cut->Draw("HIST");
  H_METPhi_cut[nfile-1]->Draw("SAME");
  HSig_METPhi_cut->Draw("HIST SAME");
  HA_METPhi_cut->Draw("E2 SAME");
  HS_METPhi_cut->GetXaxis()->SetTitle("");
  HS_METPhi_cut->GetYaxis()->SetTitle("Events");
  HS_METPhi_cut->GetXaxis()->SetLabelSize(0);
  HS_METPhi_cut->SetMaximum(10000);
  HS_METPhi_cut->SetMinimum(0.1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    if(ifile == nfile-1) lhs->AddEntry(H_METPhi_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_METPhi_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_METPhi_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  ths->Draw("SAME");
  title = HS_METPhi_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_METPhi_cut, H_METPhi_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#phi(#slash{E}_{T})[Rad]");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_njet_cut->Draw("HIST");
  H_njet_cut[nfile-1]->Draw("SAME");
  HSig_njet_cut->Draw("HIST SAME");
  HA_njet_cut->Draw("E2 SAME");
  HS_njet_cut->GetXaxis()->SetTitle("");
  HS_njet_cut->GetYaxis()->SetTitle("Events");
  HS_njet_cut->GetXaxis()->SetLabelSize(0);
  HS_njet_cut->SetMaximum(100000);
  HS_njet_cut->SetMinimum(0.1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    if(ifile == nfile-1) lhs->AddEntry(H_njet_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_njet_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_njet_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  ths->Draw("SAME");
  title = HS_njet_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_njet_cut, H_njet_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("Njet");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  // pad1->cd();
  // HS_nvtx_cut->Draw("HIST");
  // H_nvtx_cut[nfile-1]->Draw("SAME");
  // HSig_nvtx_cut->Draw("HIST SAME");
  // HA_nvtx_cut->Draw("E2 SAME");
  // HS_nvtx_cut->GetXaxis()->SetTitle("");
  // HS_nvtx_cut->GetYaxis()->SetTitle("Events");
  // HS_nvtx_cut->GetXaxis()->SetLabelSize(0);
  // HS_nvtx_cut->SetMaximum(10000);
  // HS_nvtx_cut->SetMinimum(0.1);
  // lhs->Clear();
  // lhs->SetNColumns(2);
  // for(Int_t ifile=0; ifile<nfile; ifile++){
  //   if(ifile == nfile-1) lhs->AddEntry(H_nvtx_cut[ifile], ltext[ifile], "PL");
  //   if(ifile == nfile-2) lhs->AddEntry(HSig_nvtx_cut, ltext[ifile], "L");
  //   if(ifile < nfile-1) lhs->AddEntry(H_nvtx_cut[ifile], ltext[ifile], "F");
  // }
  // lhs->Draw("SAME");
  // ths->Draw("SAME");
  // title = HS_nvtx_cut->GetName();
  // pad2->cd();
  // hratio = ratioplot(HS_nvtx_cut, H_nvtx_cut[nfile-1]);
  // hratio->SetTitle("");
  // hratio->GetXaxis()->SetTitle("Nvtx");
  // hratio->Draw("EP");
  // tg->Draw("LSAME");
  // CMS_lumi(pad1, period, 0);
  // c1->Update();
  // c1->RedrawAxis();
  // c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_dPhi_phoMET->Draw("HIST");
  H_dPhi_phoMET[nfile-1]->Draw("SAME");
  HSig_dPhi_phoMET->Draw("HIST SAME");
  HA_dPhi_phoMET->Draw("E2 SAME");
  HS_dPhi_phoMET->GetXaxis()->SetTitle("");
  HS_dPhi_phoMET->GetYaxis()->SetTitle("Events");
  HS_dPhi_phoMET->GetXaxis()->SetLabelSize(0);
  HS_dPhi_phoMET->SetMaximum(100000);
  HS_dPhi_phoMET->SetMinimum(0.1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    if(ifile == nfile-1) lhs->AddEntry(H_dPhi_phoMET[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_dPhi_phoMET, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_dPhi_phoMET[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  ths->Draw("SAME");
  title = HS_dPhi_phoMET->GetName();
  pad2->cd();
  hratio = ratioplot(HS_dPhi_phoMET, H_dPhi_phoMET[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#Delta#phi(#gamma,#slash_{E}_{T})[Rad]");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  if(isSM == 0){

    for(Int_t jj=0; jj<2; jj++){
      pad1->cd();
      HS_jetpt_cut[jj]->Draw("HIST");
      H_jetpt_cut[nfile-1][jj]->Draw("SAME");
      HSig_jetpt_cut[jj]->Draw("HIST SAME");
      HA_jetpt_cut[jj]->Draw("E2 SAME");
      HS_jetpt_cut[jj]->GetXaxis()->SetTitle("");
      HS_jetpt_cut[jj]->GetYaxis()->SetTitle("Events");
      HS_jetpt_cut[jj]->GetXaxis()->SetLabelSize(0);
      HS_jetpt_cut[jj]->SetMaximum(10000);
      HS_jetpt_cut[jj]->SetMinimum(0.1);
      lhs->Clear();
      lhs->SetNColumns(2);
      for(Int_t ifile=0; ifile<nfile; ifile++){
	if(ifile == nfile-1) lhs->AddEntry(H_jetpt_cut[ifile][jj], ltext[ifile], "PL");
	if(ifile == nfile-2) lhs->AddEntry(HSig_jetpt_cut[jj], ltext[ifile], "L");
	if(ifile < nfile-1) lhs->AddEntry(H_jetpt_cut[ifile][jj], ltext[ifile], "F");
      }
      lhs->Draw("SAME");
      ths->Draw("SAME");
      title = HS_jetpt_cut[jj]->GetName();
      pad2->cd();
      hratio = ratioplot(HS_jetpt_cut[jj], H_jetpt_cut[nfile-1][jj]);
      hratio->SetTitle("");
      hratio->GetXaxis()->SetTitle(Form("p_{T}^{jet%i}[GeV]", jj+1));
      hratio->Draw("EP");
      tg->Draw("LSAME");
      CMS_lumi(pad1, period, 0);
      c1->Update();
      c1->RedrawAxis();
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));

      pad1->cd();
      HS_jetEta_cut[jj]->Draw("HIST");
      H_jetEta_cut[nfile-1][jj]->Draw("SAME");
      HSig_jetEta_cut[jj]->Draw("HIST SAME");
      HA_jetEta_cut[jj]->Draw("E2 SAME");
      HS_jetEta_cut[jj]->GetXaxis()->SetTitle("");
      HS_jetEta_cut[jj]->GetYaxis()->SetTitle("Events");
      HS_jetEta_cut[jj]->GetXaxis()->SetLabelSize(0);
      HS_jetEta_cut[jj]->SetMaximum(10000);
      HS_jetEta_cut[jj]->SetMinimum(0.1);
      lhs->Clear();
      lhs->SetNColumns(2);
      for(Int_t ifile=0; ifile<nfile; ifile++){
	if(ifile == nfile-1) lhs->AddEntry(H_jetEta_cut[ifile][jj], ltext[ifile], "PL");
	if(ifile == nfile-2) lhs->AddEntry(HSig_jetEta_cut[jj], ltext[ifile], "L");
	if(ifile < nfile-1) lhs->AddEntry(H_jetEta_cut[ifile][jj], ltext[ifile], "F");
      }
      lhs->Draw("SAME");
      ths->Draw("SAME");
      title = HS_jetEta_cut[jj]->GetName();
      pad2->cd();
      hratio = ratioplot(HS_jetEta_cut[jj], H_jetEta_cut[nfile-1][jj]);
      hratio->SetTitle("");
      hratio->GetXaxis()->SetTitle(Form("#eta^{jet%i}", jj+1));
      hratio->Draw("EP");
      tg->Draw("LSAME");
      CMS_lumi(pad1, period, 0);
      c1->Update();
      c1->RedrawAxis();
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));

      pad1->cd();
      HS_jetPhi_cut[jj]->Draw("HIST");
      H_jetPhi_cut[nfile-1][jj]->Draw("SAME");
      HSig_jetPhi_cut[jj]->Draw("HIST SAME");
      HA_jetPhi_cut[jj]->Draw("E2 SAME");
      HS_jetPhi_cut[jj]->GetXaxis()->SetTitle("");
      HS_jetPhi_cut[jj]->GetYaxis()->SetTitle("Events");
      HS_jetPhi_cut[jj]->GetXaxis()->SetLabelSize(0);
      HS_jetPhi_cut[jj]->SetMaximum(10000);
      HS_jetPhi_cut[jj]->SetMinimum(0.1);
      lhs->Clear();
      lhs->SetNColumns(2);
      for(Int_t ifile=0; ifile<nfile; ifile++){
	if(ifile == nfile-1) lhs->AddEntry(H_jetPhi_cut[ifile][jj], ltext[ifile], "PL");
	if(ifile == nfile-2) lhs->AddEntry(HSig_jetPhi_cut[jj], ltext[ifile], "L");
	if(ifile < nfile-1) lhs->AddEntry(H_jetPhi_cut[ifile][jj], ltext[ifile], "F");
      }
      lhs->Draw("SAME");
      ths->Draw("SAME");
      title = HS_jetPhi_cut[jj]->GetName();
      pad2->cd();
      hratio = ratioplot(HS_jetPhi_cut[jj], H_jetPhi_cut[nfile-1][jj]);
      hratio->SetTitle("");
      hratio->GetXaxis()->SetTitle(Form("#phi^{jet%i}", jj+1));
      hratio->Draw("EP");
      tg->Draw("LSAME");
      CMS_lumi(pad1, period, 0);
      c1->Update();
      c1->RedrawAxis();
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    }

    // // THStack *HS_dEta_phojet;  
    // // THStack *HS_dPhi_phojet;
    // // THStack *HS_dR_phojet;
    // // THStack *HS_dEta_jetjet;  
    // // THStack *HS_dPhi_jetjet;
    // // THStack *HS_dR_jetjet;
    // // THStack *HS_dPhi_minjetMET;  
    // // THStack *HS_ptoverMET_cut;
    // // THStack *HS_dijetMass_cut;

    pad1->cd();
    HS_dEta_phojet->Draw("HIST");
    H_dEta_phojet[nfile-1]->Draw("SAME");
    HSig_dEta_phojet->Draw("HIST SAME");
    HA_dEta_phojet->Draw("E2 SAME");
    HS_dEta_phojet->GetXaxis()->SetTitle("");
    HS_dEta_phojet->GetYaxis()->SetTitle("Events");
    HS_dEta_phojet->GetXaxis()->SetLabelSize(0);
    HS_dEta_phojet->SetMaximum(10000);
    HS_dEta_phojet->SetMinimum(0.1);
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile == nfile-1) lhs->AddEntry(H_dEta_phojet[ifile], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_dEta_phojet, ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_dEta_phojet[ifile], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    ths->Draw("SAME");
    title = HS_dEta_phojet->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dEta_phojet, H_dEta_phojet[nfile-1]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#Delta#eta(#gamma,jet)");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_dPhi_phojet->Draw("HIST");
    H_dPhi_phojet[nfile-1]->Draw("SAME");
    HSig_dPhi_phojet->Draw("HIST SAME");
    HA_dPhi_phojet->Draw("E2 SAME");
    HS_dPhi_phojet->GetXaxis()->SetTitle("");
    HS_dPhi_phojet->GetYaxis()->SetTitle("Events");
    HS_dPhi_phojet->GetXaxis()->SetLabelSize(0);
    HS_dPhi_phojet->SetMaximum(10000);
    HS_dPhi_phojet->SetMinimum(0.1);
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile == nfile-1) lhs->AddEntry(H_dPhi_phojet[ifile], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_dPhi_phojet, ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_dPhi_phojet[ifile], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    ths->Draw("SAME");
    title = HS_dPhi_phojet->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dPhi_phojet, H_dPhi_phojet[nfile-1]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#Delta#phi(#gamma,jet)");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_dR_phojet->Draw("HIST");
    H_dR_phojet[nfile-1]->Draw("SAME");
    HSig_dR_phojet->Draw("HIST SAME");
    HA_dR_phojet->Draw("E2 SAME");
    HS_dR_phojet->GetXaxis()->SetTitle("");
    HS_dR_phojet->GetYaxis()->SetTitle("Events");
    HS_dR_phojet->GetXaxis()->SetLabelSize(0);
    HS_dR_phojet->SetMaximum(10000);
    HS_dR_phojet->SetMinimum(0.1);
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile == nfile-1) lhs->AddEntry(H_dR_phojet[ifile], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_dR_phojet, ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_dR_phojet[ifile], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    ths->Draw("SAME");
    title = HS_dR_phojet->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dR_phojet, H_dR_phojet[nfile-1]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#DeltaR(#gamma,jet)");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_dEta_jetjet->Draw("HIST");
    H_dEta_jetjet[nfile-1]->Draw("SAME");
    HSig_dEta_jetjet->Draw("HIST SAME");
    HA_dEta_jetjet->Draw("E2 SAME");
    HS_dEta_jetjet->GetXaxis()->SetTitle("");
    HS_dEta_jetjet->GetYaxis()->SetTitle("Events");
    HS_dEta_jetjet->GetXaxis()->SetLabelSize(0);
    HS_dEta_jetjet->SetMaximum(10000);
    HS_dEta_jetjet->SetMinimum(0.1);
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile == nfile-1) lhs->AddEntry(H_dEta_jetjet[ifile], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_dEta_jetjet, ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_dEta_jetjet[ifile], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    ths->Draw("SAME");
    title = HS_dEta_jetjet->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dEta_jetjet, H_dEta_jetjet[nfile-1]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#Delta#eta(jet,jet)");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_dPhi_jetjet->Draw("HIST");
    H_dPhi_jetjet[nfile-1]->Draw("SAME");
    HSig_dPhi_jetjet->Draw("HIST SAME");
    HA_dPhi_jetjet->Draw("E2 SAME");
    HS_dPhi_jetjet->GetXaxis()->SetTitle("");
    HS_dPhi_jetjet->GetYaxis()->SetTitle("Events");
    HS_dPhi_jetjet->GetXaxis()->SetLabelSize(0);
    HS_dPhi_jetjet->SetMaximum(10000);
    HS_dPhi_jetjet->SetMinimum(0.1);
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile == nfile-1) lhs->AddEntry(H_dPhi_jetjet[ifile], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_dPhi_jetjet, ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_dPhi_jetjet[ifile], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    ths->Draw("SAME");
    title = HS_dPhi_jetjet->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dPhi_jetjet, H_dPhi_jetjet[nfile-1]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#Delta#phi(jet,jet)");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_dR_jetjet->Draw("HIST");
    H_dR_jetjet[nfile-1]->Draw("SAME");
    HSig_dR_jetjet->Draw("HIST SAME");
    HA_dR_jetjet->Draw("E2 SAME");
    HS_dR_jetjet->GetXaxis()->SetTitle("");
    HS_dR_jetjet->GetYaxis()->SetTitle("Events");
    HS_dR_jetjet->GetXaxis()->SetLabelSize(0);
    HS_dR_jetjet->SetMaximum(10000);
    HS_dR_jetjet->SetMinimum(0.1);
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile == nfile-1) lhs->AddEntry(H_dR_jetjet[ifile], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_dR_jetjet, ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_dR_jetjet[ifile], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    ths->Draw("SAME");
    title = HS_dR_jetjet->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dR_jetjet, H_dR_jetjet[nfile-1]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#DeltaR(jet,jet)");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_dPhi_minjetMET->Draw("HIST");
    H_dPhi_minjetMET[nfile-1]->Draw("SAME");
    HSig_dPhi_minjetMET->Draw("HIST SAME");
    HA_dPhi_minjetMET->Draw("E2 SAME");
    HS_dPhi_minjetMET->GetXaxis()->SetTitle("");
    HS_dPhi_minjetMET->GetYaxis()->SetTitle("Events");
    HS_dPhi_minjetMET->GetXaxis()->SetLabelSize(0);
    HS_dPhi_minjetMET->SetMaximum(10000);
    HS_dPhi_minjetMET->SetMinimum(0.1);
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile == nfile-1) lhs->AddEntry(H_dPhi_minjetMET[ifile], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_dPhi_minjetMET, ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_dPhi_minjetMET[ifile], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    ths->Draw("SAME");
    title = HS_dPhi_minjetMET->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dPhi_minjetMET, H_dPhi_minjetMET[nfile-1]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#Delta#phi_{min}(jet,#slash{E}_{T})");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_ptoverMET_cut->Draw("HIST");
    H_ptoverMET_cut[nfile-1]->Draw("SAME");
    HSig_ptoverMET_cut->Draw("HIST SAME");
    HA_ptoverMET_cut->Draw("E2 SAME");
    HS_ptoverMET_cut->GetXaxis()->SetTitle("");
    HS_ptoverMET_cut->GetYaxis()->SetTitle("Events");
    HS_ptoverMET_cut->GetXaxis()->SetLabelSize(0);
    HS_ptoverMET_cut->GetXaxis()->SetRangeUser(0, 3);
    HS_ptoverMET_cut->SetMaximum(10000);
    HS_ptoverMET_cut->SetMinimum(0.1);
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile == nfile-1) lhs->AddEntry(H_ptoverMET_cut[ifile], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_ptoverMET_cut, ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_ptoverMET_cut[ifile], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    ths->Draw("SAME");
    title = HS_ptoverMET_cut->GetName();
    pad2->cd();
    hratio = ratioplot(HS_ptoverMET_cut, H_ptoverMET_cut[nfile-1]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("p_{T}^{#gamma}/E_{T}^{miss}");
    hratio->GetXaxis()->SetRangeUser(0, 3);
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    pad1->SetLogy(0);
    HS_dijetMass_cut->Draw("HIST");
    H_dijetMass_cut[nfile-1]->Draw("SAME");
    HSig_dijetMass_cut->Draw("HIST SAME");
    HA_dijetMass_cut->Draw("E2 SAME");
    HS_dijetMass_cut->GetXaxis()->SetTitle("");
    HS_dijetMass_cut->GetYaxis()->SetTitle("Events");
    HS_dijetMass_cut->GetXaxis()->SetLabelSize(0);
    HS_dijetMass_cut->GetXaxis()->SetRangeUser(200, 2000);
    HS_dijetMass_cut->SetMaximum(180);
    HS_dijetMass_cut->SetMinimum(0);
    //HS_dijetMass_cut->SetMaximum(10000);
    //HS_dijetMass_cut->SetMinimum(0.1);
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile == nfile-1) lhs->AddEntry(H_dijetMass_cut[ifile], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_dijetMass_cut, ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_dijetMass_cut[ifile], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    ths->Draw("SAME");
    title = HS_dijetMass_cut->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dijetMass_cut, H_dijetMass_cut[nfile-1]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("Dijet Mass[GeV]");
    hratio->GetXaxis()->SetRangeUser(200, 2000);
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    //c1->SaveAs(Form("%s/%s_linear.pdf", saveto, title));
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    
  }
  
}
