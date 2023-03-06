// simultaneously fit dijetMass sigR & cR

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
#define ncolor 4
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
  ratio->GetYaxis()->ChangeLabel( 1, -1, 0, -1, -1, -1, "");
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
vector<Double_t> TrueColl;
vector<Double_t> VBSColl;
vector<Double_t> WgColl;

vector<Double_t> info;
vector<Double_t> info_err;


// Wgamma SCratio = SR yield / CR yield
// Double_t SCratio17[3] = {1.685, 1.561, 1.309}; //avg 1.51
// Double_t SCratio18[3] = {1.782, 1.560, 1.177}; //avg 1.506
Double_t SCratio17[3] = {1.537, 1.428, 0.536}; //avg 1.51
Double_t SCratio18[3] = {1.782, 1.560, 1.177}; //avg 1.506

// TrueWgamma CRratio = True CR yield / Wg CR yield
Double_t CRratio17[3] = {1.01, 1.25, 2.53}; //avg 1.51
//Double_t SCratio18[3] = {1.782, 1.560, 1.177}; //avg 1.506

// Fake yield in ptbin
Double_t Fake17[3] = {20.0, 12.42, 3.38};
Double_t Fake18[3] = {23.51, 22.87, 6.12};

// Fake rate (in signal region) = fake yield / cand yield (from sieie fit)
Double_t Fakerate17[3] = {0.246, 0.147, 0.102};
Double_t Fakerate18[3] = {0.181, 0.184, 0.111};

//--------------------------------------------------
// Define likelihood minimization funtion
void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{

  Double_t Lsum=0.;
  Double_t Nevt=0.;
  Double_t Nevt_CR=0.;
  
  Double_t fVBS = par[0];
  Double_t fSMTrue = par[1];
  Double_t fWg_CR = par[2];
  Double_t fWg = 0.536*par[2];
  Double_t fVBS_CR = 0.008*par[0];
  Double_t fSMTrue_CR = 2.53*par[2]; //2017
  //Double_t fSMTrue_CR = 1.03*par[2]; //2018
  
  for ( int i=0; i<CandColl.size(); i++ ) {
    //PDF for signal and background
    Double_t LVBS = VBSColl[i];
    Double_t LSMTrue = TrueColl[i];
    Double_t LWg = WgColl[i];
    
    if(i<4){

      Nevt += CandColl[i];
      for (int data=0; data<CandColl[i]; data++) {
	Lsum += TMath::Log( (fVBS*LVBS + fSMTrue*LSMTrue + fWg*LWg) / (fVBS+fSMTrue+fWg) );
      }
    }
    else{
      Nevt_CR += CandColl[i];
      for (int data=0; data<CandColl[i]; data++) {
	Lsum += TMath::Log( ((fVBS_CR+fSMTrue_CR)*(LVBS+LSMTrue) + fWg_CR*LWg) / (fVBS_CR+fSMTrue_CR+fWg_CR) );
      }
    }
    
    
    // for (int data=0; data<CandColl[i]; data++) {	
    //   //Get Log Likelihood
    //   //Lsum += TMath::Log( (fVBS*LVBS + fSMTrue*LSMTrue + fSMFake*LSMFake) / (fVBS+fSMTrue+fSMFake) );
    //   if(i<4) Lsum += TMath::Log( (fVBS*LVBS + fSMTrue*LSMTrue + fWg*LWg) / (fVBS+fSMTrue+fWg) );
    //   else Lsum += TMath::Log( (fVBS_CR*LVBS + fSMTrue_CR*LSMTrue + fWg_CR*LWg) / (fVBS_CR+fSMTrue_CR+fWg_CR) );
    // }
  }

  f=2*( -1*Lsum + (fVBS+fSMTrue+fWg) + (fWg_CR+fVBS_CR+fSMTrue_CR) - Nevt*TMath::Log(fVBS+fSMTrue+fWg) - Nevt_CR*TMath::Log(fWg_CR+fVBS_CR+fSMTrue_CR) );

}

