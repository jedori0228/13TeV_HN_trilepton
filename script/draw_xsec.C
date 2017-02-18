#include "canvas_margin.h"

void draw_xsec(){

  gStyle->SetOptStat(0);
  
  double mass_dimu_8TeV[] = {
    40, 50, 60, 70,
    80, 90, 100, 125,
    150, 175, 200, 225,
    250, 275, 300, 325,
    350, 375, 400, 500,
    600, 700
  };
  double xsec_dimu_8TeV[] = {
    1515., 1071., 607.7, 212.0,
    19.06, 7.105, 3.562, 1.0767,
    0.4594, 0.23266, 0.13127, 0.07966,
    0.05093, 0.03386, 0.023214, 0.016317,
    0.011705, 0.008545, 0.006332, 0.002154,
    0.0008545, 0.0003831
  };
  
  for(int i=0; i<22;i++) cout << mass_dimu_8TeV[i] << "\t" << xsec_dimu_8TeV[i] << endl;
  
  double mass_trimu_13TeV[] ={
    5, 10, 20, 30,
    40, 50, 60, 70,
    90, 100, 150, 200,
    300, 400, 500, 700,
    1000
  };
  double xsec_trimu_13TeV[] = {
    4.046, 3.982, 3.711, 3.241,
    2.619, 1.875, 1.07, 0.3828,
    0.02333, 0.01082, 0.001488, 0.0004567,
    9.52E-05, 3.137E-05, 1.289E-05, 3.214E-06,
    6.48E-07
  };
  double xsec_ratio_dimu_to_trimu[] = {
    4.2429070198, 4.25021616493, 4.24949298982, 4.2449343587,
    4.24119041691, 4.21413955917, 4.16022218627, 4.01594083053,
    3.17, 3.17, 3.17, 3.17,
    3.17, 3.17, 3.17, 3.17,
    3.17
  };
  for(int i=0; i<17;i++) cout << mass_trimu_13TeV[i] << "\t" << xsec_ratio_dimu_to_trimu[i] << endl;
  
  double mass_dimu_13TeV_NLO[] = {
    40, 50, 60, 70, 80,
    90, 100, 125, 150, 200,
    250, 300, 400, 500, 600,
    700, 800, 900, 1000, 1100,
    1200, 1300, 1400, 1500
  };
  double xsec_dimu_13TeV_NLO[] = {
    2898, 2042.6, 1147.8, 387.98, 201.34,
    15.434, 7.76, 2.444, 1.1012, 0.3397,
    1.43E-01, 7.122E-02, 2.360E-02, 9.844E-03, 4.700E-03,
    2.467E-03, 1.379E-03, 8.112E-04, 4.955E-04, 3.117E-04,
    2.012E-04, 1.325E-04, 8.892E-05, 6.052E-05
  };
  
  for(int i=0; i<24;i++) cout << mass_dimu_13TeV_NLO[i] << "\t" << xsec_dimu_13TeV_NLO[i]/200. << endl;
  
  double xsec_dimu_13TeV[17];
  for(unsigned int i=0; i<17; i++){
    //==== convert mixing VeN=VmuN=0.1 to VeN=0,VmuN=1
    xsec_trimu_13TeV[i] = xsec_trimu_13TeV[i]*100.*2.;
    //==== convert 13TeV trimu to 13TeV mumujj
    xsec_dimu_13TeV[i] = xsec_trimu_13TeV[i]*xsec_ratio_dimu_to_trimu[i];
  }
  
  TGraph *gr_dimu_8TeV = new TGraph(22, mass_dimu_8TeV, xsec_dimu_8TeV);
  TGraph *gr_trimu_13TeV = new TGraph(17, mass_trimu_13TeV, xsec_trimu_13TeV);
  TGraph *gr_dimu_13TeV = new TGraph(17, mass_trimu_13TeV, xsec_dimu_13TeV);
  TGraph *gr_dimu_13TeV_NLO = new TGraph(24, mass_dimu_13TeV_NLO, xsec_dimu_13TeV_NLO);

  gr_dimu_8TeV->SetLineColor(kBlue);
  gr_trimu_13TeV->SetLineColor(kBlack);
  gr_dimu_13TeV->SetLineColor(kBlue);
  gr_dimu_13TeV_NLO->SetLineColor(kCyan);

  gr_dimu_8TeV->SetLineStyle(2);
  
  gr_dimu_8TeV->SetLineWidth(3);
  gr_trimu_13TeV->SetLineWidth(3);
  gr_dimu_13TeV->SetLineWidth(3);
  gr_dimu_13TeV_NLO->SetLineWidth(3);

  TLegend *lg = new TLegend(0.6, 0.5, 0.95, 0.9);
  lg->SetBorderSize(0);
  lg->SetFillStyle(0);
  lg->AddEntry(gr_dimu_13TeV_NLO, "SS #mu#mu + jj, aMC@NLO (13 TeV)", "l");
  lg->AddEntry(gr_dimu_13TeV, "SS #mu#mu + jj, MG (13 TeV)", "l");
  lg->AddEntry(gr_trimu_13TeV, "#mu#mu#mu#nu, MG (13 TeV)", "l");
  lg->AddEntry(gr_dimu_8TeV, "SS #mu#mu + jj, ALPGEN (8 TeV)", "l");
  
  TH1D *hist_dummy = new TH1D("hist_dummy", "", 2000, 1., 2001);
  
  TCanvas *c1 = new TCanvas("c1", "", 800, 800);
  TPad *c1_up = new TPad("c1", "", 0, 0.25, 1, 1);
  TPad *c1_down = new TPad("c1_down", "", 0, 0, 1, 0.25);
  canvas_margin(c1, c1_up, c1_down);
  
  c1_down->SetGridx();
  c1_down->SetGridy();
  
  c1_up->Draw();
  c1_down->Draw();

  c1_up->cd();

  hist_dummy->Draw("hist");
  hist_dummy->SetTitle("");
  hist_dummy->GetYaxis()->SetTitle("xsec [pb]");
  
  c1_up->SetLogy();
  //c1_up->SetLogx();
  hist_dummy->GetYaxis()->SetRangeUser(0.0001, 10000);
  hist_dummy->GetXaxis()->SetRangeUser(1., 1600.);
  
  gr_trimu_13TeV->Draw("lsame");
  gr_dimu_8TeV->Draw("lsame");
  gr_dimu_13TeV->Draw("lsame");
  gr_dimu_13TeV_NLO->Draw("lsame");
  lg->Draw();
  
  c1_down->cd();
  TH1D *hist_dummy_down = (TH1D*)hist_dummy->Clone();
  hist_dummy_down->Draw("hist");
  hist_dummy_down->GetXaxis()->SetTitle("m(N) [GeV]");
  hist_dummy_down->GetYaxis()->SetTitle("#splitline{13TeV/8TeV}{SS DiMu Ch.}");
  hist_dummy_down->GetYaxis()->SetRangeUser(0., 6.0);
  hist_dummy_down->GetXaxis()->SetRangeUser(1., 1100.);
  //c1_down->SetLogx();
  
  cout << "==================================" << endl;
  cout << "==== (13TeV dimu)/(8TeV dimu) ====" << endl;
  cout << "==================================" << endl;
  
  double mass_ratio[] = {
    40, 50, 60, 70,
    90, 100, 150, 200,
    300, 400, 500, 700
  };
  double xsec_ratio_13TeV_8TeV[12];
  for(unsigned int i=0;i<12;i++){
    double this_13TeV = 0.;
    for(unsigned int j=0;j<17;j++){
      if(mass_trimu_13TeV[j] == mass_ratio[i]){
        this_13TeV = xsec_dimu_13TeV[j];
        break;
      }
    }
    double this_8TeV = 0.;
    for(unsigned int j=0;j<22;j++){
      if(mass_dimu_8TeV[j] == mass_ratio[i]){
        this_8TeV = xsec_dimu_8TeV[j];
        break;
      }
    }
    //cout << "mass = " << mass_ratio[i] << endl;
    //cout << "  13 TeV = " << this_13TeV << endl;
    //cout << "  8 TeV = " << this_8TeV << endl;
    xsec_ratio_13TeV_8TeV[i] = this_13TeV/this_8TeV;
    cout << mass_ratio[i] << "\t" << xsec_ratio_13TeV_8TeV[i] << endl;
  }
  
  TGraph *ratio_8TeV_13TeV_dimu = new TGraph(12, mass_ratio, xsec_ratio_13TeV_8TeV);
  ratio_8TeV_13TeV_dimu->SetMarkerStyle(31);
  ratio_8TeV_13TeV_dimu->SetMarkerSize(3);
  ratio_8TeV_13TeV_dimu->SetMarkerColor(kBlue);
  ratio_8TeV_13TeV_dimu->SetLineColor(kBlue);
  ratio_8TeV_13TeV_dimu->SetLineWidth(2);
  ratio_8TeV_13TeV_dimu->Draw("lsame");
  hist_axis(hist_dummy, hist_dummy_down);
  
  c1_up->cd();
  TLatex latex_mixing;
  latex_mixing.SetNDC();
  latex_mixing.SetTextSize(0.05);
  latex_mixing.DrawLatex(0.65, 0.3, "#splitline{|V_{#muN}|^{2} = 1.0}{|V_{eN}|^{2} = 0}");
  
  //===============
  //==== k-factor
  //===============
  
  cout << "===========================================" << endl;
  cout << "==== (13TeV dimu NLO)/(13TeV dimu NLO) ====" << endl;
  cout << "===========================================" << endl;
  
  double mass_kfactor[] = {
    40, 50, 60, 70,
    90, 100, 150, 200,
    300, 400, 500,
    700, 1000
  };
  double kfactor[12];
  for(unsigned int i=0;i<13;i++){
    double this_LO = 0.;
    for(unsigned int j=0;j<17;j++){
      if(mass_trimu_13TeV[j] == mass_kfactor[i]){
        this_LO = xsec_dimu_13TeV[j];
        break;
      }
    }
    double this_NLO = 0.;
    for(unsigned int j=0;j<24;j++){
      if(mass_dimu_13TeV_NLO[j] == mass_kfactor[i]){
        this_NLO = xsec_dimu_13TeV_NLO[j];
        break;
      }
    }
    //cout << "mass = " << mass_ratio[i] << endl;
    //cout << "  13 TeV = " << this_13TeV << endl;
    //cout << "  8 TeV = " << this_8TeV << endl;
    kfactor[i] = this_NLO/this_LO;
    cout << mass_kfactor[i] << "\t" << kfactor[i] << endl;
  }
  
  c1->SaveAs("./13_8_xsec.pdf");
  
  
}
