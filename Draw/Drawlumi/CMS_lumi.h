#include "TPad.h"
#include "TLatex.h"
#include "TLine.h"
#include "TBox.h"
#include "TASImage.h"

//
// Global variables
//

TString cmsText     = "CMS";
float cmsTextFont   = 61;  // default is helvetic-bold

bool writeExtraText = false;
TString extraText   = "Preliminary";
float extraTextFont = 52;  // default is helvetica-italics

// text sizes and text offsets with respect to the top frame
// in unit of the top margin size
/* float lumiTextSize     = 0.6; */
/* float lumiTextOffset   = 0.2; */
/* float cmsTextSize      = 0.75; */
/* float cmsTextOffset    = 0.1;  // only used in outOfFrame version */

float lumiTextSize     = 0.6;//Stack
//float lumiTextSize     = 0.45;
float lumiTextOffset   = 0.1;
float cmsTextSize      = 0.7;//Stack
//float cmsTextSize      = 0.5;
float cmsTextOffset    = 0.05;  // only used in outOfFrame version

float relPosX    = -1;
float relPosY    = 0.035;
float relExtraDY = 1.2;

// ratio of "CMS" and extra text size
float extraOverCmsTextSize  = 0.76;//stack


TString lumi_13TeV = "36.2 fb^{-1}";
TString lumi_8TeV  = "19.7 fb^{-1}";
TString lumi_7TeV  = "5.1 fb^{-1}";
TString lumi_2016 = "36.33 fb^{-1}";
TString lumi_2017 = "41.48 fb^{-1}";
TString lumi_2018 = "59.83 fb^{-1}";

bool drawLogo      = false;

void CMS_lumi( TPad* pad, int iPeriod=3, int iPosX=10 );

