#include "canvas_margin.h"

void draw_mz(){

  gStyle->SetOptStat(0);
  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");

  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/SR/";
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/SR/mz/";

  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  vector<TString> samples = {"ZGto2LG", "WZTo3LNu_powheg", "ZZTo4L_powheg"};
  vector<TString> alias_samples = {"Z#gamma", "WZ", "ZZ"};
  vector<Color_t> color_samples = {kViolet, kSpring+7, kBlue, kRed-7};
  vector<int> sig_masses = {10, 30, 50, 70};

  TCanvas *c1 = new TCanvas("c1", "", 800, 800);
  canvas_margin(c1);
  c1->cd();
  TLegend *lg = new TLegend(0.7, 0.6, 0.95, 0.95);
  lg->SetBorderSize(0);
  lg->SetFillStyle(0);
  for(unsigned int i=0; i<samples.size(); i++){
    TFile *file = new TFile(filepath+"/trilepton_mumumu_SK"+samples.at(i)+"_dilep_cat_"+catversion+".root");
    TH1D *hist = (TH1D*)file->Get("CutStudy_m_Z_candidate");
    hist->SetLineColor(color_samples.at(i));
    hist->SetLineWidth(2);
    hist->Rebin(5);
    lg->AddEntry(hist, alias_samples.at(i), "l");
    hist->Scale(1./hist->Integral());
    hist->Draw("histsame");
    if(i==0){
      hist->SetXTitle("m(Z-candidate) [GeV]");
      hist->SetYTitle("A.U.");
      hist_axis(hist);
      hist->GetYaxis()->SetRangeUser(0, 0.5);
      hist->GetXaxis()->SetRangeUser(0., 150.);
    }
  }
  for(unsigned int i=0; i<sig_masses.size(); i++){
    TFile *file = new TFile(filepath+"/trilepton_mumumu_SKHN_MuMuMu_"+TString::Itoa(sig_masses.at(i),10)+"_cat_"+catversion+".root");
    TH1D *hist = (TH1D*)file->Get("CutStudy_m_Z_candidate");
    hist->SetLineColor(color_samples.at(i));
    hist->SetLineWidth(3);
    hist->SetLineColor(kBlack);
    hist->SetLineStyle(i+1);
    hist->Rebin(5);
    lg->AddEntry(hist, "HN"+TString::Itoa(sig_masses.at(i),10), "l");
    hist->Scale(1./hist->Integral());
    hist->Draw("histsame");
  }

  lg->Draw();
  c1->SaveAs(plotpath+"./mz.pdf");
  
}
