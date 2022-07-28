#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

#include "smallntuplizer.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "PhotonSelection.h"

Float_t WEIGHT = 1;

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

void xZgChIso(char* pathes){
  //***********************Initialization***********************//

  //Get processed events
  TFile *fopen = new TFile(pathes, "READ");
  TH1F *hEvents = (TH1F*)fopen->Get("hEvents")->Clone();
  hEvents->SetDirectory(0);
  TH1F *hSumofGenW = (TH1F*)fopen->Get("hSumofGenW")->Clone();
  hSumofGenW->SetDirectory(0);
  fopen->Close();

  //access EventTree with TreeReader class
  TreeReader data(pathes);

  //create an output .root file
  TFile *fout_;
  char foutname[50];
  string treestr = pathes;
  Int_t istr = treestr.find("ggtree");
  string treename = treestr.substr(istr);
  sprintf(foutname, "output_chIso_%s", treename.c_str());
  fout_ = new TFile(foutname,"RECREATE");


  Double_t ptbin[30] = {22, 30, 36, 50, 75, 90, 120, 170, 175, 180, 185, 190, 210,
			230, 250, 300, 350, 400, 500, 750, 1000, 1500, 2000, 3000, 10000};//22 bins, 2016
  Double_t etabin[10] = {-1.566, -1.4442, -0.8, 0, 0.8, 1.4442, 1.566};//6bins

  TH1F *h_phoEB_pt_210[2];
  for(Int_t j=0; j<2; j++){
    h_phoEB_pt_210[j] = new TH1F(Form("h_phoEB_pt_210_%i", j), "matched phoEB pt pt200 cut", 22, ptbin);
    h_phoEB_pt_210[j]->Sumw2();
  }
  
  //chIso vs chworst
  Float_t chbin[30] = {0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.05, 1.1, 1.15, 1.2, 1.25, 1.3, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.5, 4.0, 5.0, 6.0};//24 bins
  TH1F *h_phoEB_pt_chIsocut[2][22];
  TH1F *h_phoEB_pt_chworstcut_newEA[2][22];
  TH1F *h_chIso_ptcut[2][22];
  TH1F *h_chworst_ptcut[2][22];
  for(Int_t ii=0; ii<2; ii++){
    for(Int_t j=0; j<22; j++){
      h_phoEB_pt_chIsocut[ii][j] = new TH1F(Form("h_phoEB_pt_chIsocut_%i_%ibin", ii, j), Form("h_phoEB_pt_chIsocut_%i_%ibin", ii, j), 22, ptbin);
      h_phoEB_pt_chworstcut_newEA[ii][j] = new TH1F(Form("h_phoEB_pt_chworstcut_newEA_%i_%ibin", ii, j), Form("h_phoEB_pt_chworstcut_newEA_%i_%ibin", ii, j), 22, ptbin);

      h_phoEB_pt_chIsocut[ii][j]->Sumw2();
      h_phoEB_pt_chworstcut_newEA[ii][j]->Sumw2();

      h_chIso_ptcut[ii][j] = new TH1F(Form("h_chIso_ptcut_%i_%ibin", ii, j), Form("h_chIso_ptcut_%i_%ibin", ii, j), 20, 200, 1000);
      h_chworst_ptcut[ii][j] = new TH1F(Form("h_chworst_ptcut_%i_%ibin", ii, j), Form("h_chworst_ptcut_%i_%ibin", ii, j), 20, 200, 1000);

      h_chIso_ptcut[ii][j]->Sumw2();
      h_chworst_ptcut[ii][j]->Sumw2();
    }
  }

  //input variable
  Float_t*	EventWeight	= 0;
  Int_t         nPho            = 0;
  Float_t*	phoE		= 0; 
  Float_t*	phoEt		= 0;
  Float_t*	phoEta	        = 0;
  Float_t*	phoSCEta	= 0;
  Float_t*	phoPhi	       	= 0;
  Float_t*      phoPFChIso      = 0;
  Float_t*      phoPFChWorstIso = 0;
  Float_t*      phoPFChIso_rc      = 0;
  Float_t*      phoPFChWorstIso_newEA = 0;
  Int_t*	cutflowSMID	= 0; 
  Int_t*	cutflowVBSID	= 0;
  Int_t*        cutflowSMfixMETID = 0;
  Int_t*        cutflowVBSfixMETID = 0;

  //***********************Loop***********************//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    //get data from tree
    data.GetEntry(ev);

    EventWeight	= data.GetPtrFloat(	"EventWeight"		);
    //reco pho
    nPho        = data.GetInt("nPho");
    phoE	= data.GetPtrFloat(	"phoE"			);
    phoEt	= data.GetPtrFloat(	"phoEt"			);
    phoPFChIso_rc  = data.GetPtrFloat("phoPFChIso_rc");
    phoPFChWorstIso_newEA = data.GetPtrFloat("phoPFChWorstIso_newEA");
    //cutflow ID
    cutflowSMID = data.GetPtrInt("cutflowSMID");
    cutflowVBSID = data.GetPtrInt("cutflowVBSID");
    

    //Medium no chIso or chWorst
    vector<Int_t> phoNchID; phoNchID.clear();
    pho_Nchselection(1, 0, data, phoNchID);
    
    Int_t fillpho[2];
    for(Int_t ipho=0; ipho<nPho; ipho++){
      Int_t cutflow[2];
      cutflow[0] = cutflowSMID[ipho]; 
      cutflow[1] = cutflowVBSID[ipho];
      
      WEIGHT = EventWeight[ipho];

      for(Int_t icat=0; icat<2; icat++){
	if(Nm1_sel(cutflow[icat], 4) == 1 && Nm1_sel(cutflow[icat], 8) ==1){
	  for(Int_t ii=0; ii<22; ii++){
	    Int_t icut = ii+1;
	    if(phoNchID[ipho] == 1 && phoPFChIso_rc[ipho]<chbin[icut]) {
	      h_phoEB_pt_chIsocut[icat][ii]->Fill(phoEt[ipho], WEIGHT);
	      h_chIso_ptcut[icat][ii]->Fill(phoEt[ipho], WEIGHT);
	    }
	    if(phoNchID[ipho] == 1 && phoPFChWorstIso_newEA[ipho]<chbin[icut]) {
	      h_phoEB_pt_chworstcut_newEA[icat][ii]->Fill(phoEt[ipho], WEIGHT);
	      h_chworst_ptcut[icat][ii]->Fill(phoEt[ipho], WEIGHT);
	    }
	  }
	}
      }
	
    }
  }
  //****************END LOOP**********************//

  //****************Terminate*********************//
  fout_->cd();

  fout_->mkdir("chIso");
  fout_->cd("chIso");
  for(Int_t ii=0; ii<2; ii++){
    for(Int_t j=0; j<22; j++){
      h_phoEB_pt_chIsocut[ii][j]->Write();
      h_chIso_ptcut[ii][j]->Write();
    }
  }

  fout_->mkdir("chworst");
  fout_->cd("chworst");
  for(Int_t ii=0; ii<2; ii++){
    for(Int_t j=0; j<22; j++){
      h_phoEB_pt_chworstcut_newEA[ii][j]->Write();
      h_chworst_ptcut[ii][j]->Write();
    }
  }
  
  fout_->Close();
  fprintf(stderr, "%s Processed all events\n", treename.c_str());
}
  

