/// simultaneously fit sieie & dijetMass
/// Fit to the Toy data generated from the 3 fit templates

#include "TH2.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TLegend.h"
#include "TMarker.h"
#include "TLine.h"
#include "TRandom2.h"
#include "TTree.h"
#include "THStack.h"
#include "TGraph.h"

#include <fstream>
#include <iostream>
#include <TMinuit.h>
#include <vector>
#include <TMath.h>

#include "/home/jou/wkVBS/Draw/Drawlumi/CMS_lumi.C"

using namespace std;

#define NPAR 3
#define ncolor 3
#define nSTcolor 8
#define nfile 8

TH1F* ratioplot(TH1F* hdata, TH1F* hfit){

  if (!hdata) {
       Warning("TRatioPlot", "Need only a stack");
       //return;
    } 
  
  TH1F* tmpHist = (TH1F*)hfit->Clone();
  //tmpHist->Reset();
  TH1F* errHist = (TH1F*)hdata->Clone("errHist");
  errHist->Sumw2();
  Int_t nbins = errHist->GetNbinsX();
  for(Int_t bin=1; bin<=nbins; bin++){
    Double_t content = errHist->GetBinContent(bin);
    Double_t one_sigma = sqrt(content);
    errHist->SetBinContent(bin, one_sigma);
  }
  
  TH1F *ratio = (TH1F*)hdata->Clone("ratio");
  ratio->SetLineColor(kBlack);
  ratio->Sumw2();
  ratio->Add(tmpHist, -1);
  ratio->Divide(errHist);
  ratio->Draw("E3 P");
  
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerSize(0.8);
  ratio->SetMinimum(-4.5);
  ratio->SetMaximum(4.5);
  
  ratio->SetTitle("");
  ratio->GetYaxis()->SetTitle("(Data-Fit)/#sigma_{Data}");
  ratio->GetYaxis()->SetTitleFont(42);
  ratio->GetYaxis()->SetTitleSize(0.1);
  ratio->GetYaxis()->SetTitleOffset(0.36);
  ratio->GetYaxis()->SetLabelFont(42);
  ratio->GetYaxis()->SetLabelOffset(0.01);
  ratio->GetYaxis()->SetLabelSize(0.1);
  ratio->GetYaxis()->SetTickSize(0.032);
  ratio->GetYaxis()->ChangeLabel(-1, -1, 0, -1, -1, -1, "");
  ratio->GetYaxis()->SetNdivisions(9);
  
  ratio->GetXaxis()->SetTitleFont(42);
  ratio->GetXaxis()->SetTitleSize(0.1);
  ratio->GetXaxis()->SetTitleOffset(1.1);
  ratio->GetXaxis()->SetLabelFont(42);
  ratio->GetXaxis()->SetLabelOffset(0.03);
  ratio->GetXaxis()->SetLabelSize(0.08);
  ratio->GetXaxis()->SetTickSize(0.1);
  ratio->GetXaxis()->SetNdivisions(15);
  ratio->GetXaxis()->ChangeLabel(-1, -1, 0, -1, -1, -1, "");
  //ratio->GetXaxis()->ChangeLabel(1, -1, -1, -1, -1, -1, "0.05");
  TString bintext[20] = {"0.005", "0.0075", "0.008", "0.0085", "0.009", "0.0095", "0.01015", "0.0113", "0.0125", "0.015", "0.0175", "0.02/250", "500", "750", "1000", "1250", "1500", "1750", "2000", "5000"};
  for(Int_t i=0; i<20; i++){
    ratio->GetXaxis()->ChangeLabel(i+1, -1, -1, -1, -1, -1, bintext[i]);
  }
  
  // ChangeLabel()
  // Parameters
  //     [in]	labNum	Number of the label to be changed, negative numbers start from the end
  //     [in]	labAngle	New angle value
  //     [in]	labSize	New size (0 erase the label)
  //     [in]	labAlign	New alignment value
  //     [in]	labColor	New label color
  //     [in]	labFont	New label font
  //     [in]	labText	New label text

  // Notes:

  //     If an attribute should not be changed just give the value "-1".
  //     If labnum=0 the list of modified labels is reset.
  //     To erase a label set labSize to 0.
  //     If labText is not specified or is an empty string, the text label is not changed.

  return ratio;
}

Double_t xl = -1.;
Double_t xh = 1.;
Double_t bin_size = 0.05;

const double _two_pi = 2.0 * TMath::Pi();
Double_t fit_lo_edge = -1.;
Double_t fit_hi_edge = 1.;

vector<Double_t> CandColl;
vector<Double_t> FakeColl;
vector<Double_t> TrueColl;
vector<Double_t> VBSColl;

vector<Double_t> info;
vector<Double_t> info_err;

// fit result from data [ptbin][VBS, True, Fake] loose sieie/ true template from GJet for VBS & SMTrue
Double_t yield17[3][3] = {{17.35, 44.99, 52.19}, {25.02, 42.43, 35.55}, {27.13, 1.43, 9.96}};
Double_t error17[3][3] = {{4.16, 6.71, 7.22}, {5.00, 6.51, 5.96}, {5.21, 1.19, 3.16}};

Double_t yield18[3][3] = {{44.12, 60.34, 65.03}, {43.63, 61.62, 59.75}, {7.60, 37.21, 16.68}};
Double_t error18[3][3] = {{6.56, 7.81, 8.19}, {6.25, 7.18, 7.73}, {2.75, 6.82, 4.08}};

Float_t Maximum(Float_t compare, Float_t input){
  if(compare >= input) return compare;
  else return input;
}

void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{

  Double_t Lsum=0.;
  Double_t Nevt=0.;
  
  Double_t fVBS = par[0];
  Double_t fSMTrue = par[1];
  Double_t fSMFake = par[2];
  
  for ( int i=0; i<CandColl.size(); i++ ) {
    Nevt += CandColl[i]/2;
    //PDF for signal and background
    Double_t LVBS = VBSColl[i];
    Double_t LSMTrue = TrueColl[i];
    Double_t LSMFake = FakeColl[i];
    
    for (int data=0; data<CandColl[i]; data++) {	
      //Get Log Likelihood
      //Lsum += TMath::Log( ((fFake*LFake + fTrue*LTrue)*(fVBS*LVBS + fSMTrue*LSMTrue + fSMFake*LSMFake)) / ((fFake+fTrue)*(fVBS+fSMTrue+fSMFake)) );
      Lsum += TMath::Log( (fVBS*LVBS + fSMTrue*LSMTrue + fSMFake*LSMFake) / (fVBS+fSMTrue+fSMFake) );
    }
  }

  f=2*( -1*Lsum + (fVBS+fSMTrue+fSMFake) - Nevt*TMath::Log(fVBS+fSMTrue+fSMFake) );
  
}
TH1F *hCand_sieie;
TH1F *hFake_sieie;
TH1F *hCand_dijetMass;
TH1F *hCand_dijetMass_cutsieie;
TH1F *hTrue_sieie;
TH1F *hTrue_dijetMass;
TH1F *hTrue_dijetMass_cutsieie;
TH1F *hFake_dijetMass;
TH1F *hFake_dijetMass_cutsieie;
TH1F *hVBS_sieie;
TH1F *hVBS_dijetMass;
TH1F *hVBS_dijetMass_cutsieie;

TH1F* hTrue_sieie_toy;
TH1F* hFake_sieie_toy;
TH1F* hVBS_sieie_toy;
TH1F* hTrue_dijetMass_toy;
TH1F* hFake_dijetMass_toy;
TH1F* hVBS_dijetMass_toy;

Double_t nVBS_toy = 0;
Double_t nTrue_toy = 0;
Double_t nFake_toy = 0;
Double_t nVBS_toycut = 0;
Double_t nTrue_toycut = 0;
Double_t nFake_toycut = 0;

TH1F *Hsave_Fit;
TH1F *Hsave_Cand;
TH1F *Hsave_True;
TH1F *Hsave_Fake;
TH1F *Hsave_VBS;
TH1F *Hsave_SMTrue;
TH1F *Hsave_SMFake;

