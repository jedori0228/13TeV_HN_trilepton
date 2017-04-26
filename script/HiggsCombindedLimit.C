#include "canvas_margin.h"

void HiggsCombindedLimit(){

  gStyle->SetOptTitle(0);

  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");
  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");

  TString filepath = WORKING_DIR+"/plots/"+dataset+"/LimitOutput/HiggsCombinedTools/";
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/LimitOutput/HiggsCombinedTools/";

  TString dirname = "170426_dXY_0p01_dZ_0p1_usingpreviousoptcut";
  filepath += dirname+"/";
  plotpath += dirname+"/";

  TLatex latex_CMSPriliminary, latex_Lumi;
  latex_CMSPriliminary.SetNDC();
  latex_Lumi.SetNDC();

  //==== read result file
  string elline;
  ifstream in(filepath+"/result.txt");

  double mass[17], limit[17], onesig_left[17], onesig_right[17], twosig_left[17], twosig_right[17];

  int dummyint=0;
  while(getline(in,elline)){
    std::istringstream is( elline );

    is >> mass[dummyint];
    is >> limit[dummyint];
    is >> onesig_left[dummyint];
    is >> onesig_right[dummyint];
    is >> twosig_left[dummyint];
    is >> twosig_right[dummyint];

    double scale=1.;
    if(mass[dummyint]<=80) scale = 0.00001;
    else if(mass[dummyint]<=100) scale = 0.0001;
    else if(mass[dummyint]<=700) scale = 0.001;
    else scale = 0.001;

    limit[dummyint] *= scale;
    onesig_left[dummyint] *= scale;
    onesig_right[dummyint] *= scale;
    twosig_left[dummyint] *= scale;
    twosig_right[dummyint] *= scale;

    onesig_left[dummyint] = limit[dummyint]-onesig_left[dummyint];
    onesig_right[dummyint] = onesig_right[dummyint] - limit[dummyint];
    twosig_left[dummyint] = limit[dummyint]-twosig_left[dummyint];
    twosig_right[dummyint] = twosig_right[dummyint] - limit[dummyint];

    dummyint++;
  }

  TGraph *gr_13TeV_exp = new TGraph(17,mass,limit);
  gr_13TeV_exp->SetLineWidth(3);
  gr_13TeV_exp->SetLineStyle(2);
  gr_13TeV_exp->SetFillColor(kWhite);

  TGraphAsymmErrors *gr_band_1sigma = new TGraphAsymmErrors(17, mass, limit, 0, 0, onesig_left, onesig_right);
  gr_band_1sigma->SetFillColor(kGreen);
  gr_band_1sigma->SetLineColor(kGreen);
  gr_band_1sigma->SetMarkerColor(kGreen);

  TGraphAsymmErrors *gr_band_2sigma = new TGraphAsymmErrors(17, mass, limit, 0, 0, twosig_left, twosig_right);
  gr_band_2sigma->SetFillColor(kYellow);
  gr_band_2sigma->SetLineColor(kYellow);
  gr_band_2sigma->SetMarkerColor(kYellow);

  TLegend *lg = new TLegend(0.50, 0.15, 0.90, 0.50);



  lg->AddEntry(gr_13TeV_exp,"CL_{s} Expected");
  lg->AddEntry(gr_band_1sigma,"CL_{s} Expected #pm 1 #sigma");
  lg->AddEntry(gr_band_2sigma,"CL_{s} Expected #pm 2 #sigma");

  const int nm = 16;
  double mass_8TeV[nm] = {
    40, 50, 60, 70, 80,
    90, 100, 125, 150, 175, 
    200, 250, 300, 350, 400, 500
  };
  double exp[nm];

  vector<double> MixingValues8TeV = {
    2.82635E-05, 3.69206E-05, 8.52678E-05, 0.000618594, 0.00148793,
    0.00583953, 0.00257802, 0.00290533, 0.00449771, 0.00722055,
    0.0108741, 0.0194395, 0.0400305, 0.0708397, 0.149737, 0.497138
  };
  for(unsigned int j=0; j<MixingValues8TeV.size(); j++) exp[j] = MixingValues8TeV.at(j);

  TGraph *gr_8TeV_exp = new TGraph(nm, mass_8TeV, exp);
  gr_8TeV_exp->SetLineColor(kRed);
  gr_8TeV_exp->SetLineWidth(3);

  TCanvas *c_out = new TCanvas("c_out", "", 800, 800);
  canvas_margin(c_out);
  c_out->cd();
  c_out->Draw();
  c_out->SetLogy();
  c_out->SetGridx();
  c_out->SetGridy();

  gr_band_2sigma->Draw("A3");
  hist_axis(gr_band_2sigma);
  gr_band_2sigma->GetYaxis()->SetTitle("|V_{#muN}^{2}|");
  gr_band_2sigma->GetXaxis()->SetTitle("m(HN) [GeV]");
  gr_band_2sigma->GetYaxis()->SetTitleSize(0.06); 
  gr_band_2sigma->GetYaxis()->SetRangeUser(0.000002, 10.);
  gr_band_2sigma->SetTitle("");
  gr_band_1sigma->Draw("3same");
  gr_13TeV_exp->Draw("lsame");
  gr_8TeV_exp->Draw("lsame");

  lg->AddEntry(gr_8TeV_exp, "8 TeV DiMuon Ch. Expected", "l");
  lg->SetX2NDC(0.90);
  lg->SetY2NDC(0.67);
  lg->SetBorderSize(0);
  lg->SetFillStyle(0);
  lg->Draw();

  latex_CMSPriliminary.SetTextSize(0.035);
  latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} Simulation");
  latex_Lumi.SetTextSize(0.035);
  latex_Lumi.DrawLatex(0.7, 0.96, "35.9 fb^{-1} (13 TeV)");

  c_out->SaveAs(plotpath+"/13TeV_mixing.pdf");

  c_out->Close();




}
