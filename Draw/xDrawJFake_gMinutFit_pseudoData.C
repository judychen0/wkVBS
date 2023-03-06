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

#define NPAR 2

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
  ratio->GetYaxis()->SetTitleSize(0.11);
  ratio->GetYaxis()->SetTitleOffset(0.4);
  ratio->GetYaxis()->SetLabelFont(42);
  ratio->GetYaxis()->SetLabelOffset(0.008);
  ratio->GetYaxis()->SetLabelSize(0.1);
  ratio->GetYaxis()->SetTickSize(0.032);
  ratio->GetYaxis()->ChangeLabel(-1, -1, 0, -1, -1, -1, "");
  ratio->GetYaxis()->SetNdivisions(9);
  
  ratio->GetXaxis()->SetTitleFont(42);
  ratio->GetXaxis()->SetTitleSize(0.15);
  ratio->GetXaxis()->SetTitleOffset(0.9);
  ratio->GetXaxis()->SetLabelFont(42);
  ratio->GetXaxis()->SetLabelOffset(0.03);
  ratio->GetXaxis()->SetLabelSize(0.1);
  ratio->GetXaxis()->SetTickSize(0.1);
  ratio->GetXaxis()->ChangeLabel(-1, -1, 0, -1, -1, -1, "");

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

vector<Double_t> info;
vector<Double_t> info_err;

void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{

  Double_t Lsum=0.;
  Double_t Nevt=0.;
  Double_t fFake = par[0];
  Double_t fTrue = par[1];
  
  for ( int i=0; i<CandColl.size(); i++ ) {
    Nevt += CandColl[i];
    //PDF for signal and background
    Double_t LFake = FakeColl[i];
    Double_t LTrue = TrueColl[i];
    for (int data=0; data<CandColl[i]; data++) {	
      //Get Log Likelihood
      Lsum += TMath::Log( (fFake*LFake + fTrue*LTrue) / (fFake+fTrue) );
    }
  }
  f=2*( -1*Lsum + (fFake+fTrue)  - Nevt*TMath::Log(fFake+fTrue) );
  
}

