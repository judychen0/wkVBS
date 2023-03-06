#include <iostream>
#include <string.h>
using namespace std;

#include "TH1F.h"

Double_t lumiW(Int_t year, char* sample){

  string samplestr = sample;
  Int_t istr = samplestr.find("job");
  if(year == 2016) samplestr.replace(0, istr, "/data1/GMET/minitree_MC/summer16/histo/");
  else if(year == 2017) samplestr.replace(0, istr, "/data1/GMET/minitree_MC/fall17/histo/");
  else if(year == 2018) samplestr.replace(0, istr, "/data1/GMET/minitree_MC/autumn18/histo/");
  Int_t fstr = samplestr.find("ggtree_");
  Int_t estr = samplestr.find(".root");
  Int_t mstr = estr - fstr;
  samplestr.replace(fstr, mstr, "histo");

  istr = samplestr.find("job");
  fstr = samplestr.find("/histo.");
  Int_t lstr = fstr - istr;
  string samplename = samplestr.substr(istr, lstr);

  char pathes[100]; 
  sprintf(pathes, "%s", samplestr.c_str());

  // Get processed events
  TFile *fopen = new TFile(pathes, "READ");
  TH1F *hEvents = (TH1F*)fopen->Get("ggNtuplizer/hEvents")->Clone();
  TH1F *hSumofGenW = (TH1F*)fopen->Get("ggNtuplizer/hSumGenWeight")->Clone();

  Double_t entries = 0;
  Double_t outentries = 0;
  Double_t lumiW = 0;

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

  string samplelist16[80] = {
    "job_summer16_ZnunugJets_pt40to130",
    "job_summer16_ZnunugJets_pt130",
    "job_summer16_EWK_Wminus",
    "job_summer16_EWK_Wminus_ext1",
    "job_summer16_EWK_Wminus_ext2",
    "job_summer16_EWK_Wplus",
    "job_summer16_EWK_Wplus_ext1",
    "job_summer16_EWK_Wplus_ext2",
    "job_summer16_EWK_Zll",
    "job_summer16_EWK_Zll_ext1",
    "job_summer16_EWK_Zll_ext2",
    "job_summer16_EWK_Zvv",
    "job_summer16_EWK_Zvv_ext1",
    "job_summer16_EWK_Zvv_ext2",
    "job_summer16_WW",
    "job_summer16_WW_ext1",
    "job_summer16_WZ",
    "job_summer16_ZZ",
    "job_summer16_ZZ_ext1",
    "job_summer16_WWG",
    "job_summer16_WZG",
    "job_summer16_TTGJets",
    "job_summer16_TTGJets_ext1",    
    "job_summer16_ZvvJets_HT100to200",
    "job_summer16_ZvvJets_HT100to200_ext1",
    "job_summer16_ZvvJets_HT200to400",
    "job_summer16_ZvvJets_HT200to400_ext1",
    "job_summer16_ZvvJets_HT400to600",
    "job_summer16_ZvvJets_HT400to600_ext1",
    "job_summer16_ZvvJets_HT600to800",
    "job_summer16_ZvvJets_HT800to1200",
    "job_summer16_ZvvJets_HT1200to2500",
    "job_summer16_ZvvJets_HT1200to2500_ext1",
    "job_summer16_ZvvJets_HT2500toInf",
    "job_summer16_WJetsToLNu_MG_HT70to100",
    "job_summer16_WJetsToLNu_MG_HT100to200",
    "job_summer16_WJetsToLNu_MG_HT100to200_ext1",
    "job_summer16_WJetsToLNu_MG_HT100to200_ext2",
    "job_summer16_WJetsToLNu_MG_HT200to400",
    "job_summer16_WJetsToLNu_MG_HT200to400_ext1",
    "job_summer16_WJetsToLNu_MG_HT200to400_ext2",
    "job_summer16_WJetsToLNu_MG_HT400to600",
    "job_summer16_WJetsToLNu_MG_HT400to600_ext1",
    "job_summer16_WJetsToLNu_MG_HT600to800",
    "job_summer16_WJetsToLNu_MG_HT600to800_ext1",
    "job_summer16_WJetsToLNu_MG_HT800to1200",
    "job_summer16_WJetsToLNu_MG_HT800to1200_ext1",
    "job_summer16_WJetsToLNu_MG_HT1200to2500",
    "job_summer16_WJetsToLNu_MG_HT1200to2500_ext1",
    "job_summer16_WJetsToLNu_MG_HT2500toInf",
    "job_summer16_WJetsToLNu_MG_HT2500toInf_ext1",
    "job_summer16_DYJetsToLL_m50_MG_HT70to100",
    "job_summer16_DYJetsToLL_m50_MG_HT100to200",
    "job_summer16_DYJetsToLL_m50_MG_HT100to200_ext1",
    "job_summer16_DYJetsToLL_m50_MG_HT200to400",
    "job_summer16_DYJetsToLL_m50_MG_HT200to400_ext1",
    "job_summer16_DYJetsToLL_m50_MG_HT400to600",
    "job_summer16_DYJetsToLL_m50_MG_HT400to600_ext1",
    "job_summer16_DYJetsToLL_m50_MG_HT600to800",
    "job_summer16_DYJetsToLL_m50_MG_HT800to1200",
    "job_summer16_DYJetsToLL_m50_MG_HT1200to2500",
    "job_summer16_DYJetsToLL_m50_MG_HT2500toInf",
    "job_summer16_TT_aMCatNLO",
    "job_UL16_Zg_aMCatNLO_preVFP",
    "job_UL16_Wg_pt130_preVFP",
    "job_UL16_Wg_pt40to130_preVFP",
    "job_UL16_GJets_HT40to100_preVFP",
    "job_UL16_GJets_HT100to200_preVFP",
    "job_UL16_GJets_HT200to400_preVFP",
    "job_UL16_GJets_HT400to600_preVFP",
    "job_UL16_GJets_HT600toInf_preVFP",    
    "job_UL16_Zg_aMCatNLO_postVFP",
    "job_UL16_Wg_pt130_postVFP",
    "job_UL16_Wg_pt40to130_postVFP",
    "job_UL16_GJets_HT40to100_postVFP",
    "job_UL16_GJets_HT100to200_postVFP",
    "job_UL16_GJets_HT200to400_postVFP",
    "job_UL16_GJets_HT400to600_postVFP",
    "job_UL16_GJets_HT600toInf_postVFP",
  };

  string samplelist17[80] = {
    "job_fall17_ZnunugJets_pt40to130",
    "job_fall17_ZnunugJets_pt130",
    "job_fall17_EWK_Wminus",
    "job_fall17_EWK_Wplus",
    "job_fall17_EWK_Zll",
    "job_fall17_EWK_Zvv",
    "job_fall17_WW",
    "job_fall17_WZ",
    "job_fall17_ZZ",
    "job_fall17_WWG",
    "job_fall17_WZG",
    "job_fall17_TTGjets",
    "job_fall17_TTGjets_ext1",
    "job_fall17_ZvvJets_HT100to200",
    "job_fall17_ZvvJets_HT200to400",
    "job_fall17_ZvvJets_HT400to600",
    "job_fall17_ZvvJets_HT600to800",
    "job_fall17_ZvvJets_HT800to1200",
    "job_fall17_ZvvJets_HT1200to2500",
    "job_fall17_ZvvJets_HT1200to2500_ext1",
    "job_fall17_ZvvJets_HT2500toInf",
    "job_fall17_WJetsToLNu_MG_HT70to100",
    "job_fall17_WJetsToLNu_MG_HT100to200",
    "job_fall17_WJetsToLNu_MG_HT200to400",
    "job_fall17_WJetsToLNu_MG_HT400to600",
    "job_fall17_WJetsToLNu_MG_HT600to800",
    "job_fall17_WJetsToLNu_MG_HT800to1200",
    "job_fall17_WJetsToLNu_MG_HT1200to2500",
    "job_fall17_WJetsToLNu_MG_HT2500toInf",
    "job_fall17_DYJetsToLL_m50_MG_HT70to100",
    "job_fall17_DYJetsToLL_m50_MG_HT100to200",
    "job_fall17_DYJetsToLL_m50_MG_HT100to200_ext1",
    "job_fall17_DYJetsToLL_m50_MG_HT200to400",
    "job_fall17_DYJetsToLL_m50_MG_HT200to400_ext1",
    "job_fall17_DYJetsToLL_m50_MG_HT400to600",
    "job_fall17_DYJetsToLL_m50_MG_HT400to600_ext1",
    "job_fall17_DYJetsToLL_m50_MG_HT600to800",
    "job_fall17_DYJetsToLL_m50_MG_HT800to1200",
    "job_fall17_DYJetsToLL_m50_MG_HT1200to2500",
    "job_fall17_DYJetsToLL_m50_MG_HT2500toInf",
    "job_fall17_TT_aMCatNLO",
    "job_UL17_Zg_aMCatNLO",
    "job_UL17_Wg_pt130",
    "job_UL17_Wg_pt40to130",
    "job_UL17_GJets_HT40to100", 
    "job_UL17_GJets_HT100to200",
    "job_UL17_GJets_HT200to400",
    "job_UL17_GJets_HT400to600",
    "job_UL17_GJets_HT600toInf",
    "job_UL17_Znunug_VBS"
  };

  string samplelist18[80] = {
    "job_autumn18_ZnunugJets_pt40to130",
    "job_autumn18_ZnunugJets_pt130",
    "job_autumn18_EWK_Wminus",
    "job_autumn18_EWK_Wplus",
    "job_autumn18_EWK_Zll",
    "job_autumn18_EWK_Zvv",
    "job_autumn18_WW",
    "job_autumn18_WZ",
    "job_autumn18_ZZ",
    "job_autumn18_WWG",
    "job_autumn18_WZG",
    "job_autumn18_TTGjets",
    "job_autumn18_ZvvJets_HT100to200",
    "job_autumn18_ZvvJets_HT200to400",
    "job_autumn18_ZvvJets_HT400to600",
    "job_autumn18_ZvvJets_HT600to800",
    "job_autumn18_ZvvJets_HT800to1200",
    "job_autumn18_ZvvJets_HT1200to2500",
    "job_autumn18_ZvvJets_HT2500toInf",
    "job_autumn18_WJetsToLNu_MG_HT70to100",
    "job_autumn18_WJetsToLNu_MG_HT100to200",
    "job_autumn18_WJetsToLNu_MG_HT200to400",
    "job_autumn18_WJetsToLNu_MG_HT400to600",
    "job_autumn18_WJetsToLNu_MG_HT600to800",
    "job_autumn18_WJetsToLNu_MG_HT800to1200",
    "job_autumn18_WJetsToLNu_MG_HT1200to2500",
    "job_autumn18_WJetsToLNu_MG_HT2500toInf",
    "job_autumn18_DYJetsToLL_m50_MG_HT70to100",
    "job_autumn18_DYJetsToLL_m50_MG_HT100to200",
    "job_autumn18_DYJetsToLL_m50_MG_HT200to400",
    "job_autumn18_DYJetsToLL_m50_MG_HT400to600",
    "job_autumn18_DYJetsToLL_m50_MG_HT400to600_ext1",
    "job_autumn18_DYJetsToLL_m50_MG_HT600to800",
    "job_autumn18_DYJetsToLL_m50_MG_HT800to1200",
    "job_autumn18_DYJetsToLL_m50_MG_HT1200to2500",
    "job_autumn18_DYJetsToLL_m50_MG_HT2500toInf",
    "job_autumn18_TT_aMCatNLO_ext1",
    "job_autumn18_TT_aMCatNLO_ext2",
    "job_UL18_Zg_aMCatNLO",
    "job_UL18_Wg_pt130",
    "job_UL18_Wg_pt40to130",
    "job_UL18_GJets_HT40to100", 
    "job_UL18_GJets_HT100to200",
    "job_UL18_GJets_HT200to400",
    "job_UL18_GJets_HT400to600",
    "job_UL18_GJets_HT600toInf",
    "job_UL18_Znunug_VBS"
  };
  
  Double_t xsec16[80] = {2.787, 0.1837, 20.35, 20.35, 20.35, 25.81, 25.81, 25.81, 3.997, 3.997, 3.997, 10.04, 10.04, 10.04, 64.28, 64.28, 23.48, 10.16, 10.16, 0.2147, 0.04123, 3.795, 3.795,
			 269.2, 269.2, 74.11, 74.11, 9.93, 9.93, 2.394, 1.052, 0.2499, 0.2499, 0.005662,
			 1353.0, 1346, 1346, 1346, 360.1, 360.1, 360.1, 48.8, 48.8, 12.07, 12.07, 5.497, 5.497, 1.329, 1.329, 0.03209, 0.03209,
			 167.7, 147.4, 147.4, 41.04, 41.04, 5.674, 5.674, 1.358, 0.6229, 0.1512, 0.003659,
			 730.6,
			 51.1, 0.8099, 19.75, 18540, 8644, 2183, 260.2, 88.58,
			 51.1, 0.8099, 19.75, 18540, 8644, 2183, 260.2, 88.58,
			 
  };
  Double_t xsec17[80] = {3.025, 0.192, 23.24, 29.59, 4.321, 10.66, 75.8, 27.6, 12.14, 0.2147, 0.04345, 4.078, 4.078,
			 269.2, 74.11, 9.93, 2.394, 1.052, 0.2499, 0.2499, 0.005662,
			 1266, 1251, 366.4, 45.21, 10.89, 5.019, 1.159, 0.008,
			 137.8, 142.1, 142.1, 38.83, 38.83, 5.342, 5.342, 1.286, 0.5668, 0.1361, 0.003019,
			 722.8,
			 51.1, 0.8099, 19.75, 18540, 8644, 2183, 260.2, 88.58, 0.01817
			 
  };
  //1292.0, 1395, 407.9, 57.48, 12.87, 5.366, 1.074, 0.008001,
  //146.7, 161.1, 161.1, 48.66, 48.66, 6.968, 6.968, 1.743, 0.8052, 0.1933, 0.003468,
  Double_t xsec18[80] = {3.025, 0.192, 23.24, 29.59, 4.321, 10.66, 75.8, 27.6, 12.14, 0.2147, 0.04345, 4.078,
  			 269.2, 74.11, 9.93, 2.394, 1.052, 0.2499, 0.005662,
			 1266, 1251, 366.4, 45.21, 10.89, 5.019, 1.159, 0.008,
			 137.8, 142.1, 38.83, 5.342, 5.342, 1.286, 0.5668, 0.1361, 0.003019,
			 722.8, 722.8,
			 51.1, 0.8099, 19.75, 18540, 8644, 2183, 260.2, 88.58, 0.01817
  };

  entries = hSumofGenW->GetBinContent(1);

  if(year == 2016){
    for(Int_t i=0; i<80; i++){
      if(samplename == samplelist16[i]){
	outentries = xsec16[i]*1000*lumi;
	lumiW = fabs(outentries/entries);
	break;
      }
    }
  }
  else if(year == 2017){
    for(Int_t i=0; i<80; i++){
      if(samplename == samplelist17[i]){
	outentries = xsec17[i]*1000*lumi;
	lumiW = fabs(outentries/entries);
	break;
      }
    }
  }
  else if(year == 2018){
    for(Int_t i=0; i<80; i++){
      if(samplename == samplelist18[i]){
	outentries = xsec18[i]*1000*lumi;
	lumiW = fabs(outentries/entries);
	break;
      }
    }
  }

  return lumiW;

}
