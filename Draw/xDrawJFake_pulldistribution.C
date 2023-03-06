#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <math.h>
using namespace std;
#include "TH1F.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TPaveStats.h"

#include "/home/jou/wkVBS/runGG_Plot/smallntuplizer.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "/home/jou/wkVBS/Draw/Drawlumi/CMS_lumi.C"

Float_t Maximum(Float_t compare, Float_t input){
  if(compare >= input) return compare;
  else return input;
}

void xDrawJFake_pulldistribution(Int_t year, Int_t ptbin, Int_t Nexp){
  //***********************Initialization***********************//

  TFile *fopen;
  char rootname[100];

  TH1F *hpull_Cand = new TH1F("hpull_Cand", "", 100, -10, 10);
  TH1F *hpull_VBS = new TH1F("hpull_VBS", "", 100, -10, 10);
  TH1F *hpull_SMTrue = new TH1F("hpull_SMTrue", "", 100, -10, 10);
  TH1F *hpull_SMFake = new TH1F("hpull_SMFake", "", 100, -10, 10);
  hpull_Cand->Sumw2();
  hpull_VBS->Sumw2();
  hpull_SMTrue->Sumw2();
  hpull_SMFake->Sumw2();

  TH1F *hpullcutsieie_Cand = new TH1F("hpullcutsieie_Cand", "", 100, -10, 10);
  TH1F *hpullcutsieie_VBS = new TH1F("hpullcutsieie_VBS", "", 100, -10, 10);
  TH1F *hpullcutsieie_SMTrue = new TH1F("hpullcutsieie_SMTrue", "", 100, -10, 10);
  TH1F *hpullcutsieie_SMFake = new TH1F("hpullcutsieie_SMFake", "", 100, -10, 10);
  hpullcutsieie_Cand->Sumw2();
  hpullcutsieie_VBS->Sumw2();
  hpullcutsieie_SMTrue->Sumw2();
  hpullcutsieie_SMFake->Sumw2();

  TH1F *hfiterrcutsieie_Cand = new TH1F("hfiterrcutsieie_Cand", "", 100, 0, 20);
  TH1F *hfiterrcutsieie_VBS = new TH1F("hfiterrcutsieie_VBS", "", 100, 0, 20);
  TH1F *hfiterrcutsieie_SMTrue = new TH1F("hfiterrcutsieie_SMTrue", "", 100, 0, 20);
  TH1F *hfiterrcutsieie_SMFake = new TH1F("hfiterrcutsieie_SMFake", "", 100, 0, 20);
  hfiterrcutsieie_Cand->Sumw2();
  hfiterrcutsieie_VBS->Sumw2();
  hfiterrcutsieie_SMTrue->Sumw2();
  hfiterrcutsieie_SMFake->Sumw2();
  
  TH1F *hbias_Cand = new TH1F("hbias_Cand", "", 100, -10, 10);
  TH1F *hbias_VBS = new TH1F("hbias_VBS", "", 100, -10, 10);
  TH1F *hbias_SMTrue = new TH1F("hbias_SMTrue", "", 100, -10, 10);
  TH1F *hbias_SMFake = new TH1F("hbias_SMFake", "", 100, -10, 10);
  hbias_Cand->Sumw2();
  hbias_VBS->Sumw2();
  hbias_SMTrue->Sumw2();
  hbias_SMFake->Sumw2();

  TH1F *hbiascutsieie_Cand = new TH1F("hbiascutsieie_Cand", "", 100, -10, 10);
  TH1F *hbiascutsieie_VBS = new TH1F("hbiascutsieie_VBS", "", 100, -10, 10);
  TH1F *hbiascutsieie_SMTrue = new TH1F("hbiascutsieie_SMTrue", "", 100, -10, 10);
  TH1F *hbiascutsieie_SMFake = new TH1F("hbiascutsieie_SMFake", "", 100, -10, 10);
  hbiascutsieie_Cand->Sumw2();
  hbiascutsieie_VBS->Sumw2();
  hbiascutsieie_SMTrue->Sumw2();
  hbiascutsieie_SMFake->Sumw2();

  // input variables
  Int_t Ngen_Cand = 0;
  Int_t Ngen_VBS = 0;
  Int_t Ngen_SMTrue = 0;
  Int_t Ngen_SMFake = 0;
  Int_t Ngencutsieie_Cand = 0;
  Int_t Ngencutsieie_VBS = 0;
  Int_t Ngencutsieie_SMTrue = 0;
  Int_t Ngencutsieie_SMFake = 0;
  Int_t Nfit_Cand = 0;
  Int_t Nfit_VBS = 0;
  Int_t Nfit_SMTrue = 0;
  Int_t Nfit_SMFake = 0;
  Int_t Nfitcutsieie_Cand = 0;
  Int_t Nfitcutsieie_VBS = 0;
  Int_t Nfitcutsieie_SMTrue = 0;
  Int_t Nfitcutsieie_SMFake = 0;

  Int_t Nfiterrcutsieie_Cand = 0;
  Int_t Nfiterrcutsieie_VBS = 0;
  Int_t Nfiterrcutsieie_SMTrue = 0;
  Int_t Nfiterrcutsieie_SMFake = 0;
  
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

  //***********************Loop all Toys************************//
  for(Int_t iexp=0; iexp<Nexp; iexp++){
    if(year==2017){
      sprintf(rootname, "/data1/GMET/Jfake_Toy/fall17/ptbin%i/JFake_Toys_ptbin%i_exp%i.root", ptbin, ptbin, iexp+1);
    }
    else if(year==2018){
      sprintf(rootname, "/data1/GMET/Jfake_Toy/autumn18/ptbin%i/JFake_Toys_ptbin%i_exp%i.root", ptbin, ptbin, iexp+1);
    }

    fopen = new TFile(rootname, "READ");
    TreeReader data(rootname);
    data.GetEntry(0);

    // initialize input variables
    Ngen_Cand = data.GetInt("Ngen_Cand");
    Ngen_VBS = data.GetInt("Ngen_VBS");
    Ngen_SMTrue = data.GetInt("Ngen_SMTrue");
    Ngen_SMFake = data.GetInt("Ngen_SMFake");
    Ngencutsieie_Cand = data.GetInt("Ngencutsieie_Cand");
    Ngencutsieie_VBS = data.GetInt("Ngencutsieie_VBS");
    Ngencutsieie_SMTrue = data.GetInt("Ngencutsieie_SMTrue");
    Ngencutsieie_SMFake = data.GetInt("Ngencutsieie_SMFake");
    Nfit_Cand = data.GetFloat("Nfit_Cand");
    Nfit_VBS = data.GetFloat("Nfit_VBS");
    Nfit_SMTrue = data.GetFloat("Nfit_SMTrue");
    Nfit_SMFake = data.GetFloat("Nfit_SMFake");
    Nfitcutsieie_Cand = data.GetFloat("Nfitcutsieie_Cand");
    Nfitcutsieie_VBS = data.GetFloat("Nfitcutsieie_VBS");
    Nfitcutsieie_SMTrue = data.GetFloat("Nfitcutsieie_SMTrue");
    Nfitcutsieie_SMFake = data.GetFloat("Nfitcutsieie_SMFake");

    Nfiterrcutsieie_Cand = data.GetFloat("Nfiterrcutsieie_Cand");
    Nfiterrcutsieie_VBS = data.GetFloat("Nfiterrcutsieie_VBS");
    Nfiterrcutsieie_SMTrue = data.GetFloat("Nfiterrcutsieie_SMTrue");
    Nfiterrcutsieie_SMFake = data.GetFloat("Nfiterrcutsieie_SMFake");
    
    Nbias_Cand = data.GetFloat("Nbias_Cand");
    Nbias_VBS = data.GetFloat("Nbias_VBS");
    Nbias_SMTrue = data.GetFloat("Nbias_SMTrue");
    Nbias_SMFake = data.GetFloat("Nbias_SMFake");
    Nbiascutsieie_Cand = data.GetFloat("Nbiascutsieie_Cand");
    Nbiascutsieie_VBS = data.GetFloat("Nbiascutsieie_VBS");
    Nbiascutsieie_SMTrue = data.GetFloat("Nbiascutsieie_SMTrue");
    Nbiascutsieie_SMFake = data.GetFloat("Nbiascutsieie_SMFake");

    Npull_Cand = data.GetFloat("Npull_Cand");
    Npull_VBS = data.GetFloat("Npull_VBS");
    Npull_SMTrue = data.GetFloat("Npull_SMTrue");
    Npull_SMFake = data.GetFloat("Npull_SMFake");
    Npullcutsieie_Cand = data.GetFloat("Npullcutsieie_Cand");
    Npullcutsieie_VBS = data.GetFloat("Npullcutsieie_VBS");
    Npullcutsieie_SMTrue = data.GetFloat("Npullcutsieie_SMTrue");
    Npullcutsieie_SMFake = data.GetFloat("Npullcutsieie_SMFake");
    
    // fill pull to hpull
    hpull_Cand->Fill(Npull_Cand);
    hpull_VBS->Fill(Npull_VBS);
    hpull_SMTrue->Fill(Npull_SMTrue);
    hpull_SMFake->Fill(Npull_SMFake);
    hpullcutsieie_Cand->Fill(Npullcutsieie_Cand);
    hpullcutsieie_VBS->Fill(Npullcutsieie_VBS);
    hpullcutsieie_SMTrue->Fill(Npullcutsieie_SMTrue);
    hpullcutsieie_SMFake->Fill(Npullcutsieie_SMFake);

    // fill err
    hfiterrcutsieie_Cand->Fill(Nfiterrcutsieie_Cand);
    hfiterrcutsieie_VBS->Fill(Nfiterrcutsieie_VBS);
    hfiterrcutsieie_SMTrue->Fill(Nfiterrcutsieie_SMTrue);
    hfiterrcutsieie_SMFake->Fill(Nfiterrcutsieie_SMFake);
    fopen->Close();
  }

  //******************fit the pull distribution*****************//
  TCanvas *c1 = new TCanvas("c1");
  gStyle->SetOptFit(1);
  gStyle->SetOptStat("e");

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";
  
  TF1 *fitresult;
  
  Double_t binrange[20] = {75, 110, 210, 2000};//3 bins
  TLatex *ths;
  ths = new TLatex(0.16, 0.88, Form("%.0f GeV < Photon p_{T} < %.0f GeV", binrange[ptbin-1], binrange[ptbin]));
  ths->SetNDC();
  ths->SetTextAlign(13);
  ths->SetTextFont(42);
  ths->SetTextSize(0.028);

  TPad *pad1 = new TPad("pad1", "", 0., 0.02, 1., 0.99);
  pad1->SetTopMargin(0.05);
  pad1->SetBottomMargin(0.1);
  pad1->Draw();

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

  TPave *pave;
  Float_t pave_y1 = 0;
  Float_t pave_y2 = 0;

  TLatex *tpull = new TLatex(0.64, 0.75, "Candidate");; 
  tpull->SetNDC();
  tpull->SetTextAlign(13);
  tpull->SetTextFont(42);
  tpull->SetTextSize(0.04);
  
  Double_t Mean = 0;
  Double_t std = 0;
  Double_t Meanerr = 0;
  Double_t stderr = 0;
  Double_t chi2 = 0;

  pad1->cd();
  hpull_Cand->Draw("HIST");
  hpull_Cand->Fit("gaus");
  fitresult = (TF1*)hpull_Cand->GetListOfFunctions()->FindObject("gaus");
  Mean = fitresult->GetParameter(1);
  std = fitresult->GetParameter(2);
  Meanerr = fitresult->GetParError(1);
  stderr = fitresult->GetParError(2);
  hpull_Cand->GetXaxis()->SetTitle("(fit-gen)/#sigma_{fit}");
  hpull_Cand->GetXaxis()->SetLimits(-10, 10);
  hpull_Cand->SetMinimum(0);
  //hpull_Cand->SetMaximum(Maximum(hpull_Cand->GetMaximum(), fitresult->GetMaximum(-10, 10))+15);
  hpull_Cand->SetMaximum(1.5*hpull_Cand->GetMaximum());
  fitresult->SetLineColor(kBlue);
  fitresult->SetLineWidth(2);
  fitresult->Draw("SAME");
  tpull->SetTextSize(0.04);
  tpull->DrawLatexNDC(0.64, 0.88, "Candidate");
  tpull->SetTextSize(0.03);
  tpull->DrawLatexNDC(0.64, 0.83, Form("Entries = %i", Nexp));
  tpull->DrawLatexNDC(0.64, 0.79, Form("Mean = %.3f #pm %.3f", Mean, Meanerr));
  tpull->DrawLatexNDC(0.64, 0.75, Form("Sigma = %.3f #pm %.3f", std, stderr));
  ths->Draw("SAME");
  hpull_Cand->SetName(Form("pull_Cand_ptbin%i", ptbin));
  title = hpull_Cand->GetName();
  CMS_lumi(pad1, period, 0);
  c1->Update();
  pad1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  pad1->cd();
  hpull_VBS->Draw("HIST");
  hpull_VBS->Fit("gaus");
  fitresult = (TF1*)hpull_VBS->GetListOfFunctions()->FindObject("gaus");
  Mean = fitresult->GetParameter(1);
  std = fitresult->GetParameter(2);
  Meanerr = fitresult->GetParError(1);
  stderr = fitresult->GetParError(2);
  fitresult->SetLineColor(kBlue);
  fitresult->SetLineWidth(2);
  fitresult->Draw("SAME");
  hpull_VBS->GetXaxis()->SetTitle("(fit-gen)/#sigma_{fit}");
  hpull_VBS->GetXaxis()->SetLimits(-10, 10);
  hpull_VBS->SetMinimum(0);
  //hpull_VBS->SetMaximum(Maximum(hpull_VBS->GetMaximum(), fitresult->GetMaximum(-10, 10))+15);
  hpull_VBS->SetMaximum(1.5*hpull_VBS->GetMaximum());
  tpull->SetTextSize(0.04);
  tpull->DrawLatexNDC(0.64, 0.88, "Z#gamma VBS");
  tpull->SetTextSize(0.03);
  tpull->DrawLatexNDC(0.64, 0.83, Form("Entries = %i", Nexp));
  tpull->DrawLatexNDC(0.64, 0.79, Form("Mean = %.3f #pm %.3f", Mean, Meanerr));
  tpull->DrawLatexNDC(0.64, 0.75, Form("Sigma = %.3f #pm %.3f", std, stderr));
  ths->Draw("SAME");
  hpull_VBS->SetName(Form("pull_VBS_ptbin%i", ptbin));
  title = hpull_VBS->GetName();
  CMS_lumi(pad1, period, 0);
  c1->Update();
  pad1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  hpull_SMTrue->Draw("HIST");
  hpull_SMTrue->Fit("gaus");
  fitresult = (TF1*)hpull_SMTrue->GetListOfFunctions()->FindObject("gaus");
  Mean = fitresult->GetParameter(1);
  std = fitresult->GetParameter(2);
  Meanerr = fitresult->GetParError(1);
  stderr = fitresult->GetParError(2);
  fitresult->SetLineColor(kBlue);
  fitresult->SetLineWidth(2);
  fitresult->Draw("SAME");
  hpull_SMTrue->GetXaxis()->SetTitle("(fit-gen)/#sigma_{fit}");
  hpull_SMTrue->GetXaxis()->SetLimits(-10, 10);
  hpull_SMTrue->SetMinimum(0);
  //hpull_SMTrue->SetMaximum(Maximum(hpull_SMTrue->GetMaximum(), fitresult->GetMaximum(-10, 10))+15);
  hpull_SMTrue->SetMaximum(1.5*hpull_SMTrue->GetMaximum());
  tpull->SetTextSize(0.04);
  tpull->DrawLatexNDC(0.64, 0.88, "SM True #gamma");
  tpull->SetTextSize(0.03);
  tpull->DrawLatexNDC(0.64, 0.83, Form("Entries = %i", Nexp));
  tpull->DrawLatexNDC(0.64, 0.79, Form("Mean = %.3f #pm %.3f", Mean, Meanerr));
  tpull->DrawLatexNDC(0.64, 0.75, Form("Sigma = %.3f #pm %.3f", std, stderr));
  ths->Draw("SAME");
  hpull_SMTrue->SetName(Form("pull_SMTrue_ptbin%i", ptbin));
  title = hpull_SMTrue->GetName();
  CMS_lumi(pad1, period, 0);
  c1->Update();
  pad1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  hpull_SMFake->Draw("HIST");
  hpull_SMFake->Fit("gaus");
  fitresult = (TF1*)hpull_SMFake->GetListOfFunctions()->FindObject("gaus");
  Mean = fitresult->GetParameter(1);
  std = fitresult->GetParameter(2);
  Meanerr = fitresult->GetParError(1);
  stderr = fitresult->GetParError(2);
  fitresult->SetLineColor(kBlue);
  fitresult->SetLineWidth(2);
  fitresult->Draw("SAME");
  hpull_SMFake->GetXaxis()->SetTitle("(fit-gen)/#sigma_{fit}");
  hpull_SMFake->GetXaxis()->SetLimits(-10, 10);
  hpull_SMFake->SetMinimum(0);
  //hpull_SMFake->SetMaximum(Maximum(hpull_SMFake->GetMaximum(), fitresult->GetMaximum(-10, 10))+15);
  hpull_SMFake->SetMaximum(1.5*hpull_SMFake->GetMaximum());
  tpull->SetTextSize(0.04);
  tpull->DrawLatexNDC(0.64, 0.88, "Fake #gamma");
  tpull->SetTextSize(0.03);
  tpull->DrawLatexNDC(0.64, 0.83, Form("Entries = %i", Nexp));
  tpull->DrawLatexNDC(0.64, 0.79, Form("Mean = %.3f #pm %.3f", Mean, Meanerr));
  tpull->DrawLatexNDC(0.64, 0.75, Form("Sigma = %.3f #pm %.3f", std, stderr));
  ths->Draw("SAME");
  hpull_SMFake->SetName(Form("pull_SMFake_ptbin%i", ptbin));
  title = hpull_SMFake->GetName();
  CMS_lumi(pad1, period, 0);
  c1->Update();
  pad1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  hpullcutsieie_Cand->Draw("HIST");
  hpullcutsieie_Cand->Fit("gaus");
  fitresult = (TF1*)hpullcutsieie_Cand->GetListOfFunctions()->FindObject("gaus");
  Mean = fitresult->GetParameter(1);
  std = fitresult->GetParameter(2);
  Meanerr = fitresult->GetParError(1);
  stderr = fitresult->GetParError(2);
  fitresult->SetLineColor(kBlue);
  fitresult->SetLineWidth(2);
  fitresult->Draw("SAME");
  hpullcutsieie_Cand->GetXaxis()->SetTitle("(fit-gen)/#sigma_{fit}");
  hpullcutsieie_Cand->GetXaxis()->SetLimits(-10, 10);
  hpullcutsieie_Cand->SetMinimum(0);
  //hpullcutsieie_Cand->SetMaximum(Maximum(hpullcutsieie_Cand->GetMaximum(), fitresult->GetMaximum(-10, 10))+15);
  hpullcutsieie_Cand->SetMaximum(1.5*hpullcutsieie_Cand->GetMaximum());
  tpull->SetTextSize(0.04);
  tpull->DrawLatexNDC(0.64, 0.88, "Candidate");
  tpull->SetTextSize(0.03);
  tpull->DrawLatexNDC(0.64, 0.83, Form("Entries = %i", Nexp));
  tpull->DrawLatexNDC(0.64, 0.79, Form("Mean = %.3f #pm %.3f", Mean, Meanerr));
  tpull->DrawLatexNDC(0.64, 0.75, Form("Sigma = %.3f #pm %.3f", std, stderr));
  ths->Draw("SAME");
  hpullcutsieie_Cand->SetName(Form("pull_Cand_ptbin%i", ptbin));
  title = hpullcutsieie_Cand->GetName();
  CMS_lumi(pad1, period, 0);
  c1->Update();
  pad1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  hpullcutsieie_VBS->Draw("HIST");
  hpullcutsieie_VBS->Fit("gaus");
  fitresult = (TF1*)hpullcutsieie_VBS->GetListOfFunctions()->FindObject("gaus");
  Mean = fitresult->GetParameter(1);
  std = fitresult->GetParameter(2);
  Meanerr = fitresult->GetParError(1);
  stderr = fitresult->GetParError(2);
  fitresult->SetLineColor(kBlue);
  fitresult->SetLineWidth(2);
  fitresult->Draw("SAME");
  hpullcutsieie_VBS->GetXaxis()->SetTitle("(fit-gen)/#sigma_{fit}");
  hpullcutsieie_VBS->GetXaxis()->SetLimits(-10, 10);
  hpullcutsieie_VBS->SetMinimum(0);
  //hpullcutsieie_VBS->SetMaximum(Maximum(hpullcutsieie_VBS->GetMaximum(), fitresult->GetMaximum(-10, 10))+15);
  hpullcutsieie_VBS->SetMaximum(1.5*hpullcutsieie_VBS->GetMaximum());
  tpull->SetTextSize(0.04);
  tpull->DrawLatexNDC(0.64, 0.88, "Z#gamma VBS");
  tpull->SetTextSize(0.03);
  tpull->DrawLatexNDC(0.64, 0.83, Form("Entries = %i", Nexp));
  tpull->DrawLatexNDC(0.64, 0.79, Form("Mean = %.3f #pm %.3f", Mean, Meanerr));
  tpull->DrawLatexNDC(0.64, 0.75, Form("Sigma = %.3f #pm %.3f", std, stderr));
  ths->Draw("SAME");
  hpullcutsieie_VBS->SetName(Form("pull_VBS_ptbin%i", ptbin));
  title = hpullcutsieie_VBS->GetName();
  CMS_lumi(pad1, period, 0);
  c1->Update();
  pad1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  hpullcutsieie_SMTrue->Draw("HIST");
  hpullcutsieie_SMTrue->Fit("gaus");
  fitresult = (TF1*)hpullcutsieie_SMTrue->GetListOfFunctions()->FindObject("gaus");
  Mean = fitresult->GetParameter(1);
  std = fitresult->GetParameter(2);
  Meanerr = fitresult->GetParError(1);
  stderr = fitresult->GetParError(2);
  fitresult->SetLineColor(kBlue);
  fitresult->SetLineWidth(2);
  fitresult->Draw("SAME");
  hpullcutsieie_SMTrue->GetXaxis()->SetTitle("(fit-gen)/#sigma_{fit}");
  hpullcutsieie_SMTrue->GetXaxis()->SetLimits(-10, 10);
  hpullcutsieie_SMTrue->SetMinimum(0);
  //hpullcutsieie_SMTrue->SetMaximum(Maximum(hpullcutsieie_SMTrue->GetMaximum(), fitresult->GetMaximum(-10, 10))+15);
  hpullcutsieie_SMTrue->SetMaximum(1.5*hpullcutsieie_SMTrue->GetMaximum());
  tpull->SetTextSize(0.04);
  tpull->DrawLatexNDC(0.64, 0.88, "SM True #gamma");
  tpull->SetTextSize(0.03);
  tpull->DrawLatexNDC(0.64, 0.83, Form("Entries = %i", Nexp));
  tpull->DrawLatexNDC(0.64, 0.79, Form("Mean = %.3f #pm %.3f", Mean, Meanerr));
  tpull->DrawLatexNDC(0.64, 0.75, Form("Sigma = %.3f #pm %.3f", std, stderr));
  ths->Draw("SAME");
  hpullcutsieie_SMTrue->SetName(Form("pull_SMTrue_ptbin%i", ptbin));
  title = hpullcutsieie_SMTrue->GetName();
  CMS_lumi(pad1, period, 0);
  c1->Update();
  pad1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  hpullcutsieie_SMFake->Draw("HIST");
  hpullcutsieie_SMFake->Fit("gaus");
  fitresult = (TF1*)hpullcutsieie_SMFake->GetListOfFunctions()->FindObject("gaus");
  Mean = fitresult->GetParameter(1);
  std = fitresult->GetParameter(2);
  Meanerr = fitresult->GetParError(1);
  stderr = fitresult->GetParError(2);
  fitresult->SetLineColor(kBlue);
  fitresult->SetLineWidth(2);
  fitresult->Draw("SAME");
  hpullcutsieie_SMFake->GetXaxis()->SetTitle("(fit-gen)/#sigma_{fit}");
  hpullcutsieie_SMFake->GetXaxis()->SetLimits(-10, 10);
  hpullcutsieie_SMFake->SetMinimum(0);
  //hpullcutsieie_SMFake->SetMaximum(Maximum(hpullcutsieie_SMFake->GetMaximum(), fitresult->GetMaximum(-10, 10))+15);
  hpullcutsieie_SMFake->SetMaximum(1.5*hpullcutsieie_SMFake->GetMaximum());
  tpull->SetTextSize(0.04);
  tpull->DrawLatexNDC(0.64, 0.88, "Fake #gamma");
  tpull->SetTextSize(0.03);
  tpull->DrawLatexNDC(0.64, 0.83, Form("Entries = %i", Nexp));
  tpull->DrawLatexNDC(0.64, 0.79, Form("Mean = %.3f #pm %.3f", Mean, Meanerr));
  tpull->DrawLatexNDC(0.64, 0.75, Form("Sigma = %.3f #pm %.3f", std, stderr));
  ths->Draw("SAME");
  hpullcutsieie_SMFake->SetName(Form("pull_SMFake_ptbin%i", ptbin));
  title = hpullcutsieie_SMFake->GetName();
  CMS_lumi(pad1, period, 0);
  c1->Update();
  pad1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  // // plot fiterr
  // hfiterrcutsieie_Cand->Draw("HIST");
  // hfiterrcutsieie_Cand->Fit("gaus");
  // fitresult = (TF1*)hfiterrcutsieie_Cand->GetListOfFunctions()->FindObject("gaus");
  // Mean = fitresult->GetParameter(1);
  // std = fitresult->GetParameter(2);
  // Meanerr = fitresult->GetParError(1);
  // stderr = fitresult->GetParError(2);
  // fitresult->SetLineColor(kBlue);
  // fitresult->SetLineWidth(2);
  // fitresult->Draw("SAME");
  // hfiterrcutsieie_Cand->GetXaxis()->SetTitle("#sigma_{fit}");
  // hfiterrcutsieie_Cand->GetXaxis()->SetLimits(0, 20);
  // hfiterrcutsieie_Cand->SetMinimum(0);
  // hfiterrcutsieie_Cand->SetMaximum(Maximum(hfiterrcutsieie_Cand->GetMaximum(), fitresult->GetMaximum(-10, 10))+15);
  // ths->Draw("SAME");
  // hfiterrcutsieie_Cand->SetName(Form("fiterr_Cand_ptbin%i", ptbin));
  // title = hfiterrcutsieie_Cand->GetName();
  // c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  // hfiterrcutsieie_VBS->Draw("HIST");
  // hfiterrcutsieie_VBS->Fit("gaus");
  // fitresult = (TF1*)hfiterrcutsieie_VBS->GetListOfFunctions()->FindObject("gaus");
  // Mean = fitresult->GetParameter(1);
  // std = fitresult->GetParameter(2);
  // Meanerr = fitresult->GetParError(1);
  // stderr = fitresult->GetParError(2);
  // fitresult->SetLineColor(kBlue);
  // fitresult->SetLineWidth(2);
  // fitresult->Draw("SAME");
  // hfiterrcutsieie_VBS->GetXaxis()->SetTitle("#sigma_{fit}");
  // hfiterrcutsieie_VBS->GetXaxis()->SetLimits(0, 20);
  // hfiterrcutsieie_VBS->SetMinimum(0);
  // hfiterrcutsieie_VBS->SetMaximum(Maximum(hfiterrcutsieie_VBS->GetMaximum(), fitresult->GetMaximum(0, 20))+15);
  // ths->Draw("SAME");
  // hfiterrcutsieie_VBS->SetName(Form("fiterr_VBS_ptbin%i", ptbin));
  // title = hfiterrcutsieie_VBS->GetName();
  // c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  // hfiterrcutsieie_SMTrue->Draw("HIST");
  // hfiterrcutsieie_SMTrue->Fit("gaus");
  // fitresult = (TF1*)hfiterrcutsieie_SMTrue->GetListOfFunctions()->FindObject("gaus");
  // Mean = fitresult->GetParameter(1);
  // std = fitresult->GetParameter(2);
  // Meanerr = fitresult->GetParError(1);
  // stderr = fitresult->GetParError(2);
  // fitresult->SetLineColor(kBlue);
  // fitresult->SetLineWidth(2);
  // fitresult->Draw("SAME");
  // hfiterrcutsieie_SMTrue->GetXaxis()->SetTitle("#sigma_{fit}");
  // hfiterrcutsieie_SMTrue->GetXaxis()->SetLimits(0, 20);
  // hfiterrcutsieie_SMTrue->SetMinimum(0);
  // hfiterrcutsieie_SMTrue->SetMaximum(Maximum(hfiterrcutsieie_SMTrue->GetMaximum(), fitresult->GetMaximum(0, 20))+15);
  // ths->Draw("SAME");
  // hfiterrcutsieie_SMTrue->SetName(Form("fiterr_SMTrue_ptbin%i", ptbin));
  // title = hfiterrcutsieie_SMTrue->GetName();
  // c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  // hfiterrcutsieie_SMFake->Draw("HIST");
  // hfiterrcutsieie_SMFake->Fit("gaus");
  // fitresult = (TF1*)hfiterrcutsieie_SMFake->GetListOfFunctions()->FindObject("gaus");
  // Mean = fitresult->GetParameter(1);
  // std = fitresult->GetParameter(2);
  // Meanerr = fitresult->GetParError(1);
  // stderr = fitresult->GetParError(2);
  // fitresult->SetLineColor(kBlue);
  // fitresult->SetLineWidth(2);
  // fitresult->Draw("SAME");
  // hfiterrcutsieie_SMFake->GetXaxis()->SetTitle("#sigma_{fit}");
  // hfiterrcutsieie_SMFake->GetXaxis()->SetLimits(0, 20);
  // hfiterrcutsieie_SMFake->SetMinimum(0);
  // hfiterrcutsieie_SMFake->SetMaximum(Maximum(hfiterrcutsieie_SMFake->GetMaximum(), fitresult->GetMaximum(0, 20))+15);
  // ths->Draw("SAME");
  // hfiterrcutsieie_SMFake->SetName(Form("fiterr_SMFake_ptbin%i", ptbin));
  // title = hfiterrcutsieie_SMFake->GetName();
  // c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  
}