//___________________________________________________________________________
Double_t* xDrawJFake_gMinutFit_pseudoData(int ptbin, int year){

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  TCanvas *c1 = new TCanvas("c1");
  double count=0;

  CandColl.clear();
  FakeColl.clear();
  TrueColl.clear();

  TString rootname[20];
  if(year==2016){
    rootname[0] = "/data1/GMET/Jfake_Data/summer16/output_ggtree.root";
    rootname[1] = "/data1/GMET/Jfake_MC/summer16/output_ggtree.root";
    rootname[2] = "/data1/GMET/Jfake_pseudoData/summer16/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/data1/GMET/Jfake_Data/fall17/output_ggtree.root";
    rootname[1] = "/data1/GMET/Jfake_MC/fall17/output_ggtree.root";
    rootname[2] = "/data1/GMET/Jfake_pseudoData/fall17/allMCs/output_merge_allbkg.root";
    rootname[3] = "/data1/GMET/Jfake_pseudoData/fall17/output_ggtree_all.root";
    //rootname[2] = "/data1/GMET/Jfake_MC/fall17/Fake_corr.root";
  }
  else if(year==2018){
    rootname[0] = "/data1/GMET/Jfake_Data/autumn18/output_ggtree.root";
    rootname[1] = "/data1/GMET/Jfake_MC/autumn18/output_ggtree.root";
    rootname[2] = "/data1/GMET/Jfake_pseudoData/autumn18/allMCs/output_merge_allbkg.root";
    rootname[3] = "/data1/GMET/Jfake_pseudoData/autumn18/output_ggtree_all.root";
  }

  TFile* fopen = new TFile(rootname[0]);
  TH1F* hCand = (TH1F*)fopen->Get(Form("CandPho_bin_template/hbin_CandPho_sieie_%i", ptbin));
  TH1F* hFake = (TH1F*)fopen->Get(Form("FakePho_bin_template/hbin_FakePho_sieie_%i", ptbin));
  
  fopen = new TFile(rootname[1]);
  TH1F* hTrue = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_sieie_%i", ptbin));
  //fopen = new TFile(rootname[2]);
  //TH1F* hFake_corr = (TH1F*)fopen->Get(Form("hbin_FakePho_sieie_%i", ptbin));
  //hFake_corr->Scale(0.573);
  //TH1F* hFake = (TH1F*)fopen->Get(Form("hbin_FakePhoMatch_sieie_%i", ptbin));

  fopen = new TFile(rootname[2]);
  TH1F *hFake_allbkg = (TH1F*)fopen->Get(Form("FakePho_bin_template/hbin_FakePho_sieie_%i", ptbin));

  fopen = new TFile(rootname[3]);
  TH1F *hCand_pseudoTrue = (TH1F*)fopen->Get(Form("TruePho_bin_template/hbin_TruePho_sieie_%i", ptbin));
  TH1F *hCand_pseudoFake = (TH1F*)fopen->Get(Form("FakePho_bin_template/hbin_FakePho_sieie_%i", ptbin));
  TH1F *hCand_pseudoData = (TH1F*)hCand_pseudoTrue->Clone();
  hCand_pseudoData->Add(hCand_pseudoFake);

  Int_t nbins = hCand_pseudoData->GetNbinsX();
  Int_t ndata = hCand_pseudoData->Integral(1, -1);

  //hFake->Add(hFake_corr, -1);

  Float_t Fakefrac = hFake->Integral(1, -1)/ndata;
  Float_t Truefrac = hTrue->Integral(1, -1)/ndata;

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

  printf(" --------- before the fit ------------- \n");
  printf("NCand %3.3f, NFake %3.3f, NTrue %3.3f \n", hCand_pseudoData->Integral(), hFake->Integral(), hTrue->Integral());
  
  printf(" -------------------------------------- \n");

  hFake->Scale(1./hFake->Integral(1, -1));
  hTrue->Scale(1./hTrue->Integral(1, -1));
  
  for(Int_t ibin=1; ibin<=nbins; ibin++){
    CandColl.push_back(hCand_pseudoData->GetBinContent(ibin));
    FakeColl.push_back(hFake->GetBinContent(ibin));
    TrueColl.push_back(hTrue->GetBinContent(ibin));
    if(CandColl[ibin-1] == 0.0) CandColl[ibin-1] += 1e-10;
    if(FakeColl[ibin-1] == 0.0) FakeColl[ibin-1] += 1e-10;
    if(TrueColl[ibin-1] == 0.0) TrueColl[ibin-1] += 1e-10;
  }
  printf( " -----  Got %d, %d, %d events for fit ----- \n ", CandColl.size(), FakeColl.size(), TrueColl.size() );
  if ( CandColl.size() != FakeColl.size() || CandColl.size()!= TrueColl.size()) {
    printf(" error ...  inconsistent hit collection size \n");
    return fitted;
  }

  //--------------------------------------------------
  //init parameters for fit
  Double_t vstart[10] = {1., 1., 1.};
  vstart[0] = Fakefrac*ndata;
  vstart[1] = Truefrac*ndata;
  
  TMinuit *gMinuit = new TMinuit(NPAR);  
  gMinuit->Command("SET STR 1");
  gMinuit->SetFCN(fcn);
  Double_t arglist[10];
  Int_t ierflg = 0;
  
  arglist[0] = 1;
  gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);
  arglist[0] = 1;
  gMinuit->mnexcm("SET PRINT", arglist ,1,ierflg);

  Double_t step[] = { 0.1, 0.1, 1};
  gMinuit->mnparm(0,  "Fake yield"  , vstart[0],  step[0], 0., ndata*1. , ierflg);
  gMinuit->mnparm(1,  "True yield"  , vstart[1],  step[1], 0., ndata*1. , ierflg);

  printf(" --------------------------------------------------------- \n");
  printf(" Now ready for minimization step \n --------------------------------------------------------- \n");
  
  arglist[0] = 2000; // number of iteration
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
      printf(" fitted yield %1.3f \n", (para[0]+para[1])/ndata );

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

  TH1F* HCand = (TH1F*)hCand_pseudoData->Clone();
  TH1F* HFake = (TH1F*)hFake->Clone();
  TH1F* HTrue = (TH1F*)hTrue->Clone();

  HFake->Scale(para[0]);
  HTrue->Scale(para[1]);

  TH1F* hfit = (TH1F*)HFake->Clone();
  hfit->Add(HTrue);

  hfit->SetLineColor(60);
  HFake->SetLineColor(211);
  HTrue->SetLineColor(97);

  hfit->SetMarkerColor(60);
  HFake->SetMarkerColor(211);
  HTrue->SetMarkerColor(97);

  hfit->SetLineWidth(2);
  HFake->SetLineWidth(2);
  HTrue->SetLineWidth(2);

  HFake->SetLineStyle(7);
  HTrue->SetLineStyle(5);

  hfit->SetMarkerSize(0.4);
  HFake->SetMarkerSize(0.3);
  HTrue->SetMarkerSize(0.3);

  HFake->SetFillStyle(3145);
  HFake->SetFillColor(30);

  hFake_allbkg->SetLineColor(216); //216
  hFake_allbkg->SetLineWidth(2);
  hFake_allbkg->SetFillStyle(3354);
  hFake_allbkg->SetFillColor(38);
  //hFake_allbkg->SetLineStyle(7);
  hFake_allbkg->SetMarkerColor(216);
  hFake_allbkg->SetMarkerSize(0.3);
  
  Double_t binrange[20] = {75, 110, 210, 2000};//3 bins, 2016
  
  // plot
  c1->Draw();

  TLegend *lhs = new TLegend(0.55, 0.6, 0.8, 0.85);
  lhs->SetTextSize(0.042);
  lhs->SetFillStyle(0);

  TLatex *ths;
  ths = new TLatex(0.2, 0.88, Form("%.0f GeV < Photon p_{T} < %.0f GeV", binrange[ptbin-1], binrange[ptbin]));
  ths->SetNDC();
  ths->SetTextAlign(13);
  ths->SetTextFont(42);
  ths->SetTextSize(0.04);

  TLatex *tSB;
  Float_t Bkg = HFake->Integral(1, 6);
  Float_t Sig = HTrue->Integral(1, 6);
  Float_t Bkgerr = TMath::Sqrt(HFake->Integral(1, 6));
  Float_t Sigerr = TMath::Sqrt(HTrue->Integral(1, 6));
  Float_t data = HCand->Integral(1, 6);
  Float_t dataerr = TMath::Sqrt(HCand->Integral(1, 6));
  Float_t Fitdata = hfit->Integral(1, 6);
  Float_t Fitdataerr = TMath::Sqrt(hfit->Integral(1, 6));
  //tSB = new TLatex(0.56, 0.62, Form("B/(S+B) = %.4f #pm %.4f", BSratio, BSratioerr));
  tSB = new TLatex(0.56, 0.62, Form("Data events = %.0f #pm %.2f", data, dataerr));
  tSB->SetNDC();
  tSB->SetTextAlign(13);
  tSB->SetTextFont(42);
  tSB->SetTextSize(0.042);
  
  TH1F *hratio;
  Double_t x[3] = {-2000, 0.0, 2000};
  Double_t y[3] = {0, 0, 0};
  TGraph *tg = new TGraph(3, x, y);
  tg->SetLineColor(kRed);
  tg->SetLineWidth(1);
  tg->SetLineStyle(8);

  TPad *pad1 = new TPad("pad1", "", 0., 0.3, 1., 1.);
  //pad1->SetTopMargin(0.03);
  pad1->SetBottomMargin(0);
  //pad1->SetLogy(1);
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

  pad1->cd();
  HCand->Draw("EP");
  HCand->SetLineColor(kBlack);
  HCand->SetLineWidth(2);
  HCand->SetMarkerStyle(8);
  HCand->SetMarkerSize(0.7);
  HCand->SetName(Form("HF_JFakePho_pseudoData_%i", ptbin));
  HCand->GetYaxis()->SetTitleSize(0.048);
  HCand->GetYaxis()->SetTitle("Events");
  HCand->GetXaxis()->SetTitle("");
  HCand->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HCand->SetMaximum(2.*HCand->GetBinContent(HCand->GetMaximumBin()));

  hfit->Draw("HIST SAME");
  HFake->Draw("HIST SAME");
  hFake_allbkg->Draw("HIST SAME");
  HTrue->Draw("HIST SAME");
  HCand->Draw("EP SAME");

  lhs->Clear();
  lhs->AddEntry(hfit, "Fit result", "L");
  lhs->AddEntry(HTrue, "True #gamma (#gamma+jet MC)", "PL");
  lhs->AddEntry(HFake, "Fake #gamma (Data)", "PL");
  lhs->AddEntry(hFake_allbkg, "Fake #gamma (MC)", "PL");
  lhs->AddEntry(HCand, "DATA", "PL");
  lhs->Draw("SAME");
  ths->Draw("SAME");
  tSB->DrawLatexNDC(0.56, 0.58, Form("Data = %.0f", data));
  tSB->DrawLatexNDC(0.56, 0.54, Form("Fitted = %.2f #pm %.2f", Fitdata, Fitdataerr));
  tSB->DrawLatexNDC(0.56, 0.50, Form("True #gamma = %.2f #pm %.2f", Sig, Sigerr));
  tSB->DrawLatexNDC(0.56, 0.46, Form("Fake #gamma = %.2f #pm %.2f", Bkg, Bkgerr));
  title = HCand->GetName();
  pad2->cd();
  hratio = ratioplot(HCand, hfit);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#sigma_{i#etai#eta}");
  hratio->Draw("EP");
  tg->Draw("L SAME");

  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  //if(year == 2016) c1->SaveAs(Form("%s/%s_preVFP.pdf", saveto, title));
  if(year == 2016) c1->SaveAs(Form("%s/%s_postVFP.pdf", saveto, title));
  else c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  // TString folder;
  // if(year==2017) folder = "fall17";
  // else if(year==2018) folder = "autumn18";
  // TFile *fout = new TFile(Form("./%s", folder), "RECREATE");
  // hfit->Write();
  // hFake->Write();
  // hTrue->Write();
  // fout->Close();

  printf("Data %7.1f events, fitted %7.1f\n", HCand->Integral(1, -1), hfit->Integral(1, -1));
  printf("Fake %5.1f #pm %5.1f events \n", para[0], errpara[0]);
  printf("True %5.1f #pm %5.1f events \n", para[1], errpara[1]);

  printf("Data %7.1f events (sieie<0.0105), fitted %7.1f\n", HCand->Integral(1, 6), hfit->Integral(1, 6));
  printf("Fake %5.1f #pm %5.1f events (sieie<0.0105) \n", HFake->Integral(1, 6), TMath::Sqrt(HFake->Integral(1, 6)));
  printf("True %5.1f #pm %5.1f events (sieie<0.0105) \n", HTrue->Integral(1, 6), TMath::Sqrt(HTrue->Integral(1, 6)));

  printf("----- fit results with signal projection   ----------- \n");
  hCand_pseudoData->Chi2Test(hfit,"P");

  fitted[0] = para[0];
  fitted[1] = errpara[0];
  fitted[2] = para[1];
  fitted[3] = errpara[1];
  return fitted;
    
}

