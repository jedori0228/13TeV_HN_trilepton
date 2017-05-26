#include "mylib.h"
#include "canvas_margin.h"

void TrkVVL_vs_NoBias_FR(double dXYMin=4.0, double RelIsoMax=0.4, int period=0){

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);
  gStyle->SetOptStat(0);

  int Digit1 = int(dXYMin);
  int Digit0p1 = 10*dXYMin-10*Digit1;
  TString str_dXYCut = "dXYSigMin_"+TString::Itoa(Digit1,10)+"p"+TString::Itoa(Digit0p1,10);

  int iso_Digit1 = int(RelIsoMax);
  int iso_Digit0p1 = 10*RelIsoMax-10*iso_Digit1;
  TString str_iso = "LooseRelIsoMax_"+TString::Itoa(iso_Digit1,10)+"p"+TString::Itoa(iso_Digit0p1,10);

  str_dXYCut = str_dXYCut+"_"+str_iso;

  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");

  TString WhichRootFile = "NoBias";

  TFile *file_TrkVVL = new TFile(WORKING_DIR+"/plots/"+dataset+"/FR_syst/TrkVVL/Trilep_Muon_FakeRate.root");
  TFile *file_NoBias = new TFile(WORKING_DIR+"/plots/"+dataset+"/FR_syst/NoBias/Trilep_Muon_FakeRate.root");

  TH2D *hist_TrkVVL = (TH2D*)file_TrkVVL->Get(str_dXYCut+"_FR_alljet_sfed");
  TH2D *hist_NoBias = (TH2D*)file_NoBias->Get(str_dXYCut+"_FR_alljet_sfed");

  TH2D *hist_diff = (TH2D*)hist_TrkVVL->Clone();
  hist_diff->Add(hist_NoBias, -1.);

  TCanvas* c_diff = new TCanvas("c_diff", "", 1600, 1100);
  c_diff->SetLeftMargin(0.07);
  c_diff->SetRightMargin( 0.1 );
  gStyle->SetPaintTextFormat("0.4f");
  hist_diff->Draw("colztexte1");
  hist_diff->GetXaxis()->SetRangeUser(5, 60);
  hist_diff->SetXTitle("p_{T} [GeV/c]");
  hist_diff->SetYTitle("|#eta|");
  hist_diff->SetTitle("");
  hist_diff->SetMarkerSize(1.3);
  c_diff->SaveAs("Diff.pdf");
  c_diff->Close();

  TCanvas* c_ratio = new TCanvas("c_ratio", "", 1600, 1100);
  c_ratio->SetLeftMargin(0.07);
  c_ratio->SetRightMargin( 0.1 );
  gStyle->SetPaintTextFormat("0.4f");
  TH2D *hist_ratio = (TH2D*)hist_diff->Clone();
  for(int i=1; i<=hist_ratio->GetXaxis()->GetNbins(); i++){
    for(int j=1; j<=hist_ratio->GetYaxis()->GetNbins(); j++){
      hist_ratio->SetBinContent(i, j, 100.*fabs(hist_ratio->GetBinContent(i,j))/hist_TrkVVL->GetBinContent(i,j));
    }
  }
  hist_ratio->Draw("colztexte1");
  hist_ratio->GetXaxis()->SetRangeUser(5, 60);
  hist_ratio->SetXTitle("p_{T} [GeV/c]");
  hist_ratio->SetYTitle("|#eta|");
  hist_ratio->SetTitle("");
  hist_ratio->SetMarkerSize(1.3);
  c_ratio->SaveAs("Ratio.pdf");

}
