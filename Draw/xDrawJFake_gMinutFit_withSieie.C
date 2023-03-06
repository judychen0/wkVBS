/// simultaneously fit sieie & dijetMass

#include "TH2.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TF1.h"
#include <fstream>
#include <iostream>
#include <TMinuit.h>
#include <vector>
#include <TMath.h>
#include "TVirtualFitter.h"
#include "TFile.h"
#include "TLegend.h"
#include "TMarker.h"
#include "TLine.h"
#include "TRandom2.h"
#include "../Drawlumi/CMS_lumi.C"

using namespace std;

#define NPAR 3
#define ncolor 3
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
  ratio->GetYaxis()->SetTitleSize(0.105);
  ratio->GetYaxis()->SetTitleOffset(0.48);
  ratio->GetYaxis()->SetLabelFont(42);
  ratio->GetYaxis()->SetLabelOffset(0.01);
  ratio->GetYaxis()->SetLabelSize(0.1);
  ratio->GetYaxis()->SetTickSize(0.032);
  ratio->GetYaxis()->ChangeLabel(-1, -1, 0, -1, -1, -1, "");
  ratio->GetYaxis()->SetNdivisions(9);
  
  ratio->GetXaxis()->SetTitleFont(42);
  ratio->GetXaxis()->SetTitleSize(0.12);
  ratio->GetXaxis()->SetTitleOffset(1.15);
  ratio->GetXaxis()->SetLabelFont(42);
  ratio->GetXaxis()->SetLabelOffset(0.03);
  ratio->GetXaxis()->SetLabelSize(0.08);
  ratio->GetXaxis()->SetTickSize(0.1);
  //ratio->GetXaxis()->SetNdivisions(15);
  ratio->GetXaxis()->ChangeLabel(-1, -1, 0, -1, -1, -1, "");
  //ratio->GetXaxis()->ChangeLabel(1, -1, -1, -1, -1, -1, "0.05");
  // TString bintext[20] = {"0.005", "0.0075", "0.008", "0.0085", "0.009", "0.0095", "0.01015", "0.0113", "0.0125", "0.015", "0.0175", "0.02/250", "500", "1000", "2000", "5000"}; // 16 lablels for 15 bins
  // for(Int_t i=0; i<16; i++){
  //   ratio->GetXaxis()->ChangeLabel(i+1, -1, -1, -1, -1, -1, bintext[i]);
  // }
  
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

//--------------------------------------------------
// Define likelihood minimization funtion
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
      Lsum += TMath::Log( (fVBS*LVBS + fSMTrue*LSMTrue + fSMFake*LSMFake) / (fVBS+fSMTrue+fSMFake) );
    }
  }

  f=2*( -1*Lsum + (fVBS+fSMTrue+fSMFake) - Nevt*TMath::Log(fVBS+fSMTrue+fSMFake) );
  
}

