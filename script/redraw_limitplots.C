#include "canvas_margin.h"

void redraw_limitplots(){

  gStyle->SetOptTitle(0);

  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");
  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");

  TString filepath = WORKING_DIR+"/plots/"+dataset+"/LimitOutput/";
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/LimitOutput/";

  TString dirname = "170315_v8-0-4.18_punzi";
  filepath += dirname+"/";
  plotpath += dirname+"/";

  TLatex latex_CMSPriliminary, latex_Lumi;
  latex_CMSPriliminary.SetNDC();
  latex_Lumi.SetNDC();

  for(int i=0; i<2; i++){

    TString filename = "";
    if(i==0){
      filename = "exclusion_fullrange.root";
    }
    else if(i==1){
      filename = "exclusion_xsec.root";
    }
    else{}

    TFile *file = new TFile(filepath+"/"+filename);
    TCanvas *c1 = (TCanvas*)file->Get("Exclusion_Plot");

    TGraph *gr_13TeV_exp = (TGraph*)c1->GetPrimitive("expected");
    TGraphAsymmErrors *gr_band_1sigma = (TGraphAsymmErrors*)c1->GetPrimitive("band_1sigma");
    TGraphAsymmErrors *gr_band_2sigma = (TGraphAsymmErrors*)c1->GetPrimitive("band_2sigma");
    TLegend *lg = (TLegend*)c1->GetPrimitive("SM_exclusion");

    const int nm = 16;
    double mass[nm] = {
      40, 50, 60, 70, 80,
      90, 100, 125, 150, 175, 
      200, 250, 300, 350, 400, 500
    };
    double exp[nm];

    if(i==0){
       vector<double> MixingValues8TeV = {
         2.82635E-05, 3.69206E-05, 8.52678E-05, 0.000618594, 0.00148793,
         0.00583953, 0.00257802, 0.00290533, 0.00449771, 0.00722055,
         0.0108741, 0.0194395, 0.0400305, 0.0708397, 0.149737, 0.497138
       };
       for(unsigned int j=0; j<MixingValues8TeV.size(); j++) exp[j] = MixingValues8TeV.at(j);
    }
    else if(i==1){
       vector<double> XsecValues8TeV = {
         0.0574156, 0.0529912, 0.0694351, 0.175697, 0.0380223, 0.0555941,
         0.0123044, 0.00419174, 0.00276877, 0.00225111, 0.00191277,
         0.00132662, 0.00124522, 0.0011111, 0.0012705, 0.00143492
       };
       for(unsigned int j=0; j<XsecValues8TeV.size(); j++) exp[j] = XsecValues8TeV.at(j);
    }
    else{}

    TGraph *gr_8TeV_exp = new TGraph(nm, mass, exp);
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
    if(i==0) gr_band_2sigma->GetYaxis()->SetTitle("|V_{#muN}^{2}|");
    else if(i==1) gr_band_2sigma->GetYaxis()->SetTitle("#sigma #times Br(pp #rightarrow N#mu #rightarrow #mu#mu#mu#nu [pb]");
    else{}
    gr_band_2sigma->GetXaxis()->SetTitle("m(HN) [GeV]");
    //gr_band_2sigma->GetXaxis()->SetRangeUser(0, 450);
    //gr_band_2sigma->GetYaxis()->SetRangeUser(0.000001, 0.09);
    gr_band_2sigma->SetTitle("");
    gr_band_2sigma->SetLineColor(kRed);
    gr_band_2sigma->SetLineWidth(3);
    gr_band_1sigma->Draw("3same");
    gr_13TeV_exp->Draw("lsame");
    gr_8TeV_exp->Draw("lsame");

    lg->AddEntry(gr_8TeV_exp, "8 TeV", "l");
    lg->SetX2NDC(0.90);
    lg->SetY2NDC(0.67);
    lg->SetBorderSize(0);
    lg->SetFillStyle(0);
    lg->Draw();

    latex_CMSPriliminary.SetTextSize(0.035);
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} Simulation");
    latex_Lumi.SetTextSize(0.035);
    latex_Lumi.DrawLatex(0.7, 0.96, "35.9 fb^{-1} (13 TeV)");

    if(i==0) c_out->SaveAs(plotpath+"/13TeV_mixing.pdf");
    else if(i==1) c_out->SaveAs(plotpath+"/13TeV_xsec.pdf");
    else{}

    c1->Close();
    c_out->Close();

  }



}
