void c1()
{
//=========Macro generated from canvas: c1/c1
//=========  (Mon Apr 11 19:01:32 2022) by ROOT version 6.24/02
   TCanvas *c1 = new TCanvas("c1", "c1",10,52,700,500);
   c1->Range(-0.3125,-25,2.8125,225);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   Double_t xAxis1[4] = {0, 1.4442, 1.5666, 2.5}; 
   
   TH2F *Scaling_Factors_PixelSeed_R9_Inclusive__1 = new TH2F("Scaling_Factors_PixelSeed_R9_Inclusive__1","Scaling_Factors_PixelSeed_R9_Inclusive",3, xAxis1,1,0,200);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetBinContent(6,0.9673);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetBinContent(8,0.9147);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetBinError(6,0.004448);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetBinError(8,0.0125);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetEntries(2);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContour(20);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(0,0);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(1,0.048365);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(2,0.09673);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(3,0.145095);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(4,0.19346);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(5,0.241825);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(6,0.29019);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(7,0.338555);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(8,0.38692);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(9,0.435285);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(10,0.48365);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(11,0.532015);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(12,0.58038);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(13,0.628745);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(14,0.67711);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(15,0.725475);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(16,0.77384);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(17,0.822205);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(18,0.87057);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetContourLevel(19,0.918935);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.695,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *ptstats_LaTex = ptstats->AddText("Scaling_Factors_PixelSeed_R9_Inclusive");
   ptstats_LaTex->SetTextSize(0.0368);
   ptstats_LaTex = ptstats->AddText("Entries = 2      ");
   ptstats_LaTex = ptstats->AddText("Mean x =  1.359");
   ptstats_LaTex = ptstats->AddText("Mean y =    100");
   ptstats_LaTex = ptstats->AddText("Std Dev x = 0.6553");
   ptstats_LaTex = ptstats->AddText("Std Dev y =      0");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   Scaling_Factors_PixelSeed_R9_Inclusive__1->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(Scaling_Factors_PixelSeed_R9_Inclusive__1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Scaling_Factors_PixelSeed_R9_Inclusive__1->SetLineColor(ci);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->GetXaxis()->SetLabelFont(42);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->GetXaxis()->SetTitleOffset(1);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->GetXaxis()->SetTitleFont(42);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->GetYaxis()->SetLabelFont(42);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->GetYaxis()->SetTitleFont(42);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->GetZaxis()->SetLabelFont(42);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->GetZaxis()->SetTitleOffset(1);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->GetZaxis()->SetTitleFont(42);
   Scaling_Factors_PixelSeed_R9_Inclusive__1->Draw("");
   
   TPaveText *pt = new TPaveText(0.1518911,0.9344958,0.8481089,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *pt_LaTex = pt->AddText("Scaling_Factors_PixelSeed_R9_Inclusive");
   pt->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
