#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
#include "TH1F.h"
#include "TH2F.h"

#include "smallntuplizer.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "PhotonSelection.h"
#include "LumiW.h"

#define nPhobins 3

Float_t WEIGHT = 1.0;
Double_t deltaPhi(Double_t phi1, Double_t phi2) {
  Double_t dPhi = phi1 - phi2;
  if (dPhi > TMath::Pi()) dPhi -= 2.*TMath::Pi();
  if (dPhi < -TMath::Pi()) dPhi += 2.*TMath::Pi();
  return dPhi;
}

Double_t deltaR(Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2) {
  Double_t dEta, dPhi ;
  dEta = eta1 - eta2;
  dPhi = deltaPhi(phi1, phi2);
  return sqrt(dEta*dEta+dPhi*dPhi);
}

Double_t deltaEta(Double_t eta1, Double_t eta2){
  Double_t dEta;
  dEta = std::abs(eta1 - eta2);
  return dEta;
}

Int_t SetBit(Int_t nbit, Int_t bit){
  return (bit | (1<<nbit));
}

Int_t ResetBit(Int_t nbit, Int_t bit){
  return bit & ~(1<<nbit);
}

void xZg_MC(char* pathes, Int_t year, Int_t ULsamples, Int_t isSM, Int_t isTrgs){
  //***********************Initialization***********************//

  //Get processed events
  TFile *fopen = new TFile(pathes, "READ");
  TH1F *hEvents = (TH1F*)fopen->Get("hEvents")->Clone();
  hEvents->SetDirectory(0);
  TH1F *hSumofGenW = (TH1F*)fopen->Get("hSumGenWeight")->Clone();
  hSumofGenW->SetDirectory(0);
  TH1F *h_pfMET_all = (TH1F*)fopen->Get("h_pfMET_all")->Clone();
  h_pfMET_all->SetDirectory(0);
  TH1F *h_pfMET_HLTpass = (TH1F*)fopen->Get("h_pfMET_HLTpass")->Clone();
  h_pfMET_HLTpass->SetDirectory(0);
  TH1F *h_puppiMET_all = (TH1F*)fopen->Get("h_puppiMET_all")->Clone();
  h_puppiMET_all->SetDirectory(0);
  TH1F *h_puppiMET_HLTpass = (TH1F*)fopen->Get("h_puppiMET_HLTpass")->Clone();
  h_puppiMET_HLTpass->SetDirectory(0);
  fopen->Close();

  //access EventTree with TreeReader class
  TreeReader data(pathes);
  
  // get lumi weight
  Double_t lumiWei = lumiW(year, pathes);
  
  //create an output .root file
  TFile *fout_;
  char foutname[50];
  string treestr = pathes;
  Int_t istr = treestr.find("ggtree");
  string treename = treestr.substr(istr);
  sprintf(foutname, "output_%s", treename.c_str());
  fout_ = new TFile(foutname,"RECREATE");

  Double_t ptbin[20] = {75, 90, 110, 130, 150, 180, 210, 250, 300, 350, 400, 450, 500, 550, 600, 700, 800, 1000, 2000};//18 bins, 2016
  Double_t etabin[20] = {-1.4442, -0.6, 0, 0.6, 1.4442};//4bins
  Double_t METbin[20] = {0, 45, 90, 135, 180, 250, 300, 400, 500, 650, 800, 1200};//10bins
  Double_t phibin[20] = {-TMath::Pi(), -2.5, -2, -1.4, -0.8, 0.0, 0.8, 1.4, 2, 2.5, TMath::Pi()};//10bins
  Double_t dphibin[20] = {-TMath::Pi(), -2.7, -2.3, -1.9, -1.5, -1.2, -0.5, 0.0, 0.5, 1.2, 1.5, 1.9, 2.3, 2.7, TMath::Pi()};//14bins
  Double_t jetptbin[20] = {0, 30, 50, 100, 200, 300, 500, 700, 1200};//8bins
  Double_t jetetabin[30] = {-5, -4.7, -4, -3.5, -3, -2.5, -2.3, -2, -1.7, -1.4, -0.8, 0.0, 0.8, 1.4, 1.7, 2, 2.3, 2.5, 3, 3.5, 4, 4.7, 5};//22bins
  //Double_t massbin[20] = {0, 250, 500, 750, 1000, 1250, 1500, 1750, 2000, 5000};//9bins
  Double_t massbin[20] = {0, 250, 500, 1000, 2000, 5000};//5bins
  
  Double_t Phobin[4] = {75, 110, 210, 2000};
  
  // Plots for cutflow efficiency
  
  TH1F *h_phoEB_pt_phopt;
  TH1F *h_phoEB_pt_MphoID;
  TH1F *h_phoEB_pt_lepveto;
  TH1F *h_phoEB_pt_MET;
  TH1F *h_phoEB_pt_dphoMETPhi;
  TH1F *h_phoEB_pt_njet;
  // if isSM == 0
  TH1F *h_phoEB_pt_jetpt;
  TH1F *h_phoEB_pt_jetjetdEta;
  TH1F *h_phoEB_pt_jetjetdPhi;
  TH1F *h_phoEB_pt_phojetdR;
  TH1F *h_phoEB_pt_ptoverMET;
  TH1F *h_phoEB_pt_mindJMETPhi;
  TH1F *h_phoEB_pt_diejtMass;
 
  TH1F *h_MET_phopt;
  TH1F *h_MET_MphoID;
  TH1F *h_MET_lepveto;
  TH1F *h_MET_MET;
  TH1F *h_MET_dphoMETPhi;
  TH1F *h_MET_njet;
  // if isSM == 0
  TH1F *h_MET_jetpt;
  TH1F *h_MET_jetjetdEta;
  TH1F *h_MET_jetjetdPhi;
  TH1F *h_MET_phojetdR;
  TH1F *h_MET_ptoverMET;
  TH1F *h_MET_mindJMETPhi;
  TH1F *h_MET_diejtMass;

  h_phoEB_pt_phopt = new TH1F("h_phoEB_pt_phopt", "", 18, ptbin);
  h_phoEB_pt_MphoID = new TH1F("h_phoEB_pt_MphoID", "", 18, ptbin);
  h_phoEB_pt_lepveto = new TH1F("h_phoEB_pt_lepveto", "", 18, ptbin);
  h_phoEB_pt_MET = new TH1F("h_phoEB_pt_MET", "", 18, ptbin);
  h_phoEB_pt_dphoMETPhi = new TH1F("h_phoEB_pt_dphoMETPhi", "", 18, ptbin);
  h_phoEB_pt_njet = new TH1F("h_phoEB_pt_njet", "", 18, ptbin);
  h_phoEB_pt_jetpt = new TH1F("h_phoEB_pt_jetpt", "", 18, ptbin);
  h_phoEB_pt_jetjetdEta = new TH1F("h_phoEB_pt_jetjetdEta", "", 18, ptbin);
  h_phoEB_pt_jetjetdPhi = new TH1F("h_phoEB_pt_jetjetdPhi", "", 18, ptbin);
  h_phoEB_pt_phojetdR = new TH1F("h_phoEB_pt_phojetdR", "", 18, ptbin);
  h_phoEB_pt_ptoverMET = new TH1F("h_phoEB_pt_ptoverMET", "", 18, ptbin);
  h_phoEB_pt_mindJMETPhi = new TH1F("h_phoEB_pt_mindJMETPhi", "", 18, ptbin);
  h_phoEB_pt_diejtMass = new TH1F("h_phoEB_pt_diejtMass", "", 18, ptbin);

  h_phoEB_pt_phopt->Sumw2();
  h_phoEB_pt_MphoID->Sumw2();
  h_phoEB_pt_lepveto->Sumw2();
  h_phoEB_pt_MET->Sumw2();
  h_phoEB_pt_dphoMETPhi->Sumw2();
  h_phoEB_pt_njet->Sumw2();
  h_phoEB_pt_jetpt->Sumw2();
  h_phoEB_pt_jetjetdEta->Sumw2();
  h_phoEB_pt_jetjetdPhi->Sumw2();
  h_phoEB_pt_phojetdR->Sumw2();
  h_phoEB_pt_ptoverMET->Sumw2();
  h_phoEB_pt_mindJMETPhi->Sumw2();
  h_phoEB_pt_diejtMass->Sumw2();

  h_MET_phopt = new TH1F("h_MET_phopt", "", 10, METbin);
  h_MET_MphoID = new TH1F("h_MET_MphoID", "", 10, METbin);
  h_MET_lepveto = new TH1F("h_MET_lepveto", "", 10, METbin);
  h_MET_MET = new TH1F("h_MET_MET", "", 10, METbin);
  h_MET_dphoMETPhi = new TH1F("h_MET_dphoMETPhi", "", 10, METbin);
  h_MET_njet = new TH1F("h_MET_njet", "", 10, METbin);
  h_MET_jetpt = new TH1F("h_MET_jetpt", "", 10, METbin);
  h_MET_jetjetdEta = new TH1F("h_MET_jetjetdEta", "", 10, METbin);
  h_MET_jetjetdPhi = new TH1F("h_MET_jetjetdPhi", "", 10, METbin);
  h_MET_phojetdR = new TH1F("h_MET_phojetdR", "", 10, METbin);
  h_MET_ptoverMET = new TH1F("h_MET_ptoverMET", "", 10, METbin);
  h_MET_mindJMETPhi = new TH1F("h_MET_mindJMETPhi", "", 10, METbin);
  h_MET_diejtMass = new TH1F("h_MET_diejtMass", "", 10, METbin);

  h_MET_phopt->Sumw2();
  h_MET_MphoID->Sumw2();
  h_MET_lepveto->Sumw2();
  h_MET_MET->Sumw2();
  h_MET_dphoMETPhi->Sumw2();
  h_MET_njet->Sumw2();
  h_MET_jetpt->Sumw2();
  h_MET_jetjetdEta->Sumw2();
  h_MET_jetjetdPhi->Sumw2();
  h_MET_phojetdR->Sumw2();
  h_MET_ptoverMET->Sumw2();
  h_MET_mindJMETPhi->Sumw2();
  h_MET_diejtMass->Sumw2();
  
  // Plots for final stacks

  TH1F *h_phopt_cut;
  TH1F *h_phoEta_cut;
  TH1F *h_phoPhi_cut;
  TH1F *h_MET_cut;
  TH1F *h_METPhi_cut;
  TH1F *h_njet_cut;
  TH1F *h_nvtx_cut;
  
  TH1F *h_dPhi_phoMET;
  // if isSM == 0
  TH1F *h_jetpt_cut[2];
  TH1F *h_jetEta_cut[2];
  TH1F *h_jetPhi_cut[2];

  TH1F *h_dEta_phojet;  
  TH1F *h_dPhi_phojet;
  TH1F *h_dR_phojet;
  TH1F *h_dEta_jetjet;  
  TH1F *h_dPhi_jetjet;
  TH1F *h_dR_jetjet;
  TH1F *h_dPhi_minjetMET;
  
  TH1F *h_ptoverMET_cut;
  TH1F *h_dijetMass_cut;
  
  h_phopt_cut = new TH1F("h_phopt_cut", "", 18, ptbin);
  h_phoEta_cut = new TH1F("h_phoEta_cut", "", 4, etabin);
  h_phoPhi_cut = new TH1F("h_phoPhi_cut", "", 10, phibin);
  h_MET_cut = new TH1F("h_MET_cut", "", 10, METbin);
  h_METPhi_cut = new TH1F("h_METPhi_cut", "", 10, phibin);
  h_njet_cut = new TH1F("h_njet_cut", "", 10, 0, 10);
  h_nvtx_cut = new TH1F("h_nvtx_cut", "", 500, 0, 100);
  h_dPhi_phoMET = new TH1F("h_dPhi_phoMET", "", 14, dphibin);
  
  h_phopt_cut->Sumw2();
  h_phoEta_cut->Sumw2();
  h_phoPhi_cut->Sumw2();
  h_MET_cut->Sumw2();
  h_METPhi_cut->Sumw2();
  h_njet_cut->Sumw2();
  h_nvtx_cut->Sumw2();
  h_dPhi_phoMET->Sumw2();
  
  for(Int_t jj=0; jj<2; jj++){
    h_jetpt_cut[jj] = new TH1F(Form("h_jetpt_cut_%i", jj), "", 8, jetptbin);
    h_jetEta_cut[jj] = new TH1F(Form("h_jetEta_cut_%i", jj), "", 22, jetetabin);
    h_jetPhi_cut[jj] = new TH1F(Form("h_jetPhi_cut_%i", jj), "", 10, phibin);

    h_jetpt_cut[jj]->Sumw2();
    h_jetEta_cut[jj]->Sumw2();
    h_jetPhi_cut[jj]->Sumw2();
  }
  
  h_dEta_phojet = new TH1F("h_dEta_phojet", "", 32, 0, 8); 
  h_dPhi_phojet = new TH1F("h_dPhi_phojet", "", 14, dphibin);
  h_dR_phojet = new TH1F("h_dR_phojet", "", 20, 0, 8);
  h_dEta_jetjet = new TH1F("h_dEta_jetjet", "", 32, 0, 8);  
  h_dPhi_jetjet = new TH1F("h_dPhi_jetjet", "", 14, dphibin);
  h_dR_jetjet = new TH1F("h_dR_jetjet", "", 20, 0, 8);
  h_dPhi_minjetMET = new TH1F("h_dPhi_minjetMET", "", 14, dphibin);
  h_ptoverMET_cut = new TH1F("h_ptoverMET_cut", "", 60, 0, 12);
  h_dijetMass_cut = new TH1F("h_dijetMass_cut", "", 5, massbin);

  h_dEta_phojet->Sumw2();
  h_dPhi_phojet->Sumw2();
  h_dR_phojet->Sumw2();
  h_dEta_jetjet->Sumw2();
  h_dPhi_jetjet->Sumw2();
  h_dR_jetjet->Sumw2();
  h_dPhi_minjetMET->Sumw2();
  h_ptoverMET_cut->Sumw2();
  h_dijetMass_cut->Sumw2();

  // Plots for N-1 cut

  TH1F *h_MET_Nm1;
  TH1F *h_njet_Nm1;
  TH1F *h_dPhi_phoMET_Nm1;
  // isSM == 0
  TH1F *h_jetpt_Nm1[2];
  TH1F *h_jeteta_Nm1[2];
  TH1F *h_dR_phojet_Nm1;
  TH1F *h_dEta_jetjet_Nm1;
  TH1F *h_dPhi_jetjet_Nm1;
  TH1F *h_dPhi_minjetMET_Nm1;
  TH1F *h_ptoverMET_Nm1;
  TH1F *h_dijetMass_Nm1;

  h_MET_Nm1 = new TH1F("h_MET_Nm1", "", 10, METbin);
  h_njet_Nm1 = new TH1F("h_njet_Nm1", "", 10, 0, 10);
  h_dPhi_phoMET_Nm1 = new TH1F("h_dPhi_phoMET_Nm1", "", 14, dphibin);
  h_dR_phojet_Nm1 = new TH1F("h_dR_phojet_Nm1", "", 20, 0, 8);
  h_dEta_jetjet_Nm1 = new TH1F("h_dEta_jetjet_Nm1", "", 32, 0, 8);
  h_dPhi_jetjet_Nm1 = new TH1F("h_dPhi_jetjet_Nm1", "", 14, dphibin);
  h_dPhi_minjetMET_Nm1 = new TH1F("h_dPhi_minjetMET_Nm1", "", 14, dphibin);
  h_ptoverMET_Nm1 = new TH1F("h_ptoverMET_Nm1", "", 60, 0, 12);
  h_dijetMass_Nm1 = new TH1F("h_dijetMass_Nm1", "", 5, massbin);
  
  h_MET_Nm1->Sumw2();
  h_njet_Nm1->Sumw2();
  h_dPhi_phoMET_Nm1->Sumw2();
  h_dR_phojet_Nm1->Sumw2();
  h_dEta_jetjet_Nm1->Sumw2();
  h_dPhi_jetjet_Nm1->Sumw2();
  h_dPhi_minjetMET_Nm1->Sumw2();
  h_ptoverMET_Nm1->Sumw2();
  h_dijetMass_Nm1->Sumw2();

  for(Int_t jj=0; jj<2; jj++){
    h_jetpt_Nm1[jj] = new TH1F(Form("h_jetpt_Nm1_%i", jj), "", 8, jetptbin);
    h_jeteta_Nm1[jj] = new TH1F(Form("h_jeteta_Nm1_%i", jj), "", 22, jetetabin);

    h_jetpt_Nm1[jj]->Sumw2();
    h_jeteta_Nm1[jj]->Sumw2();
  }

  // plots for Wgamma CR
  TH1F *h_nLep_WgCR;
  TH1F *h_MET_WgCR;
  TH1F *h_dijetMass_WgCR;

  h_nLep_WgCR = new TH1F("h_nLep_WgCR", "", 10, 0, 10);
  h_MET_WgCR = new TH1F("h_MET_WgCR", "", 10, METbin);
  h_dijetMass_WgCR = new TH1F("h_dijetMass_WgCR", "", 5, massbin);
  h_nLep_WgCR->Sumw2();
  h_MET_WgCR->Sumw2();
  h_dijetMass_WgCR->Sumw2();
  
  // plots to replace VJet with data driven number, 3 photon pt bins
  TH1F *hJfake_phopt_cut[nPhobins];
  TH1F *hJfake_phoEta_cut[nPhobins];
  TH1F *hJfake_phoPhi_cut[nPhobins];
  TH1F *hJfake_MET_cut[nPhobins];
  TH1F *hJfake_METPhi_cut[nPhobins];
  TH1F *hJfake_njet_cut[nPhobins];
  TH1F *hJfake_nvtx_cut[nPhobins];
  
  TH1F *hJfake_dPhi_phoMET[nPhobins];
  // if isSM == 0
  TH1F *hJfake_jetpt_cut[2][nPhobins];
  TH1F *hJfake_jetEta_cut[2][nPhobins];
  TH1F *hJfake_jetPhi_cut[2][nPhobins];

  TH1F *hJfake_dEta_phojet[nPhobins];  
  TH1F *hJfake_dPhi_phojet[nPhobins];
  TH1F *hJfake_dR_phojet[nPhobins];
  TH1F *hJfake_dEta_jetjet[nPhobins];  
  TH1F *hJfake_dPhi_jetjet[nPhobins];
  TH1F *hJfake_dR_jetjet[nPhobins];
  TH1F *hJfake_dPhi_minjetMET[nPhobins];
  
  TH1F *hJfake_ptoverMET_cut[nPhobins];
  TH1F *hJfake_dijetMass_cut[nPhobins];

  for(Int_t ibin=0; ibin<nPhobins; ibin++){
    
    hJfake_phopt_cut[ibin] = new TH1F(Form("hJfake_phopt_cut_%i", ibin), "", 18, ptbin);
    hJfake_phoEta_cut[ibin] = new TH1F(Form("hJfake_phoEta_cut_%i", ibin), "", 4, etabin);
    hJfake_phoPhi_cut[ibin] = new TH1F(Form("hJfake_phoPhi_cut_%i", ibin), "", 10, phibin);
    hJfake_MET_cut[ibin] = new TH1F(Form("hJfake_MET_cut_%i", ibin), "", 10, METbin);
    hJfake_METPhi_cut[ibin] = new TH1F(Form("hJfake_METPhi_cut_%i", ibin), "", 10, phibin);
    hJfake_njet_cut[ibin] = new TH1F(Form("hJfake_njet_cut_%i", ibin), "", 10, 0, 10);
    hJfake_nvtx_cut[ibin] = new TH1F(Form("hJfake_nvtx_cut_%i", ibin), "", 500, 0, 100);
    hJfake_dPhi_phoMET[ibin] = new TH1F(Form("hJfake_dPhi_phoMET_%i", ibin), "", 14, dphibin);

    hJfake_phopt_cut[ibin]->Sumw2();
    hJfake_phoEta_cut[ibin]->Sumw2();
    hJfake_phoPhi_cut[ibin]->Sumw2();
    hJfake_MET_cut[ibin]->Sumw2();
    hJfake_METPhi_cut[ibin]->Sumw2();
    hJfake_njet_cut[ibin]->Sumw2();
    hJfake_nvtx_cut[ibin]->Sumw2();
    hJfake_dPhi_phoMET[ibin]->Sumw2();

    for(Int_t jj=0; jj<2; jj++){
      hJfake_jetpt_cut[jj][ibin] = new TH1F(Form("hJfake_jetpt_cut_%i_%i", jj, ibin), "", 8, jetptbin);
      hJfake_jetEta_cut[jj][ibin] = new TH1F(Form("hJfake_jetEta_cut_%i_%i", jj, ibin), "", 22, jetetabin);
      hJfake_jetPhi_cut[jj][ibin] = new TH1F(Form("hJfake_jetPhi_cut_%i_%i", jj, ibin), "", 10, phibin);

      hJfake_jetpt_cut[jj][ibin]->Sumw2();
      hJfake_jetEta_cut[jj][ibin]->Sumw2();
      hJfake_jetPhi_cut[jj][ibin]->Sumw2();
    }
    
    hJfake_dEta_phojet[ibin] = new TH1F(Form("hJfake_dEta_phojet_%i", ibin), "", 32, 0, 8);
    hJfake_dPhi_phojet[ibin] = new TH1F(Form("hJfake_dPhi_phojet_%i", ibin), "", 14, dphibin);
    hJfake_dR_phojet[ibin] = new TH1F(Form("hJfake_dR_phojet_%i", ibin), "", 20, 0, 8);
    hJfake_dEta_jetjet[ibin] = new TH1F(Form("hJfake_dEta_jetjet_%i", ibin), "", 32, 0, 8); 
    hJfake_dPhi_jetjet[ibin] = new TH1F(Form("hJfake_dPhi_jetjet_%i", ibin), "", 14, dphibin);
    hJfake_dR_jetjet[ibin] = new TH1F(Form("hJfake_dR_jetjet_%i", ibin), "", 20, 0, 8);
    hJfake_dPhi_minjetMET[ibin] = new TH1F(Form("hJfake_dPhi_minjetMET_%i", ibin), "", 14, dphibin);
    hJfake_ptoverMET_cut[ibin] = new TH1F(Form("hJfake_ptoverMET_cut_%i", ibin), "", 60, 0, 12);
    hJfake_dijetMass_cut[ibin] = new TH1F(Form("hJfake_dijetMass_cut_%i", ibin), "", 5, massbin);

    hJfake_dEta_phojet[ibin]->Sumw2();
    hJfake_dPhi_phojet[ibin]->Sumw2();
    hJfake_dR_phojet[ibin]->Sumw2();
    hJfake_dEta_jetjet[ibin]->Sumw2();
    hJfake_dPhi_jetjet[ibin]->Sumw2();
    hJfake_dR_jetjet[ibin]->Sumw2();
    hJfake_dPhi_minjetMET[ibin]->Sumw2();
    hJfake_ptoverMET_cut[ibin]->Sumw2();
    hJfake_dijetMass_cut[ibin]->Sumw2();

  }
  
  // 2D plots
  TH2F *h2_phoEt_MET;
  h2_phoEt_MET = new TH2F("h2_phoEt_MET", "phoEt/y, MET/x", 18, ptbin, 10, METbin);
  h2_phoEt_MET->Sumw2();

  //input variable
  Bool_t	isData	      = 0;
  Int_t		nGoodVtx      = 0;
  Bool_t	isPVGood      = 0;
  Float_t	EventWeight   = 0;
  Int_t		METTrgs	      = 0;
  Int_t		PhoTrgs	      = 0;
  Float_t	rho	      = 0;
  Int_t*        phoIsMatch    = 0;
  Int_t         nPho	      = 0;
  Float_t*	phoE	      = 0; 
  Float_t*	phoEt	      = 0;
  Float_t*	phoEta	      = 0;
  Float_t*	phoSCEta      = 0;
  Float_t*	phoPhi	      = 0;
  Float_t*	phoSeedTime   = 0;
  Float_t*	phoMIP	      = 0;
  Float_t       corrMET	      = 0;
  Float_t       corrMETPhi    = 0;
  Int_t		nJet	      = 0;
  Int_t		npfjet	      = 0;
  Int_t         nSMjet	      = 0;
  Int_t         nVBSjet	      = 0;
  Float_t*	jetPt	      = 0;
  Float_t*	jetEta	      = 0;
  Float_t*	jetPhi	      = 0;
  Float_t*	jetEn	      = 0;
  Int_t*	nonPUjetid    = 0;
  Double_t	dijetMass     = 0;
  Int_t*        jetID	      = 0;
  Int_t*        jetPUIDMVA    = 0;
  Float_t*      jetPUID	      = 0;
  Int_t		nLep	      = 0;
  Int_t*	cutflowSMID   = 0; 
  Int_t*	cutflowVBSID  = 0;
  Float_t*	phoMETdPhi    = 0;
  Float_t	phoCentral    = 0;
  Float_t       minJMETdPhi   = 0;
  Float_t*	JMETdPhi      = 0;
  Float_t	jetjetdEta    = 0;
  Float_t	jetjetdPhi    = 0;
  Float_t	jetjetdR      = 0;
  Float_t*	phojetdEta    = 0;
  Float_t*	phojetdPhi    = 0;
  Float_t*	phojetdR      = 0;
  Float_t	jetjetZdPhi   = 0;
  Float_t	jetjetphodPhi = 0;
  Float_t	jet3Central   = 0;

  
  //***********************Loop***********************//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    //get data from tree
    data.GetEntry(ev);

    isPVGood		     = data.GetBool(		"isPVGood"		);
    nGoodVtx		     = data.GetInt(		"nGoodVtx"		);
    EventWeight		     = data.GetFloat(		"EventWeight"		);
    METTrgs		     = data.GetInt(		"METTrgs"		);
    PhoTrgs		     = data.GetInt(		"PhoTrgs"		);
    rho			     = data.GetFloat(		"rho"			);
    phoIsMatch		     = data.GetPtrInt(		"phoIsMatch"		);
    ///reco pho
    nPho		     = data.GetInt(		"nPho"			);
    phoE		     = data.GetPtrFloat(	"phoE"			);
    phoEt		     = data.GetPtrFloat(	"phoEt"			);
    phoEta		     = data.GetPtrFloat(	"phoEta"		);
    phoSCEta		     = data.GetPtrFloat(	"phoSCEta"		);
    phoPhi		     = data.GetPtrFloat(	"phoPhi"		);
    phoSeedTime		     = data.GetPtrFloat(	"phoSeedTime"		);
    phoMIP		     = data.GetPtrFloat(	"phoMIPTotEnergy"	);
    ///MET
    corrMET		     = data.GetFloat(		"corrMET"		);
    corrMETPhi		     = data.GetFloat(		"corrMETPhi"		);
    ///jet
    nJet		     = data.GetInt(		"nJet"			);
    npfjet		     = data.GetInt(		"npfjet"		);
    nSMjet		     = data.GetInt(		"nSMjet"                );
    nVBSjet		     = data.GetInt(		"nVBSjet"		);
    jetPt		     = data.GetPtrFloat(	"jetPtSmear"		);
    jetEta		     = data.GetPtrFloat(	"jetEta"		);
    jetPhi		     = data.GetPtrFloat(	"jetPhi"		);
    jetEn		     = data.GetPtrFloat(	"jetEn"			);
    nonPUjetid		     = data.GetPtrInt(		"nonPUjetid"		);
    dijetMass		     = data.GetDouble(		"dijetMass"		);
    if(ULsamples == 1) jetID = data.GetPtrInt(		"jetULTID"		);
    else	jetID	     = data.GetPtrInt(		"jetTID"		);
    nLep		     = data.GetInt(		"nLep"			);
    phoMETdPhi		     = data.GetPtrFloat(	"phoMETdPhi"		);
    phoCentral		     = data.GetFloat(		"phoCentral"		);
    minJMETdPhi		     = data.GetFloat(		"minJMETdPhi"           );
    JMETdPhi		     = data.GetPtrFloat(	"JMETdPhi"		);
    jetjetdR		     = data.GetFloat(		"jetjetdR"		);
    jetjetdEta		     = data.GetFloat(		"jetjetdEta"		);
    jetjetdPhi		     = data.GetFloat(		"jetjetdPhi"		);
    phojetdR		     = data.GetPtrFloat(	"phojetdR"		);
    phojetdEta		     = data.GetPtrFloat(	"phojetdEta"		);
    phojetdPhi		     = data.GetPtrFloat(	"phojetdPhi"		);
    jetjetZdPhi		     = data.GetFloat(		"jetjetZdPhi"		);
    jetjetphodPhi	     = data.GetFloat(		"jetjetphodPhi"		);
    jet3Central		     = data.GetFloat(		"jet3Central"		);
    //cout <<						"get data" << en

    //cutflow ID
    cutflowSMID = data.GetPtrInt("cutflowSMID");
    cutflowVBSID = data.GetPtrInt("cutflowVBSID");
    //cout << "get cutflow corr" << endl;

    // event weight
    WEIGHT = EventWeight;

    Int_t fillpho;
    Int_t Trgs;
    if(isTrgs == 1) Trgs = METTrgs;
    else Trgs = PhoTrgs;
    
    for(Int_t ipho=0; ipho<nPho; ipho++){
      if(ipho>0) continue;
      if(Trgs == 0) continue;
      //cout << "Trgs : phoEt = " << Trgs << " : " << phoEt[ipho] << endl; 
      
      Int_t cutflow;
      if(isSM==1) cutflow = cutflowSMID[ipho];
      else cutflow = cutflowVBSID[ipho];

      //fillpho = 0;
      //if(pho_sel(cutflow, 16) == 1) fillpho += 1; // final selection : dijetmass
      //if(fillpho>1) continue;
      
      // Fill cutflow effciency plot in phoEt & MET

      if(pho_sel(cutflow,2) == 1) h_phoEB_pt_phopt->Fill(phoEt[ipho], WEIGHT);
      if(pho_sel(cutflow,3) == 1) h_phoEB_pt_MphoID->Fill(phoEt[ipho], WEIGHT);
      if(pho_sel(cutflow,5) == 1) h_phoEB_pt_lepveto->Fill(phoEt[ipho], WEIGHT);
      if(pho_sel(cutflow,6) == 1) h_phoEB_pt_MET->Fill(phoEt[ipho], WEIGHT);
      if(pho_sel(cutflow,7) == 1) h_phoEB_pt_dphoMETPhi->Fill(phoEt[ipho], WEIGHT);
      if(pho_sel(cutflow,8) == 1) h_phoEB_pt_njet->Fill(phoEt[ipho], WEIGHT);
      // if isSM == 0
      if(isSM == 0){
	if(pho_sel(cutflow,9) == 1) h_phoEB_pt_jetpt->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow,10) == 1) h_phoEB_pt_jetjetdEta->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow,11) == 1) h_phoEB_pt_jetjetdPhi->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow,12) == 1) h_phoEB_pt_phojetdR->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow,13) == 1) h_phoEB_pt_ptoverMET->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow,14) == 1) h_phoEB_pt_mindJMETPhi->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow,15) == 1) h_phoEB_pt_diejtMass->Fill(phoEt[ipho], WEIGHT);
      }

      if(pho_sel(cutflow,2) == 1) h_MET_phopt->Fill(corrMET, WEIGHT);
      if(pho_sel(cutflow,3) == 1) h_MET_MphoID->Fill(corrMET, WEIGHT);
      if(pho_sel(cutflow,5) == 1) h_MET_lepveto->Fill(corrMET, WEIGHT);
      if(pho_sel(cutflow,6) == 1) h_MET_MET->Fill(corrMET, WEIGHT);
      if(pho_sel(cutflow,7) == 1) h_MET_dphoMETPhi->Fill(corrMET, WEIGHT);
      if(pho_sel(cutflow,8) == 1) h_MET_njet->Fill(corrMET, WEIGHT);
      // if isSM == 0
      if(isSM == 0){
	if(pho_sel(cutflow,9) == 1) h_MET_jetpt->Fill(corrMET, WEIGHT);
	if(pho_sel(cutflow,10) == 1) h_MET_jetjetdEta->Fill(corrMET, WEIGHT);
	if(pho_sel(cutflow,11) == 1) h_MET_jetjetdPhi->Fill(corrMET, WEIGHT);
	if(pho_sel(cutflow,12) == 1) h_MET_phojetdR->Fill(corrMET, WEIGHT);
	if(pho_sel(cutflow,13) == 1) h_MET_ptoverMET->Fill(corrMET, WEIGHT);
	if(pho_sel(cutflow,14) == 1) h_MET_mindJMETPhi->Fill(corrMET, WEIGHT);
	if(pho_sel(cutflow,15) == 1) h_MET_diejtMass->Fill(corrMET, WEIGHT);
      }

      //cout << "fill eff" << endl;
      
      // Fill final stack plot
      if(pho_sel(cutflow, 15) == 1){
	h_phopt_cut->Fill(phoEt[ipho], WEIGHT);
	h_phoEta_cut->Fill(phoEta[ipho], WEIGHT);
	h_phoPhi_cut->Fill(phoPhi[ipho], WEIGHT);
	h_MET_cut->Fill(corrMET, WEIGHT);
	h_METPhi_cut->Fill(corrMETPhi, WEIGHT);
	h_njet_cut->Fill(npfjet, WEIGHT);
	h_nvtx_cut->Fill(nGoodVtx, WEIGHT);	
	h_dPhi_phoMET->Fill(phoMETdPhi[ipho], WEIGHT);

	// for(Int_t ibin=0; ibin<nPhobins; ibin++){
	//   if(phoEt[ipho] >= Phobin[ibin] && phoEt[ipho] < Phobin[ibin+1]){
	//     hJfake_phopt_cut[ibin]->Fill(phoEt[ipho], WEIGHT);
	//     hJfake_phoEta_cut[ibin]->Fill(phoEta[ipho], WEIGHT);
	//     hJfake_phoPhi_cut[ibin]->Fill(phoPhi[ipho], WEIGHT);
	//     hJfake_MET_cut[ibin]->Fill(corrMET, WEIGHT);
	//     hJfake_METPhi_cut[ibin]->Fill(corrMETPhi, WEIGHT);
	//     hJfake_njet_cut[ibin]->Fill(npfjet, WEIGHT);
	//     hJfake_nvtx_cut[ibin]->Fill(nGoodVtx, WEIGHT);	
	//     hJfake_dPhi_phoMET[ibin]->Fill(phoMETdPhi[ipho], WEIGHT);
	//   }
	// }
	
	// if isSM == 0
	if(isSM == 0){
	  for(Int_t jj=0; jj<npfjet; jj++){
	    if(jj>1) continue;
	    Int_t ijet = nonPUjetid[jj];
	  
	    h_jetpt_cut[jj]->Fill(jetPt[ijet], WEIGHT);
	    h_jetEta_cut[jj]->Fill(jetEta[ijet], WEIGHT);
	    h_jetPhi_cut[jj]->Fill(jetPhi[ijet], WEIGHT);

	    // for(Int_t ibin=0; ibin<nPhobins; ibin++){
	    //   hJfake_jetpt_cut[jj][ibin]->Fill(jetPt[ijet], WEIGHT);
	    //   hJfake_jetEta_cut[jj][ibin]->Fill(jetEta[ijet], WEIGHT);
	    //   hJfake_jetPhi_cut[jj][ibin]->Fill(jetPhi[ijet], WEIGHT);
	    // }
	  }
	
	  h_dEta_phojet->Fill(phojetdEta[ipho], WEIGHT);  
	  h_dPhi_phojet->Fill(phojetdPhi[ipho], WEIGHT);
	  h_dR_phojet->Fill(phojetdR[ipho], WEIGHT);
	  h_dEta_jetjet->Fill(jetjetdEta, WEIGHT);  
	  h_dPhi_jetjet->Fill(jetjetdPhi, WEIGHT);
	  h_dR_jetjet->Fill(jetjetdR, WEIGHT);
	  h_dPhi_minjetMET->Fill(minJMETdPhi, WEIGHT);
  
	  h_ptoverMET_cut->Fill(phoEt[ipho]/corrMET, WEIGHT);
	  h_dijetMass_cut->Fill(dijetMass, WEIGHT);

	  // for(Int_t ibin=0; ibin<nPhobins; ibin++){
	  //   hJfake_dEta_phojet[ibin]->Fill(phojetdEta[ipho], WEIGHT);  
	  //   hJfake_dPhi_phojet[ibin]->Fill(phojetdPhi[ipho], WEIGHT);
	  //   hJfake_dR_phojet[ibin]->Fill(phojetdR[ipho], WEIGHT);
	  //   hJfake_dEta_jetjet[ibin]->Fill(jetjetdEta, WEIGHT);  
	  //   hJfake_dPhi_jetjet[ibin]->Fill(jetjetdPhi, WEIGHT);
	  //   hJfake_dR_jetjet[ibin]->Fill(jetjetdR, WEIGHT);
	  //   hJfake_dPhi_minjetMET[ibin]->Fill(minJMETdPhi, WEIGHT);
  
	  //   hJfake_ptoverMET_cut[ibin]->Fill(phoEt[ipho]/corrMET, WEIGHT);
	  //   hJfake_dijetMass_cut[ibin]->Fill(dijetMass, WEIGHT);

	  // }
	}
      }
      //cout << "fill stack" << endl;
      
      // Fill N-1 plot
      
      if(Nm1_sel(cutflow,6) == 1) h_MET_Nm1->Fill(corrMET, WEIGHT);
      if(Nm1_sel(cutflow,7) == 1) h_dPhi_phoMET_Nm1->Fill(phoMETdPhi[ipho], WEIGHT);
      if(Nm1_sel(cutflow,8) == 1) h_njet_Nm1->Fill(npfjet, WEIGHT);
      // isSM == 0
      if(isSM == 0){
	for(Int_t jj=0; jj<npfjet; jj++){
	  if(jj>1) continue;
	  Int_t ijet = nonPUjetid[jj];
	  
	  if(Nm1_sel(cutflow,9) == 1) h_jetpt_Nm1[jj]->Fill(jetPt[ijet], WEIGHT);
	  if(Nm1_sel(cutflow,10) == 1) h_jeteta_Nm1[jj]->Fill(jetEta[ijet], WEIGHT);
	}
	if(Nm1_sel(cutflow,10) == 1) h_dEta_jetjet_Nm1->Fill(jetjetdEta, WEIGHT);
	if(Nm1_sel(cutflow,11) == 1) h_dPhi_jetjet_Nm1->Fill(jetjetdPhi, WEIGHT);
	if(Nm1_sel(cutflow,12) == 1) h_dR_phojet_Nm1->Fill(phojetdR[ipho], WEIGHT);
	if(Nm1_sel(cutflow,13) == 1) h_ptoverMET_Nm1->Fill(minJMETdPhi, WEIGHT);
	if(Nm1_sel(cutflow,14) == 1) h_dPhi_minjetMET_Nm1->Fill(minJMETdPhi, WEIGHT);
	if(Nm1_sel(cutflow,15) == 1) h_dijetMass_Nm1->Fill(dijetMass, WEIGHT);
      }
      //cout << "fill Nm1" << endl;
      
      // fill Wgamma CR plots
      if(Nm1_sel(cutflow,5) == 1){
	h_nLep_WgCR->Fill(nLep, WEIGHT);
	h_MET_WgCR->Fill(corrMET, WEIGHT);
	h_dijetMass_WgCR->Fill(dijetMass, WEIGHT);
      }
      
      // Fill h2 plot
      h2_phoEt_MET->Fill(corrMET, phoEt[ipho], WEIGHT);
      //cout << "fill h2" << endl;
    }    
  }
  //****************END LOOP**********************//

  //****************Terminate*********************//
  fout_->cd();
  
  hEvents->Write();
  hSumofGenW->Write();
  h_pfMET_all->Write();
  h_pfMET_HLTpass->Write();
  h_puppiMET_all->Write();
  h_puppiMET_HLTpass->Write();
  
  h_phopt_cut->Write();
  h_phoEta_cut->Write();
  h_phoPhi_cut->Write();
  h_MET_cut->Write();
  h_METPhi_cut->Write();
  h_njet_cut->Write();
  h_nvtx_cut->Write();
  h_dPhi_phoMET->Write();
  if(isSM == 0){
    for(Int_t jj=0; jj<2; jj++){
      h_jetpt_cut[jj]->Write();
      h_jetEta_cut[jj]->Write();
      h_jetPhi_cut[jj]->Write();
    }
    h_dEta_phojet->Write();  
    h_dPhi_phojet->Write();
    h_dR_phojet->Write();
    h_dEta_jetjet->Write();  
    h_dPhi_jetjet->Write();
    h_dR_jetjet->Write();
    h_dPhi_minjetMET->Write();
    h_ptoverMET_cut->Write();
    h_dijetMass_cut->Write();
  }

  fout_->mkdir("phoEB_pt");
  fout_->cd("phoEB_pt");
  h_phoEB_pt_phopt->Write();
  h_phoEB_pt_MphoID->Write();
  h_phoEB_pt_lepveto->Write();
  h_phoEB_pt_MET->Write();
  h_phoEB_pt_dphoMETPhi->Write();
  h_phoEB_pt_njet->Write();
  if(isSM == 0){
    h_phoEB_pt_jetpt->Write();
    h_phoEB_pt_jetjetdEta->Write();
    h_phoEB_pt_jetjetdPhi->Write();
    h_phoEB_pt_phojetdR->Write();
    h_phoEB_pt_ptoverMET->Write();
    h_phoEB_pt_mindJMETPhi->Write();
    h_phoEB_pt_diejtMass->Write();
  }

  fout_->mkdir("MET");
  fout_->cd("MET");
  h_MET_phopt->Write();
  h_MET_MphoID->Write();
  h_MET_lepveto->Write();
  h_MET_MET->Write();
  h_MET_dphoMETPhi->Write();
  h_MET_njet->Write();
  if(isSM == 0){
    h_MET_jetpt->Write();
    h_MET_jetjetdEta->Write();
    h_MET_jetjetdPhi->Write();
    h_MET_phojetdR->Write();
    h_MET_ptoverMET->Write();
    h_MET_mindJMETPhi->Write();
    h_MET_diejtMass->Write();
  }

  fout_->mkdir("Nm1_plot");
  fout_->cd("Nm1_plot");
  h_MET_Nm1->Write();
  h_njet_Nm1->Write();
  h_dPhi_phoMET_Nm1->Write();
  // isSM == 0
  for(Int_t jj=0; jj<2; jj++){
    h_jetpt_Nm1[jj]->Write();
    h_jeteta_Nm1[jj]->Write();
  }
  h_dR_phojet_Nm1->Write();
  h_dEta_jetjet_Nm1->Write();
  h_dPhi_jetjet_Nm1->Write();
  h_dPhi_minjetMET_Nm1->Write();
  h_ptoverMET_Nm1->Write();
  h_dijetMass_Nm1->Write();

  fout_->mkdir("WgammaCR");
  fout_->cd("WgammaCR");
  h_nLep_WgCR->Write();
  h_MET_WgCR->Write();
  h_dijetMass_WgCR->Write();

  // fout_->mkdir("Jfake_plot");
  // fout_->cd("Jfake_plot");
  // for(Int_t ibin=0; ibin<nPhobins; ibin++){
  //   hJfake_phopt_cut[ibin]->Write();
  //   hJfake_phoEta_cut[ibin]->Write();
  //   hJfake_phoPhi_cut[ibin]->Write();
  //   hJfake_MET_cut[ibin]->Write();
  //   hJfake_METPhi_cut[ibin]->Write();
  //   hJfake_njet_cut[ibin]->Write();
  //   hJfake_nvtx_cut[ibin]->Write();
  //   hJfake_dPhi_phoMET[ibin]->Write();
  //   if(isSM == 0){
  //     for(Int_t jj=0; jj<2; jj++){
  // 	hJfake_jetpt_cut[jj][ibin]->Write();
  // 	hJfake_jetEta_cut[jj][ibin]->Write();
  // 	hJfake_jetPhi_cut[jj][ibin]->Write();
  //     }
  //     hJfake_dEta_phojet[ibin]->Write();  
  //     hJfake_dPhi_phojet[ibin]->Write();
  //     hJfake_dR_phojet[ibin]->Write();
  //     hJfake_dEta_jetjet[ibin]->Write();  
  //     hJfake_dPhi_jetjet[ibin]->Write();
  //     hJfake_dR_jetjet[ibin]->Write();
  //     hJfake_dPhi_minjetMET[ibin]->Write();
  //     hJfake_ptoverMET_cut[ibin]->Write();
  //     hJfake_dijetMass_cut[ibin]->Write();
  //   }
  // }
  
  fout_->Close();
  fprintf(stderr, "%s Processed all events\n", treename.c_str());
}
