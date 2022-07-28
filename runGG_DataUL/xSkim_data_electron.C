#include <iostream>
using namespace std;
#include "TH1F.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TVector2.h"

#include "untuplizer.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "IsoCorrection.h"
#include "PhotonSelection.h"
#include "ElectronSelection.h"
#include "JetSelection.h"
#include "METXYshift.h"
#include "puweicalc.h"

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

Int_t SetBit(Int_t nbit, Int_t bit){
  return (bit | (1<<nbit));
}

Int_t ResetBit(Int_t nbit, Int_t bit){
  return (bit & ~(1<<nbit));
}

void xSkim_data_electron(char* pathes, Int_t year){

  //Get processed events
  TFile *fopen = new TFile(pathes, "READ");
  TH1F *hEvents = (TH1F*)fopen->Get("ggNtuplizer/hEvents")->Clone();
  hEvents->SetDirectory(0);
  fopen->Close();

  //access EventTree
  TreeReader data(pathes);
 
  //Creat Output skimmed tree
  TFile *fout_;
  char foutname[50];
  string treestr = pathes;
  Int_t istr = treestr.find("ggtree");
  string treename = treestr.substr(istr);
  sprintf(foutname, "mini_%s", treename.c_str());
  fout_ = new TFile(foutname,"RECREATE");

  //input variables
  ///global event
  Int_t         run_;
  Long64_t      event_;
  Bool_t        isData_;
  Int_t         nVtx_;
  Int_t         nGoodVtx_;
  Bool_t        isPVGood_;
  Float_t       rho_;

  ///electron
  Int_t         nEle_;
  Float_t*      elePt_;
  Float_t*      eleEta_;
  Float_t* elePhi_;
  Float_t* eleEn_;
  Short_t*      eleIDbit_;
  Long64_t* eleFiredSingleTrgs_;
  Float_t*  eleHoverE_;
  Float_t*  eleSigmaIEtaIEtaFull5x5_;
  Float_t*  eleR9Full5x5_;
  Float_t*  elePFChIso_;
  Float_t*  elePFPhoIso_;
  Float_t*  elePFNeuIso_;
  Int_t*    eleConvVeto_;
  Int_t*    eleMissHits_;
  
  //output branch variables
  ///global event
  Int_t                 run;
  Long64_t              event;
  Bool_t                isData;
  Int_t                 nVtx;
  Int_t                 nGoodVtx;
  Bool_t                isPVGood;
  Float_t               rho;
  Float_t               EventWeight;
  ///electron
  Int_t                 nEle;
  vector<Float_t>       elePt;
  vector<Float_t>       eleEta;
  vector<Float_t> elePhi;
  vector<Float_t> eleEn;
  vector<Float_t>       eleD0;
  vector<Float_t>       eleDz;
  vector<Short_t>       eleIDbit;
  vector<Long64_t> eleFiredSingleTrgs;
  vector<Float_t>  eleHoverE;
  vector<Float_t>  eleSigmaIEtaIEtaFull5x5;
  vector<Float_t>  eleR9Full5x5;
  vector<Float_t>  elePFChIso;
  vector<Float_t>  elePFPhoIso;
  vector<Float_t>  elePFNeuIso;
  vector<Int_t>    eleConvVeto;
  vector<Int_t>    eleMissHits;
  vector<Int_t> MelephoID;
  vector<Int_t> TeleID;
  ///muon
  Int_t                 nMu;
  vector<Float_t>       muPt;
  vector<Int_t>         muIDbit;
  vector<Float_t>       muD0;
  vector<Float_t>       muDz;
  Int_t                 nLep;
    
  TTree *outtree_;
  outtree_ = new TTree("EventTree", "EventTree");

  outtree_->Branch("run"			,&run           ,"run/I"		);
  outtree_->Branch("event"			,&event         ,"event/L"		);
  outtree_->Branch("isData"			,&isData        ,"isData/O"		);
  outtree_->Branch("nVtx"			,&nVtx          ,"nVtx/I"		);
  outtree_->Branch("nGoodVtx"			,&nGoodVtx      ,"nGoodVtx/I"		);
  outtree_->Branch("isPVGood"			,&isPVGood      ,"isPVGood/O"		);
  outtree_->Branch("rho"			,&rho           ,"rho/F"		);
  outtree_->Branch("EventWeight"		,&EventWeight   ,"EventWeight/F"	);

  outtree_->Branch("nEle"			,&nEle          ,"nEle/I"		);
  outtree_->Branch("elePt"                      ,&elePt					);
  outtree_->Branch("eleEta"                     ,&eleEta				);
  outtree_->Branch("elePhi"                     ,&elePhi				);
  outtree_->Branch("eleEn"                     ,&eleEn				);
  outtree_->Branch("eleIDbit"                   ,&eleIDbit				);
  outtree_->Branch("eleFiredSingleTrgs" ,&eleFiredSingleTrgs);
  outtree_->Branch("eleHoverE" ,&eleHoverE);
  outtree_->Branch("eleSigmaIEtaIEtaFull5x5",&eleSigmaIEtaIEtaFull5x5);
  outtree_->Branch("eleR9Full5x5",&eleR9Full5x5);
  outtree_->Branch("elePFChIso",&elePFChIso);
  outtree_->Branch("elePFPhoIso",&elePFPhoIso);
  outtree_->Branch("elePFNeuIso",&elePFNeuIso);
  outtree_->Branch("eleConvVeto",&eleConvVeto);
  outtree_->Branch("eleMissHits",&eleMissHits);
  outtree_->Branch("MelephoID",&MelephoID);
  outtree_->Branch("TeleID",&TeleID);
  
  //***********************Loop***********************//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    //cout << "get ev " << ev << endl;
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    //get data from tree
    data.GetEntry(ev);

    run_                         = data.GetInt(		"run"				);
    event_			 = data.GetLong64(	"event"				);
    isData_			 = data.GetBool(	"isData"			);
    nVtx_			 = data.GetInt(		"nVtx"				);
    nGoodVtx_			 = data.GetInt(		"nGoodVtx"			);
    isPVGood_                    = data.GetBool(	"isPVGood"			);
    rho_			 = data.GetFloat(	"rho"				);
    
    nEle_			 = data.GetInt(		"nEle"				);
    elePt_			 = data.GetPtrFloat(	"elePt"				);
    eleEta_			 = data.GetPtrFloat(	"eleEta"			);
    elePhi_ = data.GetPtrFloat("elePhi");
    eleEn_ = data.GetPtrFloat("eleEn");
    eleIDbit_			 = data.GetPtrShort(	"eleIDbit"			);
    eleFiredSingleTrgs_ = data.GetPtrLong64("eleFiredSingleTrgs");
    eleHoverE_ = data.GetPtrFloat("eleHoverE");
    eleSigmaIEtaIEtaFull5x5_ = data.GetPtrFloat("eleSigmaIEtaIEtaFull5x5");
    eleR9Full5x5_ = data.GetPtrFloat("eleR9Full5x5");
    elePFChIso_ = data.GetPtrFloat("elePFChIso");
    elePFPhoIso_ = data.GetPtrFloat("elePFPhoIso");
    elePFNeuIso_ = data.GetPtrFloat("elePFNeuIso");
    eleConvVeto_ = data.GetPtrInt("eleConvVeto");
    eleMissHits_ = data.GetPtrInt("eleMissHits");
    //cout << "get all variable" << endl;

    //init variable
    run		 = 0;
    event	 = 0;
    isData	 = 0;
    nVtx	 = 0;
    nGoodVtx	 = 0;
    isPVGood	 = 0;
    rho		 = 0;
    EventWeight	 = 1;
    nEle	 = 0;
    elePt.clear();
    eleEta.clear();
    elePhi.clear();
    eleEn.clear();
    eleIDbit.clear();
    eleFiredSingleTrgs.clear();
    eleHoverE.clear();
    eleSigmaIEtaIEtaFull5x5.clear();
    eleR9Full5x5.clear();
    elePFChIso.clear();
    elePFPhoIso.clear();
    elePFNeuIso.clear();
    eleConvVeto.clear();
    eleMissHits.clear();
    MelephoID.clear();
    TeleID.clear();

    for(Int_t iele=0; iele<nEle_; iele++){
      if(year == 2016 && (eleFiredSingleTrgs_[iele]>>13&1) == 0) continue;
      else if(year ==2017 && (eleFiredSingleTrgs_[iele]>>13&1) == 0) continue;
      else if(year == 2018 && (eleFiredSingleTrgs_[iele]>>13&1) == 0) continue;
      nEle++;
      elePt.push_back(elePt_[iele]);
      eleEta.push_back(eleEta_[iele]);
      elePhi.push_back(elePhi_[iele]);
      eleEn.push_back(eleEn_[iele]);
      eleIDbit.push_back(eleIDbit_[iele]);
      eleFiredSingleTrgs.push_back(eleFiredSingleTrgs_[iele]);
      eleHoverE.push_back(eleHoverE_[iele]);
      eleSigmaIEtaIEtaFull5x5.push_back(eleSigmaIEtaIEtaFull5x5_[iele]);
      eleR9Full5x5.push_back(eleR9Full5x5_[iele]);
      elePFChIso.push_back(elePFChIso_[iele]);
      elePFPhoIso.push_back(elePFPhoIso_[iele]);
      elePFNeuIso.push_back(elePFNeuIso_[iele]);
      eleConvVeto.push_back(eleConvVeto_[iele]);
      eleMissHits.push_back(eleMissHits_[iele]);
    }
    
    if(nEle<2) continue;

    //electron t&p ID(pass HLT)
    epho_IDselection(1, year, 0, data, MelephoID);
    eleIDcut(2, year, data, TeleID);  
        
    //fill tree
    run = run_;
    event = event_;
    isData = isData_;
    nVtx = nVtx_;
    nGoodVtx = nGoodVtx_;
    isPVGood = isPVGood_;
    rho = rho_;

    outtree_->Fill();
    //cout << "fill tree " << endl;
  }
  //****************END LOOP**********************//

  //****************Terminate*********************//
  fout_->cd();
  outtree_->Write();

  hEvents->Write();
  
  fout_->Close();
  fprintf(stderr, "%s Processed all events\n", treename.c_str());
}
