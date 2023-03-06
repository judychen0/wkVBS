#include "TMath.h"
#include <TMVA/Reader.h>
#include "TTree.h"
#include <string.h>
#include <stdio.h>
#include <vector>

using namespace std;

void METXYshift(Int_t year, char* sample, TreeReader &data, Float_t pfMET, vector<Float_t>& corrXY){
  
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
    "job_summer16_TT_aMCatNLO"
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
    "job_fall17_TT_aMCatNLO"			      
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
    "job_UL18_Znunug_VBS"
  };

  Float_t CorrX16[2][80] = {
    {-0.219, -0.222,
     -0.220, -0.206, -0.210, -0.228, -0.215, -0.210,
     -0.234, -0.206, -0.205, -0.216, -0.193, -0.210,
     -0.213, -0.216, -0.217, -0.211, -0.220,
     -0.216, -0.210, -0.211, -0.213,
     -0.216, -0.210, -0.208, -0.210, -0.183, -0.198, -0.199, -0.191, -0.119, -0.202, -0.068,
     -0.215, -0.215, -0.215, -0.216, -0.211, -0.215, -0.212, -0.223, -0.206, -0.203, -0.209, -0.209, -0.213, -0.157, -0.201, -0.206, -0.210,
     -0.211, -0.212, -0.210, -0.226, -0.214, -0.209, -0.213, -0.209, -0.221, -0.186, -0.212, -0.213
    },
    {-0.099, -0.101,
     -0.364, -0.519, -0.418,  0.044, -0.274, -0.473,
      0.104, -0.468, -0.475, -0.387, -0.857, -0.456,
     -0.370, -0.305, -0.355, -0.479, -0.245,
     -0.365, -0.398, -0.746, -0.642,
     -0.273, -0.439, -0.550, -0.519, -1.035, -0.839, -0.648, -0.697, -2.156, -0.404, -2.575,
     -0.292, -0.333, -0.377, -0.350, -0.534, -0.418, -0.530, -0.403, -0.739, -0.750, -0.646, -0.485, -0.471, -1.349, -0.718, -0.562, -0.096,
     -0.372, -0.450, -0.474, -0.216, -0.514, -0.708, -0.564, -0.731, -0.474, -1.168, -0.348, -0.666
    }
  };

  Float_t CorrY16[2][80] = {
    {-0.066, -0.055,
     -0.080, -0.054, -0.068, -0.051, -0.081, -0.056,
     -0.070, -0.046, -0.071, -0.084, -0.036, -0.068,
     -0.055, -0.067, -0.064, -0.076, -0.053,
     -0.061, -0.079, -0.079, -0.071,
     -0.066, -0.068, -0.074, -0.076, -0.114, -0.071, -0.084, -0.073, -0.097,  0.098, -0.103,
     -0.060, -0.063, -0.066, -0.065, -0.078, -0.070, -0.067, -0.071, -0.075, -0.075, -0.074  -0.102, -0.079, -0.124, -0.072, -0.109, -0.079,
     -0.055, -0.060, -0.059, -0.070, -0.062, -0.062, -0.066, -0.072, -0.068, -0.075, -0.077, -0.069
    },
    { 1.293,  0.842,
      1.526,  1.064,  1.310,  1.055,  1.558,  1.126,
      1.277,  0.843,  1.338,  1.679,  0.635,  1.378,
      1.214,  1.333,  1.384,  1.527,  1.084,
      1.228,  1.624,  1.578,  1.439,
      1.374,  1.409,  1.427,  1.466,  2.353,  1.351,  1.616,  1.183,  2.015, -1.315,  3.334,
      1.260,  1.302,  1.337,  1.326,  1.568,  1.362,  1.295,  1.344,  1.428,  1.394,  1.351,  1.848,  1.449,  2.644,  1.560,  3.171,  2.474,
      1.088,  1.202,  1.170,  1.382,  1.157,  1.209,  1.185,  1.267,  1.186,  1.554,  2.356,  1.465
    }
  };

  Float_t CorrX17[2][80] = {
    { -0.223, -0.212,
      -0.219, -0.222, -0.225, -0.222,
      -0.220, -0.217, -0.220, -0.213, -0.220, -0.216, -0.221,
      -0.217, -0.219, -0.219, -0.210, -0.193, -0.202, -0.154, -0.195,
      -0.172, -0.220, -0.219, -0.219, -0.214, -0.207, -0.212, -0.196,
      -0.219, -0.221, -0.221, -0.220, -0.222, -0.221, -0.215, -0.222, -0.217, -0.229, -0.219, -0.223
    },
    {  0.656,  0.313,
       0.547,  0.599,  0.553,  0.520,
       0.463,  0.445,  0.441,  0.319,  0.366, -0.066,  0.071,
       0.413,  0.328,  0.402,  0.271, -0.058,  0.517, -0.958,  0.506,
      -0.588,  0.449,  0.285,  0.144,  0.111, -0.054,  0.291,  0.732,
       0.502,  0.489,  0.457,  0.281,  0.347,  0.213,  0.007,  0.175,  0.089,  0.600,  1.370,  0.148
    }
  };

  Float_t CorrY17[2][80] = {
    {  0.190,  0.177,
       0.187,  0.185,  0.190,  0.196,
       0.189,  0.194,  0.189,  0.193,  0.191,  0.191,  0.191,
       0.191,  0.192,  0.181,  0.175,  0.186,  0.138,  0.068,  0.115,
      -0.065,  0.189,  0.190,  0.182,  0.180,  0.183,  0.180,  0.176,
       0.190,  0.189,  0.191,  0.191,  0.189,  0.191,  0.196,  0.189,  0.182,  0.177,  0.188,  0.191
    },
    { -0.983, -0.790,
      -0.882, -0.772, -0.947, -1.049,
      -0.824, -0.924, -0.805, -0.935, -0.936, -0.750, -0.770,
      -0.842, -0.868, -0.664, -0.817, -1.286,  0.417, -1.082,  4.455,
       0.954, -0.817, -0.829, -0.703, -0.816, -0.867, -0.607,  0.065,
      -0.872, -0.852, -0.870, -0.942, -0.863, -0.992, -1.195, -0.989, -0.742, -0.497, -0.081, -0.716
    }
  };

  Float_t CorrX18[2][80] = {
    {  0.371,  0.363,
       0.371,  0.370,  0.358,  0.369,
       0.369,  0.375,  0.365,  0.371,  0.375,  0.371,
       0.371,  0.368,  0.362,  0.338,  0.327,  0.318,  0.396,
       0.370,  0.369,  0.368,  0.367,  0.355,  0.362,  0.355,  0.339,
       0.370,  0.370,  0.368,  0.375,  0.368,  0.374,  0.362,  0.349,  0.357,  0.371,  0.372,
       0.107
    },
    { -0.981, -0.830,
      -0.761, -0.773, -0.543, -0.856,
      -0.691, -0.789, -0.612, -0.828, -0.816, -0.110,
      -0.699, -0.455, -0.276, -0.112,  0.390,  1.004, -1.192,
      -0.768, -0.633, -0.455, -0.269, -0.053, -0.061,  0.331,  0.970,
      -0.774, -0.714, -0.490, -0.465, -0.291, -0.260,  0.247,  1.070,  1.198, -0.155, -0.201,
       2.81
    }
  };

  Float_t CorrY18[2][80] = {
    {  0.162,  0.156,
       0.163,  0.162,  0.163,  0.158,
       0.162,  0.163,  0.164,  0.164,  0.149,  0.155,
       0.162,  0.160,  0.155,  0.147,  0.135,  0.105,  0.250,
       0.163,  0.162,  0.161,  0.156,  0.155,  0.148,  0.162,  0.139,
       0.165,  0.164,  0.161,  0.156,  0.159,  0.156,  0.165,  0.141,  0.177,  0.161,  0.161,
       0.258
    },
    {  0.026,  0.062,
       0.078,  0.219,  0.085,  0.268,
       0.130,  0.153,  0.156,  0.051,  0.502,  0.604,
       0.176,  0.376,  0.434,  0.448,  1.083,  2.635, -0.869,
       0.108,  0.172,  0.280,  0.467,  0.506,  0.901,  0.933,  2.680,
       0.023,  0.081,  0.242,  0.424,  0.419,  0.559,  0.571,  1.740,  1.897,  0.551,  0.539,
      -1.378
}
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
    for(Int_t i=0; i<80; i++){
      if(samplename == samplelist16[i]){
	pfMETX -= CorrX16[0][i]*nVtx + CorrX16[1][i];
	pfMETY -= CorrY16[0][i]*nVtx + CorrY16[1][i];
	break;

      }
    }
    
  }
  else if(year == 2017){
     for(Int_t i=0; i<80; i++){
      if(samplename == samplelist17[i]){
	pfMETX -= CorrX17[0][i]*nVtx + CorrX17[1][i];
	pfMETY -= CorrY17[0][i]*nVtx + CorrY17[1][i];
	break;

      }
    }
  }
  else if(year == 2018){
    for(Int_t i=0; i<80; i++){
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