//___________________________________________________________________________
Double_t* xDrawJFake_gMinutFit_withSieie(int ptbin, int year){
  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";
  
  TCanvas *c1 = new TCanvas("c1");
  double count=0;

  CandColl.clear();
  FakeColl.clear();
  TrueColl.clear();
  VBSColl.clear();

  //--------------------------------------------------
  // open templates from files
  TString rootname[20];
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
  TH1F *hCand_sieie = (TH1F*)fopen->Get(Form("CandPho_bin_template/hbin_CandPho_sieie_%i", ptbin));
  TH1F *hFake_sieie = (TH1F*)fopen->Get(Form("FakePho_bin_template/hbin_FakePho_sieie_%i", ptbin));
  TH1F *hCand_dijetMass = (TH1F*)fopen->Get(Form("CandPho_bin_template/hbin_CandPho_dijetMass_%i", ptbin));
  TH1F *hCand_dijetMass_cutsieie = (TH1F*)fopen->Get(Form("CandPho_bin_template/hbin_CandPho_dijetMass_cutsieie_%i", ptbin));

  // from GJet MC
  // fopen = new TFile(rootname[1]);
  // TH1F *hTrue_sieie = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_sieie_%i", ptbin));

  // from True photon MCs
  fopen = new TFile(rootname[2]);
  TH1F *hTrue_sieie = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_sieie_%i", ptbin));
  TH1F *hTrue_dijetMass = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_dijetMass_%i", ptbin));
  TH1F *hTrue_dijetMass_cutsieie = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_dijetMass_cutsieie_%i", ptbin));

  fopen = new TFile(rootname[3]);
  TH1F *hFake_dijetMass = (TH1F*)fopen->Get(Form("FakePho_bin_template/hbin_FakePho_dijetMass_%i", ptbin));
  TH1F *hFake_dijetMass_cutsieie = (TH1F*)fopen->Get(Form("FakePho_bin_template/hbin_FakePho_dijetMass_cutsieie_%i", ptbin));

  // from GJet MC
  // fopen = new TFile(rootname[1]);
  // TH1F *hVBS_sieie = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_sieie_%i", ptbin));

  fopen = new TFile(rootname[4]);
  TH1F *hVBS_sieie = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_sieie_%i", ptbin));
  TH1F *hVBS_dijetMass = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_dijetMass_%i", ptbin));
  TH1F *hVBS_dijetMass_cutsieie = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_dijetMass_cutsieie_%i", ptbin));

  // plots from each process
  TH1F *h_dijetMass_cutsieie[nfile];
  for(Int_t ifile=0; ifile<nfile; ifile++){
    fopen = new TFile(rootname[ifile+5]);
    if(ifile==0) h_dijetMass_cutsieie[ifile] = (TH1F*)fopen->Get(Form("FakePho_bin_template/hbin_FakePho_dijetMass_cutsieie_%i", ptbin));
    else h_dijetMass_cutsieie[ifile] = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_dijetMass_cutsieie_%i", ptbin));
  }

  //--------------------------------------------------
  // prepare normalized templates
  Int_t ndata = 0;
  ndata += hCand_sieie->Integral(1,-1);
  ndata += hCand_dijetMass->Integral(1,-1);

  Int_t ndata_sieie = 0;
  Int_t ndata_dijetMass = 0;
  ndata_sieie += hCand_sieie->Integral(1,-1);
  ndata_dijetMass += hCand_dijetMass->Integral(1,-1);

  Int_t nbins_sieie = hCand_sieie->GetNbinsX();
  Int_t nbins_dijetMass = hCand_dijetMass->GetNbinsX();
  Int_t Nbins_fit = nbins_sieie+nbins_dijetMass;

  TH1F* hCand_template = new TH1F("hCand_template", "", Nbins_fit, 0, Nbins_fit);
  TH1F* hTrue_template = new TH1F("hTrue_template", "", Nbins_fit, 0, Nbins_fit);
  TH1F* hFake_template = new TH1F("hFake_template", "", Nbins_fit, 0, Nbins_fit);
  TH1F* hVBS_template = new TH1F("hVBS_template", "", Nbins_fit, 0, Nbins_fit);

  hCand_template->Sumw2();
  hTrue_template->Sumw2();
  hFake_template->Sumw2();
  hVBS_template->Sumw2();

  hVBS_sieie->Scale(1./(hVBS_sieie->Integral(1,-1)));
  hTrue_sieie->Scale(1./(hTrue_sieie->Integral(1,-1)));
  hFake_sieie->Scale(1./(hFake_sieie->Integral(1,-1)));
  hVBS_dijetMass->Scale(1./(hVBS_dijetMass->Integral(1,-1)));
  hTrue_dijetMass->Scale(1./(hTrue_dijetMass->Integral(1,-1)));
  hFake_dijetMass->Scale(1./(hFake_dijetMass->Integral(1,-1)));
  hVBS_dijetMass_cutsieie->Scale(1./(hVBS_dijetMass_cutsieie->Integral(1,-1)));
  hTrue_dijetMass_cutsieie->Scale(1./(hTrue_dijetMass_cutsieie->Integral(1,-1)));
  hFake_dijetMass_cutsieie->Scale(1./(hFake_dijetMass_cutsieie->Integral(1,-1)));

  for(Int_t ibin=1; ibin<=nbins_sieie; ibin++){
    hCand_template->SetBinContent(ibin, hCand_sieie->GetBinContent(ibin));
    hVBS_template->SetBinContent(ibin, hVBS_sieie->GetBinContent(ibin));
    hTrue_template->SetBinContent(ibin, hTrue_sieie->GetBinContent(ibin));
    hFake_template->SetBinContent(ibin, hFake_sieie->GetBinContent(ibin));
  }
  for(Int_t ibin=1; ibin<=nbins_dijetMass; ibin++){
    if(ibin == nbins_dijetMass){
      hCand_template->SetBinContent(ibin+nbins_sieie, hCand_dijetMass->GetBinContent(ibin)+hCand_dijetMass->GetBinContent(ibin+1));
      hVBS_template->SetBinContent(ibin+nbins_sieie, hVBS_dijetMass->GetBinContent(ibin)+hVBS_dijetMass->GetBinContent(ibin+1));
      hTrue_template->SetBinContent(ibin+nbins_sieie, hTrue_dijetMass->GetBinContent(ibin)+hTrue_dijetMass->GetBinContent(ibin+1));
      hFake_template->SetBinContent(ibin+nbins_sieie, hFake_dijetMass->GetBinContent(ibin)+hFake_dijetMass->GetBinContent(ibin+1));
    }
    else{
      hCand_template->SetBinContent(ibin+nbins_sieie, hCand_dijetMass->GetBinContent(ibin));
      hVBS_template->SetBinContent(ibin+nbins_sieie, hVBS_dijetMass->GetBinContent(ibin));
      hTrue_template->SetBinContent(ibin+nbins_sieie, hTrue_dijetMass->GetBinContent(ibin));
      hFake_template->SetBinContent(ibin+nbins_sieie, hFake_dijetMass->GetBinContent(ibin));
    }
  }

  // get the fraction number of each template for fitting 
  Float_t VBSfrac = hVBS_template->Integral(1,-1)/ndata;
  Float_t Truefrac = hTrue_template->Integral(1,-1)/ndata;
  Float_t Fakefrac = hFake_template->Integral(1,-1)/ndata;  
  
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
        printf("Parameter (yeild) %d = %f +- %f\n",j,para[j],errpara[j]);
	
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
  hVBS_dijetMass->Scale(para[0]);
  hTrue_dijetMass->Scale(para[1]);
  hFake_dijetMass->Scale(para[2]);
  hVBS_dijetMass_cutsieie->Scale(para[0]);
  hTrue_dijetMass_cutsieie->Scale(para[1]);
  hFake_dijetMass_cutsieie->Scale(para[2]);
  
  // calculate sieie cut eff to normalize dijetMass distribution
  Double_t TrueEff = (hVBS_sieie->Integral(1,6)+hTrue_sieie->Integral(1,6)) / (hVBS_sieie->Integral(1,-1)+hTrue_sieie->Integral(1,-1));
  Double_t FakeEff = hFake_sieie->Integral(1,6) / hFake_sieie->Integral(1,-1);
  hVBS_dijetMass_cutsieie->Scale(TrueEff);
  hTrue_dijetMass_cutsieie->Scale(TrueEff);
  hFake_dijetMass_cutsieie->Scale(FakeEff);

  // creat Fit_sieie plots
  TH1F *HFit_sieie = (TH1F*)hVBS_sieie->Clone();
  HFit_sieie->Add(hTrue_sieie);
  HFit_sieie->Add(hFake_sieie);
  HFit_sieie->SetLineColor(60);
  HFit_sieie->SetMarkerColor(60);
  HFit_sieie->SetLineWidth(2);
  HFit_sieie->SetMarkerSize(0.4);

  // add hVBS and hTrue to make hTrue_sieie(all true pho)
  hTrue_sieie->Add(hVBS_sieie);

  // create stack plot for dijetMass
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

  hFake_dijetMass_cutsieie->SetFillColor(cnum[0]);
  hTrue_dijetMass_cutsieie->SetFillColor(cnum[1]);
  hVBS_dijetMass_cutsieie->SetFillColor(cnum[2]);

  hFake_dijetMass_cutsieie->SetLineColor(1);
  hTrue_dijetMass_cutsieie->SetLineColor(1);
  hVBS_dijetMass_cutsieie->SetLineColor(1);

  hFake_dijetMass_cutsieie->SetLineWidth(1);
  hTrue_dijetMass_cutsieie->SetLineWidth(1);
  hVBS_dijetMass_cutsieie->SetLineWidth(1);

  HS_dijetMass->Add(hFake_dijetMass_cutsieie);
  HS_dijetMass->Add(hTrue_dijetMass_cutsieie);
  HS_dijetMass->Add(hVBS_dijetMass_cutsieie);

  // create Fit_dijetMass plot
  TH1F *HFit_dijetMass = (TH1F*)hVBS_dijetMass_cutsieie->Clone();
  HFit_dijetMass->Add(hTrue_dijetMass_cutsieie);
  HFit_dijetMass->Add(hFake_dijetMass_cutsieie);
  HFit_dijetMass->SetLineColor(60);
  HFit_dijetMass->SetMarkerColor(60);
  HFit_dijetMass->SetLineWidth(2);
  HFit_dijetMass->SetMarkerSize(0.4);
  
  Double_t binrange[20] = {75, 110, 210, 2000};//3 bins

  //--------------------------------------------------
  // plot
  TLegend *lhs = new TLegend(0.46, 0.72, 0.73, 0.88);
  lhs->SetTextSize(0.038);
  lhs->SetFillStyle(0);

  TLatex *ths;
  ths = new TLatex(0.2, 0.88, Form("%.0f GeV < Photon p_{T} < %.0f GeV", binrange[ptbin-1], binrange[ptbin]));
  ths->SetNDC();
  ths->SetTextAlign(13);
  ths->SetTextFont(42);
  ths->SetTextSize(0.04);

  TLatex *tSB_sieie = new TLatex();
  Float_t Bkg_sieie = hFake_sieie->Integral(1, 6);
  Float_t Sig_sieie = hTrue_sieie->Integral(1, 6);
  Float_t Bkgerr_sieie = TMath::Sqrt(hFake_sieie->Integral(1, 6));
  Float_t Sigerr_sieie = TMath::Sqrt(hTrue_sieie->Integral(1, 6));
  Float_t data_sieie = hCand_sieie->Integral(1, 6);
  Float_t dataerr_sieie = TMath::Sqrt(hCand_sieie->Integral(1, 6));
  Float_t Fitdata_sieie = HFit_sieie->Integral(1, 6);
  Float_t Fitdataerr_sieie = TMath::Sqrt(HFit_sieie->Integral(1, 6));
  //tSB = new TLatex(0.56, 0.62, Form("Data events = %.0f #pm %.2f", data, dataerr));
  tSB_sieie->SetNDC();
  tSB_sieie->SetTextAlign(13);
  tSB_sieie->SetTextFont(42);
  tSB_sieie->SetTextSize(0.04);

  TLatex *tSB_dijetMass = new TLatex();
  Float_t BkgFake_dijetMass = hFake_dijetMass_cutsieie->Integral();
  Float_t Bkg_dijetMass = hTrue_dijetMass_cutsieie->Integral();
  Float_t Sig_dijetMass = hVBS_dijetMass_cutsieie->Integral();
  Float_t BkgFakeerr_dijetMass = TMath::Sqrt(hFake_dijetMass_cutsieie->Integral());
  Float_t Bkgerr_dijetMass = TMath::Sqrt(hTrue_dijetMass_cutsieie->Integral());
  Float_t Sigerr_dijetMass = TMath::Sqrt(hVBS_dijetMass_cutsieie->Integral());
  Float_t data_dijetMass = hCand_dijetMass_cutsieie->Integral();
  Float_t dataerr_dijetMass = TMath::Sqrt(hCand_dijetMass_cutsieie->Integral());
  Float_t Fitdata_dijetMass = HFit_dijetMass->Integral();
  Float_t Fitdataerr_dijetMass = TMath::Sqrt(HFit_dijetMass->Integral());
  tSB_dijetMass->SetNDC();
  tSB_dijetMass->SetTextAlign(13);
  tSB_dijetMass->SetTextFont(42);
  tSB_dijetMass->SetTextSize(0.04);

  TH1F *hratio;
  Double_t x[3] = {-2000, 0.0, 6000};
  Double_t y[3] = {0, 0, 0};
  TGraph *tg = new TGraph(3, x, y);
  tg->SetLineColor(kRed);
  tg->SetLineWidth(1);
  tg->SetLineStyle(8);
  
  TPad *pad1 = new TPad("pad1", "", 0., 0.3, .65, 1.);
  pad1->SetBottomMargin(0);
  pad1->SetRightMargin(0);
  pad1->Draw();

  TPad *pad2 = new TPad("pad2", "", 0., 0.02, .65, 0.3);
  pad2->SetTopMargin(0);
  pad2->SetRightMargin(0);
  pad2->SetBottomMargin(0.3);
  pad2->Draw();

  TPad *pad3 = new TPad("pad3", "", .65, 0.3, 1., 1.);
  pad3->SetBottomMargin(0);
  pad3->SetLeftMargin(0);
  pad3->Draw();

  TPad *pad4 = new TPad("pad4", "", .65, 0.02, 1., 0.3);
  pad4->SetTopMargin(0);
  pad4->SetLeftMargin(0);
  pad4->SetBottomMargin(0.3);
  pad4->Draw();
  
  TPave *pave;
  Float_t pave_y1 = 0;
  Float_t pave_y2 = 0;
  
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

  ///-------------------------------------------------
  /// plot Sieie
  ///-------------------------------------------------
  pad1->cd();
  HFit_sieie->Draw("HIST");
  HFit_sieie->SetMinimum(0);
  HFit_sieie->SetName(Form("HSF_JFakePho_withSieie_%i", ptbin));
  HFit_sieie->GetYaxis()->SetTitle("Events");
  HFit_sieie->GetYaxis()->SetTitleSize(0.045);
  HFit_sieie->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HFit_sieie->GetXaxis()->SetTitle("");
  HFit_sieie->GetXaxis()->SetNdivisions(408);
  HFit_sieie->SetMaximum(2.8*HFit_sieie->GetBinContent(HFit_sieie->GetMaximumBin()));

  // tSB_sieie->SetTextSize(0.055);
  // tSB_sieie->DrawLatexNDC(0.2, 0.75, "#sigma_{i#etai#eta}");
  tSB_sieie->SetTextSize(0.04);
  tSB_sieie->DrawLatexNDC(0.2, 0.69, Form("Data = %.0f", data_sieie));
  tSB_sieie->DrawLatexNDC(0.2, 0.65, Form("Fitted = %.2f #pm %.2f", Fitdata_sieie, Fitdataerr_sieie));
  tSB_sieie->DrawLatexNDC(0.2, 0.61, Form("True #gamma = %.2f #pm %.2f", Sig_sieie, Sigerr_sieie));
  tSB_sieie->DrawLatexNDC(0.2, 0.57, Form("Fake #gamma = %.2f #pm %.2f", Bkg_sieie, Bkgerr_sieie));

  hFake_sieie->SetLineColor(211);
  hTrue_sieie->SetLineColor(97);

  hFake_sieie->SetMarkerColor(211);
  hTrue_sieie->SetMarkerColor(97);

  hFake_sieie->SetLineWidth(2);
  hTrue_sieie->SetLineWidth(2);

  hFake_sieie->SetLineStyle(7);
  hTrue_sieie->SetLineStyle(5);

  hFake_sieie->SetMarkerSize(0.3);
  hTrue_sieie->SetMarkerSize(0.3);

  hFake_sieie->SetFillStyle(3145);
  hFake_sieie->SetFillColor(30);

  hFake_sieie->Draw("HIST SAME");
  hTrue_sieie->Draw("HIST SAME");
  hCand_sieie->Draw("EP SAME");
  hCand_sieie->SetLineColor(kBlack);
  hCand_sieie->SetLineWidth(2);
  hCand_sieie->SetMarkerStyle(8);
  hCand_sieie->SetMarkerSize(0.7);

  lhs = new TLegend(0.65, 0.68, 0.9, 0.88);
  lhs->SetTextSize(0.04);
  lhs->SetFillStyle(0);
  lhs->AddEntry(HFit_sieie, "Fit result", "L");
  lhs->AddEntry(hTrue_sieie, "True #gamma (True#gamma MCs)", "PL");
  lhs->AddEntry(hFake_sieie, "Fake #gamma (Data)", "PL");
  lhs->AddEntry(hCand_sieie, "DATA", "PL");
  lhs->Draw("SAME");
  ths->Draw("SAME");

  title = HFit_sieie->GetName();
  pad1->RedrawAxis();
  pad2->cd();
  hratio = ratioplot(hCand_sieie, HFit_sieie);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#sigma_{i#etai#eta}");
  hratio->GetXaxis()->SetNdivisions(408);
  hratio->Draw("EP");
  tg->Draw("L SAME");
  pad2->RedrawAxis();

  CMS_lumi(pad1, period, 0);
  c1->Update();
  pad1->cd();
  pave_y1 = 2.8*HFit_sieie->GetBinContent(HFit_sieie->GetMaximumBin()) + 0.5;
  pave_y2 = pave_y1 +8;
  pave = new TPave(0.015, pave_y1, 0.021, pave_y2, 0, "br");
  pave->SetLineWidth(0);
  pave->SetFillColor(0);
  pave->Draw();
  pad1->Modified();

  ///-------------------------------------------------
  /// plot dijetMass
  ///-------------------------------------------------
  pad3->cd();
  HFit_dijetMass->Draw("HIST");
  HFit_dijetMass->SetMinimum(0);
  //HFit_dijetMass->GetYaxis()->SetTitle("Events");
  HFit_dijetMass->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HFit_dijetMass->GetXaxis()->SetTitle("");
  HFit_dijetMass->GetXaxis()->SetNdivisions(10);
  HFit_dijetMass->SetMaximum(2.8*HFit_sieie->GetBinContent(HFit_sieie->GetMaximumBin()));
  
  // tSB_dijetMass->SetTextSize(0.052);
  // tSB_dijetMass->DrawLatexNDC(0.1, 0.75, "M_{jj}[GeV]");
  tSB_dijetMass->SetTextSize(0.04);
  tSB_dijetMass->DrawLatexNDC(0.12, 0.69, Form("Data = %.0f", data_dijetMass));
  tSB_dijetMass->DrawLatexNDC(0.12, 0.65, Form("Fitted = %.2f #pm %.2f", Fitdata_dijetMass, Fitdataerr_dijetMass));
  tSB_dijetMass->DrawLatexNDC(0.12, 0.61, Form("Z#gamma(VBS) = %.2f #pm %.2f", Sig_dijetMass, Sigerr_dijetMass));
  tSB_dijetMass->DrawLatexNDC(0.12, 0.57, Form("V#gamma+jets = %.2f #pm %.2f", Bkg_dijetMass, Bkgerr_dijetMass));
  tSB_dijetMass->DrawLatexNDC(0.12, 0.53, Form("Fake #gamma = %.2f #pm %.2f", BkgFake_dijetMass, BkgFakeerr_dijetMass));
  HS_dijetMass->Draw("HIST SAME");
  hCand_dijetMass_cutsieie->Draw("EP SAME");
  hCand_dijetMass_cutsieie->SetLineColor(kBlack);
  hCand_dijetMass_cutsieie->SetLineWidth(2);
  hCand_dijetMass_cutsieie->SetMarkerStyle(8);
  hCand_dijetMass_cutsieie->SetMarkerSize(0.7);

  lhs = new TLegend(0.55, 0.68, 0.75, 0.88);
  lhs->SetTextSize(0.04);
  lhs->SetFillStyle(0);
  lhs->AddEntry(hVBS_dijetMass_cutsieie, "Z#gamma (VBS)", "F");
  lhs->AddEntry(hTrue_dijetMass_cutsieie, "V#gamma+jets", "F");
  lhs->AddEntry(hFake_dijetMass_cutsieie, "Fake #gamma (V+jets)", "F");
  lhs->AddEntry(hCand_dijetMass_cutsieie, "DATA", "PL");
  lhs->Draw("SAME");
  pad3->RedrawAxis();
  pad4->cd();
  hratio = ratioplot(hCand_dijetMass_cutsieie, HFit_dijetMass);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("M_{jj} [GeV]");
  hratio->GetXaxis()->SetNdivisions(10);
  hratio->GetYaxis()->SetTitle("");
  hratio->Draw("EP");
  tg->Draw("L SAME");
  pad4->RedrawAxis();
 
  CMS_lumi(pad3, period, 0);
  c1->Update();
  pad3->cd();
  pave = new TPave(240, pave_y1, 1500, pave_y2, 0, "br");
  pave->SetLineWidth(0);
  pave->SetFillColor(0);
  pave->Draw();
  pad3->Modified();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //--------------------------------------------------
  // print out information on screen
  // printf("Data %7.1f events, fitted %7.1f\n", HCand->Integral(1, -1), HFit->Integral(1, -1));
  // printf("Fake %5.1f #pm %5.1f events \n", para[0], errpara[0]);
  // printf("True %5.1f #pm %5.1f events \n", para[1], errpara[1]);

  printf("Data sieie %7.1f events (sieie<0.01015), fitted %7.1f\n", data_sieie, Fitdata_sieie);
  printf("Fake sieie %5.1f #pm %5.1f events (sieie<0.01015) \n", Bkg_sieie, Bkgerr_sieie);
  printf("True sieie %5.1f #pm %5.1f events (sieie<0.01015) \n", Sig_sieie, Sigerr_sieie);

  printf("Data dijetMass %7.1f events , fitted %7.1f\n", data_dijetMass, Fitdata_dijetMass);
  printf("VBS  dijetMass %5.1f #pm %5.1f events  \n", Sig_dijetMass, Sigerr_dijetMass);
  printf("TruePho dijetMass %5.1f #pm %5.1f events  \n", Bkg_dijetMass, Bkgerr_dijetMass);
  printf("FakePho dijetMass %5.1f #pm %5.1f events  \n", BkgFake_dijetMass, BkgFakeerr_dijetMass);
  
  printf("----- fit results with signal projection   ----------- \n");
  hCand_sieie->Chi2Test(HFit_sieie,"P");

  fitted[0] = para[0];
  fitted[1] = errpara[0];
  fitted[2] = para[1];
  fitted[3] = errpara[1];
  return fitted;
}
