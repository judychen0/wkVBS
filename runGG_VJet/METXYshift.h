#include "TMath.h"
#include <TMVA/Reader.h>
#include "TTree.h"
#include <string.h>
#include <stdio.h>
#include <vector>

using namespace std;

void METXYshift(Int_t year, char* sample, TreeReader &data, Float_t pfMET, vector<Float_t>& corrXY){
    
  TString samplelist16[30] = {
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
			      "job_summer16_ZvvJets_HT200to400",
			      "job_summer16_ZvvJets_HT200to400_ext1",
			      "job_summer16_ZvvJets_HT400to600",
			      "job_summer16_ZvvJets_HT400to600_ext1",
			      "job_summer16_ZvvJets_HT600to800",
			      "job_summer16_ZvvJets_HT800to1200",
			      "job_summer16_ZvvJets_HT1200to2500",
			      "job_summer16_ZvvJets_HT1200to2500_ext1",
			      "job_summer16_ZvvJets_HT2500toInf",
			      "job_summer16_DYJetsToLL_m50_MG_HT200to400",
			      "job_summer16_DYJetsToLL_m50_MG_HT200to400_ext1",
			      "job_summer16_DYJetsToLL_m50_MG_HT400to600",
			      "job_summer16_DYJetsToLL_m50_MG_HT400to600_ext1",
			      "job_summer16_DYJetsToLL_m50_MG_HT600to800",
			      "job_summer16_DYJetsToLL_m50_MG_HT800to1200",
			      "job_summer16_DYJetsToLL_m50_MG_HT1200to2500",
			      "job_summer16_DYJetsToLL_m50_MG_HT2500toInf"
			      
  };

  TString samplelist17[30] = {
			      "job_fall17_WJetsToLNu_MG_HT100to200",
			      "job_fall17_WJetsToLNu_MG_HT200to400",
			      "job_fall17_WJetsToLNu_MG_HT400to600",
			      "job_fall17_WJetsToLNu_MG_HT600to800",
			      "job_fall17_WJetsToLNu_MG_HT800to1200",
			      "job_fall17_WJetsToLNu_MG_HT1200to2500",
			      "job_fall17_WJetsToLNu_MG_HT2500toInf",
			      "job_fall17_ZvvJets_HT100to200",
			      "job_fall17_ZvvJets_HT200to400",
			      "job_fall17_ZvvJets_HT400to600",
			      "job_fall17_ZvvJets_HT600to800",
			      "job_fall17_ZvvJets_HT800to1200",
			      "job_fall17_ZvvJets_HT1200to2500",
			      "job_fall17_ZvvJets_HT1200to2500_ext1",
			      "job_fall17_ZvvJets_HT2500toInf",
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
			      "job_fall17_TT_aMCatNLO"
			      
  };

  TString samplelist18[30] = {
			      "job_autumn18_WJetsToLNu_MG_HT100to200",
			      "job_autumn18_WJetsToLNu_MG_HT200to400",
			      "job_autumn18_WJetsToLNu_MG_HT400to600",
			      "job_autumn18_WJetsToLNu_MG_HT600to800",
			      "job_autumn18_WJetsToLNu_MG_HT800to1200",
			      "job_autumn18_WJetsToLNu_MG_HT1200to2500",
			      "job_autumn18_WJetsToLNu_MG_HT2500toInf",
			      "job_autumn18_ZvvJets_HT100to200",
			      "job_autumn18_ZvvJets_HT200to400",
			      "job_autumn18_ZvvJets_HT400to600",
			      "job_autumn18_ZvvJets_HT600to800",
			      "job_autumn18_ZvvJets_HT800to1200",
			      "job_autumn18_ZvvJets_HT1200to2500",
			      "job_autumn18_ZvvJets_HT2500toInf",
			      "job_autumn18_DYJetsToLL_m50_MG_HT100to200",
			      "job_autumn18_DYJetsToLL_m50_MG_HT200to400",
			      "job_autumn18_DYJetsToLL_m50_MG_HT400to600",
			      "job_autumn18_DYJetsToLL_m50_MG_HT400to600_ext2",
			      "job_autumn18_DYJetsToLL_m50_MG_HT600to800",
			      "job_autumn18_DYJetsToLL_m50_MG_HT800to1200",
			      "job_autumn18_DYJetsToLL_m50_MG_HT1200to2500",
			      "job_autumn18_DYJetsToLL_m50_MG_HT2500toInf",
			      "job_autumn18_TT_aMCatNLO_ext1",
			      "job_autumn18_TT_aMCatNLO_ext2"
  };

  Float_t CorrX16[2][30] = {
			    {-0.21271, -0.21386, -0.21358, -0.21148, -0.20734, -0.22549, -0.21212, -0.21749, -0.20913, -0.10865, -0.21224, -0.16319, -0.20827, -0.20811, -0.21022, -0.18376, -0.19880, -0.19531, -0.19143, -0.22921, -0.09826, -0.19301, -0.21992, -0.21616, -0.20774, -0.21303, -0.20909, -0.21646, -0.20241, -0.20115},
			    {-0.51573, -0.46404, -0.50979, -0.66574, -0.72296, -0.33074, -0.58516, -0.31099, -0.57287, -2.26952, -0.46622, -1.45560, -0.14948, -0.54772, -0.51273, -1.02192, -0.82228, -0.76478, -0.71878, -0.01469, -2.55552, -0.20873, -0.37947, -0.47372, -0.76212, -0.58821, -0.75011, -0.58882, -0.86564, -0.62288}
  };

  Float_t CorrY16[2][30] = {
			    {-0.06885, -0.06267, -0.05997, -0.09615, -0.06682, -0.07677, -0.06773, -0.09551, -0.06528, -0.20713, -0.06383, -0.10322, -0.08154, -0.06227, -0.06402, -0.09882, -0.06318, -0.07809, -0.07278,  0.03155,  0.10219, -0.10590, -0.05907, -0.05584, -0.05431, -0.05860, -0.06277, -0.06213, -0.11331, -0.07961},
			    { 1.33509,  1.17590,  1.12126,  1.77070,  1.23210,  1.39409,  1.18934,  1.67309,  1.10184,  4.21818,  1.33636,  2.93004,  2.49937,  1.15056,  1.18257,  1.99900,  1.15775,  1.45740,  1.15911, -0.40798, -1.27409,  3.30277,  1.11927,  1.00604,  0.99153,  1.00563,  1.03870,  1.03518,  2.25358,  2.35503}
  };

  Float_t CorrX17[2][30] = {
			    {-0.22059, -0.21986, -0.21911, -0.21443, -0.20740, -0.21291, -0.19625, -0.21766, -0.21963, -0.21994, -0.21096, -0.19392, -0.20214, -0.15456, -0.19567, -0.22161, -0.22178, -0.22010, -0.22213, -0.22144, -0.21543, -0.22212, -0.21746, -0.22907, -0.21979, -0.22348},
			    { 0.44944,  0.28599,  0.14407,  0.11144, -0.05478,  0.29183,  0.73262,  0.41305,  0.32815,  0.40221,  0.27155, -0.05883,  0.51781, -0.95864,  0.50619,  0.48998,  0.45746,  0.28181,  0.34702,  0.21345,  0.00767,  0.17543,  0.08981,  0.60095,  1.37080,  0.14817}
  };

  Float_t CorrY17[2][30] = {
			    { 0.18977,  0.19063,  0.18213,  0.18075,  0.18329,  0.18048,  0.17638,  0.19102,  0.19262,  0.18147,  0.17512,  0.18685,  0.13889,  0.06893,  0.11551,  0.18900,  0.19124,  0.19108,  0.18961,  0.19130,  0.19687,  0.18997,  0.18230,  0.17799,  0.18896,  0.19185},
			    {-0.81791, -0.82962, -0.70378, -0.81633, -0.86727, -0.60734,  0.06559, -0.84229, -0.86870, -0.66418, -0.81704, -1.28693,  0.41798, -1.08253,  4.45522, -0.85222, -0.87097, -0.94241, -0.86314, -0.99273, -1.19589, -0.98970, -0.74295, -0.49715, -0.08160, -0.71623}
  };

  Float_t CorrX18[2][30] = {
			    { 0.36932,  0.36823,  0.36706,  0.35596,  0.36244,  0.35580,  0.33929,  0.37146,  0.36878,  0.36215,  0.33873,  0.32787,  0.31876,  0.39654,  0.37053,  0.36830,  0.37539,  0.36872,  0.37401,  0.36233,  0.34945,  0.35733,  0.37131,  0.37293},
			    {-0.63349, -0.45598, -0.26962, -0.05327, -0.06118,  0.33127,  0.97004, -0.69933, -0.45524, -0.27665, -0.11272,  0.39005,  1.00424, -1.19262, -0.71479, -0.49045, -0.46547, -0.29167, -0.26017,  0.24704,  1.07084,  1.19890, -0.15573, -0.20106}
  };

  Float_t CorrY18[2][30] = {
			    { 0.16222,  0.16134,  0.15640,  0.15556,  0.14897,  0.16251,  0.13978,  0.16288,  0.16006,  0.15597,  0.14723,  0.13580,  0.10599,  0.25030,  0.16446,  0.16131,  0.15697,  0.15941,  0.15681,  0.16584,  0.14153,  0.17736,  0.16102,  0.16130},
			    { 0.17247,  0.28078,  0.46748,  0.50691,  0.90131,  0.93393,  2.68076,  0.17611,  0.37681,  0.43407,  0.44823,  1.08324,  2.63591, -0.86965,  0.08191,  0.24246,  0.42478,  0.41988,  0.55998,  0.57113,  1.74098,  1.89765,  0.55174,  0.53932}
  };
 
  //function start//
  Double_t corrMET, corrMETPhi;
  corrMET = 0;
  corrMETPhi = 0;
  corrXY.clear();

  Int_t nVtx = data.GetInt("nGoodVtx");
  //Float_t pfMET = data.GetFloat("pfMET");//replace by input jet smearing corrected MET!!!
  Float_t pfMETPhi = data.GetFloat("pfMETPhi");

  Float_t pfMETX = pfMET * cos(pfMETPhi);
  Float_t pfMETY = pfMET * sin(pfMETPhi);
  
  string samplestr = sample;
  Int_t istr = samplestr.find("job");
  Int_t fstr = samplestr.find("/ggtree");
  Int_t lstr = fstr - istr;
  string samplename = samplestr.substr(istr, lstr);
  
  if(year == 2016){
    for(Int_t i=0; i<30; i++){
      if(samplename == samplelist16[i]){
	pfMETX -= CorrX16[0][i]*nVtx + CorrX16[1][i];
	pfMETY -= CorrY16[0][i]*nVtx + CorrY16[1][i];
	break;

      }
    }
    
  }
  else if(year == 2017){
     for(Int_t i=0; i<30; i++){
      if(samplename == samplelist17[i]){
	pfMETX -= CorrX17[0][i]*nVtx + CorrX17[1][i];
	pfMETY -= CorrY17[0][i]*nVtx + CorrY17[1][i];
	break;

      }
    }
  }
  else if(year == 2018){
    for(Int_t i=0; i<30; i++){
      if(samplename == samplelist18[i]){
	pfMETX -= CorrX18[0][i]*nVtx + CorrX18[1][i];
	pfMETY -= CorrY18[0][i]*nVtx + CorrY18[1][i];
      }
    }
  }

  TVector2 *CMET = new TVector2(pfMETX, pfMETY);
  corrMET = CMET->Mod();
  corrMETPhi = CMET->Phi();
  corrXY.push_back(corrMET);
  corrXY.push_back(corrMETPhi);

  //return corrMETPhi;
}


