#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
#include "TH1F.h"
#include "TH2F.h"
#include "TVector2.h"

#include "smallntuplizer.h"
//#include "untuplizer.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "PhotonSelection.h"

void xZgEleFake(char* pathes){
  //Get processed events
  TFile *fopen = new TFile(pathes, "READ");
  TH1F *hEvents = (TH1F*)fopen->Get("hEvents")->Clone();
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
  sprintf(foutname, "EleFake_%s", treename.c_str());
  fout_ = new TFile(foutname,"RECREATE");

  ///electron
  Int_t                 nEle;
  Float_t*       elePt;
  Float_t*       eleD0;
  Float_t*       eleDz;
  Float_t*       eleEta;
  Short_t*       eleIDbit;
  Long64_t* eleFiredSingleTrgs;
  
  vector<Int_t>         cutflowSMID;
  vector<Int_t>         cutflowVBSID;
}
