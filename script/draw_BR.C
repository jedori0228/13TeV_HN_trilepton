#include "canvas_margin.h"

void draw_BR(){

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/signal_study/";
  
  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
  
  double masses[] = {90, 100, 150, 200, 300, 400, 500, 700, 1000};
  double Wl[] = {1.0, 0.8869592, 0.6733268, 0.5976384, 0.5434948, 0.5245388, 0.5157344, 0.5080432, 0.5039456};
  double Znu[] = {0, 0.11304092, 0.28953584, 0.28495392, 0.26909664, 0.26143644, 0.2575268, 0.25393308, 0.25195156};
  double Hnu[] = {0, 0, 0.03713722, 0.11740752, 0.18740848, 0.2140248, 0.22673892, 0.23802384, 0.24410288};

  TGraph *gr_Wl = new TGraph(9, masses, Wl);
  TGraph *gr_Znu = new TGraph(9, masses, Znu);
  TGraph *gr_Hnu = new TGraph(9, masses, Hnu);

  gr_Wl->SetLineColor(kBlack);
  gr_Znu->SetLineColor(kRed);
  gr_Hnu->SetLineColor(kBlue);
  gr_Wl->SetMarkerColor(kBlack);
  gr_Znu->SetMarkerColor(kRed);
  gr_Hnu->SetMarkerColor(kBlue);
  
  gr_Wl->SetLineWidth(2);
  gr_Znu->SetLineWidth(2);
  gr_Hnu->SetLineWidth(2);
  gr_Wl->SetMarkerStyle(21);;
  gr_Znu->SetMarkerStyle(21);;
  gr_Hnu->SetMarkerStyle(21);;
  

  TLegend *lg = new TLegend(0.6, 0.6, 0.9, 0.9);
  lg->SetBorderSize(0);
  lg->SetFillStyle(0);
  lg->AddEntry(gr_Wl, "N#rightarrowWl", "lp");
  lg->AddEntry(gr_Znu, "N#rightarrowZ#nu", "lp");
  lg->AddEntry(gr_Hnu, "N#rightarrowH#nu", "lp");

  TCanvas *c1 = new TCanvas("c1", "", 800, 800);
  canvas_margin(c1);
  c1->cd();
  gr_Wl->Draw("apl");
  gr_Wl->SetTitle("");
  gr_Wl->GetXaxis()->SetTitle("m(N) [GeV]");
  gr_Wl->GetYaxis()->SetTitle("BR");
  gr_Znu->Draw("plsame");
  gr_Hnu->Draw("plsame");
  hist_axis(gr_Wl);
  lg->Draw();

  gr_Wl->GetYaxis()->SetRangeUser(0, 1.1);
  
  c1->SaveAs(plotpath+"/BR.pdf");

}

