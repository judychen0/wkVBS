#include "TMath.h"
#include <TMVA/Reader.h>
#include "TTree.h"
#include <string.h>
#include <stdio.h>
#include <vector>

using namespace std;

void METXYshift(Int_t year, char* sample, TreeReader &data, Float_t pfMET, vector<Float_t>& corrXY){
  
  string samplelist16[30] = {
			     "job_summer16_GJets_MG_HT200to400",
			     "job_summer16_GJets_MG_HT400to600",
			     "job_summer16_GJets_MG_HT600toInf",
			     "job_summer16_Wg_aMCatNLO",
			     "job_summer16_Wg_aMCatNLO_ext1",
			     "job_summer16_Zg_aMCatNLO",
			     "job_summer16_TT_aMCatNLO",
			     "job_summer16_Znunug_pt130",
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
			     "job_summer16_WZ_ext1",
			     "job_summer16_ZZ",
			     "job_summer16_ZZ_ext1",
			     "job_summer16_WWG",
			     "job_summer16_WZG"
  };

  string samplelist17[30] = {
			     "job_fall17_GJets_MG_HT100to200",
			     "job_fall17_GJets_MG_HT200to400",
			     "job_fall17_GJets_MG_HT400to600",
			     "job_fall17_GJets_MG_HT600toInf",
			     "job_fall17_TTGjets",
			     "job_fall17_TTGjets_ext1",
			     "job_fall17_WW",
			     "job_fall17_WZ",
			     "job_fall17_ZZ",
			     "job_fall17_WWG",
			     "job_fall17_WZG",
			     "job_fall17_EWK_Wminus",
			     "job_fall17_EWK_Wplus",
			     "job_fall17_EWK_Zll",
			     "job_fall17_EWK_Zvv"
  };

  string samplelist18[30] = {
			     "job_autumn18_GJets_MG_HT100to200",
			     "job_autumn18_GJets_MG_HT200to400",
			     "job_autumn18_GJets_MG_HT400to600",
			     "job_autumn18_GJets_MG_HT600toInf",
			     "job_autumn18_TTGjets",
			     "job_autumn18_WW",
			     "job_autumn18_WZ",
			     "job_autumn18_ZZ",
			     "job_autumn18_WWG",
			     "job_autumn18_WZG",
			     "job_autumn18_EWK_Wminus",
			     "job_autumn18_EWK_Wplus",
			     "job_autumn18_EWK_Zll",
			     "job_autumn18_EWK_Zvv"
  };

  Float_t CorrX16[2][30] = {
    {-0.2131, -0.2141, -0.2128, -0.2158, -0.2135, -0.2125, -0.2135, 0.001,
    -0.21744, -0.20221, -0.21364, -0.22548, -0.21517, -0.20994, -0.22347, -0.21078, -0.20487, -0.22389, -0.18355, -0.20954, -0.21745, -0.21592, -0.22011, -0.21227, -0.21595, -0.20976, -0.20705, -0.21712},
    {-0.5195, -0.6019, -0.6671, -0.2198, -0.2539, -0.2701, -0.6740,
    -0.43835, -0.59998, -0.34241, -0.02688, -0.28797, -0.47705, -0.15731, -0.36865, -0.47875, -0.28434, -1.01487, -0.46797, -0.29716, -0.32971, -0.31105, -0.35398, -0.37227, -0.45407, -0.56949, -0.25175}
  };

  Float_t CorrY16[2][30] = {
    {-0.04922, -0.05463, -0.06143, -0.05038, -0.05179, -0.04736, -0.06258, 0.001,
-0.07015, -0.05401, -0.06097, -0.05254, -0.06492, -0.04835, -0.06707, -0.05262, -0.05828, -0.05977, -0.02674, -0.05872, -0.04873, -0.05683, -0.05478, -0.04603, -0.07087, -0.05141, -0.04655, -0.05705},
			    { 0.85109,  0.84161,  0.93745,  0.96791,  0.99696,  0.89369,  1.28987,
 1.33980,  1.00653,  1.12129,  1.07263,  1.15678,  0.91949,  1.20548,  0.95012,  1.04654,  1.19218,  0.35964,  1.14722,  1.05639,  1.09394,  1.16075,  0.94682,  1.36527,  1.03329,  0.89357,  1.17579}
  };

  Float_t CorrX17[2][30] = {
			    {-0.22311, -0.22344, -0.22117, -0.22160, -0.21695, -0.22193, -0.22055, -0.21798, -0.22095, -0.21365, -0.22085, -0.21983, -0.22269, -0.22517, -0.22251},
			    { 0.51635,  0.35646,  0.16826,  0.10588, -0.06691,  0.07188,  0.46392,  0.44572,  0.44105,  0.31996,  0.36605,  0.54740,  0.59935,  0.55364,  0.52028}
  };

  Float_t CorrY17[2][30] = {
			    { 0.19029,  0.18965,  0.18701,  0.18580,  0.19116,  0.19168,  0.18981,  0.19413,  0.18935,  0.19375,  0.19199,  0.18761,  0.18584,  0.19038,  0.19687},
			    {-0.88922, -0.90883, -0.95621, -0.94700, -0.75053, -0.77026, -0.82476, -0.92406, -0.80568, -0.93561, -0.93698, -0.88240, -0.77275, -0.94731, -1.04923}
  };

  Float_t CorrX18[2][30] = {
			    { 0.36885,  0.37091,  0.37130,  0.97312,  0.37121,  0.93916,  0.37514,  0.36591,  0.37135,  0.37542,  0.37169,  0.37016,  0.35898,  0.36979},
			    {-0.63814, -0.54707, -0.36567, -0.13726, -0.11064, -0.69189, -0.78981, -0.61235, -0.82881, -0.81600, -0.76181, -0.77354, -0.54339, -0.85624}
  };

  Float_t CorrY18[2][30] = {
			    { 0.16710,  0.16523,  0.16324,  0.16268,  0.15573,  0.16286,  0.16334,  0.16416,  0.16416,  0.14927,  0.16357,  0.16267,  0.16326,  0.15839},
			    {-0.00389,  0.09967,  0.22657,  0.46137,  0.60498,  0.13056,  0.15321,  0.15636,  0.05169,  0.50274,  0.07835,  0.21964,  0.08550,  0.26818}
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


