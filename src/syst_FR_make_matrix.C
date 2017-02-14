#include "canvas_margin.h"
#define n_xbins 9
#define n_ybins 4

void syst_FR_make_matrix(){

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);
  gStyle->SetOptStat(0);
  TH1::AddDirectory(kFALSE);
  //gStyle->SetPaintTextFormat("0.4f");

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");  
  TString dataset = getenv("CATANVERSION");

  TString filepath = WORKING_DIR+"/plots/"+dataset+"/FakeRateCalculator/";
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/FR_syst/";
  
  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
  
  vector<double> dXYMins = {3.0, 4.0, 5.0};
  double bins_dXYMins[] =  {3.0, 4.0, 5.0, 6.0};
  vector<double> RelIsoMaxs = {0.2, 0.3, 0.4, 0.6, 0.8, 1.0};
  double bins_RelIsoMaxs[] =  {0.2, 0.3, 0.4, 0.6, 0.8, 1.0, 1.2};
  
  TH1D *hist_dXYMins = new TH1D("hist_dXYMins", "", dXYMins.size(), 0., dXYMins.size());
  TH1D *hist_RelIsoMaxs = new TH1D("hist_RelIsoMaxs", "", RelIsoMaxs.size(), 0., RelIsoMaxs.size());
  for(int aaa=0; aaa<dXYMins.size(); aaa++) hist_dXYMins->SetBinContent(aaa+1, dXYMins.at(aaa));
  for(int bbb=0; bbb<RelIsoMaxs.size(); bbb++) hist_RelIsoMaxs->SetBinContent(bbb+1, RelIsoMaxs.at(bbb));

  TH1I* hist_bins = new TH1I("hist_bins", "", 2, 0, 2);
  int n_pt_bins = n_pt_bins = 7; // pt : 10-15-20-25-30-35-45-60
  int n_eta_bins = 4; // eta : 0.0-0.8-1.479-2.0-2.5
  hist_bins->SetBinContent(1, n_pt_bins);
  hist_bins->SetBinContent(2, n_eta_bins);
  
  TFile *file_FRs = new TFile(plotpath+"Trilep_Muon_FakeRate.root", "RECREATE");
  file_FRs->cd();
  hist_dXYMins->Write();
  hist_RelIsoMaxs->Write();
  hist_bins->Write();
  
  TH2D *hist_n_muons = new TH2D("hist_n_muons", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_HighdXY_FR = new TH2D("hist_HighdXY_FR", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_QCD_FR_Large = new TH2D("hist_QCD_FR_Large", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_QCD_FR_Small = new TH2D("hist_QCD_FR_Small", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_QCD_FRSF = new TH2D("hist_QCD_FRSF", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_TagZ_FR_Large = new TH2D("hist_TagZ_FR_Large", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_TagZ_FR_Small = new TH2D("hist_TagZ_FR_Small", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_TagZ_FRSF = new TH2D("hist_TagZ_FRSF", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  
  for(int aaa=0; aaa<dXYMins.size(); aaa++){
    for(int bbb=0; bbb<RelIsoMaxs.size(); bbb++){

      int Digit1 = int(dXYMins.at(aaa));
      int Digit0p1 = 10*dXYMins.at(aaa)-10*Digit1;
      TString str_dXYCut = "dXYSigMin_"+TString::Itoa(Digit1,10)+"p"+TString::Itoa(Digit0p1,10);
      //cout << str_dXYCut << endl;

      int iso_Digit1 = int(RelIsoMaxs[bbb]);
      int iso_Digit0p1 = 10*RelIsoMaxs[bbb]-10*iso_Digit1;
      TString str_iso = "LooseRelIsoMax_"+TString::Itoa(iso_Digit1,10)+"p"+TString::Itoa(iso_Digit0p1,10);

      str_dXYCut = str_dXYCut+"_"+str_iso;
      
      //cout << "["<<str_dXYCut<<"]"<< endl;

      //==== Get HigdXY FR rootfile
      TFile *file_FR = new TFile(filepath+str_dXYCut+"/13TeV_trimuon_FR_SingleMuonTrigger_HighdXY.root");
      //==== FR matrix
      TH2D *hist_FR = (TH2D*)file_FR->Get(str_dXYCut+"_SingleMuonTrigger_HighdXY_events_F")->Clone();
      hist_FR->SetName(str_dXYCut+"_FR");
      //==== # of muons in Denomonator
      TH1D *hist_n_data_prompt_subtraction_den = (TH1D*)file_FR->Get("hist_n_data_prompt_subtraction_den");
      TH1D *hist_n_data_prompt_subtraction_num = (TH1D*)file_FR->Get("hist_n_data_prompt_subtraction_num");
      hist_n_muons->SetBinContent(aaa+1, bbb+1, hist_n_data_prompt_subtraction_den->GetBinContent(1));
      //==== one-binned FR
      hist_HighdXY_FR->SetBinContent(aaa+1, bbb+1, hist_n_data_prompt_subtraction_num->GetBinContent(1)/hist_n_data_prompt_subtraction_den->GetBinContent(1));
      
      //==== Get QCD FR/FRSF
      //==== 2D
      TFile *file_FR_QCD = new TFile(filepath+str_dXYCut+"/13TeV_trimuon_FR_SingleMuonTrigger_QCD_mu.root");
      TH2D *hist_FR_QCD = (TH2D*)file_FR_QCD->Get("FR_Small_dXYSig")->Clone();
      TH2D *hist_FRSF_QCD = (TH2D*)file_FR_QCD->Get("FRSF")->Clone();
      hist_FR_QCD->SetName(str_dXYCut+"_FR_QCD");
      hist_FRSF_QCD->SetName(str_dXYCut+"_FRSF_QCD");
      //=== 1D
      TH1D *hist_QCD_tmp = (TH1D*)file_FR_QCD->Get("hist_FR_QCD");
      hist_QCD_FR_Large->SetBinContent(aaa+1, bbb+1, hist_QCD_tmp->GetBinContent(1));
      hist_QCD_FR_Small->SetBinContent(aaa+1, bbb+1, hist_QCD_tmp->GetBinContent(2));
      hist_QCD_FRSF->SetBinContent(aaa+1, bbb+1, hist_QCD_tmp->GetBinContent(3));

      file_FRs->cd();
      hist_FR->Write(); 
      hist_FR_QCD->Write();
      hist_FRSF_QCD->Write();

      //==== Get TagZ FR/FRSF
      TFile *file_TagZ_tmp = new TFile(filepath+str_dXYCut+"/13TeV_trimuon_FR_DiMuonTrigger_TagZ.root");
      TH1D *hist_TagZ_tmp = (TH1D*)file_TagZ_tmp->Get("hist_FR_TagZ");
      hist_TagZ_FR_Large->SetBinContent(aaa+1, bbb+1, hist_TagZ_tmp->GetBinContent(1));
      hist_TagZ_FR_Small->SetBinContent(aaa+1, bbb+1, hist_TagZ_tmp->GetBinContent(2));
      hist_TagZ_FRSF->SetBinContent(aaa+1, bbb+1, hist_TagZ_tmp->GetBinContent(3));

    }
  }

  file_FRs->Close();

  //==== n_muons
  TCanvas *c_n_muons = new TCanvas("c_n_muons", "", 800, 800);
  canvas_margin(c_n_muons);
  c_n_muons->cd();
  hist_n_muons->SetMarkerSize(2);
  hist_n_muons->Draw("coltext");
  hist_n_muons->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_n_muons->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_n_muons);
  //TGaxis::SetMaxDigits(2);
  c_n_muons->SaveAs(plotpath+"n_muons.png");

  //==== HighdXY one-binned FR
  TCanvas *c_HighdXY_FR = new TCanvas("c_HighdXY_FR", "", 800, 800);
  canvas_margin(c_HighdXY_FR);
  c_HighdXY_FR->cd();
  hist_HighdXY_FR->SetMarkerSize(2);
  hist_HighdXY_FR->Draw("coltext");
  hist_HighdXY_FR->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_HighdXY_FR->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_HighdXY_FR);
  c_HighdXY_FR->SaveAs(plotpath+"/HighdXY_FR.png");

  //==== QCD Large
  TCanvas *c_QCD_FR_Large = new TCanvas("c_QCD_FR_Large", "", 800, 800);
  canvas_margin(c_QCD_FR_Large);
  c_QCD_FR_Large->cd();
  hist_QCD_FR_Large->SetMarkerSize(2);
  hist_QCD_FR_Large->Draw("coltext");
  hist_QCD_FR_Large->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_QCD_FR_Large->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_QCD_FR_Large);
  c_QCD_FR_Large->SaveAs(plotpath+"/QCD_FR_Large.png");
  //==== QCD Small
  TCanvas *c_QCD_FR_Small = new TCanvas("c_QCD_FR_Small", "", 800, 800);
  canvas_margin(c_QCD_FR_Small);
  c_QCD_FR_Small->cd();
  hist_QCD_FR_Small->SetMarkerSize(2);
  hist_QCD_FR_Small->Draw("coltext");
  hist_QCD_FR_Small->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_QCD_FR_Small->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_QCD_FR_Small);
  c_QCD_FR_Small->SaveAs(plotpath+"/QCD_FR_Small.png");
  //==== QCD SF
  TCanvas *c_QCD_FRSF = new TCanvas("c_QCD_FRSF", "", 800, 800);
  canvas_margin(c_QCD_FRSF);
  c_QCD_FRSF->cd();
  hist_QCD_FRSF->SetMarkerSize(2);
  hist_QCD_FRSF->Draw("coltexte1");
  hist_QCD_FRSF->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_QCD_FRSF->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_QCD_FRSF);
  c_QCD_FRSF->SaveAs(plotpath+"/QCD_FRSF.png");

  //==== TagZ Large
  TCanvas *c_TagZ_FR_Large = new TCanvas("c_TagZ_FR_Large", "", 800, 800);
  canvas_margin(c_TagZ_FR_Large);
  c_TagZ_FR_Large->cd();
  hist_TagZ_FR_Large->SetMarkerSize(2);
  hist_TagZ_FR_Large->Draw("coltext");
  hist_TagZ_FR_Large->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_TagZ_FR_Large->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_TagZ_FR_Large);
  c_TagZ_FR_Large->SaveAs(plotpath+"/TagZ_FR_Large.png");
  //==== TagZ Small
  TCanvas *c_TagZ_FR_Small = new TCanvas("c_TagZ_FR_Small", "", 800, 800);
  canvas_margin(c_TagZ_FR_Small);
  c_TagZ_FR_Small->cd();
  hist_TagZ_FR_Small->SetMarkerSize(2);
  hist_TagZ_FR_Small->Draw("coltexte1");
  hist_TagZ_FR_Small->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_TagZ_FR_Small->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_TagZ_FR_Small);
  c_TagZ_FR_Small->SaveAs(plotpath+"/TagZ_FR_Small.png");
  //==== TagZ SF
  TCanvas *c_TagZ_FRSF = new TCanvas("c_TagZ_FRSF", "", 800, 800);
  canvas_margin(c_TagZ_FRSF);
  c_TagZ_FRSF->cd();
  hist_TagZ_FRSF->SetMarkerSize(2);
  hist_TagZ_FRSF->Draw("coltext");
  hist_TagZ_FRSF->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_TagZ_FRSF->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_TagZ_FRSF);
  c_TagZ_FRSF->SaveAs(plotpath+"/TagZ_FRSF.png");

 

}