//___________________________________________________________________________
//Double_t* xDrawJFake_gMinutFit_ToyExp(int ptbin, int iexp, int year){
Double_t* gMinutFit(int ptbin, int iexp, int year){
  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  //TCanvas *c1 = new TCanvas("c1");
  double count=0;

  CandColl.clear();
  FakeColl.clear();
  TrueColl.clear();
  VBSColl.clear();

  TString rootname[20];
  Double_t yield[3] = {0};
  Double_t error[3] = {0};
  Double_t yield_cutsieie[3];
  Double_t error_cutsieie[3];
  if(year==2016){
    rootname[0] = "/data1/GMET/Jfake_Data/summer16/output_ggtree.root";
    rootname[1] = "/data1/GMET/Jfake_MC/summer16/output_ggtree.root";
    rootname[2] = "/data1/GMET/Jfake_pseudoData/summer16/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/data1/GMET/Jfake_Data/fall17/output_ggtree.root";
    rootname[1] = "/data1/GMET/Jfake_MC/fall17/output_ggtree.root";
    rootname[2] = "/data1/GMET/Jfake_pseudoData/fall17/output_ggtree_SMTrue.root";
    rootname[3] = "/data1/GMET/Jfake_pseudoData/fall17/output_ggtree_SMFake.root";
    rootname[4] = "/data1/GMET/Jfake_pseudoData/fall17/output_ggtree_VBS.root";
    
    yield[0] = yield17[ptbin-1][0];
    yield[1] = yield17[ptbin-1][1];
    yield[2] = yield17[ptbin-1][2];
    error[0] = error17[ptbin-1][0];
    error[1] = error17[ptbin-1][1];
    error[2] = error17[ptbin-1][2];

    rootname[5] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/JetFake/output_merge_JetFake.root";
    rootname[6] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/GJet/output_merge_GJet.root";    
    rootname[7] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/VG/output_merge_VG.root";
    rootname[8] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/monoboson/output_merge_monoBoson.root";
    rootname[9] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/diboson/output_merge_diBoson.root";
    rootname[10] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/triboson/output_merge_triBoson.root";
    rootname[11] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/TT/output_merge_TT.root";
    rootname[12] = "/data1/GMET/Jfake_pseudoData/fall17/job_UL17_Znunug_VBS/output_ggtree.root";

  }
  else if(year==2018){
    rootname[0] = "/data1/GMET/Jfake_Data/autumn18/output_ggtree.root";
    rootname[1] = "/data1/GMET/Jfake_MC/autumn18/output_ggtree.root";
    rootname[2] = "/data1/GMET/Jfake_pseudoData/autumn18/output_ggtree_SMTrue.root";
    rootname[3] = "/data1/GMET/Jfake_pseudoData/autumn18/output_ggtree_SMFake.root";
    rootname[4] = "/data1/GMET/Jfake_pseudoData/autumn18/output_ggtree_VBS.root";

    yield[0] = yield18[ptbin-1][0];
    yield[1] = yield18[ptbin-1][1];
    yield[2] = yield18[ptbin-1][2];
    error[0] = error18[ptbin-1][0];
    error[1] = error18[ptbin-1][1];
    error[2] = error18[ptbin-1][2];
    
    rootname[5] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/JetFake/output_merge_JetFake.root";
    rootname[6] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/GJet/output_merge_GJet.root";    
    rootname[7] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/VG/output_merge_VG.root";
    rootname[8] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/monoboson/output_merge_monoBoson.root";
    rootname[9] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/diboson/output_merge_diBoson.root";
    rootname[10] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/triboson/output_merge_triBoson.root";
    rootname[11] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/TT/output_merge_TT.root";
    rootname[12] = "/data1/GMET/Jfake_pseudoData/autumn18/job_UL18_Znunug_VBS/output_ggtree.root";
  }

  TFile *fopen = new TFile(rootname[0]);
  hCand_sieie = (TH1F*)fopen->Get(Form("CandPho_bin_template/hbin_CandPho_sieie_%i", ptbin));
  hFake_sieie = (TH1F*)fopen->Get(Form("FakePho_bin_template/hbin_FakePho_sieie_%i", ptbin));
  hCand_dijetMass = (TH1F*)fopen->Get(Form("CandPho_bin_template/hbin_CandPho_dijetMass_%i", ptbin));
  hCand_dijetMass_cutsieie = (TH1F*)fopen->Get(Form("CandPho_bin_template/hbin_CandPho_dijetMass_cutsieie_%i", ptbin));
  hCand_sieie->SetDirectory(0);
  hFake_sieie->SetDirectory(0);
  hCand_dijetMass->SetDirectory(0);
  hCand_dijetMass_cutsieie->SetDirectory(0);
  fopen->Close();
  
  // // from GJet MC
  fopen = new TFile(rootname[1]);
  hTrue_sieie = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_sieie_%i", ptbin));
  hTrue_sieie->SetDirectory(0);
  fopen->Close();
  
  // from True photon MCs
  fopen = new TFile(rootname[2]);
  //TH1F *hTrue_sieie = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_sieie_%i", ptbin));
  hTrue_dijetMass = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_dijetMass_%i", ptbin));
  hTrue_dijetMass_cutsieie = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_dijetMass_cutsieie_%i", ptbin));
  hTrue_dijetMass->SetDirectory(0);
  hTrue_dijetMass_cutsieie->SetDirectory(0);
  fopen->Close();

  fopen = new TFile(rootname[3]);
  hFake_dijetMass = (TH1F*)fopen->Get(Form("FakePho_bin_template/hbin_FakePho_dijetMass_%i", ptbin));
  hFake_dijetMass_cutsieie = (TH1F*)fopen->Get(Form("FakePho_bin_template/hbin_FakePho_dijetMass_cutsieie_%i", ptbin));
  hFake_dijetMass->SetDirectory(0);
  hFake_dijetMass_cutsieie->SetDirectory(0);
  fopen->Close();
  
  // // from GJet MC
  fopen = new TFile(rootname[1]);
  hVBS_sieie = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_sieie_%i", ptbin));
  hVBS_sieie->SetDirectory(0);
  fopen->Close();

  fopen = new TFile(rootname[4]);
  //TH1F *hVBS_sieie = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_sieie_%i", ptbin));
  hVBS_dijetMass = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_dijetMass_%i", ptbin));
  hVBS_dijetMass_cutsieie = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_dijetMass_cutsieie_%i", ptbin));
  hVBS_dijetMass->SetDirectory(0);
  hVBS_dijetMass_cutsieie->SetDirectory(0);
  fopen->Close();
  
  // plots from each process
  TH1F *h_dijetMass_cutsieie[nfile];
  for(Int_t ifile=0; ifile<nfile; ifile++){
    fopen = new TFile(rootname[ifile+5]);
    if(ifile==0){
      h_dijetMass_cutsieie[ifile] = (TH1F*)fopen->Get(Form("FakePho_bin_template/hbin_FakePho_dijetMass_cutsieie_%i", ptbin));
      h_dijetMass_cutsieie[ifile]->SetDirectory(0);
    }
    else{
      h_dijetMass_cutsieie[ifile] = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_dijetMass_cutsieie_%i", ptbin));
      h_dijetMass_cutsieie[ifile]->SetDirectory(0);
    }
    fopen->Close();
  }

  // create output tree
  Int_t Ngen_Cand = 0;
  Int_t Ngen_VBS = 0;
  Int_t Ngen_SMTrue = 0;
  Int_t Ngen_SMFake = 0;
  Int_t Ngencutsieie_Cand = 0;
  Int_t Ngencutsieie_VBS = 0;
  Int_t Ngencutsieie_SMTrue = 0;
  Int_t Ngencutsieie_SMFake = 0;
  Float_t Nfit_Cand = 0;
  Float_t Nfit_VBS = 0;
  Float_t Nfit_SMTrue = 0;
  Float_t Nfit_SMFake = 0;
  Float_t Nfitcutsieie_Cand = 0;
  Float_t Nfitcutsieie_VBS = 0;
  Float_t Nfitcutsieie_SMTrue = 0;
  Float_t Nfitcutsieie_SMFake = 0;

  Float_t Nfiterr_Cand = 0;
  Float_t Nfiterr_VBS = 0;
  Float_t Nfiterr_SMTrue = 0;
  Float_t Nfiterr_SMFake = 0;
  Float_t Nfiterrcutsieie_Cand = 0;
  Float_t Nfiterrcutsieie_VBS = 0;
  Float_t Nfiterrcutsieie_SMTrue = 0;
  Float_t Nfiterrcutsieie_SMFake = 0;
  
  Float_t Nbias_Cand = 0;
  Float_t Nbias_VBS = 0;
  Float_t Nbias_SMTrue = 0;
  Float_t Nbias_SMFake = 0;
  Float_t Nbiascutsieie_Cand = 0;
  Float_t Nbiascutsieie_VBS = 0;
  Float_t Nbiascutsieie_SMTrue = 0;
  Float_t Nbiascutsieie_SMFake = 0;
  Float_t Npull_Cand = 0;
  Float_t Npull_VBS = 0;
  Float_t Npull_SMTrue = 0;
  Float_t Npull_SMFake = 0;
  Float_t Npullcutsieie_Cand = 0;
  Float_t Npullcutsieie_VBS = 0;
  Float_t Npullcutsieie_SMTrue = 0;
  Float_t Npullcutsieie_SMFake = 0;

  TH1F *h_output_numbers = new TH1F("h_output_numbers", "", 24, 0, 24);

  
  Int_t ndata = 0;
  ndata += hCand_sieie->Integral(1,-1);
  ndata += hCand_dijetMass->Integral(1,-1);

  Int_t nbins_sieie = hCand_sieie->GetNbinsX();
  Int_t nbins_dijetMass = hCand_dijetMass->GetNbinsX();
  Int_t Nbins_fit = nbins_sieie+nbins_dijetMass;

  
  for(Int_t ibin=1; ibin<=nbins_sieie; ibin++){
    if(hVBS_sieie->GetBinContent(ibin)<0) hVBS_sieie->SetBinContent(ibin, 1e-10);
    if(hTrue_sieie->GetBinContent(ibin)<0) hTrue_sieie->SetBinContent(ibin, 1e-10);
    if(hFake_sieie->GetBinContent(ibin)<0) hFake_sieie->SetBinContent(ibin, 1e-10);
  }
  for(Int_t ibin=1; ibin<=nbins_dijetMass; ibin++){
    if(hVBS_dijetMass->GetBinContent(ibin)<0) hVBS_dijetMass->SetBinContent(ibin, 1e-10);
    if(hTrue_dijetMass->GetBinContent(ibin)<0) hTrue_dijetMass->SetBinContent(ibin, 1e-10);
    if(hFake_dijetMass->GetBinContent(ibin)<0) hFake_dijetMass->SetBinContent(ibin, 1e-10);
    if(hVBS_dijetMass_cutsieie->GetBinContent(ibin)<0) hVBS_dijetMass_cutsieie->SetBinContent(ibin, 1e-10);
    if(hTrue_dijetMass_cutsieie->GetBinContent(ibin)<0) hTrue_dijetMass_cutsieie->SetBinContent(ibin, 1e-10);
    if(hFake_dijetMass_cutsieie->GetBinContent(ibin)<0) hFake_dijetMass_cutsieie->SetBinContent(ibin, 1e-10);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(h_dijetMass_cutsieie[ifile]->GetBinContent(ibin)<0) h_dijetMass_cutsieie[ifile]->SetBinContent(ibin, 1e-10);
    }
  }

  hVBS_sieie->Scale(1./(hVBS_sieie->Integral(1,-1)));
  hTrue_sieie->Scale(1./(hTrue_sieie->Integral(1,-1)));
  hFake_sieie->Scale(1./(hFake_sieie->Integral(1,-1)));
  hVBS_dijetMass->Scale(1./(hVBS_dijetMass->Integral(1,-1)));
  hTrue_dijetMass->Scale(1./(hTrue_dijetMass->Integral(1,-1)));
  hFake_dijetMass->Scale(1./(hFake_dijetMass->Integral(1,-1)));
  hVBS_dijetMass_cutsieie->Scale(1./(hVBS_dijetMass_cutsieie->Integral(1,-1)));
  hTrue_dijetMass_cutsieie->Scale(1./(hTrue_dijetMass_cutsieie->Integral(1,-1)));
  hFake_dijetMass_cutsieie->Scale(1./(hFake_dijetMass_cutsieie->Integral(1,-1)));

  TH1F* hCand_template = new TH1F("hCand_template", "", Nbins_fit, 0, Nbins_fit);
  TH1F* hTrue_template = new TH1F("hTrue_template", "", Nbins_fit, 0, Nbins_fit);
  TH1F* hFake_template = new TH1F("hFake_template", "", Nbins_fit, 0, Nbins_fit);
  TH1F* hVBS_template = new TH1F("hVBS_template", "", Nbins_fit, 0, Nbins_fit);

  hCand_template->Sumw2();
  hTrue_template->Sumw2();
  hFake_template->Sumw2();
  hVBS_template->Sumw2();

  for(Int_t ibin=1; ibin<=nbins_sieie; ibin++){
    //hCand_template->SetBinContent(ibin, hCand_sieie->GetBinContent(ibin));
    hVBS_template->SetBinContent(ibin, hVBS_sieie->GetBinContent(ibin));
    hTrue_template->SetBinContent(ibin, hTrue_sieie->GetBinContent(ibin));
    hFake_template->SetBinContent(ibin, hFake_sieie->GetBinContent(ibin));
  }
  for(Int_t ibin=1; ibin<=nbins_dijetMass; ibin++){
    if(ibin == nbins_dijetMass){
      //hCand_template->SetBinContent(ibin+nbins_sieie, hCand_dijetMass->GetBinContent(ibin)+hCand_dijetMass->GetBinContent(ibin+1));
      hVBS_template->SetBinContent(ibin+nbins_sieie, hVBS_dijetMass->GetBinContent(ibin)+hVBS_dijetMass->GetBinContent(ibin+1));
      hTrue_template->SetBinContent(ibin+nbins_sieie, hTrue_dijetMass->GetBinContent(ibin)+hTrue_dijetMass->GetBinContent(ibin+1));
      hFake_template->SetBinContent(ibin+nbins_sieie, hFake_dijetMass->GetBinContent(ibin)+hFake_dijetMass->GetBinContent(ibin+1));
    }
    else{
      //hCand_template->SetBinContent(ibin+nbins_sieie, hCand_dijetMass->GetBinContent(ibin));
      hVBS_template->SetBinContent(ibin+nbins_sieie, hVBS_dijetMass->GetBinContent(ibin));
      hTrue_template->SetBinContent(ibin+nbins_sieie, hTrue_dijetMass->GetBinContent(ibin));
      hFake_template->SetBinContent(ibin+nbins_sieie, hFake_dijetMass->GetBinContent(ibin));
    }
  }

  hVBS_template->Scale(1./2.);
  hTrue_template->Scale(1./2.);
  hFake_template->Scale(1./2.);

  TH1F* hTrue_sieie_template = new TH1F("hTrue_sieie_template", "", Nbins_fit, 0, Nbins_fit);
  TH1F* hFake_sieie_template = new TH1F("hFake_sieie_template", "", Nbins_fit, 0, Nbins_fit);
  TH1F* hVBS_sieie_template = new TH1F("hVBS_sieie_template", "", Nbins_fit, 0, Nbins_fit);
  TH1F* hTrue_dijetMass_template = new TH1F("hTrue_dijetMass_template", "", Nbins_fit, 0, Nbins_fit);
  TH1F* hFake_dijetMass_template = new TH1F("hFake_dijetMass_template", "", Nbins_fit, 0, Nbins_fit);
  TH1F* hVBS_dijetMass_template = new TH1F("hVBS_dijetMass_template", "", Nbins_fit, 0, Nbins_fit);

  hTrue_sieie_template->Sumw2();
  hFake_sieie_template->Sumw2();
  hVBS_sieie_template->Sumw2();
  hTrue_dijetMass_template->Sumw2();
  hFake_dijetMass_template->Sumw2();
  hVBS_dijetMass_template->Sumw2();

  for(Int_t ibin=1; ibin<=nbins_sieie; ibin++){
    hVBS_sieie_template->SetBinContent(ibin, hVBS_sieie->GetBinContent(ibin));
    hTrue_sieie_template->SetBinContent(ibin, hTrue_sieie->GetBinContent(ibin));
    hFake_sieie_template->SetBinContent(ibin, hFake_sieie->GetBinContent(ibin));
  }
  for(Int_t ibin=1; ibin<=nbins_dijetMass; ibin++){
    if(ibin == nbins_dijetMass){
      hVBS_dijetMass_template->SetBinContent(ibin+nbins_sieie, hVBS_dijetMass->GetBinContent(ibin)+hVBS_dijetMass->GetBinContent(ibin+1));
      hTrue_dijetMass_template->SetBinContent(ibin+nbins_sieie, hTrue_dijetMass->GetBinContent(ibin)+hTrue_dijetMass->GetBinContent(ibin+1));
      hFake_dijetMass_template->SetBinContent(ibin+nbins_sieie, hFake_dijetMass->GetBinContent(ibin)+hFake_dijetMass->GetBinContent(ibin+1));
    }
    else{
      hVBS_dijetMass_template->SetBinContent(ibin+nbins_sieie, hVBS_dijetMass->GetBinContent(ibin));
      hTrue_dijetMass_template->SetBinContent(ibin+nbins_sieie, hTrue_dijetMass->GetBinContent(ibin));
      hFake_dijetMass_template->SetBinContent(ibin+nbins_sieie, hFake_dijetMass->GetBinContent(ibin));
    }
  }

  ////// Build up Toy data //////

  // RNG to random generated number of events by a Poisson law
  TRandom *rng_toy = new TRandom2(0);
  TRandom2 *rngVBS_toy = new TRandom2(0);
  TRandom2 *rngTrue_toy = new TRandom2(0);
  TRandom2 *rngFake_toy = new TRandom2(0);

  while(fabs(Ngen_Cand-(yield[0]+yield[1]+yield[2])) > sqrt(yield[0]+yield[1]+yield[2])){
    while(fabs(nVBS_toy-yield[0]) > error[0]){
      nVBS_toy = rngVBS_toy->Poisson(yield[0]);
      cout << "nVBS_toy" << nVBS_toy << " mean " << yield[0] << endl;
    }
    while(fabs(nTrue_toy-yield[1]) > error[1]){
      nTrue_toy = rngTrue_toy->Poisson(yield[1]);
      cout << "nTrue_toy" << nTrue_toy << " mean " << yield[1] << endl;
    }
    while(fabs(nFake_toy-yield[2]) > error[2]){
      nFake_toy = rngFake_toy->Poisson(yield[2]);
      cout << "nFake_toy" << nFake_toy << " mean " << yield[2] << endl;
    }

    Ngen_Cand = nVBS_toy + nTrue_toy + nFake_toy;
  }
  // nVBS_toy = yield[0];
  // nTrue_toy = yield[1];
  // nFake_toy = yield[2];
  //Ngen_Cand = nVBS_toy + nTrue_toy + nFake_toy;
  Ngen_VBS = nVBS_toy;
  Ngen_SMTrue = nTrue_toy;
  Ngen_SMFake = nFake_toy;
  
  hTrue_sieie_toy = new TH1F("hTrue_sieie_toy", "", Nbins_fit, 0, Nbins_fit);
  hFake_sieie_toy = new TH1F("hFake_sieie_toy", "", Nbins_fit, 0, Nbins_fit);
  hVBS_sieie_toy = new TH1F("hVBS_sieie_toy", "", Nbins_fit, 0, Nbins_fit);
  hTrue_dijetMass_toy = new TH1F("hTrue_dijetMass_toy", "", Nbins_fit, 0, Nbins_fit);
  hFake_dijetMass_toy = new TH1F("hFake_dijetMass_toy", "", Nbins_fit, 0, Nbins_fit);
  hVBS_dijetMass_toy = new TH1F("hVBS_dijetMass_toy", "", Nbins_fit, 0, Nbins_fit);

  hTrue_sieie_toy->Sumw2();
  hFake_sieie_toy->Sumw2();
  hVBS_sieie_toy->Sumw2();
  hTrue_dijetMass_toy->Sumw2();
  hFake_dijetMass_toy->Sumw2();
  hVBS_dijetMass_toy->Sumw2();

  hTrue_sieie_toy->FillRandom(hTrue_sieie_template, fabs(nTrue_toy));
  hFake_sieie_toy->FillRandom(hFake_sieie_template, fabs(nFake_toy));
  hVBS_sieie_toy->FillRandom(hVBS_sieie_template, fabs(nVBS_toy));
  hTrue_dijetMass_toy->FillRandom(hTrue_dijetMass_template, fabs(nTrue_toy));
  hFake_dijetMass_toy->FillRandom(hFake_dijetMass_template, fabs(nFake_toy));
  hVBS_dijetMass_toy->FillRandom(hVBS_dijetMass_template, fabs(nVBS_toy));  
  
  hCand_template->Add(hTrue_sieie_toy);
  hCand_template->Add(hFake_sieie_toy);
  hCand_template->Add(hVBS_sieie_toy);
  hCand_template->Add(hTrue_dijetMass_toy);
  hCand_template->Add(hFake_dijetMass_toy);
  hCand_template->Add(hVBS_dijetMass_toy);
  
  // TH1F* hTrue_toy = new TH1F("hTrue_toy", "", Nbins_fit, 0, Nbins_fit);
  // TH1F* hFake_toy = new TH1F("hFake_toy", "", Nbins_fit, 0, Nbins_fit);
  // TH1F* hVBS_toy = new TH1F("hVBS_toy", "", Nbins_fit, 0, Nbins_fit);
  
  // hTrue_toy->FillRandom(hTrue_template, 2*fabs(nTrue_toy));
  // hFake_toy->FillRandom(hFake_template, 2*fabs(nFake_toy));
  // hVBS_toy->FillRandom(hVBS_template, 2*fabs(nVBS_toy));
  
  // hCand_template->Add(hTrue_toy);
  // hCand_template->Add(hFake_toy);
  // hCand_template->Add(hVBS_toy);

  // hCand_template->Draw("HIST");
  // return nullptr;

  Int_t ndata_sieie = 0;
  Int_t ndata_dijetMass = 0;
  ndata_sieie += hCand_template->Integral(1,11);
  ndata_dijetMass += hCand_template->Integral(12,19);
  
  ////// Toy data ready for Fit //////

  Float_t VBSfrac = hVBS_template->Integral(1,-1)/ndata;
  Float_t Truefrac = hTrue_template->Integral(1,-1)/ndata;
  Float_t Fakefrac = hFake_template->Integral(1,-1)/ndata;
  cout << "fraction" << VBSfrac << " " << Truefrac << " " << Fakefrac << endl;

  Double_t* fitted = new Double_t[6];
  if(ndata_sieie==0) {
    printf(" ---  no events in the fit \n");
    fitted[0] = 0.;
    fitted[1] = 0.;
    fitted[2] = 0.;
    fitted[3] = 0.;
    fitted[4] = 0.;
    fitted[5] = 0.;
    return fitted;
  }

  Float_t Ncand = hCand_template->Integral(1,-1);
  Float_t NVBS = hVBS_template->Integral(1,-1);
  Float_t NTrue = hTrue_template->Integral(1,-1);
  Float_t NFake = hFake_template->Integral(1,-1);
  
  printf(" --------- before the fit ------------- \n");
  printf("NCand %3.3f, NVBS %3.3f, NTrue %3.3f, NFake %3.3f \n", Ncand, NVBS, NTrue, NFake);
  printf(" -------------------------------------- \n");

  for(Int_t ibin=1; ibin<=Nbins_fit; ibin++){
    CandColl.push_back(hCand_template->GetBinContent(ibin));
    VBSColl.push_back(hVBS_template->GetBinContent(ibin));
    TrueColl.push_back(hTrue_template->GetBinContent(ibin));
    FakeColl.push_back(hFake_template->GetBinContent(ibin));
    if(CandColl[ibin-1] == 0.0) CandColl[ibin-1] += pow(10, -10);
    if(VBSColl[ibin-1] == 0.0) VBSColl[ibin-1] += pow(10, -10);
    if(FakeColl[ibin-1] == 0.0) FakeColl[ibin-1] += pow(10, -10);
    if(TrueColl[ibin-1] == 0.0) TrueColl[ibin-1] += pow(10, -10);
  }

  TH1F* hTrue_dijetMass_cutsieie_template = new TH1F("hTrue_dijetMass_cutsieie_template", "", Nbins_fit, 0, Nbins_fit);
  TH1F* hFake_dijetMass_cutsieie_template = new TH1F("hFake_dijetMass_cutsieie_template", "", Nbins_fit, 0, Nbins_fit);
  TH1F* hVBS_dijetMass_cutsieie_template = new TH1F("hVBS_dijetMass_cutsieie_template", "", Nbins_fit, 0, Nbins_fit);

  hTrue_dijetMass_cutsieie_template->Sumw2();
  hFake_dijetMass_cutsieie_template->Sumw2();
  hVBS_dijetMass_cutsieie_template->Sumw2();
    
  for(Int_t ibin=1; ibin<=nbins_dijetMass; ibin++){
    if(ibin == nbins_dijetMass){
      hVBS_dijetMass_cutsieie_template->SetBinContent(ibin+nbins_sieie, hVBS_dijetMass_cutsieie->GetBinContent(ibin)+hVBS_dijetMass_cutsieie->GetBinContent(ibin+1));
      hTrue_dijetMass_cutsieie_template->SetBinContent(ibin+nbins_sieie, hTrue_dijetMass_cutsieie->GetBinContent(ibin)+hTrue_dijetMass_cutsieie->GetBinContent(ibin+1));
      hFake_dijetMass_cutsieie_template->SetBinContent(ibin+nbins_sieie, hFake_dijetMass_cutsieie->GetBinContent(ibin)+hFake_dijetMass_cutsieie->GetBinContent(ibin+1));
    }
    else{
      hVBS_dijetMass_cutsieie_template->SetBinContent(ibin+nbins_sieie, hVBS_dijetMass_cutsieie->GetBinContent(ibin));
      hTrue_dijetMass_cutsieie_template->SetBinContent(ibin+nbins_sieie, hTrue_dijetMass_cutsieie->GetBinContent(ibin));
      hFake_dijetMass_cutsieie_template->SetBinContent(ibin+nbins_sieie, hFake_dijetMass_cutsieie->GetBinContent(ibin));
    }
  }
  
  //nVBS_toy = 0; nTrue_toy = 0; nFake_toy = 0;
  cout << "for drawings" << endl;
  nVBS_toycut = hVBS_sieie_toy->Integral(1,6);
  cout << "nVBS_toy" << nVBS_toycut << endl;
  nTrue_toycut = hTrue_sieie_toy->Integral(1,6);
  cout << "nTrue_toy" << nTrue_toycut << endl;
  nFake_toycut = hFake_sieie_toy->Integral(1,6);
  cout << "nFake_toy" << nFake_toycut << endl;

  Ngencutsieie_Cand = nVBS_toycut + nTrue_toycut + nFake_toycut;
  Ngencutsieie_VBS = nVBS_toycut;
  Ngencutsieie_SMTrue = nTrue_toycut;
  Ngencutsieie_SMFake = nFake_toycut;

  TH1F* hTrue_dijetMass_cutsieie_toy = new TH1F("hTrue_dijetMass_cutsieie_toy", "", Nbins_fit, 0, Nbins_fit);
  TH1F* hFake_dijetMass_cutsieie_toy = new TH1F("hFake_dijetMass_cutsieie_toy", "", Nbins_fit, 0, Nbins_fit);
  TH1F* hVBS_dijetMass_cutsieie_toy = new TH1F("hVBS_dijetMass_cutsieie_toy", "", Nbins_fit, 0, Nbins_fit);

  hTrue_dijetMass_cutsieie_toy->Sumw2();
  hFake_dijetMass_cutsieie_toy->Sumw2();
  hVBS_dijetMass_cutsieie_toy->Sumw2();

  hTrue_dijetMass_cutsieie_toy->FillRandom(hTrue_dijetMass_cutsieie_template, fabs(nTrue_toycut));
  hFake_dijetMass_cutsieie_toy->FillRandom(hFake_dijetMass_cutsieie_template, fabs(nFake_toycut));
  hVBS_dijetMass_cutsieie_toy->FillRandom(hVBS_dijetMass_cutsieie_template, fabs(nVBS_toycut));

  for(Int_t ibin=1; ibin<=nbins_dijetMass; ibin++){hCand_template->SetBinContent(ibin+nbins_sieie, 0.0);}
  //hCand_template->Draw("HIST");
  
  hCand_template->Add(hTrue_dijetMass_cutsieie_toy);
  hCand_template->Add(hFake_dijetMass_cutsieie_toy);
  hCand_template->Add(hVBS_dijetMass_cutsieie_toy);

  TH1F* H_dijetMass_cutsieie[nfile];
  for(Int_t ifile=0; ifile<nfile; ifile++){
    H_dijetMass_cutsieie[ifile] = new TH1F(Form("H_dijetMass_cutsieie_%i", ifile+1), "", Nbins_fit, 0, Nbins_fit);
    H_dijetMass_cutsieie[ifile]->Sumw2();
    for(Int_t ibin=0; ibin<=nbins_dijetMass; ibin++){
      if(ibin == nbins_dijetMass) H_dijetMass_cutsieie[ifile]->SetBinContent(ibin+nbins_sieie, h_dijetMass_cutsieie[ifile]->GetBinContent(ibin)+h_dijetMass_cutsieie[ifile]->GetBinContent(ibin+1));
      else H_dijetMass_cutsieie[ifile]->SetBinContent(ibin+nbins_sieie, h_dijetMass_cutsieie[ifile]->GetBinContent(ibin));
    }
  }
  
  printf( " -----  Got %d, %d, %d, %d events for fit ----- \n ", CandColl.size(), VBSColl.size(), TrueColl.size(), FakeColl.size() );
  if ( CandColl.size()!=VBSColl.size() || CandColl.size()!=TrueColl.size() || CandColl.size()!= FakeColl.size() ) {
    printf(" error ...  inconsistent hit collection size \n");
    return fitted;
  }

  //--------------------------------------------------
  //init parameters for fit
  Double_t vstart[NPAR] = {1., 1., 1.};

  vstart[0] = VBSfrac*ndata;
  vstart[1] = Truefrac*ndata;
  vstart[2] = Fakefrac*ndata;

  TMinuit *gMinuit = new TMinuit(NPAR);  
  gMinuit->Command("SET STR 1");
  gMinuit->SetFCN(fcn);
  Double_t arglist[10];
  Int_t ierflg = 0;

  arglist[0] = 1;
  gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);
  arglist[0] = 1;
  gMinuit->mnexcm("SET PRINT", arglist ,1,ierflg);

  Double_t step[NPAR] = {0.01, 0.01, 0.01};
  gMinuit->mnparm(0,  "VBS yield"  , vstart[0],  step[0], 0., ndata*1 , ierflg);
  gMinuit->mnparm(1,  "TruePho yield"  , vstart[1],  step[1], 0., ndata*1 , ierflg);
  gMinuit->mnparm(2,  "FakePho yield"  , vstart[2],  step[2], 0., ndata*1 , ierflg);

  printf(" --------------------------------------------------------- \n");
  printf(" Now ready for minimization step \n --------------------------------------------------------- \n");
  
  arglist[0] = 1000; // number of iteration
  arglist[1] = 1.;
  gMinuit->mnexcm("MIGRAD", arglist, 2, ierflg);
  printf (" -------------------------------------------- \n");
  printf("Finished.  ierr = %2.2f \n", ierflg);

  info.clear();
  info_err.clear();

  double para[NPAR+1],errpara[NPAR+1];
  if ( ierflg == 0 ) 
    {
      for(int j=0; j<=NPAR-1;j++) {
        gMinuit->GetParameter(j, para[j],errpara[j]);
        para[NPAR] = CandColl.size();
        info.push_back(para[j]);
        info_err.push_back(errpara[j]);
        printf("Parameter (yield) %d = %f +- %f\n",j,para[j],errpara[j]);
	
      }
      printf(" fitted yield %1.3f \n", (para[0]+para[1]+para[2]) );

      //info.push_back(sigColl.size());

      //do minos if fit sucessed.
      //       printf("         ---------------------------------------------------------\n");
      //       printf("          Now call for minos step \n");
      //       printf("         ---------------------------------------------------------\n");
      
      //       arglist[0] = 200; // number of iteration
      //       arglist[1] = 1;
      //       gMinuit->mnexcm("MINOS", arglist ,2,ierflg);
      //       printf("         --------------------------------------------------------- \n");
      //       printf("         Done Minos.  ierr = %d \n", ierflg);
      //       Double_t amin;
      //       gMinuit->mnprin(1,amin);
    }
  else {
    printf(" *********** Fit failed! ************\n");
    gMinuit->GetParameter(0, para[0],errpara[0]);
    gMinuit->GetParameter(1, para[1],errpara[1]);
    gMinuit->GetParameter(2, para[2],errpara[2]);
    para[0]=0.; errpara[0]=0.;
  }

  // Print results
  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;
  gMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  gMinuit->mnprin(1,amin);  
  gMinuit->mnmatu(1);
  printf(" ========= happy ending !? =========================== \n");
  printf("FCN =  %3.3f \n", amin);

  double yerr[20];
  for(int i=0;i<20;i++){
    yerr[i] = 0.;
  }

  // scale to fit result
  hVBS_sieie->Scale(para[0]);
  hTrue_sieie->Scale(para[1]);
  hFake_sieie->Scale(para[2]);
  hVBS_dijetMass_cutsieie->Scale(para[0]);
  hTrue_dijetMass_cutsieie->Scale(para[1]);
  hFake_dijetMass_cutsieie->Scale(para[2]);

  Nfit_Cand = hVBS_sieie->Integral() + hTrue_sieie->Integral() + hFake_sieie->Integral();
  Nfit_VBS = hVBS_dijetMass_cutsieie->Integral();
  Nfit_SMTrue = hTrue_dijetMass_cutsieie->Integral();
  Nfit_SMFake = hFake_dijetMass_cutsieie->Integral();
  Nfiterr_Cand = TMath::Sqrt(Nfit_Cand);
  Nfiterr_VBS = TMath::Sqrt(Nfit_VBS);
  Nfiterr_SMTrue = TMath::Sqrt(Nfit_SMTrue);
  Nfiterr_SMFake = TMath::Sqrt(Nfit_SMFake);
    
  Double_t TrueEff = (hVBS_sieie->Integral(1,6)+hTrue_sieie->Integral(1,6)) / (hVBS_sieie->Integral(1,-1)+hTrue_sieie->Integral(1,-1));
  Double_t FakeEff = hFake_sieie->Integral(1,6) / hFake_sieie->Integral(1,-1);

  TH1F *HFit;
  TH1F *HCand;
  TH1F *HTrue;
  TH1F *HFake;
  TH1F *HVBS;
  TH1F *HSMTrue;
  TH1F *HSMFake;

  HCand = new TH1F("HCand", "", Nbins_fit, 0, Nbins_fit);
  HTrue = new TH1F("HTrue", "", Nbins_fit, 0, Nbins_fit);
  HFake = new TH1F("HFake", "", Nbins_fit, 0, Nbins_fit);
  HVBS = new TH1F("HVBS", "", Nbins_fit, 0, Nbins_fit);
  HSMTrue = new TH1F("HSMTrue", "", Nbins_fit, 0, Nbins_fit);
  HSMFake = new TH1F("HSMake", "", Nbins_fit, 0, Nbins_fit);

  HCand->Sumw2();
  HTrue->Sumw2();
  HFake->Sumw2();
  HVBS->Sumw2();
  HSMTrue->Sumw2();
  HSMFake->Sumw2();

  for(Int_t ibin=1; ibin<=nbins_sieie; ibin++){
    HCand->SetBinContent(ibin, hCand_template->GetBinContent(ibin));
    HTrue->SetBinContent(ibin, hVBS_sieie->GetBinContent(ibin)+hTrue_sieie->GetBinContent(ibin));
    HFake->SetBinContent(ibin, hFake_sieie->GetBinContent(ibin));

    HCand->SetBinError(ibin, hCand_template->GetBinError(ibin));
    HTrue->SetBinError(ibin, hVBS_sieie->GetBinError(ibin)+hTrue_sieie->GetBinError(ibin));
    HFake->SetBinError(ibin, hFake_sieie->GetBinError(ibin)); 
  }
  for(Int_t ibin=1; ibin<=nbins_dijetMass; ibin++){
    if(ibin == nbins_dijetMass){
      HCand->SetBinContent(ibin+nbins_sieie, hCand_template->GetBinContent(ibin+nbins_sieie)+hCand_template->GetBinContent(ibin+nbins_sieie+1));
      HVBS->SetBinContent(ibin+nbins_sieie, TrueEff*(hVBS_dijetMass_cutsieie->GetBinContent(ibin)+hVBS_dijetMass_cutsieie->GetBinContent(ibin+1)));
      HSMTrue->SetBinContent(ibin+nbins_sieie, TrueEff*(hTrue_dijetMass_cutsieie->GetBinContent(ibin)+hTrue_dijetMass_cutsieie->GetBinContent(ibin+1)));
      HSMFake->SetBinContent(ibin+nbins_sieie, FakeEff*(hFake_dijetMass_cutsieie->GetBinContent(ibin)+hFake_dijetMass_cutsieie->GetBinContent(ibin+1)));
      

      HCand->SetBinError(ibin+nbins_sieie, hCand_template->GetBinError(ibin+nbins_sieie)+hCand_template->GetBinError(ibin+nbins_sieie+1));
      HVBS->SetBinError(ibin+nbins_sieie, TrueEff*(hVBS_dijetMass_cutsieie->GetBinError(ibin)+hVBS_dijetMass_cutsieie->GetBinError(ibin+1)));
      HSMTrue->SetBinError(ibin+nbins_sieie, TrueEff*(hTrue_dijetMass_cutsieie->GetBinError(ibin)+hTrue_dijetMass_cutsieie->GetBinError(ibin+1)));
      HSMFake->SetBinError(ibin+nbins_sieie, FakeEff*(hFake_dijetMass_cutsieie->GetBinError(ibin)+hFake_dijetMass_cutsieie->GetBinError(ibin+1)));
    }
    else{
      HCand->SetBinContent(ibin+nbins_sieie, hCand_template->GetBinContent(ibin+nbins_sieie));
      HVBS->SetBinContent(ibin+nbins_sieie, TrueEff*hVBS_dijetMass_cutsieie->GetBinContent(ibin));
      HSMTrue->SetBinContent(ibin+nbins_sieie, TrueEff*hTrue_dijetMass_cutsieie->GetBinContent(ibin));
      HSMFake->SetBinContent(ibin+nbins_sieie, FakeEff*hFake_dijetMass_cutsieie->GetBinContent(ibin));
      
      HCand->SetBinError(ibin+nbins_sieie, hCand_template->GetBinError(ibin));
      HVBS->SetBinError(ibin+nbins_sieie, TrueEff*hVBS_dijetMass_cutsieie->GetBinError(ibin));
      HSMTrue->SetBinError(ibin+nbins_sieie, TrueEff*hTrue_dijetMass_cutsieie->GetBinError(ibin));
      HSMFake->SetBinError(ibin+nbins_sieie, FakeEff*hFake_dijetMass_cutsieie->GetBinError(ibin));
    }
  }

  Nfitcutsieie_Cand = HVBS->Integral() + HSMTrue->Integral() + HSMFake->Integral();
  Nfitcutsieie_VBS = HVBS->Integral();
  Nfitcutsieie_SMTrue = HSMTrue->Integral();
  Nfitcutsieie_SMFake = HSMFake->Integral();
  Nfiterrcutsieie_Cand = TMath::Sqrt(Nfitcutsieie_Cand);
  Nfiterrcutsieie_VBS = TMath::Sqrt(Nfitcutsieie_VBS);
  Nfiterrcutsieie_SMTrue = TMath::Sqrt(Nfitcutsieie_SMTrue);
  Nfiterrcutsieie_SMFake = TMath::Sqrt(Nfitcutsieie_SMFake);
  
  HFit = new TH1F("HFit", "", Nbins_fit, 0, Nbins_fit);
  HFit->Sumw2();
  for(Int_t ibin=1; ibin<=Nbins_fit; ibin++){
    HFit->AddBinContent(ibin, HFake->GetBinContent(ibin));
    HFit->AddBinContent(ibin, HTrue->GetBinContent(ibin));
    HFit->AddBinContent(ibin, HVBS->GetBinContent(ibin));
    HFit->AddBinContent(ibin, HSMTrue->GetBinContent(ibin));
    HFit->AddBinContent(ibin, HSMFake->GetBinContent(ibin));
  }

  HFit->SetLineColor(60);
  HFake->SetLineColor(211);
  HTrue->SetLineColor(97);

  HFit->SetMarkerColor(60);
  HFake->SetMarkerColor(211);
  HTrue->SetMarkerColor(97);

  HFit->SetLineWidth(2);
  HFake->SetLineWidth(2);
  HTrue->SetLineWidth(2);

  HFake->SetLineStyle(7);
  HTrue->SetLineStyle(5);

  HFit->SetMarkerSize(0.4);
  HFake->SetMarkerSize(0.3);
  HTrue->SetMarkerSize(0.3);

  HFake->SetFillStyle(3145);
  HFake->SetFillColor(30);

  THStack *HS_dijetMass = new THStack("HS_dijetMass", "HS_dijetMass");

  string hexcolor[ncolor] = {"#03a9f4", "#ff9e00", "#67ccc1"};
  TColor *color[ncolor];
  Int_t cnum[ncolor];
  for(Int_t j=0; j<ncolor; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  }

  HSMFake->SetFillColor(cnum[0]);
  HSMTrue->SetFillColor(cnum[1]);
  HVBS->SetFillColor(cnum[2]);

  HSMFake->SetLineColor(1);
  HSMTrue->SetLineColor(1);
  HVBS->SetLineColor(1);

  HSMFake->SetLineWidth(1);
  HSMTrue->SetLineWidth(1);
  HVBS->SetLineWidth(1);

  HS_dijetMass->Add(HSMFake);
  HS_dijetMass->Add(HSMTrue);
  HS_dijetMass->Add(HVBS);
  
  Double_t binrange[20] = {75, 110, 210, 2000};//3 bins

  // plot
  //c1->Draw();
  
  TLegend *lhs = new TLegend(0.46, 0.72, 0.73, 0.88);
  lhs->SetTextSize(0.038);
  lhs->SetFillStyle(0);

  // TLatex *ths_year;
  // ths_year = new TLatex(0.16, 0.88, Form("%i", year));
  // ths_year->SetNDC();
  // ths_year->SetTextAlign(13);
  // ths_year->SetTextFont(42);
  // ths_year->SetTextSize(0.04);

  TLatex *ths;
  ths = new TLatex(0.16, 0.88, Form("%.0f GeV < Photon p_{T} < %.0f GeV", binrange[ptbin-1], binrange[ptbin]));
  ths->SetNDC();
  ths->SetTextAlign(13);
  ths->SetTextFont(42);
  ths->SetTextSize(0.04);

  TLatex *tSB_sieie = new TLatex();
  Float_t Bkg_sieie = HFake->Integral(1, 6);
  Float_t Sig_sieie = HTrue->Integral(1, 6);
  Float_t Bkgerr_sieie = TMath::Sqrt(HFake->Integral(1, 6));
  Float_t Sigerr_sieie = TMath::Sqrt(HTrue->Integral(1, 6));
  Float_t data_sieie = HCand->Integral(1, 6);
  Float_t dataerr_sieie = TMath::Sqrt(HCand->Integral(1, 6));
  Float_t Fitdata_sieie = HFit->Integral(1, 6);
  Float_t Fitdataerr_sieie = TMath::Sqrt(HFit->Integral(1, 6));
  //tSB = new TLatex(0.56, 0.62, Form("Data events = %.0f #pm %.2f", data, dataerr));
  tSB_sieie->SetNDC();
  tSB_sieie->SetTextAlign(13);
  tSB_sieie->SetTextFont(42);
  tSB_sieie->SetTextSize(0.04);

  TLatex *tSB_dijetMass = new TLatex();
  Float_t BkgFake_dijetMass = HSMFake->Integral(12, 19);
  Float_t Bkg_dijetMass = HSMTrue->Integral(12, 19);
  Float_t Sig_dijetMass = HVBS->Integral(12, 19);
  Float_t BkgFakeerr_dijetMass = TMath::Sqrt(HSMFake->Integral(12, 19));
  Float_t Bkgerr_dijetMass = TMath::Sqrt(HSMTrue->Integral(12, 19));
  Float_t Sigerr_dijetMass = TMath::Sqrt(HVBS->Integral(12, 19));
  Float_t data_dijetMass = HCand->Integral(12, 19);
  Float_t dataerr_dijetMass = TMath::Sqrt(HCand->Integral(12, 19));
  Float_t Fitdata_dijetMass = HFit->Integral(12, 19);
  Float_t Fitdataerr_dijetMass = TMath::Sqrt(HFit->Integral(12, 19));
  tSB_dijetMass->SetNDC();
  tSB_dijetMass->SetTextAlign(13);
  tSB_dijetMass->SetTextFont(42);
  tSB_dijetMass->SetTextSize(0.04);

  TH1F *hratio;
  Double_t x[3] = {-2000, 0.0, 2000};
  Double_t y[3] = {0, 0, 0};
  TGraph *tg = new TGraph(3, x, y);
  tg->SetLineColor(kRed);
  tg->SetLineWidth(1);
  tg->SetLineStyle(8);

  Double_t x_v[3] = {11, 11, 11};
  Double_t y_v[3] = {-120, 60, 220};
  TGraph *tg_v = new TGraph(3, x_v, y_v);
  tg_v->SetLineColor(kBlack);
  tg_v->SetLineWidth(3);
  tg_v->SetLineStyle(9);

  //TPad *pad1 = new TPad("pad1", "", 0., 0.3, 1., 1.);
  //pad1->SetTopMargin(0.03);
  //pad1->SetBottomMargin(0);
  //pad1->SetLogy(1);
  //pad1->Draw();
  
  // TPad *pad2 = new TPad("pad2", "", 0., 0.02, 1., 0.3);
  // pad2->SetTopMargin(0);
  // pad2->SetBottomMargin(0.3);
  //pad2->Draw();

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

  //pad1->cd();
  
  //HFit->Draw("HIST");
  // HFit->SetMaximum(100);
  // HFit->SetName(Form("HSFToy_JFakePho_%i", ptbin));
  // HFit->GetYaxis()->SetTitle("Events");
  // HFit->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  // HFit->GetXaxis()->SetTitle("");
  // HFit->GetXaxis()->SetNdivisions(15);
  // HFit->SetMaximum(2.8*HFit->GetBinContent(HFit->GetMaximumBin()));
  
  // tSB_sieie->SetTextSize(0.055);
  // tSB_sieie->DrawLatexNDC(0.16, 0.7, "#sigma_{i#etai#eta}");
  // tSB_sieie->SetTextSize(0.04);
  // tSB_sieie->DrawLatexNDC(0.16, 0.64, Form("Toy Data events = %.0f", data_sieie));
  // tSB_sieie->DrawLatexNDC(0.16, 0.60, Form("Fitted events = %.2f #pm %.2f", Fitdata_sieie, Fitdataerr_sieie));
  // tSB_sieie->DrawLatexNDC(0.16, 0.56, Form("True events = %.2f #pm %.2f", Sig_sieie, Sigerr_sieie));
  // tSB_sieie->DrawLatexNDC(0.16, 0.52, Form("Fake events = %.2f #pm %.2f", Bkg_sieie, Bkgerr_sieie));

  // tSB_dijetMass->SetTextSize(0.055);
  // tSB_dijetMass->DrawLatexNDC(0.75, 0.7, "M_{jj}[GeV]");
  // tSB_dijetMass->SetTextSize(0.04);
  // tSB_dijetMass->DrawLatexNDC(0.75, 0.64, Form("Toy Data = %.0f", data_dijetMass));
  // tSB_dijetMass->DrawLatexNDC(0.75, 0.60, Form("Fitted = %.2f #pm %.2f", Fitdata_dijetMass, Fitdataerr_dijetMass));
  // tSB_dijetMass->DrawLatexNDC(0.75, 0.56, Form("Z#gamma(VBS) = %.2f #pm %.2f (gen %.0f)", Sig_dijetMass, Sigerr_dijetMass, nVBS_toycut));
  // tSB_dijetMass->DrawLatexNDC(0.75, 0.52, Form("V#gammaJets = %.2f #pm %.2f (gen %.0f)", Bkg_dijetMass, Bkgerr_dijetMass, nTrue_toycut));
  // tSB_dijetMass->DrawLatexNDC(0.75, 0.48, Form("Fake#gamma = %.2f #pm %.2f (gen %.0f)", BkgFake_dijetMass, BkgFakeerr_dijetMass, nFake_toycut));
  // HFake->Draw("HIST SAME");
  // HTrue->Draw("HIST SAME");
  // HS_dijetMass->Draw("HIST SAME");
  // HCand->Draw("EP SAME");
  // HCand->SetLineColor(kBlack);
  // HCand->SetLineWidth(2);
  // HCand->SetMarkerStyle(8);
  // HCand->SetMarkerSize(0.7);
  
  // lhs = new TLegend(0.46, 0.72, 0.60, 0.88);
  // lhs->SetTextSize(0.038);
  // lhs->SetFillStyle(0);
  // //lhs->Clear();
  // //lhs->SetNColumns(2);
  // lhs->AddEntry(HFit, "Fit result", "L");
  // //lhs->AddEntry(HTrue, "True (#gamma+jet MC)", "PL");
  // lhs->AddEntry(HTrue, "True (True#gamma MCs)", "PL");
  // lhs->AddEntry(HFake, "Fake (Data)", "PL");
  // lhs->AddEntry(HCand, "DATA", "PL");
  // lhs->Draw("SAME");

  // lhs = new TLegend(0.75, 0.72, 0.90, 0.88);
  // lhs->SetTextSize(0.038);
  // lhs->SetFillStyle(0);
  // lhs->AddEntry(HVBS, "Z#gamma (VBS)", "F");
  // lhs->AddEntry(HSMTrue, "V#gamma+jets", "F");
  // lhs->AddEntry(HSMFake, "V+jets", "F");
  // lhs->AddEntry(HCand, "DATA", "PL");
  // lhs->Draw("SAME");
  // //ths_year->Draw("SAME");
  // ths->Draw("SAME");
  // tg_v->Draw("L SAME");
  // title = HFit->GetName();
  // pad1->RedrawAxis();
  // pad2->cd();
  // hratio = ratioplot(HCand, HFit);
  // hratio->SetTitle("");
  // hratio->Draw("EP");
  // //hratio->GetXaxis()->SetTitle("#sigma_{i#etai#eta}");
  // tg->Draw("L SAME");
  // tg_v->Draw("L SAME");
  // CMS_lumi(pad1, period, 0);
  // c1->Update();
  // //c1->cd();
  // pad2->RedrawAxis();
  // //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  // plot stack with all process
  
  // THStack *HS_dijetMass_cutsieie = new THStack("HS_dijetMass_cutsieie", "HS_dijetMass_cutsieie");
  
  // string hexcolorST[nSTcolor] = {"#03a9f4", "#fff176", "#ff9e00", "#b23b8c", "#a983d3", "#96ff5a", "#FFCCCC", "#67ccc1"};
  // TColor *colorST[nSTcolor];
  // Int_t cnumST[nSTcolor];
  // for(Int_t j=0; j<nSTcolor; j++){
  //   colorST[j] = new TColor();
  //   Int_t n = hexcolorST[j].length();
  //   char chararray[n+1];
  //   strcpy(chararray, hexcolorST[j].c_str());
  //   cnumST[j] = colorST[j]->GetColor(chararray);
  // }
  
  // Double_t VBSyield=0;
  // Double_t Trueyield=0;
  // Double_t Fakeyield=0;
  // for(Int_t ifile=0; ifile<nfile; ifile++){
  //   if(ifile==nfile-1) VBSyield += h_dijetMass_cutsieie[ifile]->Integral(1,-1);
  //   if(ifile==0) Fakeyield += h_dijetMass_cutsieie[ifile]->Integral(1,-1);
  //   if(ifile>0 && ifile<nfile-1) Trueyield += h_dijetMass_cutsieie[ifile]->Integral(1,-1);
  // }
  
  // for(Int_t ifile=0; ifile<nfile; ifile++){
  //   if(ifile==nfile-1) H_dijetMass_cutsieie[ifile]->Scale(TrueEff*para[0]/VBSyield);
  //   if(ifile==0) H_dijetMass_cutsieie[ifile]->Scale(FakeEff*para[2]/Fakeyield);
  //   if(ifile>0 && ifile<nfile-1) H_dijetMass_cutsieie[ifile]->Scale(TrueEff*para[1]/Trueyield);
  //   H_dijetMass_cutsieie[ifile]->SetFillColor(cnumST[ifile]);
  //   H_dijetMass_cutsieie[ifile]->SetLineColor(1);
  //   H_dijetMass_cutsieie[ifile]->SetLineWidth(1);
    
  //   HS_dijetMass_cutsieie->Add(H_dijetMass_cutsieie[ifile]);
  // }
  
  // pad1->cd();
  // for(Int_t ibin=1; ibin<=nbins_dijetMass; ibin++){
  //   HFit->SetBinContent(ibin+nbins_sieie, 1e-10);
  // }
  // HFit->Draw("HIST");
  // HFit->SetMaximum(100);
  // HFit->SetName(Form("HSFToyST_JFakePho_%i", ptbin));
  // HFit->GetYaxis()->SetTitle("Events");
  // HFit->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  // HFit->GetXaxis()->SetTitle("");
  // HFit->GetXaxis()->SetNdivisions(15);
  // HFit->SetMaximum(2.8*HFit->GetBinContent(HFit->GetMaximumBin()));
  
  // tSB_sieie->SetTextSize(0.055);
  // tSB_sieie->DrawLatexNDC(0.16, 0.7, "#sigma_{i#etai#eta}");
  // tSB_sieie->SetTextSize(0.04);
  // tSB_sieie->DrawLatexNDC(0.16, 0.64, Form("Toy Data events = %.0f", data_sieie));
  // tSB_sieie->DrawLatexNDC(0.16, 0.60, Form("Fitted events = %.2f #pm %.2f", Fitdata_sieie, Fitdataerr_sieie));
  // tSB_sieie->DrawLatexNDC(0.16, 0.56, Form("True events = %.2f #pm %.2f", Sig_sieie, Sigerr_sieie));
  // tSB_sieie->DrawLatexNDC(0.16, 0.52, Form("Fake events = %.2f #pm %.2f", Bkg_sieie, Bkgerr_sieie));

  // tSB_dijetMass->SetTextSize(0.055);
  // tSB_dijetMass->DrawLatexNDC(0.75, 0.7, "M_{jj}[GeV]");
  // tSB_dijetMass->SetTextSize(0.04);
  // tSB_dijetMass->DrawLatexNDC(0.75, 0.64, Form("Toy Data = %.0f", data_dijetMass));
  // tSB_dijetMass->DrawLatexNDC(0.75, 0.60, Form("Fitted = %.2f #pm %.2f", Fitdata_dijetMass, Fitdataerr_dijetMass));
  // tSB_dijetMass->DrawLatexNDC(0.75, 0.56, Form("Z#gamma(VBS) = %.2f #pm %.2f (gen %.0f)", Sig_dijetMass, Sigerr_dijetMass, nVBS_toycut));
  // tSB_dijetMass->DrawLatexNDC(0.75, 0.52, Form("V#gammaJets = %.2f #pm %.2f (gen %.0f)", Bkg_dijetMass, Bkgerr_dijetMass, nTrue_toycut));
  // tSB_dijetMass->DrawLatexNDC(0.75, 0.48, Form("Fake#gamma = %.2f #pm %.2f (gen %.0f)", BkgFake_dijetMass, BkgFakeerr_dijetMass, nFake_toycut));
  // HFake->Draw("HIST SAME");
  // HTrue->Draw("HIST SAME");
  // HS_dijetMass_cutsieie->Draw("HIST SAME");
  // HCand->Draw("EP SAME");
  // HCand->SetLineColor(kBlack);
  // HCand->SetLineWidth(2);
  // HCand->SetMarkerStyle(8);
  // HCand->SetMarkerSize(0.7);

  // TString ltext[20] = {"jet#rightarrow#gamma", "#gamma+jet", "V#gamma+jets", "Monoboson", "Diboson", "Triboson", "tt#gamma", "Z#gamma#rightarrow#nu#nu#gamma(VBS)", "DATA"};//Znunug VBS

  // lhs = new TLegend(0.46, 0.72, 0.58, 0.88);
  // lhs->SetTextSize(0.038);
  // lhs->SetFillStyle(0);
  // lhs->Clear();
  // //lhs->SetNColumns();
  // lhs->AddEntry(HFit, "Fit result", "L");
  // lhs->AddEntry(HTrue, "True (#gamma+jet MC)", "PL");
  // lhs->AddEntry(HFake, "Fake (Data)", "PL");
  // lhs->AddEntry(HCand, "Toy Data", "PL");
  // lhs->Draw("SAME");
  // lhs = new TLegend(0.75, 0.72, 0.96, 0.92);
  // lhs->SetTextSize(0.035);
  // lhs->SetFillStyle(0);
  // lhs->Clear();
  // lhs->SetNColumns(2);
  // lhs->AddEntry(H_dijetMass_cutsieie[0], ltext[0], "F");
  // lhs->AddEntry(H_dijetMass_cutsieie[1], ltext[1], "F");
  // lhs->AddEntry(H_dijetMass_cutsieie[2], ltext[2], "F");
  // lhs->AddEntry(H_dijetMass_cutsieie[3], ltext[3], "F");
  // lhs->AddEntry(H_dijetMass_cutsieie[4], ltext[4], "F");
  // lhs->AddEntry(H_dijetMass_cutsieie[5], ltext[5], "F");
  // lhs->AddEntry(H_dijetMass_cutsieie[6], ltext[6], "F");
  // lhs->AddEntry(H_dijetMass_cutsieie[7], ltext[7], "F");
  // lhs->AddEntry(HCand, "Toy Data", "PL");
  // lhs->Draw("SAME");
  // //ths_year->Draw("SAME");
  // ths->Draw("SAME");
  // tg_v->Draw("L SAME");
  // title = HFit->GetName();
  // pad1->RedrawAxis();
  // pad2->cd();
  // // hratio = ratioplot(HCand, HFit);
  // // hratio->SetTitle("");
  // hratio->Draw("EP");
  // //hratio->GetXaxis()->SetTitle("#sigma_{i#etai#eta}");
  // tg->Draw("L SAME");
  // tg_v->Draw("L SAME");
  // CMS_lumi(pad1, period, 0);
  // c1->Update();
  // //c1->cd();
  // pad2->RedrawAxis();
  // //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  
  printf("ToyData %7.1f events, fitted %7.1f\n", HCand->Integral(1, -1), HFit->Integral(1, -1));
  printf("Fake %5.1f #pm %5.1f events \n", para[0], errpara[0]);
  printf("True %5.1f #pm %5.1f events \n", para[1], errpara[1]);

  printf("ToyData sieie %7.1f events (sieie<0.01015), fitted %7.1f\n", data_sieie, Fitdata_sieie);
  printf("Fake sieie %5.1f #pm %5.1f eventTFitResultPtr fitresult = s (sieie<0.01015) \n", Bkg_sieie, Bkgerr_sieie);
  printf("True sieie %5.1f #pm %5.1f events (sieie<0.01015) \n", Sig_sieie, Sigerr_sieie);

  printf("ToyData dijetMass %7.1f events , fitted %7.1f\n", data_dijetMass, Fitdata_dijetMass);
  printf("VBS  dijetMass %5.1f #pm %5.1f events  \n", Sig_dijetMass, Sigerr_dijetMass);
  printf("TruePho dijetMass %5.1f #pm %5.1f events  \n", Bkg_dijetMass, Bkgerr_dijetMass);
  printf("FakePho dijetMass %5.1f #pm %5.1f events  \n", BkgFake_dijetMass, BkgFakeerr_dijetMass);

  printf("----- fit results with signal projection   ----------- \n");
  HCand->Chi2Test(HFit,"P");

  // save plots and numbers to .root file
  TFile *fout;
  if(year==2017) fout = new TFile(Form("/data1/GMET/Jfake_Toy/fall17/ptbin%i/JFake_Toys_ptbin%i_exp%i.root", ptbin, ptbin, iexp), "RECREATE");
  else if(year==2018) fout = new TFile(Form("/data1/GMET/Jfake_Toy/autumn18/ptbin%i/JFake_Toys_ptbin%i_exp%i.root", ptbin, ptbin, iexp), "RECREATE");
  fout->mkdir("HFit_plots");
  fout->mkdir("HCand_plots");
  fout->mkdir("HFake_plots");
  fout->mkdir("HTrue_plots");
  fout->mkdir("HSMTrue_plots");
  fout->mkdir("HSMFake_plots");
  fout->mkdir("HVBS_plots");

  // Nbias_Cand = (Nfit_Cand - Ngen_Cand) / TMath::Sqrt(Ngen_Cand);
  // Nbias_VBS = (Nfit_VBS - Ngen_VBS) / TMath::Sqrt(Ngen_VBS);
  // Nbias_SMTrue = (Nfit_SMTrue - Ngen_SMTrue) / TMath::Sqrt(Ngen_SMTrue);
  // Nbias_SMFake = (Nfit_SMFake - Ngen_SMFake) / TMath::Sqrt(Ngen_SMFake);
  // Nbiascutsieie_Cand = (Nfitcutsieie_Cand - Ngencutsieie_Cand) / TMath::Sqrt(Ngencutsieie_Cand);
  // Nbiascutsieie_VBS = (Nfitcutsieie_VBS - Ngencutsieie_VBS) / TMath::Sqrt(Ngencutsieie_VBS);
  // Nbiascutsieie_SMTrue = (Nfitcutsieie_SMTrue - Ngencutsieie_SMTrue) / TMath::Sqrt(Ngencutsieie_SMTrue);
  // Nbiascutsieie_SMFake = (Nfitcutsieie_SMFake - Ngencutsieie_SMFake) / TMath::Sqrt(Ngencutsieie_SMFake);

  Npull_Cand = (Nfit_Cand - Ngen_Cand) / TMath::Sqrt(Nfit_Cand);
  Npull_VBS = (Nfit_VBS - Ngen_VBS) / TMath::Sqrt(Nfit_VBS);
  Npull_SMTrue = (Nfit_SMTrue - Ngen_SMTrue) / TMath::Sqrt(Nfit_SMTrue);
  Npull_SMFake = (Nfit_SMFake - Ngen_SMFake) / TMath::Sqrt(Nfit_SMFake);
  Npullcutsieie_Cand = (Nfitcutsieie_Cand - Ngencutsieie_Cand) / TMath::Sqrt(Nfitcutsieie_Cand);
  Npullcutsieie_VBS = (Nfitcutsieie_VBS - Ngencutsieie_VBS) / TMath::Sqrt(Nfitcutsieie_VBS);
  Npullcutsieie_SMTrue = (Nfitcutsieie_SMTrue - Ngencutsieie_SMTrue) / TMath::Sqrt(Nfitcutsieie_SMTrue);
  Npullcutsieie_SMFake = (Nfitcutsieie_SMFake - Ngencutsieie_SMFake) / TMath::Sqrt(Nfitcutsieie_SMFake);

  TTree *outtree_;
  outtree_ = new TTree("EventTree", "EventTree");
  outtree_->Branch("Ngen_Cand",&Ngen_Cand, "Ngen_Cand/I");
  outtree_->Branch("Ngen_VBS",&Ngen_VBS, "Ngen_VBS/I");
  outtree_->Branch("Ngen_SMTrue",&Ngen_SMTrue, "Ngen_SMTrue/I");
  outtree_->Branch("Ngen_SMFake",&Ngen_SMFake, "Ngen_SMFake/I");
  outtree_->Branch("Ngencutsieie_Cand",&Ngencutsieie_Cand, "Ngencutsieie_Cand/I");
  outtree_->Branch("Ngencutsieie_VBS",&Ngencutsieie_VBS, "Ngencutsieie_VBS/I");
  outtree_->Branch("Ngencutsieie_SMTrue",&Ngencutsieie_SMTrue, "Ngencutsieie_SMTrue/I");
  outtree_->Branch("Ngencutsieie_SMFake",&Ngencutsieie_SMFake, "Ngencutsieie_SMFake/I");
  outtree_->Branch("Nfit_Cand",&Nfit_Cand, "Nfit_Cand/F");
  outtree_->Branch("Nfit_VBS",&Nfit_VBS, "Nfit_VBS/F");
  outtree_->Branch("Nfit_SMTrue",&Nfit_SMTrue, "Nfit_SMTrue/F");
  outtree_->Branch("Nfit_SMFake",&Nfit_SMFake, "Nfit_SMFake/F");
  outtree_->Branch("Nfitcutsieie_Cand",&Nfitcutsieie_Cand, "Nfitcutsieie_Cand/F");
  outtree_->Branch("Nfitcutsieie_VBS",&Nfitcutsieie_VBS, "Nfitcutsieie_VBS/F");
  outtree_->Branch("Nfitcutsieie_SMTrue",&Nfitcutsieie_SMTrue, "Nfitcutsieie_SMTrue/F");
  outtree_->Branch("Nfitcutsieie_SMFake",&Nfitcutsieie_SMFake, "Nfitcutsieie_SMFake/F");

  outtree_->Branch("Nfiterr_Cand",&Nfiterr_Cand, "Nfiterr_Cand/F");
  outtree_->Branch("Nfiterr_VBS",&Nfiterr_VBS, "Nfiterr_VBS/F");
  outtree_->Branch("Nfiterr_SMTrue",&Nfiterr_SMTrue, "Nfiterr_SMTrue/F");
  outtree_->Branch("Nfiterr_SMFake",&Nfiterr_SMFake, "Nfiterr_SMFake/F");
  outtree_->Branch("Nfiterrcutsieie_Cand",&Nfiterrcutsieie_Cand, "Nfiterrcutsieie_Cand/F");
  outtree_->Branch("Nfiterrcutsieie_VBS",&Nfiterrcutsieie_VBS, "Nfiterrcutsieie_VBS/F");
  outtree_->Branch("Nfiterrcutsieie_SMTrue",&Nfiterrcutsieie_SMTrue, "Nfiterrcutsieie_SMTrue/F");
  outtree_->Branch("Nfiterrcutsieie_SMFake",&Nfiterrcutsieie_SMFake, "Nfiterrcutsieie_SMFake/F");
  
  outtree_->Branch("Nbias_Cand",&Nbias_Cand, "Nbias_Cand/F");
  outtree_->Branch("Nbias_VBS",&Nbias_VBS, "Nbias_VBS/F");
  outtree_->Branch("Nbias_SMTrue",&Nbias_SMTrue, "Nbias_SMTrue/F");
  outtree_->Branch("Nbias_SMFake",&Nbias_SMFake, "Nbias_SMFake/F");
  outtree_->Branch("Nbiascutsieie_Cand",&Nbiascutsieie_Cand, "Nbiascutsieie_Cand/F");
  outtree_->Branch("Nbiascutsieie_VBS",&Nbiascutsieie_VBS, "Nbiascutsieie_VBS/F");
  outtree_->Branch("Nbiascutsieie_SMTrue",&Nbiascutsieie_SMTrue, "Nbiascutsieie_SMTrue/F");
  outtree_->Branch("Nbiascutsieie_SMFake",&Nbiascutsieie_SMFake, "Nbiascutsieie_SMFake/F");
  outtree_->Branch("Npull_Cand",&Npull_Cand, "Npull_Cand/F");
  outtree_->Branch("Npull_VBS",&Npull_VBS, "Npull_VBS/F");
  outtree_->Branch("Npull_SMTrue",&Npull_SMTrue, "Npull_SMTrue/F");
  outtree_->Branch("Npull_SMFake",&Npull_SMFake, "Npull_SMFake/F");
  outtree_->Branch("Npullcutsieie_Cand",&Npullcutsieie_Cand, "Npullcutsieie_Cand/F");
  outtree_->Branch("Npullcutsieie_VBS",&Npullcutsieie_VBS, "Npullcutsieie_VBS/F");
  outtree_->Branch("Npullcutsieie_SMTrue",&Npullcutsieie_SMTrue, "Npullcutsieie_SMTrue/F");
  outtree_->Branch("Npullcutsieie_SMFake",&Npullcutsieie_SMFake, "Npullcutsieie_SMFake/F");

  outtree_->Fill();

  fout->cd();
  outtree_->Write();
  fout->Close();
  fprintf(stderr, "Processed %ith ToyExp\n", iexp);

  fitted[0] = para[0];
  fitted[1] = errpara[0];
  fitted[2] = para[1];
  fitted[3] = errpara[1];
  return fitted;

}

void xDrawJFake_gMinutFit_ToyExp(int ptbin, int Nexp, int year){
  for(Int_t iexp=1; iexp<=Nexp; iexp++){
    gMinutFit(ptbin, iexp, year);
  }
}
