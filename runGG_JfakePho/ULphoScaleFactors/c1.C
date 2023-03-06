void c1()
{
//=========Macro generated from canvas: c1/c1
//=========  (Wed Mar 23 13:43:29 2022) by ROOT version 6.24/02
   TCanvas *c1 = new TCanvas("c1", "c1",10,52,700,500);
   c1->Range(-0.7500001,0.835,6.75,1.085);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TH1F *SF_HasPix_MediumID__1 = new TH1F("SF_HasPix_MediumID__1","",6,0,6);
   SF_HasPix_MediumID__1->SetBinContent(1,0.9699591);
   SF_HasPix_MediumID__1->SetBinContent(2,0.9702275);
   SF_HasPix_MediumID__1->SetBinContent(3,0.9740562);
   SF_HasPix_MediumID__1->SetBinContent(4,0.9031286);
   SF_HasPix_MediumID__1->SetBinContent(5,0.904644);
   SF_HasPix_MediumID__1->SetBinContent(6,0.9228234);
   SF_HasPix_MediumID__1->SetBinError(1,0.002348222);
   SF_HasPix_MediumID__1->SetBinError(2,0.002941756);
   SF_HasPix_MediumID__1->SetBinError(3,0.002114436);
   SF_HasPix_MediumID__1->SetBinError(4,0.007726773);
   SF_HasPix_MediumID__1->SetBinError(5,0.007540686);
   SF_HasPix_MediumID__1->SetBinError(6,0.01364669);
   SF_HasPix_MediumID__1->SetMinimum(0.86);
   SF_HasPix_MediumID__1->SetMaximum(1.06);
   SF_HasPix_MediumID__1->SetEntries(6);
   SF_HasPix_MediumID__1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   SF_HasPix_MediumID__1->SetLineColor(ci);
   SF_HasPix_MediumID__1->SetMarkerStyle(20);
   SF_HasPix_MediumID__1->GetXaxis()->SetBinLabel(1,"EB Inc.");
   SF_HasPix_MediumID__1->GetXaxis()->SetBinLabel(2,"EB high R9");
   SF_HasPix_MediumID__1->GetXaxis()->SetBinLabel(3,"EB low R9");
   SF_HasPix_MediumID__1->GetXaxis()->SetBinLabel(4,"EE Inc.");
   SF_HasPix_MediumID__1->GetXaxis()->SetBinLabel(5,"EE high R9");
   SF_HasPix_MediumID__1->GetXaxis()->SetBinLabel(6,"EE low R9");
   SF_HasPix_MediumID__1->GetXaxis()->SetLabelFont(42);
   SF_HasPix_MediumID__1->GetXaxis()->SetTitleOffset(1);
   SF_HasPix_MediumID__1->GetXaxis()->SetTitleFont(42);
   SF_HasPix_MediumID__1->GetYaxis()->SetTitle("Scale Factors");
   SF_HasPix_MediumID__1->GetYaxis()->SetLabelFont(42);
   SF_HasPix_MediumID__1->GetYaxis()->SetTitleFont(42);
   SF_HasPix_MediumID__1->GetZaxis()->SetLabelFont(42);
   SF_HasPix_MediumID__1->GetZaxis()->SetTitleOffset(1);
   SF_HasPix_MediumID__1->GetZaxis()->SetTitleFont(42);
   SF_HasPix_MediumID__1->Draw("");
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