//___________________________________________________________________________
Double_t* xDrawJFake_gMinutFit_simulMjj(int ptbin, int year){
  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  TCanvas *c1 = new TCanvas("c1");
  double count=0;
  Double_t Fakeyield = 0.;
  Double_t Fakerate = 0.;
  Double_t SCratio = 0.;

  CandColl.clear();
  VBSColl.clear();
  TrueColl.clear();
  WgColl.clear();

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
    rootname[1] = "/data1/GMET/Jfake_pseudoData/fall17/Wgamma/output_ggtree_WG.root";
    rootname[2] = "/data1/GMET/Jfake_pseudoData/fall17/Wgamma/output_ggtree_SMFake.root";
    rootname[3] = "/data1/GMET/Jfake_pseudoData/fall17/Wgamma/output_ggtree_SMTrue.root";
    rootname[4] = "/data1/GMET/Jfake_pseudoData/fall17/Wgamma/output_ggtree_VBS.root";

    rootname[5] = "/data1/GMET/WgCR_Data/fall17/output_ggtree.root";
    rootname[6] = "/data1/GMET/WgCR_MC/fall17/output_merge_WG.root";
    rootname[7] = "/data1/GMET/WgCR_MC/fall17/output_merge_SMFake.root";
    rootname[8] = "/data1/GMET/WgCR_MC/fall17/output_merge_SMTrue.root";
    rootname[9] = "/data1/GMET/WgCR_MC/fall17/output_merge_VBS.root";

    Fakeyield = Fake17[ptbin-1];
    Fakerate = Fakerate17[ptbin-1];
    SCratio = SCratio17[ptbin-1];
  }
  else if(year==2018){
    rootname[0] = "/data1/GMET/Jfake_Data/autumn18/output_ggtree.root";
    rootname[1] = "/data1/GMET/Jfake_pseudoData/autumn18/Wgamma/output_ggtree_WG.root";
    rootname[2] = "/data1/GMET/Jfake_pseudoData/autumn18/Wgamma/output_ggtree_SMFake.root";
    rootname[3] = "/data1/GMET/Jfake_pseudoData/autumn18/Wgamma/output_ggtree_SMTrue.root";
    rootname[4] = "/data1/GMET/Jfake_pseudoData/autumn18/Wgamma/output_ggtree_VBS.root";

    rootname[5] = "/data1/GMET/WgCR_Data/autumn18/output_ggtree.root";
    rootname[6] = "/data1/GMET/WgCR_MC/autumn18/output_merge_WG.root";
    rootname[7] = "/data1/GMET/WgCR_MC/autumn18/output_merge_SMFake.root";
    rootname[8] = "/data1/GMET/WgCR_MC/autumn18/output_merge_SMTrue.root";
    rootname[9] = "/data1/GMET/WgCR_MC/autumn18/output_merge_VBS.root";
    
    Fakeyield = Fake18[ptbin-1];
    Fakerate = Fakerate18[ptbin-1];
    SCratio = SCratio18[ptbin-1];
    
    // rootname[5] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/JetFake/output_merge_JetFake.root";
    // rootname[6] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/GJet/output_merge_GJet.root";    
    // rootname[7] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/VG/output_merge_VG.root";
    // rootname[8] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/monoboson/output_merge_monoBoson.root";
    // rootname[9] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/diboson/output_merge_diBoson.root";
    // rootname[10] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/triboson/output_merge_triBoson.root";
    // rootname[11] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/TT/output_merge_TT.root";
    // rootname[12] = "/data1/GMET/Jfake_pseudoData/autumn18/job_UL18_Znunug_VBS/output_ggtree.root";
  }

  TFile *fopen = new TFile(rootname[0]);
  TH1F *hCand = (TH1F*)fopen->Get(Form("CandPho_bin_template/hbin_CandPho_dijetMass_cutsieie_%i", ptbin));
  
  fopen = new TFile(rootname[1]);
  TH1F *hWgTrue = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_dijetMass_cutsieie_%i", ptbin));

  fopen = new TFile(rootname[2]);
  TH1F *hFake = (TH1F*)fopen->Get(Form("FakePho_bin_template/hbin_FakePho_dijetMass_cutsieie_%i", ptbin));
    
  fopen = new TFile(rootname[3]);
  TH1F *hTrue = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_dijetMass_cutsieie_%i", ptbin));

  fopen = new TFile(rootname[4]);
  TH1F *hVBS = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_dijetMass_cutsieie_%i", ptbin));
  
  fopen = new TFile(rootname[5]);
  TH1F *hCandCR = (TH1F*)fopen->Get(Form("hbin_WgCR_dijetMass_cutsieie_%i", ptbin));

  fopen = new TFile(rootname[6]);
  TH1F *hWgCR = (TH1F*)fopen->Get(Form("hbin_WgCR_dijetMass_cutsieie_%i", ptbin));

  fopen = new TFile(rootname[7]);
  TH1F *hFakeCR = (TH1F*)fopen->Get(Form("hbin_WgCR_dijetMass_cutsieie_%i", ptbin));

  fopen = new TFile(rootname[8]);
  TH1F *hTrueCR = (TH1F*)fopen->Get(Form("hbin_WgCR_dijetMass_cutsieie_%i", ptbin));

  fopen = new TFile(rootname[9]);
  TH1F *hVBSCR = (TH1F*)fopen->Get(Form("hbin_WgCR_dijetMass_cutsieie_%i", ptbin));

  // check zero bins
  for(Int_t ibin=1; ibin<=4; ibin++){
    if(hCand->GetBinContent(ibin) < 1) hCand->SetBinContent(ibin, 1e-10);
    if(hCandCR->GetBinContent(ibin) < 1) hCandCR->SetBinContent(ibin, 1e-10);
  }

  //--------------------------------------------------
  // prepare normalized templates
  hVBS->Scale(1./hVBS->Integral());
  hTrue->Scale(1./hTrue->Integral());
  hWgTrue->Scale(1./hWgTrue->Integral());
  hVBSCR->Scale(1./hVBSCR->Integral());
  hTrueCR->Scale(1./hTrueCR->Integral());
  hWgCR->Scale(1./hWgCR->Integral());
  
  //--------------------------------------------------
  // prepare templates for fit (Cand teplate with fake subtracted)

  Int_t nbins = hCand->GetNbinsX();
  Int_t nbins_fit = 2*nbins;
  
  hFake->Scale(Fakeyield/hFake->Integral());
  hFakeCR->Scale(Fakerate*hCandCR->Integral()/hFakeCR->Integral());

  TH1F* hCand_template = new TH1F("hCand_template", "", nbins_fit, 0, nbins_fit);

  TH1F* hVBS_template = new TH1F("hVBS_template", "", nbins_fit, 0, nbins_fit);
  TH1F* hTrue_template = new TH1F("hTrue_template", "", nbins_fit, 0, nbins_fit);
  TH1F* hWg_template = new TH1F("hWg_template", "", nbins_fit, 0, nbins_fit);

  for(Int_t ibin=1; ibin<=nbins; ibin++){
    hCand_template->SetBinContent(ibin, hCand->GetBinContent(ibin)-hFake->GetBinContent(ibin));
    hVBS_template->SetBinContent(ibin, hVBS->GetBinContent(ibin));
    hTrue_template->SetBinContent(ibin, hTrue->GetBinContent(ibin));
    hWg_template->SetBinContent(ibin, hWgTrue->GetBinContent(ibin));
  }
 
  for(Int_t ibin=1; ibin<=nbins_fit; ibin++){
    hCand_template->SetBinContent(ibin+nbins, hCandCR->GetBinContent(ibin)-hFakeCR->GetBinContent(ibin));
    hVBS_template->SetBinContent(ibin+nbins, hVBSCR->GetBinContent(ibin));
    hTrue_template->SetBinContent(ibin+nbins, hTrueCR->GetBinContent(ibin));
    hWg_template->SetBinContent(ibin+nbins, hWgCR->GetBinContent(ibin));
  }

  // get the fraction number of each template for fitting
  Int_t ndata = 0;
  ndata += hCand_template->Integral(1,nbins);
  Int_t ndata_CR = 0;
  ndata_CR += hCandCR->Integral(1,nbins);

  Float_t VBSfrac = hVBS_template->Integral(1, nbins)/ndata;
  Float_t Truefrac = hTrue_template->Integral(1, nbins)/ndata;
  Float_t VBSCRfrac = hVBS_template->Integral(nbins+1, nbins_fit)/ndata_CR;
  Float_t TrueCRfrac = hTrue_template->Integral(nbins+1, nbins_fit)/ndata_CR;
  Float_t WgCRfrac = hWg_template->Integral(nbins+1, nbins_fit)/ndata_CR;

  Double_t* fitted = new Double_t[6];
  if(ndata==0) {
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
  Float_t NWg = hWg_template->Integral(1,-1);

  printf(" --------- before the fit ------------- \n");
  printf("NCand %3.3f, NVBS %3.3f, NTrue %3.3f, NWg %3.3f \n", Ncand, NVBS, NTrue, NWg);
  printf(" -------------------------------------- \n");

  for(Int_t ibin=1; ibin<=nbins_fit; ibin++){
    CandColl.push_back(hCand_template->GetBinContent(ibin));
    VBSColl.push_back(hVBS_template->GetBinContent(ibin));
    TrueColl.push_back(hTrue_template->GetBinContent(ibin));
    WgColl.push_back(hWg_template->GetBinContent(ibin));
    if(CandColl[ibin-1] == 0.0) CandColl[ibin-1] += 1e-10;
    if(VBSColl[ibin-1] == 0.0) VBSColl[ibin-1] += 1e-10;
    if(TrueColl[ibin-1] == 0.0) TrueColl[ibin-1] += 1e-10;
    if(WgColl[ibin-1] == 0.0) WgColl[ibin-1] += 1e-10;
  }

  printf( " -----  Got %d, %d, %d, %d events for fit ----- \n ", CandColl.size(), VBSColl.size(), TrueColl.size(), WgColl.size() );
  if ( CandColl.size()!=VBSColl.size() || CandColl.size()!=TrueColl.size() || CandColl.size()!= WgColl.size() ) {
    printf(" error ...  inconsistent hit collection size \n");
    return fitted;
  }

  //--------------------------------------------------
  //init parameters for fit
  Double_t vstart[NPAR] = {1., 1., 1.};

  vstart[0] = VBSfrac*ndata;
  vstart[1] = Truefrac*ndata;
  vstart[2] = WgCRfrac*ndata_CR;
  //vstart[3] = VBSCRfrac*ndata_CR;
  vstart[3] = TrueCRfrac*ndata_CR;

  TMinuit *gMinuit = new TMinuit(NPAR);  
  gMinuit->Command("SET STR 1");
  gMinuit->SetFCN(fcn);
  Double_t arglist[10];
  Int_t ierflg = 0;

  arglist[0] = 1;
  gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);
  arglist[0] = 1;
  gMinuit->mnexcm("SET PRINT", arglist ,1,ierflg);

  Double_t step[NPAR] = {0.1, 0.1, 0.1};
  gMinuit->mnparm(0,  "VBS yield"  , vstart[0],  step[0], 0., ndata*1 , ierflg);
  gMinuit->mnparm(1,  "TruePho yield"  , vstart[1],  step[1], 0., ndata*1 , ierflg);
  gMinuit->mnparm(2,  "WgCR yield"  , vstart[2],  step[2], 0., ndata_CR*1 , ierflg);
  //gMinuit->mnparm(3,  "VBSCR yield"  , vstart[3],  step[3], 0., ndata_CR*1 , ierflg);
  //gMinuit->mnparm(3,  "TrueCR yield"  , vstart[3],  step[3], 0., ndata_CR*1 , ierflg);

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
    gMinuit->GetParameter(3, para[3],errpara[3]);
    gMinuit->GetParameter(4, para[4],errpara[4]);
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

  hVBS->Scale(para[0]);
  hTrue->Scale(para[1]);
  hWgTrue->Scale(para[2]*SCratio);
  hWgCR->Scale(para[2]);
  hVBSCR->Scale(0.008*para[0]);
  hTrueCR->Scale(1.57*para[2]);

  string hexcolor[ncolor] = {"#03a9f4", "#ff9e00", "#b23b8c", "#67ccc1"};
  TColor *color[ncolor];
  Int_t cnum[ncolor];
  for(Int_t j=0; j<ncolor; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  }

  // create stack plots
  THStack *HSF_Mjj = new THStack("HSF_Mjj", "HSF_Mjj");
  hFake->SetFillColor(cnum[0]);
  hTrue->SetFillColor(cnum[1]);
  hWgTrue->SetFillColor(cnum[2]);
  hVBS->SetFillColor(cnum[3]);

  hFake->SetLineColor(1);
  hTrue->SetLineColor(1);
  hWgTrue->SetLineColor(1);
  hVBS->SetLineColor(1);

  hFake->SetLineWidth(1);
  hTrue->SetLineWidth(1);
  hWgTrue->SetLineWidth(1);
  hVBS->SetLineWidth(1);

  HSF_Mjj->Add(hFake);
  HSF_Mjj->Add(hTrue);
  HSF_Mjj->Add(hWgTrue);
  HSF_Mjj->Add(hVBS);

  TH1F *HFit = (TH1F*)hVBS->Clone();
  HFit->Add(hTrue);
  HFit->Add(hFake);
  HFit->Add(hWgTrue);

  THStack *HSF_MjjCR = new THStack("HSF_MjjCR", "HSF_MjjCR");
  hFakeCR->SetFillColor(cnum[0]);
  hTrueCR->SetFillColor(cnum[1]);
  hWgCR->SetFillColor(cnum[2]);
  hVBSCR->SetFillColor(cnum[3]);

  hFakeCR->SetLineColor(1);
  hTrueCR->SetLineColor(1);
  hWgCR->SetLineColor(1);
  hVBSCR->SetLineColor(1);

  hFakeCR->SetLineWidth(1);
  hTrueCR->SetLineWidth(1);
  hWgCR->SetLineWidth(1);
  hVBSCR->SetLineWidth(1);

  HSF_MjjCR->Add(hFakeCR);
  HSF_MjjCR->Add(hTrueCR);
  HSF_MjjCR->Add(hWgCR);
  HSF_MjjCR->Add(hVBSCR);

  TH1F *HFitCR = (TH1F*)hVBSCR->Clone();
  HFitCR->Add(hTrueCR);
  HFitCR->Add(hFakeCR);
  HFitCR->Add(hWgCR);

  Double_t binrange[20] = {75, 110, 210, 2000};//3 bins

  TLegend *lhs = new TLegend(0.4, 0.6, 0.73, 0.88);
  lhs->SetTextSize(0.038);
  lhs->SetFillStyle(0);

  TLatex *ths;
  ths = new TLatex(0.2, 0.88, Form("%.0f GeV < Photon p_{T} < %.0f GeV", binrange[ptbin-1], binrange[ptbin]));
  ths->SetNDC();
  ths->SetTextAlign(13);
  ths->SetTextFont(42);
  ths->SetTextSize(0.04);

  TLatex *tSB = new TLatex();
  Float_t BkgFake = hFake->Integral();
  //Float_t Bkg = hTrue->Integral() + hWgTrue->Integral();
  Float_t BkgZg = hTrue->Integral();
  Float_t BkgWg = hWgTrue->Integral();
  Float_t Sig = hVBS->Integral();
  Float_t data = hCand->Integral(1,-1);
  Float_t Fitdata = HFit->Integral();
  Float_t BkgFakeerr = TMath::Sqrt(hFake->Integral());
  //Float_t Bkgerr = TMath::Sqrt(hTrue->Integral() + hWgTrue->Integral());
  Float_t BkgZgerr = TMath::Sqrt(hTrue->Integral());
  Float_t BkgWgerr = TMath::Sqrt(hWgTrue->Integral());
  Float_t Sigerr = TMath::Sqrt(hVBS->Integral());
  Float_t Fitdataerr = TMath::Sqrt(HFit->Integral());
  tSB->SetNDC();
  tSB->SetTextAlign(13);
  tSB->SetTextFont(42);
  tSB->SetTextSize(0.04);

  TLatex *tSB_CR = new TLatex();
  Float_t BkgFake_CR = hFakeCR->Integral();
  //Float_t Bkg_CR = hTrueCR->Integral() + hWgCR->Integral();
  Float_t BkgZg_CR = hTrueCR->Integral();
  Float_t BkgWg_CR = hWgCR->Integral();
  Float_t Sig_CR = hVBSCR->Integral();
  Float_t data_CR = hCandCR->Integral(1,-1);
  Float_t Fitdata_CR = HFitCR->Integral();
  Float_t BkgFakeerr_CR = TMath::Sqrt(hFakeCR->Integral());
  //  Float_t Bkgerr_CR = TMath::Sqrt(hTrueCR->Integral() + hWgCR->Integral());
  Float_t BkgZgerr_CR = TMath::Sqrt(hTrueCR->Integral());
  Float_t BkgWgerr_CR = TMath::Sqrt(hWgCR->Integral());
  Float_t Sigerr_CR = TMath::Sqrt(hVBSCR->Integral());
  Float_t Fitdataerr_CR = TMath::Sqrt(HFitCR->Integral());
  tSB_CR->SetNDC();
  tSB_CR->SetTextAlign(13);
  tSB_CR->SetTextFont(42);
  tSB_CR->SetTextSize(0.04);

  TH1F *hratio;
  Double_t x[3] = {-2000, 0.0, 6000};
  Double_t y[3] = {0, 0, 0};
  TGraph *tg = new TGraph(3, x, y);
  tg->SetLineColor(kRed);
  tg->SetLineWidth(1);
  tg->SetLineStyle(8);

  TPad *pad1 = new TPad("pad1", "", 0., 0.3, .5, 1.);
  pad1->SetBottomMargin(0);
  pad1->SetRightMargin(0);
  pad1->Draw();

  TPad *pad2 = new TPad("pad2", "", 0., 0.02, .5, 0.3);
  pad2->SetTopMargin(0);
  pad2->SetRightMargin(0);
  pad2->SetBottomMargin(0.3);
  pad2->Draw();

  TPad *pad3 = new TPad("pad3", "", .5, 0.3, 1., 1.);
  pad3->SetBottomMargin(0);
  pad3->SetLeftMargin(0);
  pad3->Draw();

  TPad *pad4 = new TPad("pad4", "", .5, 0.02, 1., 0.3);
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
  /// plot dijetMass
  ///-------------------------------------------------
  pad1->cd();
  HFit->Draw("HIST");
  HFit->SetMinimum(0);
  HFit->SetName(Form("HSF_Mjj_%i", ptbin));
  HFit->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HFit->GetXaxis()->SetTitle("");
  HFit->GetXaxis()->SetNdivisions(10);
  HFit->GetYaxis()->SetTitle("Events");
  HFit->GetYaxis()->SetTitleSize(0.045);
  HFit->SetMaximum(2.5*HFit->GetBinContent(HFit->GetMaximumBin()));

  tSB->SetTextSize(0.055);
  tSB->DrawLatexNDC(0.24, 0.79, "Z#gamma(VBS) SR");
  tSB->SetTextSize(0.04);
  tSB->DrawLatexNDC(0.24, 0.73, Form("Data = %.0f", data));
  tSB->DrawLatexNDC(0.24, 0.69, Form("Fitted = %.2f #pm %.2f", Fitdata, Fitdataerr));
  tSB->DrawLatexNDC(0.24, 0.65, Form("Z#gamma(VBS) = %.2f #pm %.2f", Sig, Sigerr));
  tSB->DrawLatexNDC(0.24, 0.61, Form("W#gamma+jets = %.2f #pm %.2f", BkgWg, BkgWgerr));
  tSB->DrawLatexNDC(0.24, 0.57, Form("True #gamma bkg = %.2f #pm %.2f", BkgZg, BkgZgerr));
  tSB->DrawLatexNDC(0.24, 0.53, Form("Fake #gamma = %.2f #pm %.2f", BkgFake, BkgFakeerr));
  HSF_Mjj->Draw("HIST SAME");
  hCand->Draw("EP SAME");
  hCand->SetLineColor(kBlack);
  hCand->SetLineWidth(2);
  hCand->SetMarkerStyle(8);
  hCand->SetMarkerSize(0.7);
  ths->Draw("SAME");
  pad1->RedrawAxis();
  pad2->cd();
  hratio = ratioplot(hCand, HFit);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetNdivisions(10);
  //hratio->GetYaxis()->SetTitle("");
  hratio->Draw("EP");
  tg->Draw("L SAME");
  pad2->RedrawAxis();

  CMS_lumi(pad1, period, 0);
  c1->Update();
  pad1->cd();
  pave_y1 = 2.5*HFit->GetBinContent(HFit->GetMaximumBin()) + 0.25;
  pave_y2 = pave_y1 +8;
  
  pave = new TPave(3500, pave_y1, 5000, pave_y2, 0, "br");
  pave->SetLineWidth(0);
  pave->SetFillColor(0);
  pave->Draw();
  pad1->Modified();
  

  ///-------------------------------------------------
  /// plot WgCR
  ///-------------------------------------------------
  pad3->cd();
  HFitCR->Draw("HIST");
  HFitCR->SetMinimum(0);
  HFitCR->GetYaxis()->SetLabelSize(0);
  HFitCR->GetXaxis()->SetTitle("");
  HFitCR->GetXaxis()->SetNdivisions(10);
  HFitCR->SetMaximum(2.5*HFit->GetBinContent(HFit->GetMaximumBin()));

  tSB_CR->SetTextSize(0.055);
  tSB_CR->DrawLatexNDC(0.1, 0.79, "W#gamma CR");
  tSB_CR->SetTextSize(0.04);
  tSB_CR->DrawLatexNDC(0.1, 0.73, Form("Data = %.0f", data_CR));
  tSB_CR->DrawLatexNDC(0.1, 0.69, Form("Fitted = %.2f #pm %.2f", Fitdata_CR, Fitdataerr_CR));
  tSB_CR->DrawLatexNDC(0.1, 0.65, Form("Z#gamma(VBS) = %.2f #pm %.2f", Sig_CR, Sigerr_CR));
  tSB_CR->DrawLatexNDC(0.1, 0.61, Form("W#gamma+jets = %.2f #pm %.2f", BkgWg_CR, BkgWgerr_CR));
  tSB_CR->DrawLatexNDC(0.1, 0.57, Form("True #gamma bkg = %.2f #pm %.2f", BkgZg_CR, BkgZgerr_CR));
  tSB_CR->DrawLatexNDC(0.1, 0.53, Form("Fake #gamma = %.2f #pm %.2f", BkgFake_CR, BkgFakeerr_CR));
  HSF_MjjCR->Draw("HIST SAME");
  hCandCR->Draw("EP SAME");
  hCandCR->SetLineColor(kBlack);
  hCandCR->SetLineWidth(2);
  hCandCR->SetMarkerStyle(8);
  hCandCR->SetMarkerSize(0.7);
 
  lhs = new TLegend(0.58, 0.66, 0.82, 0.88);
  lhs->SetTextSize(0.04);
  lhs->SetFillStyle(0);
  lhs->AddEntry(hVBS, "Z#gamma (VBS)", "F");
  lhs->AddEntry(hWgTrue, "W#gamma+jets", "F");
  lhs->AddEntry(hTrue, "True #gamma", "F");
  lhs->AddEntry(hFake, "Fake #gamma (V+jets)", "F");
  lhs->AddEntry(hCand, "DATA", "PL");
  lhs->Draw("SAME");
  pad3->RedrawAxis();
  pad4->cd();
  hratio = ratioplot(hCandCR, HFitCR);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetNdivisions(10);
  hratio->GetXaxis()->SetTitle("M_{jj}[GeV]");
  hratio->GetYaxis()->SetTitle("");
  hratio->GetYaxis()->SetLabelSize(0);
  hratio->Draw("EP");
  tg->Draw("L SAME");
  pad4->RedrawAxis();
 
  CMS_lumi(pad3, period, 0);
  c1->Update();
  pad3->cd();
  
  pave = new TPave(250, pave_y1, 1500, pave_y2, 0, "br");
  pave->SetLineWidth(0);
  pave->SetFillColor(0);
  pave->Draw();
  pad3->Modified();
  title = HFit->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  return nullptr;
  printf("----- fit results with signal projection   ----------- \n");
  hCandCR->Chi2Test(HFitCR,"P");
  
  fitted[0] = para[0];
  fitted[1] = errpara[0];
  fitted[2] = para[1];
  fitted[3] = errpara[1];
  return fitted;
}
