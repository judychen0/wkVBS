#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "TText.h"
#include "../Drawlumi/CMS_lumi.C"

using namespace std;
#define nfile 9
#define nhisto 3
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
  ratio->GetYaxis()->SetTitleSize(0.11);
  ratio->GetYaxis()->SetTitleOffset(0.4);
  ratio->GetYaxis()->SetLabelFont(42);
  ratio->GetYaxis()->SetLabelOffset(0.008);
  ratio->GetYaxis()->SetLabelSize(0.1);
  ratio->GetYaxis()->SetTickSize(0.032);
  ratio->GetYaxis()->SetNdivisions(8);
  
  ratio->GetXaxis()->SetTitleFont(42);
  ratio->GetXaxis()->SetTitleSize(0.12);
  ratio->GetXaxis()->SetTitleOffset(1.05);
  ratio->GetXaxis()->SetLabelFont(42);
  ratio->GetXaxis()->SetLabelOffset(0.03);
  ratio->GetXaxis()->SetLabelSize(0.1);
  ratio->GetXaxis()->SetTickSize(0.1);
  ratio->GetXaxis()->SetNdivisions(210);
  ratio->GetXaxis()->ChangeLabel(-1, -1, 0, -1, -1, -1, "");

  return ratio;
}

void xDrawStack_WgCRptbin(Int_t year){
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
    rootname[0] = "/data1/GMET/WgCR_MC/fall17/allmc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/WgCR_MC/fall17/allmc/VJet/output_merge_VJet.root";
    rootname[2] = "/data1/GMET/WgCR_MC/fall17/allmc/VG/output_merge_VG.root";
    rootname[3] = "/data1/GMET/WgCR_MC/fall17/allmc/monoboson/output_merge_monoBoson.root";
    rootname[4] = "/data1/GMET/WgCR_MC/fall17/allmc/diboson/output_merge_diBoson.root";
    rootname[5] = "/data1/GMET/WgCR_MC/fall17/allmc/triboson/output_merge_triBoson.root";
    rootname[6] = "/data1/GMET/WgCR_MC/fall17/allmc/TT/output_merge_TT.root";
    rootname[7] = "/data1/GMET/WgCR_MC/fall17/allmc/job_UL17_Znunug_VBS/output_ggtree.root";
    rootname[8] = "/data1/GMET/WgCR_Data/fall17/output_ggtree.root";
    
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
    rootname[8] = "/data1/GMET/WgCR_Data/autumn18/output_ggtree.root";
    
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
  TH1F *H_dijetMass_WgCR[nfile][nhisto];
  TH1F *Hsum_dijetMass_WgCR[nfile];

  // plots of MC sum
  TH1F *HA_dijetMass_WgCR[nhisto];
  TH1F *HAsum_dijetMass_WgCR;

  // plots of sig MC
  TH1F *HSig_dijetMass_WgCR[nhisto];
  TH1F *HSigsum_dijetMass_WgCR;

  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    for(Int_t ihis=0; ihis<nhisto; ihis++){

      // Get first plot for MC sum
      if(i==0){
	HA_dijetMass_WgCR[ihis] = (TH1F*)fopen->Get(Form("hbin_WgCR_dijetMass_cutsieie_%i", ihis+1))->Clone();
	
	// fill settings
	HA_dijetMass_WgCR[ihis]->SetFillStyle(3145);
	
	// fill color settings
	HA_dijetMass_WgCR[ihis]->SetFillColor(12);

	if(ihis==0){
	  HAsum_dijetMass_WgCR = (TH1F*)fopen->Get(Form("hbin_WgCR_dijetMass_cutsieie_%i", ihis+1))->Clone();
	  HAsum_dijetMass_WgCR->SetFillStyle(3145);
	  HAsum_dijetMass_WgCR->SetFillColor(12);
	}
      }

      // Get sig MC plot
      if(i==nfile-2){
	HSig_dijetMass_WgCR[ihis] = (TH1F*)fopen->Get(Form("hbin_WgCR_dijetMass_cutsieie_%i", ihis+1))->Clone();
	
	// line color setting
	HSig_dijetMass_WgCR[ihis]->SetLineColor(kRed);

	// line width settings
	HSig_dijetMass_WgCR[ihis]->SetLineWidth(2);

	if(ihis==0){
	  HSigsum_dijetMass_WgCR = (TH1F*)fopen->Get(Form("hbin_WgCR_dijetMass_cutsieie_%i", ihis+1))->Clone();
	  HSigsum_dijetMass_WgCR->SetLineColor(kRed);
	  HSigsum_dijetMass_WgCR->SetLineWidth(2);
	}
      }

      // Get plots from every file
      H_dijetMass_WgCR[i][ihis] = (TH1F*)fopen->Get(Form("hbin_WgCR_dijetMass_cutsieie_%i", ihis+1));
      if(ihis==0) Hsum_dijetMass_WgCR[i] = (TH1F*)fopen->Get(Form("hbin_WgCR_dijetMass_cutsieie_%i", ihis+1));

      // Set fill color for each sample (except data)
      if(i<nfile-1){
	H_dijetMass_WgCR[i][ihis]->SetFillColor(cnum[i]);
	if(ihis==0) Hsum_dijetMass_WgCR[i]->SetFillColor(cnum[i]);
      }

      // Set Line Color for each sample
      H_dijetMass_WgCR[i][ihis]->SetLineColor(1);
      if(ihis==0) Hsum_dijetMass_WgCR[i]->SetLineColor(1);

      // Set properties for data plots
      if(i == nfile-1){
	// Set Line width
	H_dijetMass_WgCR[i][ihis]->SetLineWidth(2);

	// Set Marker style
	H_dijetMass_WgCR[i][ihis]->SetMarkerStyle(8);
	
	// Set Marker Size
	H_dijetMass_WgCR[i][ihis]->SetMarkerSize(0.9);

	if(ihis==0){
	  Hsum_dijetMass_WgCR[i]->SetLineWidth(2);
	  Hsum_dijetMass_WgCR[i]->SetMarkerStyle(8);
	  Hsum_dijetMass_WgCR[i]->SetMarkerSize(0.9);
	}
      }
    }
  }

  for(Int_t i=0; i<nfile; i++){
    if(i>0 && i<nfile-1){
      for(Int_t ihis=0; ihis<nhisto; ihis++){
	HA_dijetMass_WgCR[ihis]->Add(H_dijetMass_WgCR[i][ihis]);
	if(i==nfile-2 && ihis>0) HSigsum_dijetMass_WgCR->Add(HSig_dijetMass_WgCR[ihis]);
	Hsum_dijetMass_WgCR[i]->Add(H_dijetMass_WgCR[i][ihis]);
	HAsum_dijetMass_WgCR->Add(H_dijetMass_WgCR[i][ihis]);
      }
    }
    else if(i==0){
      for(Int_t ihis=1; ihis<nhisto; ihis++){
	HAsum_dijetMass_WgCR->Add(H_dijetMass_WgCR[i][ihis]);
	Hsum_dijetMass_WgCR[i]->Add(H_dijetMass_WgCR[i][ihis]);
      }
    }
    else if(i==nfile-1){
      for(Int_t ihis=0; ihis<nhisto; ihis++){
	Hsum_dijetMass_WgCR[i]->Add(H_dijetMass_WgCR[i][ihis]);
      }
    }
  }

  // create stack plots
  THStack *HS_dijetMass_WgCR[nhisto];
  THStack *HSsum_dijetMass_WgCR;

  for(Int_t ihis=0; ihis<nhisto; ihis++){
    HS_dijetMass_WgCR[ihis] = new THStack(Form("HS_dijetMass_WgCR_%i", ihis+1), Form("HS_dijetMass_WgCR_%i", ihis+1));
  }
  HSsum_dijetMass_WgCR = new THStack("HSsum_dijetMass_WgCR", "HSsum_dijetMass_WgCR");

  for(Int_t i=0; i<nfile-1; i++){
    for(Int_t ihis=0; ihis<nhisto; ihis++){
      HS_dijetMass_WgCR[ihis]->Add(H_dijetMass_WgCR[i][ihis]);
    }
    HSsum_dijetMass_WgCR->Add(Hsum_dijetMass_WgCR[i]);
  }

  TLegend *lhs = new TLegend(0.50, 0.66, 0.90, 0.90);//VBS
  lhs->SetTextSize(0.035);
  lhs->SetFillStyle(0);
  TString ltext[20] = {"#gamma+jet", "V+jets", "V#gamma+jets", "Monoboson", "Diboson", "Triboson", "tt#gamma", "Z#gamma#rightarrow#nu#nu#gamma(VBS)", "DATA"};//Znunug VBS

  // Data/MC event yield text
  Int_t dataIntegral =  Hsum_dijetMass_WgCR[nfile-1]->Integral(1, -1);
  Float_t mcIntegral = HAsum_dijetMass_WgCR->Integral(1, -1);
  // Int_t dataIntegral =  0;
  // Float_t mcIntegral = 0;
  TLatex *ths = new TLatex(0.51, 0.63, Form("Data/MC = %i/%.2f", dataIntegral, mcIntegral));
  // TLatex *ths = new TLatex();
  ths->SetNDC();
  ths->SetTextAlign(13);
  ths->SetTextFont(42);
  ths->SetTextSize(0.04);

  TH1F *hratio;
  Double_t x[3] = {-2000, 0.0, 5000};
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

  // save plots //
  pad1->cd();
  pad1->SetLogy(0);
  HSsum_dijetMass_WgCR->Draw("HIST");
  Hsum_dijetMass_WgCR[nfile-1]->Draw("SAME");
  HSigsum_dijetMass_WgCR->Draw("HIST SAME");
  HAsum_dijetMass_WgCR->Draw("E2 SAME");
  HSsum_dijetMass_WgCR->GetXaxis()->SetTitle("");
  HSsum_dijetMass_WgCR->GetYaxis()->SetTitle("Events");
  HSsum_dijetMass_WgCR->GetXaxis()->SetLabelSize(0);
  HSsum_dijetMass_WgCR->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  //HS_dijetMass_WgCR->GetXaxis()->SetRangeUser(200, 2000);
  HSsum_dijetMass_WgCR->SetMaximum(140);
  HSsum_dijetMass_WgCR->SetMinimum(0);
  //HS_dijetMass_WgCR->SetMaximum(10000);
  //HS_dijetMass_WgCR->SetMinimum(0.1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    if(ifile == nfile-1) lhs->AddEntry(Hsum_dijetMass_WgCR[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSigsum_dijetMass_WgCR, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(Hsum_dijetMass_WgCR[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  ths->Draw("SAME");
  title = HSsum_dijetMass_WgCR->GetName();
  pad2->cd();
  hratio = ratioplot(HSsum_dijetMass_WgCR, Hsum_dijetMass_WgCR[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("M_{jj} [GeV]");
  //hratio->GetXaxis()->SetRangeUser(200, 2000);
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  //c1->SaveAs(Form("%s/%s_linear.pdf", saveto, title));
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));


  //save plots for each ptbin//
  // for(Int_t ihis=0; ihis<nhisto; ihis++){
  //   dataIntegral = H_dijetMass_WgCR[nfile-1][ihis]->Integral();
  //   mcIntegral = HA_dijetMass_WgCR[ihis]->Integral();
  //   ths = new TLatex(0.51, 0.63, Form("Data/MC = %i/%.2f", dataIntegral, mcIntegral));
  //   ths->SetNDC();
  //   ths->SetTextAlign(13);
  //   ths->SetTextFont(42);
  //   ths->SetTextSize(0.04);
  
  //   pad1->cd();
  //   pad1->SetLogy(0);
  //   HS_dijetMass_WgCR[ihis]->Draw("HIST");
  //   H_dijetMass_WgCR[nfile-1][ihis]->Draw("SAME");
  //   HSig_dijetMass_WgCR[ihis]->Draw("HIST SAME");
  //   HA_dijetMass_WgCR[ihis]->Draw("E2 SAME");
  //   HS_dijetMass_WgCR[ihis]->GetXaxis()->SetTitle("");
  //   HS_dijetMass_WgCR[ihis]->GetYaxis()->SetTitle("Events");
  //   HS_dijetMass_WgCR[ihis]->GetXaxis()->SetLabelSize(0);
  //   HS_dijetMass_WgCR[ihis]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  //   //HS_dijetMass_WgCR->GetXaxis()->SetRangeUser(200, 2000);
  //   HS_dijetMass_WgCR[ihis]->SetMaximum(100);
  //   HS_dijetMass_WgCR[ihis]->SetMinimum(0);
  //   //HS_dijetMass_WgCR->SetMaximum(10000);
  //   //HS_dijetMass_WgCR->SetMinimum(0.1);
  //   lhs->Clear();
  //   lhs->SetNColumns(2);
  //   for(Int_t ifile=0; ifile<nfile; ifile++){
  //     if(ifile == nfile-1) lhs->AddEntry(H_dijetMass_WgCR[ifile][ihis], ltext[ifile], "PL");
  //     if(ifile == nfile-2) lhs->AddEntry(HSig_dijetMass_WgCR[ihis], ltext[ifile], "L");
  //     if(ifile < nfile-1) lhs->AddEntry(H_dijetMass_WgCR[ifile][ihis], ltext[ifile], "F");
  //   }
  //   lhs->Draw("SAME");
  //   ths->Draw("SAME");
  //   title = HS_dijetMass_WgCR[ihis]->GetName();
  //   pad2->cd();
  //   hratio = ratioplot(HS_dijetMass_WgCR[ihis], H_dijetMass_WgCR[nfile-1][ihis]);
  //   hratio->SetTitle("");
  //   hratio->GetXaxis()->SetTitle("Dijet Mass[GeV]");
  //   //hratio->GetXaxis()->SetRangeUser(200, 2000);
  //   hratio->Draw("EP");
  //   tg->Draw("LSAME");
  //   CMS_lumi(pad1, period, 0);
  //   c1->Update();
  //   c1->RedrawAxis();
  //   //c1->SaveAs(Form("%s/%s_linear.pdf", saveto, title));
  //   c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  // }
  
}
