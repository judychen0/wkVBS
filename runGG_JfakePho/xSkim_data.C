#include <iostream>
using namespace std;
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TVector2.h"

#include "untuplizer.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "IsoCorrection.h"
#include "JetSelection.h"
#include "PhotonSelection_Data.h"


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
  Double_t dEta, dPhi;
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
  return (bit & ~(1<<nbit));
}

Float_t Maximum(Float_t compare, Float_t input){
  if(compare >= input) return compare;
  else return input;
}

bool getBit(int n, int index) {
    return ((n & (1 << index)) > 0);
}

void xSkim_data(char* pathes, Int_t year, Int_t isMET){

  // Get processed events
  TFile *fopen = new TFile(pathes, "READ");
  TH1F *hEvents = (TH1F*)fopen->Get("ggNtuplizer/hEvents")->Clone();
  hEvents->SetDirectory(0);
  fopen->Close();

  // access EventTree
  TreeReader data(pathes);
 
  // Creat Output skimmed tree
  TFile *fout_;
  char foutname[50];
  string treestr = pathes;
  Int_t istr = treestr.find("ggtree");
  string treename = treestr.substr(istr);
  sprintf(foutname, "Jfake_%s", treename.c_str());
  fout_ = new TFile(foutname,"RECREATE");

  ////
  //input variables
  ////

  // global event
  Int_t         run_;
  Long64_t      event_;
  Bool_t        isData_;
  Int_t         nGoodVtx_;
  Bool_t        isPVGood_;
  Float_t       rho_;
  Long64_t	HLTJet_;	// MET trigger HLT_PFMET120_PFMHT120_IDTight_v

  // MET
  Float_t       pfMET_;
  Float_t       pfMETPhi_;
  Float_t	puppiMET_;
  Float_t	puppiMETPhi_;
  Int_t         metFilters_;
  
// photon
  Int_t         nPho_;
  Float_t*      phoE_;
  Float_t*      phoEt_;
  Float_t*      phoEta_;
  Float_t*      phoPhi_;
  Float_t*      phoSCEta_;
  Int_t*        phohasPixelSeed_;
  Int_t*        phoEleVeto_;
  Float_t*      phoHoverE_;
  Float_t*      phoSigmaIEtaIEtaFull5x5_;
  Float_t*      phoR9Full5x5_;
  Float_t*      phoPFChIso_;
  Float_t*      phoPFPhoIso_;
  Float_t*      phoPFNeuIso_;
  Float_t*      phoPFChWorstIso_;
  Long64_t*     phoFiredSingleTrgs_;
  Float_t*      phoSeedTime_;
  Float_t*      phoSeedEnergy_;
  Float_t*      phoMIPTotEnergy_;
  Short_t*      phoIDbit_;
  Float_t*	phoEtop_;
  Float_t*	phoEleft_;
  Float_t*	phoEright_;
  Float_t*	phoEbottom_;

  // electron
  Int_t         nEle_;
  Float_t*      elePt_;
  Float_t*      eleEta_;
  Float_t*      eleD0_;
  Float_t*      eleDz_;
  Short_t*      eleIDbit_;
    
  // muon
  Int_t         nMu_;
  Float_t*      muPt_;
  Float_t*      muEta_;
  Int_t*        muIDbit_;
  Float_t*      muD0_;
  Float_t*      muDz_;

  // jets
  Int_t         nJet_;
  Float_t*      jetPt_;
  Float_t*      jetEn_;
  Float_t*      jetEta_;
  Float_t*      jetPhi_;
  
  ////
  // output branch variables
  ////
  
  // global event
  Long64_t      event;
  Bool_t        isData;
  Int_t         nGoodVtx;
  Bool_t        isPVGood;
  Float_t       rho;
  Float_t       EventWeight;
  Long64_t	HLTJet;		// MET trigger HLT_PFMET120_PFMHT120_IDTight_v
  Int_t METTrgs;
  Int_t PhoTrgs;
  
  // MET
  Float_t               pfMET;
  Float_t               pfMETPhi;
  Float_t               puppiMET;
  Float_t               puppiMETPhi;
  Int_t                 metFilters;
// photon
  Int_t                 nPho;	//HLT pho
  vector<Float_t>       phoE;
  vector<Float_t>       phoEt;
  vector<Float_t>       phoEta;
  vector<Float_t>       phoPhi;
  vector<Float_t>       phoSCEta;
  vector<Int_t>         phohasPixelSeed;
  vector<Int_t>         phoEleVeto;
  vector<Float_t>       phoHoverE;
  vector<Float_t>       phoSigmaIEtaIEtaFull5x5;
  vector<Float_t>       phoR9Full5x5;
  vector<Float_t>       phoPFChIso;
  vector<Float_t>       phoPFPhoIso;
  vector<Float_t>       phoPFNeuIso;
  vector<Float_t>       phoPFChWorstIso;
  vector<Float_t>       phoPFChIso_rc;
  vector<Float_t>       phoPFPhoIso_rc;
  vector<Float_t>       phoPFNeuIso_rc;
  vector<Float_t>       phoPFChWorstIso_rc;
  vector<Float_t>       phoPFChWorstIso_newEA;
  vector<Long64_t>      phoFiredSingleTrgs;
  vector<Float_t>       phoSeedTime;
  vector<Float_t>       phoSeedEnergy;
  vector<Float_t>       phoMIPTotEnergy;
  vector<Float_t>       phoMETdPhi;
  vector<Short_t>       phoIDbit;
  vector<Int_t>         MphoID;
  vector<Float_t>	phoEtop;
  vector<Float_t>	phoEleft;
  vector<Float_t>	phoEright;
  vector<Float_t>	phoEbottom;
  vector<Float_t>	phoE4;
  vector<Float_t>	phoE4overE1;
  vector<Float_t>	phoEtaWing;
  vector<Float_t>	phoEtaWingoverE1;
  Float_t		phoCentral;	//photon centrality

  vector<Int_t> passChIso;
  vector<Int_t> passNhIso;
  vector<Int_t> passPhoIso;
  vector<Int_t> passHoverE;
  vector<Int_t> passSieie;
  vector<Int_t> passLChIso;
  vector<Int_t> passLNhIso;
  vector<Int_t> passLPhoIso;

  // selection ID
  vector<Int_t> CandPhoID;
  vector<Int_t> FakePhoID;

  // electron
  Int_t                 nEle;
  vector<Float_t>       elePt;
  vector<Float_t>       eleEta;
  vector<Float_t>       eleD0;
  vector<Float_t>       eleDz;
  vector<Short_t>       eleIDbit;

  // muon
  Int_t                 nMu;
  vector<Float_t>       muPt;
  vector<Float_t>       muEta;
  vector<Int_t>         muIDbit;
  vector<Float_t>       muD0;
  vector<Float_t>       muDz;
  Int_t                 nLep;

  // jets
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
  vector<Int_t>         jetTID;
  Float_t		jet3Central;	// 3rd leading jet centrality
  
  // angular variables
  Float_t               minJMETdPhi;
  vector<Float_t>       JMETdPhi;
  Float_t               jetjetdR;
  Float_t               jetjetdEta;
  Float_t               jetjetdPhi;
  vector<Float_t>       phojetdR;
  vector<Float_t>       phojetdEta;
  vector<Float_t>       phojetdPhi;
  Float_t		jetjetZdPhi;
  Float_t		jetjetphodPhi;
  Float_t               ZgsysPhi;
  Float_t               jetjetsysPhi;
    
  //[iWP L/M/T][region EB/EE]fall17
  Float_t chIso_WP[3][2] = {{1.694, 2.089},{1.141, 1.051}, {0.65, 0.517}};
  Float_t phoIso_WP[3][2] = {{2.876, 4.162}, {2.08, 3.867}, {2.044, 3.032}};
  Float_t nhIso_WP[3][2] = {{24.032, 19.722}, {1.189, 2.718}, {0.317, 2.716}};
  Float_t HoverE_WP[3][2] = {{0.04596, 0.059}, {0.02197, 0.0326}, {0.02148, 0.0321}};
  Float_t sieie_WP[3][2] = {{0.0106, 0.0272}, {0.01015, 0.0272}, {0.00996, 0.0271}};

  // histograms
  Double_t ptbin[20] = {75, 110, 210, 2000};//3 bins, 2016
  Int_t Nbin = 3;
  
  TH1F* hbin_FakePho_sieie[Nbin];
  TH1F* hbin_FakePho_sieie_chIsobin[Nbin][4];
  TH1F* hbin_CandPho_sieie[Nbin];
  TH1F* hbin_CandPho_dijetMass[Nbin];
  TH1F* hbin_CandPho_dijetMass_cutsieie[Nbin];
    
  Double_t sieiebin[20] = {0.005, 0.0075, 0.008, 0.0085, 0.009, 0.0095, 0.01015, 0.0113, 0.0125, 0.015, 0.0175, 0.02}; // 11 bins
  Double_t massbin[20] = {250, 500, 1000, 2000, 5000};//4bins
  
  for(Int_t jj=0; jj<Nbin; jj++){
    hbin_FakePho_sieie[jj] = new TH1F(Form("hbin_FakePho_sieie_%i", jj+1), "", 11, sieiebin);
    hbin_CandPho_sieie[jj] = new TH1F(Form("hbin_CandPho_sieie_%i", jj+1), "", 11, sieiebin);
    hbin_FakePho_sieie[jj]->Sumw2();
    hbin_CandPho_sieie[jj]->Sumw2();
    
    hbin_CandPho_dijetMass[jj] = new TH1F(Form("hbin_CandPho_dijetMass_%i", jj+1), "", 4, massbin);
    hbin_CandPho_dijetMass_cutsieie[jj] = new TH1F(Form("hbin_CandPho_dijetMass_cutsieie_%i", jj+1), "", 4, massbin);
    hbin_CandPho_dijetMass[jj]->Sumw2();
    hbin_CandPho_dijetMass_cutsieie[jj]->Sumw2();

    for(Int_t iIsobin=0; iIsobin<4; iIsobin++){
      hbin_FakePho_sieie_chIsobin[jj][iIsobin] = new TH1F(Form("hbin_FakePho_sieie_chIsobin%i_%i", iIsobin+1, jj+1), "", 11, sieiebin);
      hbin_FakePho_sieie_chIsobin[jj][iIsobin]->Sumw2();
    }
  }

  TH2F* h2_sieie_chIso = new TH2F("h2_sieie_chIso", "", 150, 0, 0.03, 200, 0, 20);
  h2_sieie_chIso->Sumw2();

  //**************************************************//
  //***********************Loop***********************//
  //**************************************************//

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    //cout << "get ev " << ev << endl;
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    //get data from tree
    data.GetEntry(ev);
    
    run_			= data.GetInt(		"run"				);
    event_			= data.GetLong64(	"event"				);
    isData_			= data.GetBool(		"isData"			);
    nGoodVtx_			= data.GetInt(		"nGoodVtx"			);
    isPVGood_			= data.GetBool(		"isPVGood"			);
    rho_			= data.GetFloat(	"rho"				);
    HLTJet_ = data.GetLong64("HLTJet");
    pfMET_			= data.GetFloat(	"pfMET"				);
    pfMETPhi_			= data.GetFloat(	"pfMETPhi"			);
    puppiMET_			= data.GetFloat(	"puppiMET"			);
    puppiMETPhi_		= data.GetFloat(	"puppiMETPhi"			);
    metFilters_			= data.GetInt(		"metFilters"			);
    nPho_			= data.GetInt(		"nPho"				);
    phoE_			= data.GetPtrFloat(	"phoE"				);
    phoEt_			= data.GetPtrFloat(	"phoEt"				);
    phoEta_			= data.GetPtrFloat(	"phoEta"			);
    phoPhi_			= data.GetPtrFloat(	"phoPhi"			);
    phoSCEta_			= data.GetPtrFloat(	"phoSCEta"			);
    phohasPixelSeed_		= data.GetPtrInt(	"phohasPixelSeed"		);
    phoEleVeto_			= data.GetPtrInt(	"phoEleVeto"			);
    phoHoverE_			= data.GetPtrFloat(	"phoHoverE"			);
    phoSigmaIEtaIEtaFull5x5_	= data.GetPtrFloat(	"phoSigmaIEtaIEtaFull5x5"	);
    phoR9Full5x5_		= data.GetPtrFloat(	"phoR9Full5x5"			);
    phoPFChIso_			= data.GetPtrFloat(	"phoPFChIso"			);
    phoPFPhoIso_		= data.GetPtrFloat(	"phoPFPhoIso"			);
    phoPFNeuIso_		= data.GetPtrFloat(	"phoPFNeuIso"			);
    phoPFChWorstIso_		= data.GetPtrFloat(	"phoPFChWorstIso"		);
    phoFiredSingleTrgs_		= data.GetPtrLong64(	"phoFiredSingleTrgs"		);
    phoSeedTime_		= data.GetPtrFloat(	"phoSeedTime"			);
    phoSeedEnergy_		= data.GetPtrFloat(	"phoSeedEnergy"			);
    phoMIPTotEnergy_		= data.GetPtrFloat(	"phoMIPTotEnergy"		);
    phoIDbit_			= data.GetPtrShort(	"phoIDbit"			);
    phoEtop_			= data.GetPtrFloat(	"phoEtop"			);
    phoEleft_			= data.GetPtrFloat(	"phoEleft"			);
    phoEright_			= data.GetPtrFloat(	"phoEright"			);
    phoEbottom_			= data.GetPtrFloat(	"phoEbottom"			);
        
    nEle_			= data.GetInt(		"nEle"				);
    elePt_			= data.GetPtrFloat(	"elePt"				);
    eleEta_			= data.GetPtrFloat(	"eleEta"			);
    eleD0_			= data.GetPtrFloat(	"eleD0"				);
    eleDz_			= data.GetPtrFloat(	"eleDz"				);
    eleIDbit_			= data.GetPtrShort(	"eleIDbit"			);
    nMu_			= data.GetInt(		"nMu"				);
    muPt_			= data.GetPtrFloat(	"muPt"				);
    muEta_			= data.GetPtrFloat(	"muEta"				);
    muIDbit_			= data.GetPtrInt(	"muIDbit"			);
    muD0_			= data.GetPtrFloat(	"muD0"				);
    muDz_			= data.GetPtrFloat(	"muDz"				);
    nJet_			= data.GetInt(		"nJet"				);
    jetPt_			= data.GetPtrFloat(	"jetPt"				);
    jetEn_			= data.GetPtrFloat(	"jetEn"				);
    jetEta_			= data.GetPtrFloat(	"jetEta"			);
    jetPhi_			= data.GetPtrFloat(	"jetPhi"			);

    //init output variable

    event	 = 0;
    isData	 = 0;
    nGoodVtx	 = 0;
    isPVGood	 = 0;
    rho		 = 0;
    EventWeight	 = 1;
    HLTJet = 0;
    METTrgs = 0;
    PhoTrgs = 0;
    
    pfMET	 = 0;
    pfMETPhi	 = 0;
    puppiMET	 = 0;
    puppiMETPhi	 = 0;
    metFilters	 = 0;

    nPho	 = 0;
    phoE.clear();
    phoEt.clear();
    phoEta.clear();
    phoPhi.clear();
    phoSCEta.clear();
    phohasPixelSeed.clear();
    phoEleVeto.clear();
    phoHoverE.clear();
    phoSigmaIEtaIEtaFull5x5.clear();
    phoR9Full5x5.clear();
    phoPFChIso.clear();
    phoPFPhoIso.clear();
    phoPFNeuIso.clear();
    phoPFChWorstIso.clear();
    phoPFChIso_rc.clear();
    phoPFPhoIso_rc.clear();
    phoPFNeuIso_rc.clear();
    phoPFChWorstIso_rc.clear();
    phoPFChWorstIso_newEA.clear();
    phoFiredSingleTrgs.clear();
    phoSeedTime.clear();
    phoSeedEnergy.clear();
    phoMIPTotEnergy.clear();
    phoMETdPhi.clear();
    phoIDbit.clear();
    MphoID.clear();
    phoCentral = 0;
    phoEtop.clear();
    phoEleft.clear();
    phoEright.clear();
    phoEbottom.clear();
    phoE4.clear();
    phoE4overE1.clear();
    phoEtaWing.clear();
    phoEtaWingoverE1.clear();

    passChIso.clear();
    passNhIso.clear();
    passPhoIso.clear();
    passHoverE.clear();
    passSieie.clear();
    passLChIso.clear();
    passLNhIso.clear();
    passLPhoIso.clear();
    
    CandPhoID.clear();
    FakePhoID.clear();

    nEle	 = 0;
    elePt.clear();
    eleEta.clear();
    eleD0.clear();
    eleDz.clear();
        
    nMu		 = 0;
    muPt.clear();
    muEta.clear();
    muIDbit.clear();
    muD0.clear();
    muDz.clear();
    nLep	 = 0;
    
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
    jetTID.clear();
    jet3Central = 0;
    minJMETdPhi	 = 0;
    JMETdPhi.clear();
    jetjetdR	 = 0;
    jetjetdEta	 = 0;
    jetjetdPhi	 = 0;
    phojetdR.clear();
    phojetdEta.clear();
    phojetdPhi.clear();
    jetjetZdPhi	= 0;
    jetjetphodPhi	= 0;
    ZgsysPhi = 0;
    jetjetsysPhi = 0;
    
    if(isPVGood_ != 1) continue;

    // MET
    puppiMET = puppiMET_;
    puppiMETPhi = puppiMETPhi_;
    pfMET = pfMET_;
    pfMETPhi = pfMETPhi_;

    metFilters = metFilters_;
    if(metFilters>0 && metFilters!=256) continue;
    
    //rho correction of pho isolation
    vector<vector<Float_t>> Iso_rc; //[ch, pho, nh, chw, chw_newEA]
    Iso_rc.clear();
    for(Int_t iso = 0; iso < 5; iso++){
      vector <Float_t> isolist;
      rhoCorrection(iso, data, isolist);
      Iso_rc.push_back(isolist);
    }

    for(Int_t ipho=0; ipho<nPho_; ipho++){
      Int_t boo = 0;
      
      if(phoEt_[ipho]>75){
	if(isMET == 1){
	  if((HLTJet_>>27&1) == 0) boo = 1;
	}
	else if(isMET == 0){
	  if(phoEt_[ipho]>210){
	    if(year == 2016 && (phoFiredSingleTrgs_[ipho]>>7&1) == 0) boo = 1;
	    else if(year == 2017 && (phoFiredSingleTrgs_[ipho]>>8&1) == 0) boo = 1;
	    else if(year == 2018 && (phoFiredSingleTrgs_[ipho]>>7&1) == 0) boo = 1;
	  }
	  else boo = 1;
	}
      }
      else boo = 1;
      
      if(boo == 1) continue;
      if(fabs(phoSCEta_[ipho]) > 1.4442) continue;
      
      nPho++;
      phoE.push_back(phoE_[ipho]);
      //if(phoEt_[ipho]>200) cout << "event " << ev << " phoEt : " << phoEt_[ipho] << endl;
      phoEt.push_back(phoEt_[ipho]);
      phoEta.push_back(phoEta_[ipho]);
      phoPhi.push_back(phoPhi_[ipho]);
      phoSCEta.push_back(phoSCEta_[ipho]);
      phohasPixelSeed.push_back(phohasPixelSeed_[ipho]);
      phoEleVeto.push_back(phoEleVeto_[ipho]);
      phoHoverE.push_back(phoHoverE_[ipho]);
      phoSigmaIEtaIEtaFull5x5.push_back(phoSigmaIEtaIEtaFull5x5_[ipho]);
      phoR9Full5x5.push_back(phoR9Full5x5_[ipho]);
      phoPFChIso.push_back(phoPFChIso_[ipho]);
      phoPFPhoIso.push_back(phoPFPhoIso_[ipho]);
      phoPFNeuIso.push_back(phoPFNeuIso_[ipho]);
      phoPFChWorstIso.push_back(phoPFChWorstIso_[ipho]);
      phoFiredSingleTrgs.push_back(phoFiredSingleTrgs_[ipho]);
      phoSeedTime.push_back(phoSeedTime_[ipho]);
      phoSeedEnergy.push_back(phoSeedEnergy_[ipho]);
      phoMIPTotEnergy.push_back(phoMIPTotEnergy_[ipho]);
      phoIDbit.push_back(phoIDbit_[ipho]);
      phoEtop.push_back(phoEtop_[ipho]);
      phoEleft.push_back(phoEleft_[ipho]);
      phoEright.push_back(phoEright_[ipho]);
      phoEbottom.push_back(phoEbottom_[ipho]);
      
      phoPFChIso_rc.push_back(Iso_rc[0][ipho]);
      phoPFPhoIso_rc.push_back(Iso_rc[1][ipho]);
      phoPFNeuIso_rc.push_back(Iso_rc[2][ipho]);
      phoPFChWorstIso_rc.push_back(Iso_rc[3][ipho]);
      phoPFChWorstIso_newEA.push_back(Iso_rc[4][ipho]);

      // Float_t sumphoE4 = 0;
      // sumphoE4 = phoEtop_[ipho]+phoEleft_[ipho]+phoEright_[ipho]+phoEbottom_[ipho];
      // phoE4.push_back(sumphoE4);

      // Float_t E4overE = 0;
      // E4overE = sumphoE4/phoSeedEnergy_[ipho];
      // phoE4overE1.push_back(E4overE);
      
      // Float_t compWing = 0;
      // if(phoEleft_[ipho] > phoEright_[ipho]) compWing = phoEleft_[ipho];
      // else compWing = phoEright_[ipho];
      // phoEtaWing.push_back(compWing);
      
      // Float_t WingoverE = 0;
      // WingoverE = compWing/phoSeedEnergy_[ipho];
      // phoEtaWingoverE1.push_back(WingoverE);
    }
    //cout << "get rho corr" << endl;
    if(nPho<1) continue;

    //lepton veto selection
    Int_t nLele = 0;
    for(Int_t iele=0; iele<nEle_; iele++){
      
      /// tighter ele selection ///
      if((eleIDbit_[iele]>>1&1) == 1 && elePt_[iele] > 10 && fabs(eleEta_[iele]) < 2.5) {
      	if(fabs(eleEta_[iele]) <1.4442 && fabs(eleD0_[iele]) < 0.05 && fabs(eleDz_[iele]) < 0.1) nLele++;
      	else if(fabs(eleEta_[iele]) > 1.5666 && fabs(eleD0_[iele]) < 0.1 && fabs(eleDz_[iele]) < 0.2) nLele++;
      }
    }
    
    Int_t nLmu = 0;
    for(Int_t imu=0; imu<nMu_; imu++){

      /// tighter mu selection///
      if((muIDbit_[imu]>>0&1) == 1 && muPt_[imu] > 10 && fabs(muEta_[imu]) < 2.4){
      	if(fabs(muD0_[imu]) < 0.2 && fabs(muDz_[imu]) < 0.5) nLmu++;
      }
    }
    Int_t nlep = 0;
    nLep = nLele + nLmu;

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
    //cout << "get # jets" << endl;

    //dijet stuff
    TLorentzVector  leadjetP4, subjetP4, dijetP4;
    
    if(npfjet >= 2){
      Int_t ilead = nonPUjetid[0];
      Int_t isub = nonPUjetid[1];

      //mindjetMETPhi
      if(fabs(deltaPhi(jetPhi_[ilead], puppiMETPhi)) < fabs(deltaPhi(jetPhi_[isub], puppiMETPhi))) minJMETdPhi = deltaPhi(jetPhi_[ilead], puppiMETPhi);
      else if(fabs(deltaPhi(jetPhi_[ilead], puppiMETPhi)) > fabs(deltaPhi(jetPhi_[isub], puppiMETPhi))) minJMETdPhi = deltaPhi(jetPhi_[isub], puppiMETPhi);

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
      for(Int_t ipho=0; ipho<nPho; ipho++){
	if(ipho>0) continue;
	phojetdR.push_back(deltaR(phoSCEta[ipho], phoPhi[ipho], jetEta_[ilead], jetPhi_[ilead]));
	phojetdR.push_back(deltaR(phoSCEta[ipho], phoPhi[ipho], jetEta_[isub], jetPhi_[isub]));

	phojetdEta.push_back(deltaEta(phoSCEta[ipho], jetEta_[ilead]));
	phojetdEta.push_back(deltaEta(phoSCEta[ipho], jetEta_[isub]));

	phojetdPhi.push_back(deltaPhi(phoPhi[ipho], jetPhi_[ilead]));
	phojetdPhi.push_back(deltaPhi(phoPhi[ipho], jetPhi_[isub]));
      }

      //jetMET dPhi
      JMETdPhi.push_back(deltaPhi(jetPhi_[ilead], puppiMETPhi));
      JMETdPhi.push_back(deltaPhi(jetPhi_[isub], puppiMETPhi));
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
    //cout << "get Tlorentz" << endl;

    // Fake pho
    Int_t iWP = 1; Int_t region = 0; Float_t base = 0.0;
    for(Int_t ipho=0; ipho<nPho; ipho++){
      if(Maximum(base, phoPFChIso_rc[ipho]) < chIso_WP[iWP][region]) passChIso.push_back(1);
      else passChIso.push_back(0);
      
      if(Maximum(base, phoPFNeuIso_rc[ipho]) < nhIso_WP[iWP][region]) passNhIso.push_back(1);
      else passNhIso.push_back(0);

      if(Maximum(base, phoPFPhoIso_rc[ipho]) < phoIso_WP[iWP][region]) passPhoIso.push_back(1);
      else passPhoIso.push_back(0);

      if(phoHoverE[ipho] < HoverE_WP[iWP][region]) passHoverE.push_back(1);
      else passHoverE.push_back(0);

      if(phoSigmaIEtaIEtaFull5x5[ipho] < sieie_WP[iWP][region]) passSieie.push_back(1);
      else passSieie.push_back(0);

      //if(passChIso[ipho] == 1 && passNhIso[ipho] == 1 && passPhoIso[ipho] == 1 && passHoverE[ipho] == 1 && passSieie[ipho] == 1) MphoID.push_back(1);
      //else MphoID.push_back(0);
    }

    Int_t bit = 0;
    Int_t fillCandsieie = 0; Int_t fillCanddijet = 0;
    for(Int_t ipho=0; ipho<nPho; ipho++){
      
      //if(ipho>0) continue;
      
      bit = 0;
      if(phohasPixelSeed[ipho] == 0) bit = SetBit(0, bit);
      if(fabs(phoSCEta[ipho]) < 1.4442)							bit = SetBit(1, bit);
      if(phoEt[ipho] > 75)							bit = SetBit(2, bit);
      if(passNhIso[ipho] == 1 && passPhoIso[ipho] == 1 && passHoverE[ipho] == 1) bit = SetBit(3, bit); 
      if(fabs(phoSeedTime[ipho])<3) bit = SetBit(4, bit);
      if(nLep<1) bit = SetBit(5, bit);
      if(puppiMET>180) bit = SetBit(6, bit);
      if(phoSigmaIEtaIEtaFull5x5[ipho] > 0.001 && phoMIPTotEnergy[ipho] < 4.9) bit = SetBit(7, bit);
      if(passChIso[ipho] == 1) bit = SetBit(8, bit);

      
      if(npfjet>=2){
	Int_t ilead = nonPUjetid[0];
	Int_t isub = nonPUjetid[1];
	if(jetPt_[ilead] >50 && jetPt_[isub] >30) bit = SetBit(9, bit);
	if((jetEta_[ilead] * jetEta_[isub]) < 0 && jetjetdEta>2.5)		       bit = SetBit(10, bit);
	if(fabs(jetjetdPhi) < 2.7)							bit = SetBit(11, bit);
	if(phojetdR[0]>0.5 && phojetdR[1]>0.5)						bit = SetBit(12, bit);
	if((phoEt[ipho]/puppiMET) < 2.4 ) bit = SetBit(13, bit);
	if(fabs(minJMETdPhi)>0.5) bit = SetBit(14, bit);
	if(dijetMass>250)								bit = SetBit(15, bit);
      }

      CandPhoID.push_back(bit);
      // fill sieie &  dijetMass, sieie loosed
      if(pho_sel(bit,15) == 1){
	for(Int_t ibin=0; ibin<Nbin; ibin++){
	  if(phoEt[ipho] >= ptbin[ibin] && phoEt[ipho] < ptbin[ibin+1] && ipho == 0){
	    hbin_CandPho_sieie[ibin]->Fill(phoSigmaIEtaIEtaFull5x5[ipho]);
	    hbin_CandPho_dijetMass[ibin]->Fill(dijetMass);
	  }
	}
      }

      // fill dijetMass, cut sieie
      bit = ResetBit(3, bit);
      if(passNhIso[ipho] == 1 && passPhoIso[ipho] == 1 && passHoverE[ipho] == 1 && passSieie[ipho] == 1) bit = SetBit(3, bit);
      if(pho_sel(bit,15) == 1){
	for(Int_t ibin=0; ibin<Nbin; ibin++){
	  if(phoEt[ipho] >= ptbin[ibin] && phoEt[ipho] < ptbin[ibin+1] && ipho == 0){
	    hbin_CandPho_dijetMass_cutsieie[ibin]->Fill(dijetMass);
	  }
	}
      }

      // fill Fake sieie
      bit = ResetBit(3, bit);
      if(passNhIso[ipho] == 1 && passPhoIso[ipho] == 1 && passHoverE[ipho] == 1) bit = SetBit(3, bit);
      bit = ResetBit(8, bit);
      if(Maximum(base, phoPFChIso_rc[ipho]) > 7 && Maximum(base, phoPFChIso_rc[ipho]) < 14) bit = SetBit(8, bit);
      for(Int_t i=0; i<7; i++){bit = SetBit(i+9,bit);}
      FakePhoID.push_back(bit);
      if(pho_sel(bit,15) == 1){
	for(Int_t ibin=0; ibin<Nbin; ibin++){
	  if(phoEt[ipho] >= ptbin[ibin] && phoEt[ipho] < ptbin[ibin+1]){
	    hbin_FakePho_sieie[ibin]->Fill(phoSigmaIEtaIEtaFull5x5[ipho]);
	  }
	}
      }
      
      // CandPhoID.push_back(bit);

      // // CandPho_sieie
      // if(pho_sel(bit, 15) == 1){
      // 	// h_CandPho_HasSeed->Fill(phohasPixelSeed[ipho]);
      // 	// h_CandPho_HoverE->Fill(phoHoverE[ipho]);
      // 	// h_CandPho_SeedTime->Fill(phoSeedTime[ipho]);
      // 	// h_CandPho_MET->Fill(pfMET);
      // 	// h_CandPho_ChIso->Fill(phoPFChIso_rc[ipho]);
      // 	// h_CandPho_NhIso->Fill(phoPFNeuIso_rc[ipho]);
      // 	// h_CandPho_PhoIso->Fill(phoPFPhoIso_rc[ipho]);
      // 	// h_CandPho_sieie->Fill(phoSigmaIEtaIEtaFull5x5[ipho]);

      // 	for(Int_t ibin = 0; ibin < Nbin; ibin++){
      // 	  if(phoEt[ipho] >= ptbin[ibin] && phoEt[ipho] < ptbin[ibin+1] && ipho == 0) {
      // 	    hbin_CandPho_sieie[ibin]->Fill(phoSigmaIEtaIEtaFull5x5[ipho]);
      // 	  }
      // 	}
      // }

      // // CandPho_dijetMass
      // bit = ResetBit(3, bit);
      // if(passNhIso[ipho] == 1 && passPhoIso[ipho] == 1 && passHoverE[ipho] == 1 && passSieie[ipho] == 1) bit = SetBit(3, bit);
      // //bit = ResetBit(15,bit); bit = SetBit(15, bit);

      // if(pho_sel(bit, 15) == 1){
      // 	for(Int_t ibin=0; ibin<Nbin; ibin++){
      // 	  if(phoEt[ipho] >= ptbin[ibin] && phoEt[ipho] < ptbin[ibin+1] && ipho == 0){
      // 	    if(dijetMass>0) hbin_CandPho_dijetMass[ibin]->Fill(dijetMass);
      // 	  }
      // 	}
      // }
      
      // // Fake template
      // for(Int_t i=0; i<7; i++){bit = SetBit(i+9, bit);}
      // bit = ResetBit(3, bit);
      // if(passNhIso[ipho] == 1 && passPhoIso[ipho] == 1 && passHoverE[ipho] == 1) bit = SetBit(3, bit);
      // bit = ResetBit(8, bit);
      // if(Maximum(base, phoPFChIso_rc[ipho]) > 7 && Maximum(base, phoPFChIso_rc[ipho]) < 14) bit = SetBit(8, bit);
      // //if(Maximum(base, phoPFChIso_rc[ipho]) > 7 && Maximum(base, phoPFChIso_rc[ipho]) < 14) cout << phoPFChIso_rc[ipho] << " " << phoSigmaIEtaIEtaFull5x5[ipho] << endl;
      // //bit = SetBit(8, bit);
      // FakePhoID.push_back(bit);

      // // for(Int_t i=15; i>=0; i--){
      // // 	printf("fake bit %d ", getBit(bit,i));
      // // 	printf("\n");	
      // // }
      
      // // FakePho_sieie
      // if(pho_sel(bit, 15) == 1){

      // 	// h_FakePho_HasSeed->Fill(phohasPixelSeed[ipho]);
      // 	// h_FakePho_HoverE->Fill(phoHoverE[ipho]);
      // 	// h_FakePho_SeedTime->Fill(phoSeedTime[ipho]);
      // 	// h_FakePho_MET->Fill(pfMET);
      // 	// h_FakePho_ChIso->Fill(phoPFChIso_rc[ipho]);
      // 	// h_FakePho_NhIso->Fill(phoPFNeuIso_rc[ipho]);
      // 	// h_FakePho_PhoIso->Fill(phoPFPhoIso_rc[ipho]);
      // 	// h_FakePho_sieie->Fill(phoSigmaIEtaIEtaFull5x5[ipho]);      	
		
      // 	for(Int_t ibin = 0; ibin < Nbin; ibin++){
      // 	  if(phoEt[ipho] >= ptbin[ibin] && phoEt[ipho] < ptbin[ibin+1]){
      // 	    hbin_FakePho_sieie[ibin]->Fill(phoSigmaIEtaIEtaFull5x5[ipho]);
      // 	  }
      // 	}
      // }

      // // FakePho_dijetMass
      // bit = ResetBit(3, bit);
      // if(passNhIso[ipho] == 1 && passPhoIso[ipho] == 1 && passHoverE[ipho] == 1 && passSieie[ipho] == 1) bit = SetBit(3, bit);

      // if(pho_sel(bit, 8) == 1){
      // 	for(Int_t ibin = 0; ibin < Nbin; ibin++){
      // 	  if(phoEt[ipho] >= ptbin[ibin] && phoEt[ipho] < ptbin[ibin+1]){
      // 	    if(dijetMass>0) hbin_FakePho_dijetMass[ibin]->Fill(dijetMass);
      // 	  }
      // 	}
      // }
      
      if(Nm1_sel(FakePhoID[ipho], 8) == 1 || Nm1_sel(CandPhoID[ipho], 8) == 1) h2_sieie_chIso->Fill(phoSigmaIEtaIEtaFull5x5[ipho], phoPFChIso_rc[ipho]);

      if(Nm1_sel(FakePhoID[ipho], 8) == 1){
	for(Int_t ibin=0; ibin<Nbin; ibin++){
	if(Maximum(base, phoPFChIso_rc[ipho]) > 4 && Maximum(base, phoPFChIso_rc[ipho]) < 11) hbin_FakePho_sieie_chIsobin[ibin][0]->Fill(phoSigmaIEtaIEtaFull5x5[ipho]);
	if(Maximum(base, phoPFChIso_rc[ipho]) > 5 && Maximum(base, phoPFChIso_rc[ipho]) < 12) hbin_FakePho_sieie_chIsobin[ibin][1]->Fill(phoSigmaIEtaIEtaFull5x5[ipho]);
	if(Maximum(base, phoPFChIso_rc[ipho]) > 6 && Maximum(base, phoPFChIso_rc[ipho]) < 13) hbin_FakePho_sieie_chIsobin[ibin][2]->Fill(phoSigmaIEtaIEtaFull5x5[ipho]);
	if(Maximum(base, phoPFChIso_rc[ipho]) > 7 && Maximum(base, phoPFChIso_rc[ipho]) < 14) hbin_FakePho_sieie_chIsobin[ibin][3]->Fill(phoSigmaIEtaIEtaFull5x5[ipho]);
	}
      }
      
    }

  }

  //**************************************************//
  //********************END LOOP**********************//
  //**************************************************//
  
  //**************************************************//
  //********************Terminate*********************//
  //**************************************************//

  fout_->cd();

  hEvents->Write();

      // TH1F* hbin_FakePho_sieie[Nbin];
      // TH1F* hbin_CandPho_sieie[Nbin];
      // TH1F* hbin_CandPho_dijetMass[Nbin];
      // TH1F* hbin_CandPho_dijetMass_cutsieie[Nbin];

  //h2_sieie_chIso->Write();
  
  fout_->mkdir("FakePho_bin_template");
  fout_->cd("FakePho_bin_template");
  for(Int_t jj=0; jj<Nbin; jj++){
    hbin_FakePho_sieie[jj]->Write();
    for(Int_t iIsobin=0; iIsobin<4; iIsobin++){
      hbin_FakePho_sieie_chIsobin[jj][iIsobin]->Write();
    }
  }
  
  fout_->mkdir("CandPho_bin_template");
  fout_->cd("CandPho_bin_template");
  for(Int_t jj=0; jj<Nbin; jj++){
    hbin_CandPho_sieie[jj]->Write();
    hbin_CandPho_dijetMass[jj]->Write();
    hbin_CandPho_dijetMass_cutsieie[jj]->Write();
  }

  fout_->Close();
  fprintf(stderr, "%s Processed all events\n", treename.c_str());  
}
