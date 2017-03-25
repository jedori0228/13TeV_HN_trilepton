#include "canvas_margin.h"

void CutStudy_mz(){

  gStyle->SetOptStat(0);
  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");

  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/SR/";
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/CutStudy/";

  //==== Select this_cut_left <= <cut var> <= this_cut_right
  double cut_left = 91.1876-15.;
  double cut_right = 91.1876+15.;

  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  vector<TString> samples = {"ZGto2LG", "WZTo3LNu_powheg", "ZZTo4L_powheg", "fake_sfed_HighdXY"};
  vector<TString> alias_samples = {"Z#gamma", "WZ", "ZZ", "Misd."};
  vector<Color_t> color_samples = {kSpring-7, kGreen, kRed-7, kAzure+10};
  vector<int> sig_masses = {10, 40, 200, 500};

  TCanvas *c1 = new TCanvas("c1", "", 800, 800);
  canvas_margin(c1);
  c1->cd();
  TLegend *lg = new TLegend(0.7, 0.6, 0.95, 0.95);
  lg->SetBorderSize(0);
  lg->SetFillStyle(0);
  for(unsigned int i=0; i<samples.size(); i++){
    TFile *file = new TFile(filepath+"/trilepton_mumumu_SK"+samples.at(i)+"_dilep_cat_"+catversion+".root");
    TH1D *hist = (TH1D*)file->Get("CutStudy_m_Z_candidate");

    int bin_cut_left = hist->GetXaxis()->FindBin(cut_left);
    int bin_cut_right = hist->GetXaxis()->FindBin(cut_right);
    double TotalIntegral = hist->Integral(-1,hist->GetXaxis()->GetNbins()+1);
    cout << samples.at(i) << " : (|m(ll)-m(Z)| > 15 GeV) = " << 1.-hist->Integral(bin_cut_left, bin_cut_right)/TotalIntegral << endl;

    hist->SetLineColor(color_samples.at(i));
    hist->SetFillColor(color_samples.at(i));
    hist->SetFillStyle(3305);
    hist->SetLineWidth(1);
    hist->Rebin(5);
    lg->AddEntry(hist, alias_samples.at(i), "f");
    hist->Scale(1./hist->Integral(-1,hist->GetXaxis()->GetNbins()+1));
    hist->Draw("histsame");
    if(i==0){
      hist->SetXTitle("m(Z-candidate) [GeV]");
      hist->SetYTitle("A. U.");
      hist_axis(hist);
      hist->GetYaxis()->SetRangeUser(0, 0.5);
      hist->GetXaxis()->SetRangeUser(0., 150.);
    }
  }
  for(unsigned int i=0; i<sig_masses.size(); i++){
    TFile *file = new TFile(filepath+"/trilepton_mumumu_SKHN_MuMuMu_"+TString::Itoa(sig_masses.at(i),10)+"_cat_"+catversion+".root");
    TH1D *hist = (TH1D*)file->Get("CutStudy_m_Z_candidate");

    int bin_cut_left = hist->GetXaxis()->FindBin(cut_left);
    int bin_cut_right = hist->GetXaxis()->FindBin(cut_right);
    double TotalIntegral = hist->Integral(-1,hist->GetXaxis()->GetNbins()+1);
    cout << sig_masses.at(i) << " : (|m(ll)-m(Z)| > 15 GeV) = " << 1.-hist->Integral(bin_cut_left, bin_cut_right)/TotalIntegral << endl;

    hist->SetLineColor(color_samples.at(i));
    hist->SetLineWidth(3);
    hist->SetLineColor(kBlack);
    hist->SetLineStyle(i+1);
    hist->Rebin(5);
    lg->AddEntry(hist, "HN ("+TString::Itoa(sig_masses.at(i),10)+" GeV)", "l");
    hist->Scale(1./hist->Integral(-1,hist->GetXaxis()->GetNbins()+1));
    hist->Draw("histsame");
  }

  lg->Draw();
  c1->SaveAs(plotpath+"./mz.pdf");
  
}
