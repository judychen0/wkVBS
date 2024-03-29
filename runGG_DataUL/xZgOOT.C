// selections suite for MET trigger Zg VBS analysis

#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
#include "TH1F.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TVector2.h"

//#include "smallntuplizer.h"
#include "untuplizer.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "PhotonSelection.h"
#include "JetSelection.h"

Double_t deltaPhi(Double_t phi1, Double_t phi2) {
  Double_t dPhi = phi1 - phi2;
  if (dPhi > TMath::Pi()) dPhi -= 2.*TMath::Pi();
  if (dPhi < -TMath::Pi()) dPhi += 2.*TMath::Pi();
  return dPhi;
}

Double_t Phi_mpi_pi(Double_t phi){
  if(phi > TMath::Pi()) phi -= 2.*TMath::Pi();
  if(phi < -TMath::Pi()) phi += 2.*TMath::Pi();
  return phi;
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

void xZgOOT(char* pathes, Int_t year){

  //Get processed events
  TFile *fopen = new TFile(pathes, "READ");
  TH1F *hEvents = (TH1F*)fopen->Get("ggNtuplizer/hEvents")->Clone();
  hEvents->SetDirectory(0);
  fopen->Close();

  //access EventTree with TreeReader class
  TreeReader data(pathes);
  //cout << "get tree" << endl;
  //create an output .root file
  TFile *fout_;
  char foutname[50];
  string treestr = pathes;
  Int_t istr = treestr.find("ggtree");
  string treename = treestr.substr(istr);
  sprintf(foutname, "OOT_%s", treename.c_str());
  fout_ = new TFile(foutname,"RECREATE");

  //input variables
  ///MET
  Float_t       pfMET_;
  Float_t       pfMETPhi_;
  Float_t puppiMET_;
  Float_t puppiMETPhi_;
  Int_t         metFilters_;
  ///photon
  Int_t         nPho_;
  Float_t*      phoEt_;
  Float_t*      phoEta_;
  Float_t*      phoPhi_;
  Float_t*      phoSCEta_;
  Int_t*        phohasPixelSeed_;
  Float_t*      phoHoverE_;
  Float_t*      phoSigmaIEtaIEtaFull5x5_;
  Float_t*      phoR9Full5x5_;
  Float_t*      phoSeedTime_;
  Float_t*      phoSeedEnergy_;
  Float_t*      phoMIPTotEnergy_;
  Float_t* phoEtop_;
  Float_t* phoEleft_;
  Float_t* phoEright_;
  Float_t* phoEbottom_;

  ///OOT photon
  Int_t         nOOTPho_;
  Float_t*      ootPhoEt_;
  Float_t*      ootPhoEta_;
  Float_t*      ootPhoPhi_;
  Float_t*      ootPhoSCEta_;
  Int_t* ootPhohasPixelSeed_;
  Float_t*      ootPhoHoverE_;
  Float_t*      ootPhoSigmaIEtaIEtaFull5x5_;
  Float_t*      ootPhoR9Full5x5_;
  Float_t*      ootPhoSeedTime_;
  Float_t*      ootPhoSeedEnergy_;
  Float_t*      ootPhoMIPTotEnergy_;
  Float_t* ootPhoEtop_;
  Float_t* ootPhoEleft_;
  Float_t* ootPhoEright_;
  Float_t* ootPhoEbottom_;

  ///jets
  Int_t         nJet_;
  Float_t*      jetPt_;
  Float_t*      jetEn_;
  Float_t*      jetEta_;
  Float_t*      jetPhi_;
  Float_t*      jetPUID_;
  Int_t*        jetID_;

  //output variable
  ///MET
  Float_t               pfMET;
  Float_t               pfMETPhi;
  Float_t               puppiMET;
  Float_t               puppiMETPhi;
  Float_t		corrMET;
  Float_t		corrMETPhi;
  Int_t                 metFilters;
  
  ///photon
  Int_t                 nPho;//HLT pho
  vector<Float_t>       phoEt;
  vector<Float_t>       phoEta;
  vector<Float_t>       phoPhi;
  vector<Float_t>       phoSCEta;
  vector<Int_t>         phohasPixelSeed;
  vector<Float_t>       phoHoverE;
  vector<Float_t>       phoSigmaIEtaIEtaFull5x5;
  vector<Float_t>       phoR9Full5x5;
  vector<Float_t>       phoSeedTime;
  vector<Float_t>       phoSeedEnergy;
  vector<Float_t>       phoMIPTotEnergy;
  vector<Float_t> phoEtop;
  vector<Float_t> phoEleft;
  vector<Float_t> phoEright;
  vector<Float_t> phoEbottom;
  vector<Float_t> phoE4;
  vector<Float_t> phoE4overE1;
  vector<Float_t> phoEtaWing;
  vector<Float_t> phoEtaWingoverE1;

  ///jets
  Int_t                 nJet;
  Int_t                 npfjet;
  Int_t                 nSMjet;
  Int_t                 nVBSjet;
  vector<Int_t>         nonPUjetid;
  vector<Float_t>       jetPt;
  vector<Float_t>       jetEn;
  vector<Float_t>       jetEta;
  vector<Float_t>       jetPhi;
  Double_t              dijetMass;
  vector<Int_t>         jetPUIDMVA;
  vector<Float_t>       jetPUID;
  vector<Int_t>         jetID;
  vector<Int_t>         jetTID;
  ///angular variables
  Float_t               minJMETdPhi;
  vector<Float_t>       JMETdPhi;
  Float_t               jetjetdR;
  Float_t               jetjetdEta;
  Float_t               jetjetdPhi;
  vector<Float_t>       phojetdR;
  vector<Float_t>       phojetdEta;
  vector<Float_t>       phojetdPhi;
  vector<Float_t>       phoMETdPhi;
  
  ///OOT photon
  Int_t			nOOTPho;
  vector<Float_t>       ootPhoEt;
  vector<Float_t>       ootPhoEta;
  vector<Float_t>       ootPhoPhi;
  vector<Float_t>       ootPhoSCEta;
  vector<Int_t> ootPhohasPixelSeed;
  vector<Float_t>       ootPhoHoverE;
  vector<Float_t>       ootPhoSigmaIEtaIEtaFull5x5;
  vector<Float_t>       ootPhoR9Full5x5;
  vector<Float_t>       ootPhoSeedTime;
  vector<Float_t>       ootPhoSeedEnergy;
  vector<Float_t>       ootPhoMIPTotEnergy;
  vector<Float_t> ootPhoEtop;
  vector<Float_t> ootPhoEleft;
  vector<Float_t> ootPhoEright;
  vector<Float_t> ootPhoEbottom;
  vector<Float_t> ootPhoE4;
  vector<Float_t> ootPhoE4overE1;
  vector<Float_t> ootPhoEtaWing;
  vector<Float_t> ootPhoEtaWingoverE1;

  Int_t nINCPho = 0;
  vector<Int_t> isOOTPho;
  vector<Float_t> incPhoEt;
  vector<Float_t> incPhoEta;
  vector<Float_t> incPhoPhi;
  vector<Float_t> incPhoSieie;
  vector<Float_t> incPhoHoverE;
  vector<Float_t> incPhoR9;
  vector<Float_t> incPhoMIP;
  vector<Float_t> incPhoSeedTime;
  vector<Float_t> incPhoSeedEn;
  vector<Int_t> incPhoHasPixelSeed;
  vector<Float_t> incPhoE4overE1;
  vector<Float_t> incPhoEtaWingoverE1;
  // Int_t npOOTPho = 0;
  // vector<Float_t> pOOTPhoEt;
  // vector<Float_t> pOOTPhoEta;
  // vector<Float_t> pOOTPhoPhi;
  // vector<Float_t> pOOTPhoSieie;
  // vector<Float_t> pOOTPhoHoverE;
  // vector<Float_t> pOOTPhoR9;
  // vector<Float_t> pOOTPhoMIP;
  // vector<Float_t> pOOTPhoSeedTime;
  // vector<Float_t> pOOTPhoSeedEn;
  // vector<Int_t> pOOTPhoHasPixelSeed;
  // vector<Float_t> pOOTPhoE4overE1;
  // vector<Float_t> pOOTPhoEtaWingoverE1;

  // histos of variables

  Double_t ptbin[30] = {0, 20, 40, 75, 90, 110, 130, 150, 180, 210, 250, 300, 350, 400, 450, 500, 550, 600, 700, 800, 1000};//20 bins
  Double_t METbin[20] = {0, 45, 90, 135, 180, 250, 300, 400, 500, 650, 800, 1200};//10bins
  Double_t sieiebin[20] = {0.0, 0.001, 0.005, 0.0075, 0.008, 0.0085, 0.009, 0.0095, 0.01015, 0.0113, 0.0125, 0.015, 0.0175, 0.02, 0.05}; // 14 bins
  
  TH1F *h_pfMET;
  TH1F *h_puppiMET;
  TH1F *h_corrMET;
  
  h_pfMET = new TH1F("h_pfMET", "", 10, METbin);
  h_puppiMET = new TH1F("h_puppiMET", "", 10, METbin);
  h_corrMET = new TH1F("h_corrMET", "", 10, METbin);
  h_pfMET->Sumw2();
  h_puppiMET->Sumw2();
  h_corrMET->Sumw2();
  
  // template histos
  
  TH1F *h_Cand_SeedTime;
  TH1F *h_Cand_phoEt;
  TH1F *h_Cand_MET;
  TH1F *h_Cand_sieie;
  TH1F *h_Cand_MIP;
  h_Cand_SeedTime = new TH1F("h_Cand_SeedTime", "", 100, -25, 25);
  h_Cand_phoEt = new TH1F("h_Cand_phoEt", "", 20, ptbin);
  h_Cand_MET = new TH1F("h_Cand_MET", "", 10, METbin);
  h_Cand_sieie = new TH1F("h_Cand_sieie", "", 14, sieiebin);
  h_Cand_MIP = new TH1F("h_Cand_MIP", "", 10, 0, 10);
  h_Cand_SeedTime->Sumw2();
  h_Cand_phoEt->Sumw2();
  h_Cand_MET->Sumw2();
  h_Cand_sieie->Sumw2();
  h_Cand_MIP->Sumw2();
  
  TH1F *h_prompt_SeedTime;
  TH1F *h_prompt_phoEt;
  TH1F *h_prompt_MET;
  TH1F *h_prompt_sieie;
  TH1F *h_prompt_MIP;
  h_prompt_SeedTime = new TH1F("h_prompt_SeedTime", "", 100, -25, 25);
  h_prompt_phoEt = new TH1F("h_prompt_phoEt", "", 20, ptbin);
  h_prompt_MET = new TH1F("h_prompt_MET", "", 10, METbin);
  h_prompt_sieie = new TH1F("h_prompt_sieie", "", 14, sieiebin);
  h_prompt_MIP = new TH1F("h_prompt_MIP", "", 10, 0, 10);
  h_prompt_SeedTime->Sumw2();
  h_prompt_phoEt->Sumw2();
  h_prompt_MET->Sumw2();
  h_prompt_sieie->Sumw2();
  h_prompt_MIP->Sumw2();

  TH1F *h_ootBH_SeedTime;
  TH1F *h_ootBH_phoEt;
  TH1F *h_ootBH_MET;
  TH1F *h_ootBH_sieie;
  TH1F *h_ootBH_MIP;
  h_ootBH_SeedTime = new TH1F("h_ootBH_SeedTime", "", 100, -25, 25);
  h_ootBH_phoEt = new TH1F("h_ootBH_phoEt", "", 20, ptbin);
  h_ootBH_MET = new TH1F("h_ootBH_MET", "", 10, METbin);
  h_ootBH_sieie = new TH1F("h_ootBH_sieie", "", 14, sieiebin);
  h_ootBH_MIP = new TH1F("h_ootBH_MIP", "", 10, 0, 10);
  h_ootBH_SeedTime->Sumw2();
  h_ootBH_phoEt->Sumw2();
  h_ootBH_MET->Sumw2();
  h_ootBH_sieie->Sumw2();
  h_ootBH_MIP->Sumw2();

  TH1F *h_ootSP_SeedTime;
  TH1F *h_ootSP_phoEt;
  TH1F *h_ootSP_MET;
  TH1F *h_ootSP_sieie;
  TH1F *h_ootSP_MIP;
  h_ootSP_SeedTime = new TH1F("h_ootSP_SeedTime", "", 100, -25, 25);
  h_ootSP_phoEt = new TH1F("h_ootSP_phoEt", "", 20, ptbin);
  h_ootSP_MET = new TH1F("h_ootSP_MET", "", 10, METbin);
  h_ootSP_sieie = new TH1F("h_ootSP_sieie", "", 14, sieiebin);
  h_ootSP_MIP = new TH1F("h_ootSP_MIP", "", 10, 0, 10);
  h_ootSP_SeedTime->Sumw2();
  h_ootSP_phoEt->Sumw2();
  h_ootSP_MET->Sumw2();
  h_ootSP_sieie->Sumw2();
  h_ootSP_MIP->Sumw2();


  //**************************************************//
  //***********************Loop***********************//
  //**************************************************//

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    //get data from tree
    data.GetEntry(ev);

    pfMET_			 = data.GetFloat(	"pfMET"				);
    pfMETPhi_			 = data.GetFloat(	"pfMETPhi"			);
    puppiMET_			 = data.GetFloat(	"puppiMET"				);
    puppiMETPhi_			 = data.GetFloat(	"puppiMETPhi"			);
    metFilters_		         = data.GetInt(		"metFilters"			);
    nPho_			 = data.GetInt(		"nPho"				);
    phoEt_			 = data.GetPtrFloat(	"phoEt"				);
    phoEta_			 = data.GetPtrFloat(	"phoEta"			);
    phoPhi_			 = data.GetPtrFloat(	"phoPhi"			);
    phoSCEta_			 = data.GetPtrFloat(	"phoSCEta"			);
    phohasPixelSeed_		 = data.GetPtrInt(	"phohasPixelSeed"		);
    phoHoverE_			 = data.GetPtrFloat(	"phoHoverE"			);
    phoSigmaIEtaIEtaFull5x5_     = data.GetPtrFloat(	"phoSigmaIEtaIEtaFull5x5"	);
    phoR9Full5x5_		 = data.GetPtrFloat(	"phoR9Full5x5"			);
    phoSeedTime_		 = data.GetPtrFloat(	"phoSeedTime"			);
    phoSeedEnergy_		 = data.GetPtrFloat(	"phoSeedEnergy"			);
    phoMIPTotEnergy_		 = data.GetPtrFloat(	"phoMIPTotEnergy"		);
    phoEtop_ = data.GetPtrFloat("phoEtop");
    phoEleft_ = data.GetPtrFloat("phoEleft");
    phoEright_ = data.GetPtrFloat("phoEright");
    phoEbottom_ = data.GetPtrFloat("phoEbottom");

    nJet_			 = data.GetInt(		"nJet"				);
    jetPt_			 = data.GetPtrFloat(	"jetPt"				);
    jetEn_			 = data.GetPtrFloat(	"jetEn"				);
    jetEta_			 = data.GetPtrFloat(	"jetEta"			);
    jetPhi_			 = data.GetPtrFloat(	"jetPhi"			);
    jetPUID_			 = data.GetPtrFloat(	"jetPUID"			);
    jetID_			 = data.GetPtrInt(	"jetID"				);
    
    nOOTPho_			 = data.GetInt(		"nOOTPho"			);
    ootPhoEt_			 = data.GetPtrFloat(	"ootPhoEt"			);
    ootPhoEta_			 = data.GetPtrFloat(	"ootPhoEta"			);
    ootPhoPhi_			 = data.GetPtrFloat(	"ootPhoPhi"			);
    ootPhoSCEta_		 = data.GetPtrFloat(	"ootPhoSCEta"			);
    ootPhohasPixelSeed_ = data.GetPtrInt("ootPhohasPixelSeed");
    ootPhoHoverE_		 = data.GetPtrFloat(	"ootPhoHoverE"			);
    ootPhoSigmaIEtaIEtaFull5x5_  = data.GetPtrFloat(	"ootPhoSigmaIEtaIEtaFull5x5"	);
    ootPhoR9Full5x5_		 = data.GetPtrFloat(	"ootPhoR9Full5x5"		);
    ootPhoSeedTime_              = data.GetPtrFloat(	"ootPhoSeedTime"		);
    ootPhoSeedEnergy_              = data.GetPtrFloat(	"ootPhoSeedEnergy"		);
    ootPhoMIPTotEnergy_          = data.GetPtrFloat(	"ootPhoMIPTotEnergy"		);
    ootPhoEtop_ = data.GetPtrFloat("ootPhoEtop");
    ootPhoEleft_ = data.GetPtrFloat("ootPhoEleft");
    ootPhoEright_ = data.GetPtrFloat("ootPhoEright");
    ootPhoEbottom_ = data.GetPtrFloat("ootPhoEbottom");
    //cout<< "get all variables" << endl;
    
    //init output variables
    pfMET	 = 0;
    pfMETPhi	 = 0;
    puppiMET	 = 0;
    puppiMETPhi	 = 0;
    corrMET	 = 0;
    corrMETPhi	 = 0;
    metFilters	 = 0;

    nPho	 = 0;
    phoEt.clear();
    phoEta.clear();
    phoPhi.clear();
    phoSCEta.clear();
    phohasPixelSeed.clear();
    phoHoverE.clear();
    phoSigmaIEtaIEtaFull5x5.clear();
    phoR9Full5x5.clear();
    phoSeedTime.clear();
    phoSeedEnergy.clear();
    phoMIPTotEnergy.clear();
    phoEtop.clear();
    phoEleft.clear();
    phoEright.clear();
    phoEbottom.clear();
    phoE4.clear();
    phoE4overE1.clear();
    phoEtaWing.clear();
    phoEtaWingoverE1.clear();

    nJet	 = 0;
    npfjet	 = 0;
    nSMjet	 = 0;
    nVBSjet	 = 0;
    nonPUjetid.clear();
    jetPt.clear();
    jetEn.clear();
    jetEta.clear();
    jetPhi.clear();
    dijetMass	 = 0.0;
    jetPUIDMVA.clear();
    jetPUID.clear();
    jetID.clear();
    jetTID.clear();
    minJMETdPhi	 = 0;
    JMETdPhi.clear();
    jetjetdR	 = 0;
    jetjetdEta	 = 0;
    jetjetdPhi	 = 0;
    phojetdR.clear();
    phojetdEta.clear();
    phojetdPhi.clear();
    phoMETdPhi.clear();
    
    nOOTPho	 = 0;
    ootPhoEt.clear();
    ootPhoEta.clear();
    ootPhoPhi.clear();
    ootPhoSCEta.clear();
    ootPhohasPixelSeed.clear();
    ootPhoHoverE.clear();
    ootPhoSigmaIEtaIEtaFull5x5.clear();
    ootPhoR9Full5x5.clear();
    ootPhoSeedTime.clear();
    ootPhoSeedEnergy.clear();
    ootPhoMIPTotEnergy.clear();
    ootPhoEtop.clear();
    ootPhoEleft.clear();
    ootPhoEright.clear();
    ootPhoEbottom.clear();
    ootPhoE4.clear();
    ootPhoE4overE1.clear();
    ootPhoEtaWing.clear();
    ootPhoEtaWingoverE1.clear();

    nINCPho = 0;
    isOOTPho.clear();
    incPhoEt.clear();
    incPhoEta.clear();
    incPhoPhi.clear();
    incPhoSieie.clear();
    incPhoHoverE.clear();
    incPhoR9.clear();
    incPhoMIP.clear();
    incPhoSeedTime.clear();
    incPhoSeedEn.clear();
    incPhoHasPixelSeed.clear();
    incPhoE4overE1.clear();
    incPhoEtaWingoverE1.clear();
    // npOOTPho = 0;
    // pOOTPhoEt.clear();
    // pOOTPhoEta.clear();
    // pOOTPhoPhi.clear();
    // pOOTPhoSieie.clear();
    // pOOTPhoHoverE.clear();
    // pOOTPhoR9.clear();
    // pOOTPhoMIP.clear();
    // pOOTPhoSeedTime.clear();
    // pOOTPhoSeedEn.clear();
    // pOOTPhoHasPixelSeed.clear();
    // pOOTPhoE4overE1.clear();
    // pOOTPhoEtaWingoverE1.clear();
    
    pfMET	 = pfMET_;
    pfMETPhi	 = pfMETPhi_;
    puppiMET	 = puppiMET_;
    puppiMETPhi	 = puppiMETPhi_;
    metFilters   = metFilters_;

    ////
    // Build selection variables
    ////

    nPho = nPho_;
    for(Int_t ipho=0; ipho<nPho_; ipho++){
      //if(year == 2016 && (phoFiredSingleTrgs_[ipho]>>7&1) == 0) continue;
      //else if(year == 2017 && (phoFiredSingleTrgs_[ipho]>>8&1) == 0) continue;
      //else if(year == 2018 && (phoFiredSingleTrgs_[ipho]>>7&1) == 0) continue;
     
      phoEt.push_back(phoEt_[ipho]);
      phoEta.push_back(phoEta_[ipho]);
      phoPhi.push_back(phoPhi_[ipho]);
      phoSCEta.push_back(phoSCEta_[ipho]);
      phohasPixelSeed.push_back(phohasPixelSeed_[ipho]);
      phoHoverE.push_back(phoHoverE_[ipho]);
      phoSigmaIEtaIEtaFull5x5.push_back(phoSigmaIEtaIEtaFull5x5_[ipho]);
      phoR9Full5x5.push_back(phoR9Full5x5_[ipho]);
      phoSeedTime.push_back(phoSeedTime_[ipho]);
      phoSeedEnergy.push_back(phoSeedEnergy_[ipho]);
      phoMIPTotEnergy.push_back(phoMIPTotEnergy_[ipho]);
      phoEtop.push_back(phoEtop_[ipho]);
      phoEleft.push_back(phoEleft_[ipho]);
      phoEright.push_back(phoEright_[ipho]);
      phoEbottom.push_back(phoEbottom_[ipho]);
      
      Float_t sumphoE4 = 0;
      sumphoE4 = phoEtop_[ipho]+phoEleft_[ipho]+phoEright_[ipho]+phoEbottom_[ipho];
      phoE4.push_back(sumphoE4);

      Float_t E4overE = 0;
      E4overE = sumphoE4/phoSeedEnergy_[ipho];
      phoE4overE1.push_back(E4overE);
      
      Float_t compWing = 0;
      if(phoEleft_[ipho] > phoEright_[ipho]) compWing = phoEleft_[ipho];
      else compWing = phoEright_[ipho];
      phoEtaWing.push_back(compWing);
      
      Float_t WingoverE = 0;
      WingoverE = compWing/phoSeedEnergy_[ipho];
      phoEtaWingoverE1.push_back(WingoverE);
    }

    nOOTPho = nOOTPho_;
    for(Int_t i=0; i<nOOTPho_; i++){
      ootPhoEt.push_back(ootPhoEt_[i]);
      ootPhoEta.push_back(ootPhoEta_[i]);
      ootPhoPhi.push_back(ootPhoPhi_[i]);
      ootPhoSCEta.push_back(ootPhoSCEta_[i]);
      ootPhohasPixelSeed.push_back(ootPhohasPixelSeed_[i]);
      ootPhoHoverE.push_back(ootPhoHoverE_[i]);
      ootPhoSigmaIEtaIEtaFull5x5.push_back(ootPhoSigmaIEtaIEtaFull5x5_[i]);
      ootPhoR9Full5x5.push_back(ootPhoR9Full5x5_[i]);
      ootPhoSeedTime.push_back(ootPhoSeedTime_[i]);
      ootPhoSeedEnergy.push_back(ootPhoSeedEnergy_[i]);
      ootPhoMIPTotEnergy.push_back(ootPhoMIPTotEnergy_[i]);
      ootPhoEtop.push_back(ootPhoEtop_[i]);
      ootPhoEleft.push_back(ootPhoEleft_[i]);
      ootPhoEright.push_back(ootPhoEright_[i]);
      ootPhoEbottom.push_back(ootPhoEbottom_[i]);
      
      Float_t sumphoE4 = 0;
      sumphoE4 = ootPhoEtop_[i]+ootPhoEleft_[i]+ootPhoEright_[i]+ootPhoEbottom_[i];
      ootPhoE4.push_back(sumphoE4);

      Float_t E4overE = 0;
      E4overE = sumphoE4/ootPhoSeedEnergy_[i];
      ootPhoE4overE1.push_back(E4overE);
      
      Float_t compWing = 0;
      if(ootPhoEleft_[i] >= ootPhoEright_[i]) compWing = ootPhoEleft_[i];
      else compWing = ootPhoEright_[i];
      ootPhoEtaWing.push_back(compWing);
      
      Float_t WingoverE = 0;
      WingoverE = compWing/ootPhoSeedEnergy_[i];
      ootPhoEtaWingoverE1.push_back(WingoverE);

    }

    if(nOOTPho==0){
      for(Int_t ipho=0; ipho<nPho; ipho++){
	nINCPho++;
	
	isOOTPho.push_back(0);
	incPhoEt.push_back(phoEt[ipho]);
	incPhoEta.push_back(phoSCEta[ipho]);
	incPhoPhi.push_back(phoPhi[ipho]);
	incPhoSieie.push_back(phoSigmaIEtaIEtaFull5x5[ipho]);
	incPhoHoverE.push_back(phoHoverE[ipho]);
	incPhoR9.push_back(phoR9Full5x5[ipho]);
	incPhoMIP.push_back(phoMIPTotEnergy[ipho]);
	incPhoSeedTime.push_back(phoSeedTime[ipho]);
	incPhoSeedEn.push_back(phoSeedEnergy[ipho]);
	incPhoHasPixelSeed.push_back(phohasPixelSeed[ipho]);
	incPhoE4overE1.push_back(phoE4overE1[ipho]);
	incPhoEtaWingoverE1.push_back(phoEtaWingoverE1[ipho]);
      }
      corrMET = puppiMET;
      corrMETPhi = puppiMETPhi;
    }
    else if(nOOTPho>0){
      TVector2 METv2, phov2, ootphov2, sumphov2, sumootphov2;
      TVector2 corrMETv2;
      METv2.SetMagPhi(puppiMET, puppiMETPhi);
      
      for(Int_t ipho=0; ipho<nPho; ipho++){
	phov2.SetMagPhi(phoEt[ipho], phoPhi[ipho]);
	sumphov2 += phov2;
      }
      for(Int_t opho=0; opho<nOOTPho; opho++){
	ootphov2.SetMagPhi(ootPhoEt[opho], ootPhoPhi[opho]);
	sumootphov2 += ootphov2;
	
	nINCPho++;
	
	isOOTPho.push_back(1);
	incPhoEt.push_back(ootPhoEt[opho]);
	incPhoEta.push_back(ootPhoSCEta[opho]);
	incPhoPhi.push_back(ootPhoPhi[opho]);
	incPhoSieie.push_back(ootPhoSigmaIEtaIEtaFull5x5[opho]);
	incPhoHoverE.push_back(ootPhoHoverE[opho]);
	incPhoR9.push_back(ootPhoR9Full5x5[opho]);
	incPhoMIP.push_back(ootPhoMIPTotEnergy[opho]);
	incPhoSeedTime.push_back(ootPhoSeedTime[opho]);
	incPhoSeedEn.push_back(ootPhoSeedEnergy[opho]);
	incPhoHasPixelSeed.push_back(ootPhohasPixelSeed[opho]);
	incPhoE4overE1.push_back(ootPhoE4overE1[opho]);
	incPhoEtaWingoverE1.push_back(ootPhoEtaWingoverE1[opho]);

	// npOOTPho++;
	// pOOTPhoEt.push_back(ootPhoEt[opho]);
	// pOOTPhoEta.push_back(ootPhoSCEta[opho]);
	// pOOTPhoPhi.push_back(ootPhoPhi[opho]);
	// pOOTPhoSieie.push_back(ootPhoSigmaIEtaIEtaFull5x5[opho]);
	// pOOTPhoHoverE.push_back(ootPhoHoverE[opho]);
	// pOOTPhoR9.push_back(ootPhoR9Full5x5[opho]);
	// pOOTPhoMIP.push_back(ootPhoMIPTotEnergy[opho]);
	// pOOTPhoSeedTime.push_back(ootPhoSeedTime[opho]);
	// pOOTPhoSeedEn.push_back(ootPhoSeedEnergy[opho]);
	// pOOTPhoHasPixelSeed.push_back(ootPhohasPixelSeed[opho]);
	// pOOTPhoE4overE1.push_back(ootPhoE4overE1[opho]);
	// pOOTPhoEtaWingoverE1.push_back(ootPhoEtaWingoverE1[opho]);
      }

      for(Int_t ipho=0; ipho<nPho; ipho++){
	if(ipho==0){
	  if(phoEt[ipho]>=incPhoEt[0]){
	    nINCPho++;

	    isOOTPho.insert(isOOTPho.begin(), 0);
	    incPhoEt.insert(incPhoEt.begin(), phoEt[ipho]);
	    incPhoEta.insert(incPhoEta.begin(), phoSCEta[ipho]);
	    incPhoPhi.insert(incPhoPhi.begin(), phoPhi[ipho]);
	    incPhoSieie.insert(incPhoSieie.begin(), phoSigmaIEtaIEtaFull5x5[ipho]);
	    incPhoHoverE.insert(incPhoHoverE.begin(), phoHoverE[ipho]);
	    incPhoR9.insert(incPhoR9.begin(), phoR9Full5x5[ipho]);
	    incPhoMIP.insert(incPhoMIP.begin(), phoMIPTotEnergy[ipho]);
	    incPhoSeedTime.insert(incPhoSeedTime.begin(), phoSeedTime[ipho]);
	    incPhoSeedEn.insert(incPhoSeedEn.begin(), phoSeedEnergy[ipho]);
	    incPhoHasPixelSeed.insert(incPhoHasPixelSeed.begin(), phohasPixelSeed[ipho]);
	    incPhoE4overE1.insert(incPhoE4overE1.begin(), phoE4overE1[ipho]);
	    incPhoEtaWingoverE1.insert(incPhoEtaWingoverE1.begin(), phoEtaWingoverE1[ipho]);
	  }
	  else{
	    nINCPho++;
	  
	    isOOTPho.insert(isOOTPho.begin()+1, 0);
	    incPhoEt.insert(incPhoEt.begin()+1, phoEt[ipho]);
	    incPhoEta.insert(incPhoEta.begin()+1, phoSCEta[ipho]);
	    incPhoPhi.insert(incPhoPhi.begin()+1, phoPhi[ipho]);
	    incPhoSieie.insert(incPhoSieie.begin()+1, phoSigmaIEtaIEtaFull5x5[ipho]);
	    incPhoHoverE.insert(incPhoHoverE.begin()+1, phoHoverE[ipho]);
	    incPhoR9.insert(incPhoR9.begin()+1, phoR9Full5x5[ipho]);
	    incPhoMIP.insert(incPhoMIP.begin()+1, phoMIPTotEnergy[ipho]);
	    incPhoSeedTime.insert(incPhoSeedTime.begin()+1, phoSeedTime[ipho]);
	    incPhoSeedEn.insert(incPhoSeedEn.begin()+1, phoSeedEnergy[ipho]);
	    incPhoHasPixelSeed.insert(incPhoHasPixelSeed.begin()+1, phohasPixelSeed[ipho]);
	    incPhoE4overE1.insert(incPhoE4overE1.begin()+1, phoE4overE1[ipho]);
	    incPhoEtaWingoverE1.insert(incPhoEtaWingoverE1.begin()+1, phoEtaWingoverE1[ipho]);
	  }
	}
	else{
	  nINCPho++;
	
	  isOOTPho.push_back(0);
	  incPhoEt.push_back(phoEt[ipho]);
	  incPhoEta.push_back(phoSCEta[ipho]);
	  incPhoPhi.push_back(phoPhi[ipho]);
	  incPhoSieie.push_back(phoSigmaIEtaIEtaFull5x5[ipho]);
	  incPhoHoverE.push_back(phoHoverE[ipho]);
	  incPhoR9.push_back(phoR9Full5x5[ipho]);
	  incPhoMIP.push_back(phoMIPTotEnergy[ipho]);
	  incPhoSeedTime.push_back(phoSeedTime[ipho]);
	  incPhoSeedEn.push_back(phoSeedEnergy[ipho]);
	  incPhoHasPixelSeed.push_back(phohasPixelSeed[ipho]);
	  incPhoE4overE1.push_back(phoE4overE1[ipho]);
	  incPhoEtaWingoverE1.push_back(phoEtaWingoverE1[ipho]);
	}
      }
      corrMETv2 = METv2 + sumphov2 - sumootphov2;
      corrMET = corrMETv2.Mod();
      corrMETPhi = corrMETv2.Phi();

      h_pfMET->Fill(pfMET);
      h_puppiMET->Fill(puppiMET);
      h_corrMET->Fill(corrMET);
    }
    //cout<< "merge photon collection" << endl;

    //jetID selection TLepVeto
    jet_ULTIDsel(year, data, jetTID);
    jet_ULPUIDsel(year, data, jetPUIDMVA);

    //count # jet
    for(Int_t i=0; i<nJet_; i++){
      if((jetPUIDMVA[i] == 1) && (jetPt_[i] > 30) && fabs(jetEta_[i]) < 4.7 && (jetTID[i] == 1)){// UL data with PU
	nonPUjetid.push_back(i);
	npfjet++;
      }
    }
    //cout<< "get # jets" << endl;

    //dijet stuff
    Int_t bit=0; Int_t fixbit=0;
    TLorentzVector  leadjetP4, subjetP4, dijetP4;
    if(npfjet >= 2){
      Int_t ilead = nonPUjetid[0];
      Int_t isub = nonPUjetid[1];

      //mindjetMETPhi
      if(fabs(deltaPhi(jetPhi_[ilead], corrMETPhi)) < fabs(deltaPhi(jetPhi_[isub], corrMETPhi))) minJMETdPhi = deltaPhi(jetPhi_[ilead], corrMETPhi);
      else if(fabs(deltaPhi(jetPhi_[ilead], corrMETPhi)) > fabs(deltaPhi(jetPhi_[isub], corrMETPhi))) minJMETdPhi = deltaPhi(jetPhi_[isub], corrMETPhi);

      //dijet dEta & dPhi & dR
      jetjetdEta = deltaEta(jetEta_[ilead], jetEta_[isub]);
      jetjetdPhi = deltaPhi(jetPhi_[ilead], jetPhi_[isub]);
      jetjetdR = deltaR(jetEta_[ilead], jetPhi_[ilead], jetEta_[isub], jetPhi_[isub]);

      //dijet Mass
      leadjetP4.SetPtEtaPhiE(jetPt_[ilead], jetEta_[ilead], jetPhi_[ilead], jetEn_[ilead]);
      subjetP4.SetPtEtaPhiE(jetPt_[isub], jetEta_[isub], jetPhi_[isub], jetEn_[isub]);
      dijetP4 = leadjetP4 + subjetP4;
      dijetMass = dijetP4.M();

      //phojet dEta & dPhi & dR
      for(Int_t ipho=0; ipho<nINCPho; ipho++){
	if(ipho>0) continue;
	phojetdR.push_back(deltaR(incPhoEta[ipho], incPhoPhi[ipho], jetEta_[ilead], jetPhi_[ilead]));
	phojetdR.push_back(deltaR(incPhoEta[ipho], incPhoPhi[ipho], jetEta_[isub], jetPhi_[isub]));

	phojetdEta.push_back(deltaEta(incPhoEta[ipho], jetEta_[ilead]));
	phojetdEta.push_back(deltaEta(incPhoEta[ipho], jetEta_[isub]));

	phojetdPhi.push_back(deltaPhi(incPhoPhi[ipho], jetPhi_[ilead]));
	phojetdPhi.push_back(deltaPhi(incPhoPhi[ipho], jetPhi_[isub]));
      }

      //jetMET dPhi
      JMETdPhi.push_back(deltaPhi(jetPhi_[ilead], corrMETPhi));
      JMETdPhi.push_back(deltaPhi(jetPhi_[isub], corrMETPhi));
    }
    else{
      minJMETdPhi = -999;
      
      jetjetdEta = -999;
      jetjetdPhi = -999;
      jetjetdR = -999;
      
      for(Int_t i=0; i<2; i++){
	//if(ipho>0) continue;
	phojetdR.push_back(-999);
	phojetdEta.push_back(-999);
	phojetdPhi.push_back(-999);
      }
      
      JMETdPhi.push_back(-999);
	
    }
    //cout<< "get jet variables" << endl;

    //phoMET dPhi
    for(Int_t ipho=0; ipho<nINCPho; ipho++){
      if(ipho > 0) continue;
      phoMETdPhi.push_back(deltaPhi(incPhoPhi[ipho], corrMETPhi));
    }

    ////
    // Fill Templates
    ////

    Int_t fill;
    // Candidates
    fill=0;
    for(Int_t ipho=0; ipho<nINCPho; ipho++){
      if(abs(incPhoEta[ipho])<1.4442 && incPhoR9[ipho]>0.8  && incPhoEt[ipho]>20 && incPhoHoverE[ipho]<0.2){
	if(isOOTPho[ipho]==0){
	  if(incPhoMIP[ipho]<4.9 && incPhoSieie[ipho]>0.001 && incPhoSieie[ipho]<0.01015 && metFilters==0 && incPhoEt[ipho]>75 && corrMET>180 && npfjet>=2 && jetjetdEta > 2.5 && fabs(minJMETdPhi) > 0.5 && dijetMass>250){
	    h_Cand_SeedTime->Fill(incPhoSeedTime[ipho]);
	    h_Cand_phoEt->Fill(incPhoEt[ipho]);
	    if(fill == 0) h_Cand_MET->Fill(corrMET);
	    h_Cand_sieie->Fill(incPhoSieie[ipho]);
	    h_Cand_MIP->Fill(incPhoMIP[ipho]);
	    fill++;
	  }
	}
	else{
	  if(incPhoMIP[ipho]<4.9 && incPhoSieie[ipho]>0.001 && incPhoSieie[ipho]<0.01015 && metFilters==0 && corrMET>180 && npfjet>=2 && jetjetdEta > 2.5 && fabs(minJMETdPhi) > 0.5 && dijetMass > 250){
	    //if(incPhoMIP[ipho]<4.9 && incPhoSieie[ipho]>0.001 && incPhoSieie[ipho]<0.01015 && metFilters==0 && npfjet>=2 && jetjetdEta > 2.5){
	    h_Cand_SeedTime->Fill(incPhoSeedTime[ipho]);
	    h_Cand_phoEt->Fill(incPhoEt[ipho]);
	    if(fill == 0) h_Cand_MET->Fill(corrMET);
	    h_Cand_sieie->Fill(incPhoSieie[ipho]);
	    h_Cand_MIP->Fill(incPhoMIP[ipho]);
	    fill++;
	  }
	}
      }
    }

    // Prompt
    fill = 0;
    for(Int_t ipho=0; ipho<nINCPho; ipho++){
      if(abs(incPhoEta[ipho])<1.4442 && incPhoR9[ipho]>0.8 && incPhoEt[ipho]>75 && incPhoHoverE[ipho]<0.2){
	if(incPhoMIP[ipho]<4.9 && incPhoHasPixelSeed[ipho]==1 && incPhoSieie[ipho]<0.01015 && metFilters==0 && corrMET>180 && npfjet>=2 && jetjetdEta > 2.5 && fabs(minJMETdPhi) > 0.5 && dijetMass>250){
	  //if(incPhoMIP[ipho]<4.9 && incPhoHasPixelSeed[ipho]==1 && incPhoSieie[ipho]<0.01015 && metFilters==0 && puppiMET>30){
	  h_prompt_SeedTime->Fill(incPhoSeedTime[ipho]);
	  h_prompt_phoEt->Fill(incPhoEt[ipho]);
	  if(fill==0) h_prompt_MET->Fill(corrMET);
	  h_prompt_sieie->Fill(incPhoSieie[ipho]);
	  h_prompt_MIP->Fill(incPhoMIP[ipho]);
	  fill++;
	}
      }
      //if(fill>0) break;
    }

    // Beam halo bkg
    fill=0;
    for(Int_t ipho=0; ipho<nINCPho; ipho++){
      if(abs(incPhoEta[ipho])<1.4442 && incPhoR9[ipho]>0.8 && incPhoEt[ipho]>20 && incPhoHoverE[ipho]<0.2){
	if(incPhoMIP[ipho]>4.9 && incPhoSieie[ipho]>0.001 && metFilters==8 && incPhoSieie[ipho] <0.03){
	  h_ootBH_SeedTime->Fill(incPhoSeedTime[ipho]);
	  h_ootBH_phoEt->Fill(incPhoEt[ipho]);
	  if(fill==0) h_ootBH_MET->Fill(puppiMET);
	  h_ootBH_sieie->Fill(incPhoSieie[ipho]);
	  h_ootBH_MIP->Fill(incPhoMIP[ipho]);
	  fill++;
	}
      }
    }

    // Spike bkg
    fill = 0;
    for(Int_t ipho=0; ipho<nINCPho; ipho++){
      if(abs(incPhoEta[ipho])<1.4442 && incPhoR9[ipho]>0.8 && incPhoEt[ipho]>20 && incPhoHoverE[ipho]<0.2){
	//if(incPhoMIP[ipho]<4.9  && metFilters==0 && incPhoSieie[ipho]<0.001){ // Sieie method
	if(incPhoMIP[ipho]<4.9 && metFilters==0 && incPhoE4overE1[ipho]<0.05){ // Swiss cross method
	  h_ootSP_SeedTime->Fill(incPhoSeedTime[ipho]);
	  h_ootSP_phoEt->Fill(incPhoEt[ipho]);
	  if(fill==0) h_ootSP_MET->Fill(puppiMET);
	  h_ootSP_sieie->Fill(incPhoSieie[ipho]);
	  h_ootSP_MIP->Fill(incPhoMIP[ipho]);
	  fill++;
	}
      }
    }    
    
  }
  //****************END LOOP**********************//

  //****************Terminate*********************//
  fout_->cd();

  h_pfMET->Write();
  h_puppiMET->Write();
  h_corrMET->Write();

  h_Cand_SeedTime->Write();
  h_Cand_phoEt->Write();
  h_Cand_MET->Write();
  h_Cand_sieie->Write();
  h_Cand_MIP->Write();
  
  h_prompt_SeedTime->Write();
  h_prompt_phoEt->Write();
  h_prompt_MET->Write();
  h_prompt_sieie->Write();
  h_prompt_MIP->Write();

  h_ootBH_SeedTime->Write();
  h_ootBH_phoEt->Write();
  h_ootBH_MET->Write();
  h_ootBH_sieie->Write();
  h_ootBH_MIP->Write();

  h_ootSP_SeedTime->Write();
  h_ootSP_phoEt->Write();
  h_ootSP_MET->Write();
  h_ootSP_sieie->Write();
  h_ootSP_MIP->Write();

  fout_->Close();
  fprintf(stderr, "%s Processed all events\n", treename.c_str());
  
}
