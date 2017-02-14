#include "canvas_margin.h"
#define n_xbins 9
#define n_ybins 4

double sumw2ratioerror(double A, double dA, double B, double dB);

void syst_FR_get_unct(){
  
  gStyle->SetOptStat(0);
  TH1::AddDirectory(kFALSE);

  TString cmssw_version = getenv("CATVERSION");
  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");  
  TString dataset = getenv("CATANVERSION");

  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/FR_syst/";
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/FR_syst/";

  TString channel = "Preselection";
  
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
  
  double dXYMin_cent = 4.0;
  double RelIsoMax_cent = 0.4;
  double n_events_cent, error_cent;
 
  TH2D *hist_yield = new TH2D("hist_yield", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_n_TTL = new TH2D("hist_n_TTL", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_n_TLL = new TH2D("hist_n_TLL", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_n_LLL = new TH2D("hist_n_LLL", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  
  TFile *file_tmp = new TFile(filepath+"/trilepton_mumumu_syst_FR_data_DoubleMuon_cat_"+cmssw_version+".root");
  
  for(int aaa=0; aaa<dXYMins.size(); aaa++){
    for(int bbb=0; bbb<RelIsoMaxs.size(); bbb++){
      
      int Digit1 = int(dXYMins.at(aaa));
      int Digit0p1 = 10*dXYMins.at(aaa)-10*Digit1;
      TString str_dXYCut = "dXYSigMin_"+TString::Itoa(Digit1,10)+"p"+TString::Itoa(Digit0p1,10);
      //cout << str_dXYCut << endl;
      
      int iso_Digit1 = int(RelIsoMaxs.at(bbb));
      int iso_Digit0p1 = 10*RelIsoMaxs.at(bbb)-10*iso_Digit1;
      TString str_iso = "LooseRelIsoMax_"+TString::Itoa(iso_Digit1,10)+"p"+TString::Itoa(iso_Digit0p1,10);
      
      str_dXYCut = str_dXYCut+"_"+str_iso;
      
      //cout << "["<<str_dXYCut<<"]"<< endl;
      
      TH1D *hist_tmp_n_events = (TH1D*)file_tmp->Get(str_dXYCut+"_n_events_"+channel)->Clone();
      TH1D *hist_tmp_n_events_up = (TH1D*)file_tmp->Get(str_dXYCut+"_n_events_"+channel+"_up")->Clone();
      TH1D *hist_tmp_n_events_down = (TH1D*)file_tmp->Get(str_dXYCut+"_n_events_"+channel+"_down")->Clone();
      TH1D *hist_tmp_Loose = (TH1D*)file_tmp->Get(str_dXYCut+"_n_OnlyLoose_"+channel)->Clone();

      double n_yield_cent = hist_tmp_n_events->GetBinContent(1);
      double n_yield_up = hist_tmp_n_events_up->GetBinContent(1);
      double n_yield_down = hist_tmp_n_events_down->GetBinContent(1);
      double n_yield_error_propagated = n_yield_up-n_yield_cent;
      double n_yeild_error_sumw2 = hist_tmp_n_events->GetBinError(1);

      hist_yield->SetBinContent(aaa+1, bbb+1, n_yield_cent);
      hist_yield->SetBinError(aaa+1, bbb+1, sqrt( n_yield_error_propagated*n_yield_error_propagated + n_yeild_error_sumw2*n_yeild_error_sumw2 ));

      hist_n_TTL->SetBinContent(aaa+1, bbb+1, hist_tmp_Loose->GetBinContent(2));
      hist_n_TLL->SetBinContent(aaa+1, bbb+1, hist_tmp_Loose->GetBinContent(3));
      hist_n_LLL->SetBinContent(aaa+1, bbb+1, hist_tmp_Loose->GetBinContent(4));

      if(dXYMins.at(aaa)==dXYMin_cent && RelIsoMaxs.at(bbb)==RelIsoMax_cent){
        n_events_cent = n_yield_cent;
        error_cent = sqrt( n_yield_error_propagated*n_yield_error_propagated + n_yeild_error_sumw2*n_yeild_error_sumw2 );
      }
      
    }
  }
  
  //==== TTL
  TCanvas *c_TTL = new TCanvas("c_TTL", "", 800, 800);
  canvas_margin(c_TTL);
  c_TTL->cd();
  hist_n_TTL->SetMarkerSize(2);
  hist_n_TTL->Draw("coltext");
  hist_n_TTL->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_n_TTL->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_n_TTL);
  //TGaxis::SetMaxDigits(2);
  c_TTL->SaveAs(plotpath+"/n_TTL.png");
  c_TTL->Close();

  //==== TLL
  TCanvas *c_TLL = new TCanvas("c_TLL", "", 800, 800);
  canvas_margin(c_TLL);
  c_TLL->cd();
  hist_n_TLL->SetMarkerSize(2);
  hist_n_TLL->Draw("coltext");
  hist_n_TLL->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_n_TLL->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_n_TLL);
  //TGaxis::SetMaxDigits(2);
  c_TLL->SaveAs(plotpath+"/n_TLL.png");
  c_TLL->Close();

  //==== LLL
  TCanvas *c_LLL = new TCanvas("c_LLL", "", 800, 800);
  canvas_margin(c_LLL);
  c_LLL->cd();
  hist_n_LLL->SetMarkerSize(2);
  hist_n_LLL->Draw("coltext");
  hist_n_LLL->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_n_LLL->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_n_LLL);
  //TGaxis::SetMaxDigits(2);
  c_LLL->SaveAs(plotpath+"/n_LLL.png");
  c_LLL->Close();

  //==== HARD CODED
  //==== # of (TTL/TLL/LLL) vs RelIso
  TCanvas *c_n_Loose_curves = new TCanvas("c_n_Loose_curves", "", 800, 800);
  canvas_margin(c_n_Loose_curves);
  c_n_Loose_curves->cd();
  double relisos[] =  {0.2, 0.3, 0.4, 0.6, 0.8, 1.0};
  double n_TTL[] = {1517, 2524, 3484, 5281, 6945, 8382};
  double n_TLL[] = {248, 617, 1012, 1843, 2586, 3192};
  double n_LLL[] = {19, 68, 137, 323, 517, 702};
  double oneoverFR[] = {100./0.49, 100./0.30, 100./0.21, 100./0.13, 100./0.10, 100./0.08};
  TGraph *gr_TTL = new TGraph(6, relisos, n_TTL);
  TGraph *gr_TLL = new TGraph(6, relisos, n_TLL);
  TGraph *gr_LLL = new TGraph(6, relisos, n_LLL);
  //gr_TTL->Fit("pol1");
  //gr_TLL->Fit("pol1");
  //gr_LLL->Fit("pol1");
  TGraph *gr_oneoverFR = new TGraph(6, relisos, oneoverFR);
  gr_TTL->SetLineColor(kBlack);
  gr_TLL->SetLineColor(kBlue);
  gr_LLL->SetLineColor(kRed);
  gr_oneoverFR->SetLineColor(kGreen);
  gr_TTL->SetMarkerColor(kBlack);
  gr_TLL->SetMarkerColor(kBlue);
  gr_LLL->SetMarkerColor(kRed);
  gr_oneoverFR->SetMarkerColor(kGreen);
  gr_TTL->SetMarkerStyle(21);
  gr_TLL->SetMarkerStyle(21);
  gr_LLL->SetMarkerStyle(21);
  gr_oneoverFR->SetMarkerStyle(21);
  gr_TTL->SetLineWidth(2);
  gr_TLL->SetLineWidth(2);
  gr_LLL->SetLineWidth(2);
  gr_oneoverFR->SetLineWidth(2);
  TGaxis::SetMaxDigits(4);
  TLegend *lg = new TLegend(0.2, 0.7, 0.4, 0.9);
  lg->SetBorderSize(0);
  lg->SetFillStyle(0);
  lg->AddEntry(gr_TTL, "TTL", "lp");
  lg->AddEntry(gr_TLL, "TLL", "lp");
  lg->AddEntry(gr_LLL, "LLL", "lp");
  lg->AddEntry(gr_oneoverFR, "100/FR", "lp");
  gr_TTL->Draw("alp");
  gr_TTL->SetTitle("");
  gr_TTL->GetYaxis()->SetTitle("# of data");
  gr_TTL->GetXaxis()->SetTitle("Maximum Loose RelIso");
  gr_TTL->GetYaxis()->SetRangeUser(0, 10000);
  hist_axis(gr_TTL);
  gr_TLL->Draw("lpsame");
  gr_LLL->Draw("lpsame");
  gr_oneoverFR->Draw("lpsame");
  lg->Draw();
  c_n_Loose_curves->SaveAs(plotpath+"/n_Loose_curves.png");
  c_n_Loose_curves->Close();


  //==== yield
  TCanvas *c_yield = new TCanvas("c_yield", "", 800, 800);
  canvas_margin(c_yield);
  c_yield->cd();
  hist_yield->SetMarkerSize(2);
  hist_yield->Draw("coltexte1");
  hist_yield->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_yield->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_yield);
  gStyle->SetPaintTextFormat("0.2f");
  c_yield->SaveAs(plotpath+"/yield.png");
  c_yield->Close();

  //==== syst
  TCanvas *c_syst = new TCanvas("c_syst", "", 800, 800);
  canvas_margin(c_syst);
  c_syst->cd();
  TH2D *hist_yield_cent = new TH2D("hist_yield_cent", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  for(int aaa=0; aaa<dXYMins.size(); aaa++){
    for(int bbb=0; bbb<RelIsoMaxs.size(); bbb++){
      hist_yield_cent->SetBinContent( aaa+1, bbb+1, n_events_cent  );
      hist_yield_cent->SetBinError( aaa+1, bbb+1, error_cent  );
    }
  }
  TH2D *hist_syst = (TH2D*)hist_yield->Clone();
  hist_syst->Add(hist_yield_cent, -1.);
  hist_syst->Divide(hist_yield_cent);
  hist_syst->Scale(100.);
  hist_syst->SetMarkerSize(2);
  hist_syst->Draw("coltexte1");
  hist_syst->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_syst->GetXaxis()->SetTitle("Minimum |dXYSig|");
  c_syst->SaveAs(plotpath+"/syst.png");
  
  
}

double sumw2ratioerror(double A, double dA, double B, double dB){

  //==== return error of A/B by sumw2 method

  return (A/B)*sqrt( (dA/A)*(dA/A) + (dB/B)*(dB/B) );


}

