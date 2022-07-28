#include <TMath.h>
#include <TMVA/Reader.h>
#include <TTree.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;

void epho_IDselection(Int_t iWP, Int_t year, Int_t region, TreeReader &data, vector<int>& passed){
  passed.clear();

  ofstream ftext;
  ftext.open("Errmessage.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }

  //load from data
  Int_t    nEle = data.GetInt("nEle");
  Float_t* elePt = data.GetPtrFloat("elePt");
  Float_t* eleEta = data.GetPtrFloat("eleEta");
  Float_t* eleHoverE = data.GetPtrFloat("eleHoverE");
  Float_t* eleSigmaIEtaIEtaFull5x5 = data.GetPtrFloat("eleSigmaIEtaIEtaFull5x5");
  Float_t* elePFChIso = data.GetPtrFloat("elePFChIso");
  Float_t* elePFPhoIso = data.GetPtrFloat("elePFPhoIso");
  Float_t* elePFNeuIso = data.GetPtrFloat("elePFNeuIso");
  Float_t  rho = data.GetFloat("rho");
  Long64_t* eleFiredSingleTrgs = data.GetPtrLong64("eleFiredSingleTrgs");

  //[iWP L/M/T][region EB/EE]fall17 photon ID working point
  Float_t chIso_WP[3][2] = {{1.694, 2.089},{1.141, 1.051}, {0.65, 0.517}};
  Float_t phoIso_WP[3][2] = {{2.876, 4.162}, {2.08, 3.867}, {2.044, 3.032}};
  Float_t nhIso_WP[3][2] = {{24.032, 19.722}, {1.189, 2.718}, {0.317, 2.716}};
  Float_t HoverE_WP[3][2] = {{0.04596, 0.059}, {0.02197, 0.0326}, {0.02148, 0.0321}};
  Float_t sieie_WP[3][2] = {{0.0106, 0.0272}, {0.01015, 0.0272}, {0.00996, 0.0271}};

  Int_t iEA = 0;
  Float_t EA[3][7] = {{0.0112, 0.0108, 0.0106, 0.01002, 0.0098, 0.0089, 0.000087},
		      {0.0668, 0.1054, 0.0786, 0.0223, 0.0078, 0.0028, 0.0137},
		      {0.1113, 0.0953, 0.0619, 0.0837, 0.1070, 0.1212, 0.1446}
  };//fall17 v2ID

  ftext << "";
  vector<Int_t> epho_MID; epho_MID.clear();
  
  for(Int_t iele=0; iele<nEle; iele++){
    if(fabs(eleEta[iele] < 1.0)) iEA = 0;
    else if(fabs(eleEta[iele]) > 1.0 && fabs(eleEta[iele]) < 1.479) iEA = 1;
    else if(fabs(eleEta[iele]) > 1.479 && fabs(eleEta[iele]) < 2.0) iEA = 2;
    else if(fabs(eleEta[iele]) > 2.0 && fabs(eleEta[iele]) < 2.2) iEA = 3;
    else if(fabs(eleEta[iele]) > 2.2 && fabs(eleEta[iele]) < 2.3) iEA = 4;
    else if(fabs(eleEta[iele]) > 2.3 && fabs(eleEta[iele]) < 2.4) iEA = 5;
    else if(fabs(eleEta[iele]) > 2.4) iEA = 6;
    
    if( fabs(eleEta[iele]) > 1.4442) continue;

    if(eleHoverE[iele] > HoverE_WP[iWP][region]) continue;

    Float_t corr = 0.0;
    //chIso cut
    corr = elePFChIso[iele] - rho*EA[0][iEA];
    if( corr >= chIso_WP[iWP][region]) continue;
    if(elePFChIso[iele] >= chIso_WP[iWP][region]) continue;
    ftext << "";

    //nhIso cut
    corr = elePFNeuIso[iele] - rho*EA[2][iEA];
    Float_t corrnh[2] = {0.01512, 0.0117};
    Float_t corrnh_2nd[2] = {0.00002259, 0.000023};
    nhIso_WP[iWP][region] += (corrnh[region]*elePt[iele] + corrnh_2nd[region]*elePt[iele]*elePt[iele]);
    if( corr >= nhIso_WP[iWP][region]) continue;
    ftext << "";
    
    //phoIso cut
    corr = elePFPhoIso[iele] - rho*EA[1][iEA];
    Float_t corrpho[2] = {0.004017, 0.0037};
    phoIso_WP[iele][region] += (corrpho[region]*elePt[iele]);
    if( corr >= phoIso_WP[iWP][region]) continue;
    ftext << "";

    if( eleSigmaIEtaIEtaFull5x5[iele] > sieie_WP[iWP][region]) continue;
    ftext << "";
    epho_MID.push_back(iele);
  }

  ftext << "";
  for(Int_t iele=0; iele<nEle; iele++){
    if(year == 2016 && (eleFiredSingleTrgs[iele]>>13&1) == 0) continue;
    else if(year ==2017 && (eleFiredSingleTrgs[iele]>>13&1) == 0) continue;
    else if(year == 2018 && (eleFiredSingleTrgs[iele]>>13&1) == 0) continue;
    Int_t pass = 0;
    for(Int_t ii=0; ii<epho_MID.size(); ii++){
      if(iele == epho_MID[ii]) pass = 1;
    }
    passed.push_back(pass);
  }

  ftext.close();
}

void eleIDcut(Int_t iWP, Int_t year, TreeReader &data, vector<int>& passed){
  passed.clear();

  Int_t    nEle = data.GetInt("nEle");
  Float_t* eleEta = data.GetPtrFloat("eleEta");
  Short_t* eleID = data.GetPtrShort("eleIDbit");
  Long64_t* eleFiredSingleTrgs = data.GetPtrLong64("eleFiredSingleTrgs");
  
  for(Int_t iele=0; iele<nEle; iele++){
    if(year == 2016 && (eleFiredSingleTrgs[iele]>>13&1) == 0) continue;
    else if(year ==2017 && (eleFiredSingleTrgs[iele]>>13&1) == 0) continue;
    else if(year == 2018 && (eleFiredSingleTrgs[iele]>>13&1) == 0) continue;
    Int_t pass = 0;

    if((eleID[iele]>> iWP&1) == 1) pass = 1;
    else pass = 0;
    passed.push_back(pass);
  }  
}
