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
#include "PhotonSelection_MC.h"
#include "JetSelection_MC.h"
#include "puweicalc.h"
#include "LumiW.h"

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

void xSkim_mc(char* pathes, char* PUpathes, char* IDpathes, char* PSVpathes, Int_t dcount, Int_t year, Int_t sig, Int_t ULsamples){

  // Get processed events
  TFile *fopen = new TFile(pathes, "READ");
  TH1F *hEvents = (TH1F*)fopen->Get("ggNtuplizer/hEvents")->Clone();
  hEvents->SetDirectory(0);
  TH1F *hSumofGenW = (TH1F*)fopen->Get("ggNtuplizer/hSumGenWeight")->Clone();
  hSumofGenW->SetDirectory(0);
  fopen->Close();
  
  // Get phoID SF
  fopen = new TFile(IDpathes, "READ");
  TH2F *h_phoIDSF = (TH2F*)fopen->Get("EGamma_SF2D")->Clone();
  h_phoIDSF->SetDirectory(0);
  fopen->Close();

  // Get EleVeto SF
  fopen = new TFile(PSVpathes, "READ");
  TH2F *h2_PSVSF;
  TH1F *h1_PSVSF;
  if(ULsamples == 0){ h2_PSVSF = (TH2F*)fopen->Get("Scaling_Factors_PixelSeed_R9_Inclusive")->Clone();//Legacy MC
    h2_PSVSF->SetDirectory(0);
  }
  else{ h1_PSVSF = (TH1F*)fopen->Get("MediumID/SF_HasPix_MediumID")->Clone();// UL MC
    h1_PSVSF->SetDirectory(0);
  }
  fopen->Close();
  
  //access EventTree
  TreeReader data(pathes);
  
  //Creat Output skimmed tree
  TFile *fout_;
  char foutname[50];
  string treestr = pathes;
  Int_t istr = treestr.find("ggtree");
  string treename = treestr.substr(istr);
  sprintf(foutname, "Jfake_%s", treename.c_str());
  fout_ = new TFile(foutname,"RECREATE");

  // lumi scale
  Double_t LumiWei = lumiW(year, pathes);
  
  ////
  // input variables
  ////
  
  // global event
  Int_t         run_;
  Long64_t      event_;
  Bool_t        isData_;
  Int_t         nGoodVtx_;
  Bool_t        isPVGood_;
  Float_t       rho_;
  Float_t       genWeight_;
  Int_t         nPUInfo_;
  Int_t*        puBX_;
  Float_t*      puTrue_;
  Long64_t HLTJet_; // MET trigger
  
  // gen particles
  Int_t         nMC_;
  Int_t*        mcPID_;
  Float_t*      mcPt_;
  Float_t*      mcMass_;
  Float_t*      mcEta_;
  Float_t*      mcPhi_;
  Float_t*      mcE_;
  Float_t*      mcEt_;
  Int_t*        mcMomPID_;
  Float_t*      mcMomPt_;
  Float_t*      mcMomMass_;
  Float_t*      mcMomEta_;
  Float_t*      mcMomPhi_;
  Short_t*      mcStatusFlag_;

  // MET
  Float_t       pfMET_;
  Float_t       pfMETPhi_;
  Float_t	puppiMET_;
  Float_t	puppiMETPhi_;
  
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
  Float_t*      phoMIPTotEnergy_;
  Short_t*      phoIDbit_;

  // electron
  Int_t         nEle_;
  Float_t*      elePt_;
  Float_t*      eleD0_;
  Float_t*      eleDz_;
  Float_t*      eleEta_;
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
  Float_t*      jetP4Smear_;

  ////
  //output branch variables
  ////
  
  // global event
  Long64_t              event;
  Bool_t                isData;
  Int_t                 nGoodVtx;
  Bool_t                isPVGood;
  Float_t               rho;
  Long64_t HLTJet;
  Int_t METTrgs;
  Int_t PhoTrgs;
  Float_t               genWeight;
  Float_t               genWeightSign;
  Float_t		EventWeight;
  Int_t                 nPUInfo;
  vector<Int_t>         puBX;
  vector<Float_t>       puTrue;

  // gen particle
  Int_t                 nMC;
  vector<Int_t>         mcPID;
  vector<Float_t>       mcPt;
  vector<Float_t>       mcMass;
  vector<Float_t>       mcEta;
  vector<Float_t>       mcPhi;
  vector<Float_t>       mcE;
  vector<Float_t>       mcEt;
  vector<Int_t>         mcMomPID;
  vector<Float_t>       mcMomPt;
  vector<Float_t>       mcMomMass;
  vector<Float_t>       mcMomEta;
  vector<Float_t>       mcMomPhi;
  vector<Short_t>       mcStatusFlag;
  Int_t                 nMCpho;
  vector<Int_t>         phoIsMatch;
  vector<Float_t>       phodR;
  vector<Float_t>       phodPt;

  // MET
  Float_t       pfMET;
  Float_t       pfMETPhi;
  Float_t	puppiMET;
  Float_t	puppiMETPhi;
  Float_t       corrMET;
  Float_t       corrMETPhi;
  
  // photon
  Int_t                 nPho;
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
  vector<Float_t>       phoMIPTotEnergy;
  vector<Float_t>       phoMETdPhi;
  vector<Short_t>       phoIDbit;
  vector<Int_t>         MphoID;
  vector<Int_t> passChIso;
  vector<Int_t> passNhIso;
  vector<Int_t> passPhoIso;
  vector<Int_t> passHoverE;
  vector<Int_t> passLChIso;
  vector<Int_t> passLNhIso;
  vector<Int_t> passLPhoIso;
  vector<Int_t> passSieie;

  // selection ID
  vector<Int_t> TruePhoID;
  vector<Int_t> FakePhoID;
  vector<Int_t> FakePhoMatchID;

  Int_t                 nLep;
  
  // jets
  Int_t                 nJet;
  Int_t                 npfjet;
  vector<Int_t>         nonPUjetid;
  vector<Float_t>       jetPt;
  vector<Float_t>       jetEn;
  vector<Float_t>       jetEta;
  vector<Float_t>       jetPhi;
  vector<Int_t>         jetPUIDMVA;
  vector<Int_t>         jetULTID;
  vector<Int_t>         jetTID;
  vector<Float_t>       jetP4Smear;
  vector<Float_t>       jetPtSmear;
  Double_t              dijetMass;

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

  // pileup reweighting
  PUWeightCalculator puCalc;
  puCalc.Init(PUpathes);

  //[iWP L/M/T][region EB/EE]fall17
  Float_t chIso_WP[3][2] = {{1.694, 2.089},{1.141, 1.051}, {0.65, 0.517}};
  Float_t phoIso_WP[3][2] = {{2.876, 4.162}, {2.08, 3.867}, {2.044, 3.032}};
  Float_t nhIso_WP[3][2] = {{24.032, 19.722}, {1.189, 2.718}, {0.317, 2.716}};
  Float_t HoverE_WP[3][2] = {{0.04596, 0.059}, {0.02197, 0.0326}, {0.02148, 0.0321}};
  Float_t sieie_WP[3][2] = {{0.0106, 0.0272}, {0.01015, 0.0272}, {0.00996, 0.0271}};
  
  // histograms
  Double_t ptbin[20] = {75, 110, 210, 2000};//3 bins, 2016
  Int_t Nbin = 3;

  TH1F* hbin_TruePho_sieie[Nbin];
  Double_t sieiebin[20] = {0.005, 0.0075, 0.008, 0.0085, 0.009, 0.0095, 0.01015, 0.0113, 0.0125, 0.015, 0.0175, 0.02}; // 10 bins
  
  for(Int_t jj=0; jj<Nbin; jj++){
    hbin_TruePho_sieie[jj] = new TH1F(Form("hbin_TruePho_sieie_%i", jj+1), "", 11, sieiebin);
    hbin_TruePho_sieie[jj]->Sumw2();
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

    run_                        = data.GetInt(		"run"				);
    event_			= data.GetLong64(	"event"				);
    isData_			= data.GetBool(		"isData"			);
    nGoodVtx_			= data.GetInt(		"nGoodVtx"			);
    isPVGood_                   = data.GetBool(		"isPVGood"			);
    rho_			= data.GetFloat(	"rho"				);
    HLTJet_ = data.GetLong64("HLTJet");
    genWeight_			= data.GetFloat(	"genWeight"			);
    nPUInfo_			= data.GetInt(		"nPUInfo"			);
    puBX_			= data.GetPtrInt(	"puBX"				);
    puTrue_			= data.GetPtrFloat(	"puTrue"			);
    nMC_			= data.GetInt(		"nMC"				);
    mcPID_			= data.GetPtrInt(	"mcPID"				);
    mcPt_			= data.GetPtrFloat(	"mcPt"				);
    mcMass_			= data.GetPtrFloat(	"mcMass"			);
    mcEta_			= data.GetPtrFloat(	"mcEta"				);
    mcPhi_			= data.GetPtrFloat(	"mcPhi"				);
    mcE_			= data.GetPtrFloat(	"mcE"				);
    mcEt_			= data.GetPtrFloat(	"mcEt"				);
    mcMomPID_			= data.GetPtrInt(	"mcMomPID"			);
    mcMomPt_			= data.GetPtrFloat(	"mcMomPt"			);
    mcMomMass_			= data.GetPtrFloat(	"mcMomMass"			);
    mcMomEta_			= data.GetPtrFloat(	"mcMomEta"			);
    mcMomPhi_			= data.GetPtrFloat(	"mcMomPhi"			);
    mcStatusFlag_		= data.GetPtrShort(	"mcStatusFlag"			);
    pfMET_			= data.GetFloat(	"pfMET"				);
    pfMETPhi_			= data.GetFloat(	"pfMETPhi"			);
    if(ULsamples == 1 && sig == 0){
      puppiMET_			= data.GetFloat(	"puppiMET"				);
      puppiMETPhi_			= data.GetFloat(	"puppiMETPhi"			);
    }
    
    nPho_			= data.GetInt(		"nPho"				);
    phoE_			= data.GetPtrFloat(	"phoE"				);
    phoEt_			= data.GetPtrFloat(	"phoEt"				);
    phoEta_			= data.GetPtrFloat(	"phoEta"			);
    phoPhi_			= data.GetPtrFloat(	"phoPhi"			);
    phoSCEta_			= data.GetPtrFloat(	"phoSCEta"			);
    phohasPixelSeed_		= data.GetPtrInt(	"phohasPixelSeed"		);
    phoEleVeto_		        = data.GetPtrInt(	"phoEleVeto"			);
    phoHoverE_			= data.GetPtrFloat(	"phoHoverE"			);
    phoSigmaIEtaIEtaFull5x5_    = data.GetPtrFloat(	"phoSigmaIEtaIEtaFull5x5"	);
    phoR9Full5x5_		= data.GetPtrFloat(	"phoR9Full5x5"			);
    phoPFChIso_		        = data.GetPtrFloat(	"phoPFChIso"			);
    phoPFPhoIso_		= data.GetPtrFloat(	"phoPFPhoIso"			);
    phoPFNeuIso_		= data.GetPtrFloat(	"phoPFNeuIso"			);
    phoPFChWorstIso_		= data.GetPtrFloat(	"phoPFChWorstIso"		);
    phoFiredSingleTrgs_	        = data.GetPtrLong64(	"phoFiredSingleTrgs"		);
    phoSeedTime_		= data.GetPtrFloat(	"phoSeedTime"			);
    phoMIPTotEnergy_		= data.GetPtrFloat(	"phoMIPTotEnergy"		);
    phoIDbit_			= data.GetPtrShort(	"phoIDbit"			);

    nEle_			= data.GetInt(		"nEle"				);
    elePt_			= data.GetPtrFloat(	"elePt"				);
    eleD0_			= data.GetPtrFloat(	"eleD0"				);
    eleDz_			= data.GetPtrFloat(	"eleDz"				);
    eleEta_			= data.GetPtrFloat(	"eleEta"			);
    eleIDbit_			= data.GetPtrShort(	"eleIDbit"			);
    nMu_			= data.GetInt(		"nMu"				);
    muPt_ 			= data.GetPtrFloat(	"muPt"				);
    muEta_			= data.GetPtrFloat(	"muEta"				);
    muIDbit_			= data.GetPtrInt(	"muIDbit"			);
    muD0_			= data.GetPtrFloat(	"muD0"				);
    muDz_			= data.GetPtrFloat(	"muDz"				);
    
    nJet_			= data.GetInt(		"nJet"				);
    jetPt_			= data.GetPtrFloat(	"jetPt"				);
    jetEn_			= data.GetPtrFloat(	"jetEn"				);
    jetEta_			= data.GetPtrFloat(	"jetEta"			);
    jetPhi_			= data.GetPtrFloat(	"jetPhi"			);
    jetP4Smear_                 = data.GetPtrFloat(	"jetP4Smear"			);

    //cout <<						"get all variable" << endl;
    
    //init variable
    event		= 0;
    isData		= 0;
    nGoodVtx		= 0;
    isPVGood		= 0;
    rho			= 0;
    HLTJet = 0;
    METTrgs = 0;
    PhoTrgs = 0;
    genWeight		= 0;
    genWeightSign	= 0;
    EventWeight = 0;
    nPUInfo		= 0;
    puBX.clear();
    puTrue.clear();
    
    nMC			= 0;
    mcPID.clear();
    mcPt.clear();
    mcMass.clear();
    mcEta.clear();
    mcPhi.clear();
    mcE.clear();
    mcEt.clear();
    mcMomPID.clear();
    mcMomPt.clear();
    mcMomMass.clear();
    mcMomEta.clear();
    mcMomPhi.clear();
    mcStatusFlag.clear();
    nMCpho		= 0;
    phoIsMatch.clear();
    phodR.clear();
    phodPt.clear();
    
    pfMET		= 0;
    pfMETPhi		= 0;
    puppiMET = 0;
    puppiMETPhi = 0;
    corrMET		= 0;
    corrMETPhi		= 0;
    
    nPho		= 0;
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
    phoMIPTotEnergy.clear();
    phoMETdPhi.clear();
    phoIDbit.clear();
    MphoID.clear();

    nLep		= 0;
 
    passChIso.clear();
    passNhIso.clear();
    passPhoIso.clear();
    passHoverE.clear();
    passLChIso.clear();
    passLNhIso.clear();
    passLPhoIso.clear();
    passSieie.clear();

    TruePhoID.clear();
    FakePhoID.clear();

    nJet		= 0;
    npfjet		= 0;
    nonPUjetid.clear();
    jetPt.clear();
    jetEn.clear();
    jetEta.clear();
    jetPhi.clear();
    jetPUIDMVA.clear();
    jetULTID.clear();
    jetTID.clear();
    jetP4Smear.clear();
    jetPtSmear.clear();
    dijetMass = 0;

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

    //rho correction of pho isolation
    vector<vector<Float_t>> Iso_rc; //[ch, pho, nh, chw, chw_newEA]
    Iso_rc.clear();
    for(Int_t iso = 0; iso < 5; iso++){
      vector <Float_t> isolist;
      rhoCorrection(iso, data, isolist);
      Iso_rc.push_back(isolist);
    }

    //jet smear
    for(Int_t ijet=0; ijet<nJet_; ijet++){
      //if(year == 2017 && ULsamples == 0 && jetPt_[ijet]<50 && fabs(jetEta_[ijet])>2.56 && fabs(jetEta_[ijet])<3.139) jetPt_[ijet] = 0;//remove 2017 ecal noise
      if((jetPt_[ijet]*jetP4Smear_[ijet]) <= 0) jetPtSmear.push_back(0);
      else jetPtSmear.push_back(jetPt_[ijet]*jetP4Smear_[ijet]);
    }
    //cout << "get jet smearing" << endl;

    //propagate JEC to MET
    TVector2 jetv2[nJet_], corrjetv2[nJet_];
    TVector2 sumjet;
    TVector2 sumcorrjet;
    for(Int_t ijet=0; ijet<nJet_; ijet++){
      jetv2[ijet].SetMagPhi(jetPt_[ijet], jetPhi_[ijet]);
      corrjetv2[ijet].SetMagPhi(jetPtSmear[ijet], jetPhi_[ijet]);
      if(jetPtSmear[ijet]>10){
	sumjet += jetv2[ijet];
	sumcorrjet += corrjetv2[ijet];
      }
    }
    TVector2 METv2;
    if(ULsamples == 1 && sig == 0) METv2.SetMagPhi(puppiMET_, puppiMETPhi_);
    else METv2.SetMagPhi(pfMET_, pfMETPhi_);
    TVector2 corrMETv2;
    corrMETv2 = METv2 + sumjet - sumcorrjet;
    corrMET = corrMETv2.Mod();
    corrMETPhi = Phi_mpi_pi(corrMETv2.Phi());
    //cout << "get MET corr" << endl;

    //MET XY corr
    // if(ULsamples == 0 || sig == 1){
    //   vector<Float_t> corrXY; corrXY.clear();
    //   METXYshift(year, pathes, data, corrMET, corrXY);
    //   corrMET = corrXY[0];
    //   corrMETPhi = Phi_mpi_pi(corrXY[1]);
    // }
    //cout << "get METPhi corr" << endl;

    for(Int_t ipho=0; ipho<nPho_; ipho++){
      Int_t boo_MET = 0;
      Int_t boo_pho = 0;
      
      if(phoEt_[ipho]>75){
	if((HLTJet_>>27&1) == 0) boo_MET = 1;

	if(phoEt_[ipho]>210){
	  if(year == 2016 && (phoFiredSingleTrgs_[ipho]>>7&1) == 0) boo_pho = 1;
	  else if(year == 2017 && (phoFiredSingleTrgs_[ipho]>>8&1) == 0) boo_pho = 1;
	  else if(year == 2018 && (phoFiredSingleTrgs_[ipho]>>7&1) == 0) boo_pho = 1;
	}
	else boo_pho = 1;
      }
      else{
	boo_MET = 1;
	boo_pho = 1;
      }
      
      if(boo_MET == 1 && boo_pho == 1) continue;
      if(fabs(phoEta_[ipho]) > 1.4442) continue;
      
      nPho++;
      phoE.push_back(phoE_[ipho]);
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
      phoMIPTotEnergy.push_back(phoMIPTotEnergy_[ipho]);
      phoIDbit.push_back(phoIDbit_[ipho]);

      phoPFChIso_rc.push_back(Iso_rc[0][ipho]);
      phoPFPhoIso_rc.push_back(Iso_rc[1][ipho]);
      phoPFNeuIso_rc.push_back(Iso_rc[2][ipho]);
      phoPFChWorstIso_rc.push_back(Iso_rc[3][ipho]);
      phoPFChWorstIso_newEA.push_back(Iso_rc[4][ipho]);
    }
    //cout << "get rho corr" << endl;
    if(nPho<1) continue;

    // Set Trgs variables
    for(Int_t ipho=0; ipho<nPho; ipho++){
      if(ipho>0) continue;
      if((HLTJet_>>27&1) == 1) METTrgs = 1;

      if(year == 2016 && (phoFiredSingleTrgs[ipho]>>7&1) == 1) PhoTrgs = 1;
      else if(year == 2017 && (phoFiredSingleTrgs[ipho]>>8&1) == 1) PhoTrgs = 1;
      else if(year == 2018 && (phoFiredSingleTrgs[ipho]>>7&1) == 1) PhoTrgs = 1;
      
    }
    
    //get mc photon id
    vector<Int_t> mc_phoid; mc_phoid.clear();
    for(Int_t k=0; k < nMC_; k++){
      if(fabs(mcMomPID_[k]) > 6 && fabs(mcMomPID_[k]) != 21) continue;
      if((mcStatusFlag_[k]>>1&1) == 0) continue;//isPromptFinalState passed
      if(mcPID_[k] == 22){
	mc_phoid.push_back(k);
	nMCpho++;
      }
    }
    
    //gen-matching
    Int_t isMatched;
    for(Int_t ipho=0; ipho < nPho; ipho++){
      isMatched = 0;
      for(Int_t nn=0; nn < nMCpho; nn++){
	Int_t k = mc_phoid[nn];
	Float_t dr = deltaR(phoEta[ipho], phoPhi[ipho], mcEta_[k], mcPhi_[k]);
	Float_t dpt = fabs((phoEt[ipho] - mcPt_[k])/mcPt_[k]);
	if(dr < 0.2) phodPt.push_back(dpt);
	if(dpt < 0.2) phodR.push_back(dr);
	if(dr < 0.2 && dpt < 0.2){
	  isMatched = 1;
	  break;
	}
      }
      phoIsMatch.push_back(isMatched);
    }

    //genweight refill sign only
    if(genWeight_ > 0) genWeightSign = 1.0;
    else if(genWeight_ == 0) genWeightSign = 0.0;
    else if(genWeight_ < 0) genWeightSign = -1.0;
    //cout<< "get norm genwei" << endl;

    //pileup
    Int_t nPU;
    Float_t puwei_ = 1;
    for (Int_t i=0; i<nPUInfo_; ++i) {
      if (puBX_[i] == 0) nPU = puTrue_[i];
    }      
    puwei_ = 1.;
    puwei_ = (Float_t)puCalc.GetWeight(run_, puTrue_[1]);
    //cout << "get PU wei" << endl;
    
    //cal tot weight
    Double_t phoSF = 1.0;
    Double_t PSVSF = 1.0;
    for(Int_t ipho=0; ipho<nPho; ipho++){
      if(ipho>0) continue;
      if(isData_ == 0){
	Int_t ptbin = 0;
	Int_t etabin = 0;
	
	if(ULsamples == 0 && phoEt[ipho] >= 200) ptbin = 5;
	else if(ULsamples == 1 && phoEt[ipho] >= 0 && phoEt[ipho] < 35) ptbin = 1;
	else if(ULsamples == 1 && phoEt[ipho] >= 35 && phoEt[ipho] < 50) ptbin = 2;
	else if(ULsamples == 1 && phoEt[ipho] >= 50 && phoEt[ipho] < 80) ptbin = 3;
	else if(ULsamples == 1 && phoEt[ipho] >= 80 && phoEt[ipho] < 120) ptbin = 4;
	else if(ULsamples == 1 && phoEt[ipho] >= 120) ptbin = 5;
	
	//else if(phoEt[ipho] >= 250 && phoEt[ipho] < 300) ptbin = 2;
	//else if(phoEt[ipho] >= 300 && phoEt[ipho] < 350) ptbin = 3;
	//else if(phoEt[ipho] >= 350 && phoEt[ipho] < 450) ptbin = 4;
	//else if(phoEt[ipho] >= 450 && phoEt[ipho] < 1000) ptbin = 5;
	
	if(phoSCEta[ipho] > -1.4442 && phoSCEta[ipho] < -0.8) etabin = 1;
	else if(phoSCEta[ipho] >=-0.8 && phoSCEta[ipho] < 0.0) etabin = 2;
	else if(phoSCEta[ipho] >=0.0 && phoSCEta[ipho] < 0.8) etabin = 3;
	else if(phoSCEta[ipho] >=0.8 && phoSCEta[ipho] < 1.4442) etabin = 4;

	if(ptbin>0 && etabin>0) phoSF = h_phoIDSF->GetBinContent(etabin+3, ptbin);

	if(ULsamples == 0)PSVSF = h2_PSVSF->GetBinContent(1, 1);// Legacy MC
	else PSVSF = h1_PSVSF->GetBinContent(1);// UL MC
      }
      Double_t tot_wei = 1;
      tot_wei = phoSF*puwei_*genWeightSign;
      EventWeight = tot_wei*LumiWei;
    }
    //cout << "get tot wei" << endl;

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
    //cout << "get lepton veto" << endl;

    //jetID selection TLepVeto
    if(ULsamples == 0){
      jet_TIDsel(year, data, jetTID);
      jet_PUIDsel(data, jetPUIDMVA);
    }
    else{
      jet_ULTIDsel(year, data, jetULTID);
      jet_ULPUIDsel(year, data, jetPUIDMVA);
    }
    
    //count # jet
    for(Int_t i=0; i<nJet_; i++){

      if(ULsamples == 0 && (jetPUIDMVA[i] == 1) && (jetPtSmear[i] > 30) && fabs(jetEta_[i]) < 4.7 && (jetTID[i] == 1) ){//for legacy MC with PU
	nonPUjetid.push_back(i);
	npfjet++;
      }
      else if(ULsamples == 1 && (jetPUIDMVA[i] == 1) && (jetPtSmear[i] > 30) && fabs(jetEta_[i]) < 4.7 && (jetULTID[i] == 1)){// UL MC with PU
	nonPUjetid.push_back(i);
	npfjet++;
      }
      
    }

    //dijet stuff
    TLorentzVector  leadjetP4, subjetP4, dijetP4;

    if(npfjet>=2){
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
      leadjetP4.SetPtEtaPhiE(jetPtSmear[ilead], jetEta_[ilead], jetPhi_[ilead], jetEn_[ilead]);
      subjetP4.SetPtEtaPhiE(jetPtSmear[isub], jetEta_[isub], jetPhi_[isub], jetEn_[isub]);   
      dijetP4 = leadjetP4 + subjetP4;
      dijetMass = dijetP4.M();
      
      //phojet dEta & dPhi & dR
      for(Int_t ipho=0; ipho<nPho; ipho++){
	if(ipho>0) continue;
	phojetdR.push_back(deltaR(phoEta[ipho], phoPhi[ipho], jetEta_[ilead], jetPhi_[ilead]));
	phojetdR.push_back(deltaR(phoEta[ipho], phoPhi[ipho], jetEta_[isub], jetPhi_[isub]));

	phojetdEta.push_back(deltaEta(phoEta[ipho], jetEta_[ilead]));
	phojetdEta.push_back(deltaEta(phoEta[ipho], jetEta_[isub]));

	phojetdPhi.push_back(deltaPhi(phoPhi[ipho], jetPhi_[ilead]));
	phojetdPhi.push_back(deltaPhi(phoPhi[ipho], jetPhi_[isub]));
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
	phojetdR.push_back(-999);
	phojetdEta.push_back(-999);
	phojetdPhi.push_back(-999);
      }
      
      JMETdPhi.push_back(-999);
    }

    // photon ID with sieie relaxed
    Int_t iWP = 1;
    Int_t region = 0;
    Float_t base = 0.0;

    for(Int_t ipho=0; ipho<nPho; ipho++){
      /// medium like pho
      if(Maximum(base, phoPFChIso_rc[ipho]) < chIso_WP[iWP][region]) passChIso.push_back(1);
      else passChIso.push_back(0);

      //nhIso_WP[iWP][region] += (corrnh[region]*phoEt[ipho] + corrnh_2nd[region]*phoEt[ipho]*phoEt[ipho]);
      if(Maximum(base, phoPFNeuIso_rc[ipho]) < nhIso_WP[iWP][region]) passNhIso.push_back(1);
      else passNhIso.push_back(0);

      //phoIso_WP[iWP][region] += (corrpho[region]*phoEt[ipho]);
      if(Maximum(base, phoPFPhoIso_rc[ipho]) < phoIso_WP[iWP][region]) passPhoIso.push_back(1);
      else passPhoIso.push_back(0);

      if(phoHoverE[ipho] < HoverE_WP[iWP][region]) passHoverE.push_back(1);
      else passHoverE.push_back(0);
    }
    
    Int_t bit = 0;
    for(Int_t ipho=0; ipho<nPho; ipho++){
      if(METTrgs == 0) continue;

      bit = 0;
      if(phoIsMatch[ipho] == 1) bit = SetBit(0, bit);
      if(fabs(phoSCEta[ipho])<1.4442 && phohasPixelSeed[ipho] == 0) bit = SetBit(1, bit);
      if(phoEt[ipho]>75) bit = SetBit(2, bit);
      if(passNhIso[ipho] == 1 && passPhoIso[ipho] == 1 && passHoverE[ipho] == 1) bit = SetBit(3, bit);
      if(fabs(phoSeedTime[ipho])<3) bit = SetBit(4, bit);
      if(nLep<1) bit = SetBit(5, bit);
      //cout << corrMET << endl;
      if(corrMET>180) bit = SetBit(6, bit);
      if(phoSigmaIEtaIEtaFull5x5[ipho] > 0.001 && phoMIPTotEnergy[ipho]<4.9) bit = SetBit(7, bit);
      if(passChIso[ipho] == 1) bit = SetBit(8, bit);
      if(npfjet >=2){
	Int_t ilead = nonPUjetid[0];
	Int_t isub = nonPUjetid[1];
	if(jetPtSmear[ilead] >50 && jetPtSmear[isub] >30) bit = SetBit(9, bit);
	if((jetEta_[ilead] * jetEta_[isub]) < 0 && jetjetdEta > 2.5)					bit = SetBit(10, bit);
	if(fabs(jetjetdPhi) < 2.7)							bit = SetBit(11, bit);
	if(phojetdR[0]>0.5 && phojetdR[1]>0.5)						bit = SetBit(12, bit);
	if((phoEt[ipho]/corrMET) < 2.4 ) bit = SetBit(13, bit);
	if(fabs(minJMETdPhi)   > 0.5)							bit = SetBit(14, bit);
	if(dijetMass>250)								bit = SetBit(15, bit);
      }

      TruePhoID.push_back(bit);
      //for(Int_t i=0; i<7; i++){bit = SetBit(i+9, bit);}
      //cout << "selection: " << pho_sel(bit,0) << pho_sel(bit,1) << pho_sel(bit,2) << pho_sel(bit,3) << pho_sel(bit,4) << pho_sel(bit,5) << pho_sel(bit,6) << pho_sel(bit,7) << pho_sel(bit,8) << endl;
      if(pho_sel(bit, 8) == 1){
	for(Int_t ibin = 0; ibin<Nbin; ibin++){
	  if(phoEt[ipho]>=ptbin[ibin] && phoEt[ipho]<ptbin[ibin+1]){
	    hbin_TruePho_sieie[ibin]->Fill(phoSigmaIEtaIEtaFull5x5[ipho], EventWeight);
	    
	  }
	}
      }

      //if(Nm1_sel(TruePhoID[ipho],8)==1 && Nm1_sel(FakePhoMatchID[ipho],8)==1 && Nm1_sel(FakePhoID[ipho],8)==1) h2_sieie_chIso->Fill(phoSigmaIEtaIEtaFull5x5[ipho], phoPFChIso_rc[ipho], EventWeight); 
    }
    //cout << "get setbit" << endl;
    
  }

  //**************************************************//
  //********************END LOOP**********************//
  //**************************************************//

  //**************************************************//
  //********************Terminate*********************//
  //**************************************************//
  
  fout_->cd();
    
  hSumofGenW->Write();
  hEvents->Write();

  fout_->mkdir("TruePho_bin_template");
  fout_->cd("TruePho_bin_template");
  for(Int_t jj=0; jj<Nbin; jj++){
    hbin_TruePho_sieie[jj]->Write();
  }
  
  h2_sieie_chIso->Write();
  
  fout_->Close();
  fprintf(stderr, "%s Processed all events\n", treename.c_str());
}
