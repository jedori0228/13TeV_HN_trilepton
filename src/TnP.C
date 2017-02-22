#include "canvas_margin.h"

void TnP(int period){

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  bool DrawFitResult = true;
  
  TString DataPeriod = "";
  if(period==1) DataPeriod="BCDEF";
  else if(period==2) DataPeriod="GH";
  else{
    cout << "period wrong" << endl;
    return;
  }
  
  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString filepath = WORKING_DIR+"/rootfiles/TnP_Results/Run"+DataPeriod+"/";
  TString plotpath = WORKING_DIR+"/plots/TnP_Results/Run"+DataPeriod+"/";

  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
  gSystem->mkdir(plotpath+"ID", kTRUE);
  gSystem->mkdir(plotpath+"ID/fitresult/", kTRUE);
  gSystem->mkdir(plotpath+"ID/fitresult/Data", kTRUE);
  gSystem->mkdir(plotpath+"ID/fitresult/MC", kTRUE);
  gSystem->mkdir(plotpath+"PR", kTRUE);
  gSystem->mkdir(plotpath+"PR/fitresult/", kTRUE);
  gSystem->mkdir(plotpath+"PR/fitresult/Data", kTRUE);

  vector<double> abseta = {0.0, 0.8, 1.479, 2.0, 2.5};
  vector<double> pt = {10., 20., 25., 30., 35., 45., 60., 80., 100., 200.};

  vector<TString> fitftns = {"vpvPlusExpo", "vpvPlusCheb", "vpvPlusCheb_4th", "gaussPlusExpo"};

  //============
  //==== ID SF
  //============

  TFile *file_ID_Data = new TFile(filepath+"TnP_Muon_ID_Data_"+DataPeriod+".root");
  TFile *file_ID_MC = new TFile(filepath+"TnP_Muon_ID_MC_"+DataPeriod+".root");

  TString dirname = "tpTree/HN_TRI_TIGHT_pt_eta/";
  if(DataPeriod=="BCDEF") dirname = "tpTree/HN_muonsel_pt_eta/";
  
  //==== FitResult
  if(DrawFitResult){
    for(unsigned int i_eta = 0; i_eta<abseta.size()-1; i_eta++){
      for(unsigned int i_pt = 0; i_pt<pt.size()-1; i_pt++){

        TString dirname_fit_result = "abseta_bin"+TString::Itoa(i_eta,10)+"__pt_bin"+TString::Itoa(i_pt,10)+"__";

        for(unsigned int i_fn = 0; i_fn<fitftns.size(); i_fn++){

          TCanvas *c_data = (TCanvas*)file_ID_Data->Get(dirname+dirname_fit_result+fitftns.at(i_fn)+"/fit_canvas");
          if(c_data) c_data->SaveAs(plotpath+"ID/fitresult/Data/"+dirname_fit_result+fitftns.at(i_fn)+".pdf");

          TCanvas *c_MC = (TCanvas*)file_ID_MC->Get(dirname+dirname_fit_result+fitftns.at(i_fn)+"/fit_canvas");
          if(c_MC) c_MC->SaveAs(plotpath+"ID/fitresult/MC/"+dirname_fit_result+fitftns.at(i_fn)+".pdf");

        }

      }
    }
  }

  //==== Eff vs pt for each eta region
  for(unsigned int i_eta = 0; i_eta<abseta.size()-1; i_eta++){
   
    TGraphAsymmErrors *eff_Data = (TGraphAsymmErrors*)file_ID_Data->Get(dirname+"fit_eff_plots/pt_PLOT_abseta_bin"+TString::Itoa(i_eta,10))->FindObject("hxy_fit_eff");
    TGraphAsymmErrors *eff_MC = (TGraphAsymmErrors*)file_ID_MC->Get(dirname+"fit_eff_plots/pt_PLOT_abseta_bin"+TString::Itoa(i_eta,10))->FindObject("hxy_fit_eff");
 
    TCanvas *c_eff = new TCanvas("c_eff", "", 800, 800);
    c_eff->Draw();
    TPad *c1_up = new TPad("c1", "", 0, 0.25, 1, 1);
    TPad *c1_down = new TPad("c1_down", "", 0, 0, 1, 0.25);
    canvas_margin(c_eff, c1_up, c1_down);
    c1_down->SetGridx();
    c1_down->SetGridy();
    c1_up->Draw();
    c1_down->Draw();
    c1_up->cd();

    eff_Data->SetLineColor(kBlack);
    eff_MC->SetLineColor(kBlue);
    eff_Data->SetMarkerColor(kBlack);
    eff_MC->SetMarkerColor(kBlue);
    eff_MC->SetMarkerStyle(21);

    eff_Data->Draw("ap");
    eff_MC->Draw("psame");

    TLegend *lg = new TLegend(0.5, 0.5, 0.8, 0.8);
    lg->SetFillStyle(0);
    lg->SetBorderSize(0);
    lg->AddEntry(eff_Data, "Run "+DataPeriod, "lp");
    lg->AddEntry(eff_MC, "MC", "lp");
    lg->Draw();

    eff_Data->SetTitle("");
    eff_Data->GetYaxis()->SetTitle("Efficiency");
    eff_Data->GetYaxis()->SetRangeUser(0, 1.1);

    c1_down->cd();

    if (eff_Data->GetN() == 0 || eff_MC->GetN() == 0) return;
    TGraphAsymmErrors *ratio = new TGraphAsymmErrors(eff_Data->GetN());
    double max = 0;
    for (size_t i = 0, n = eff_Data->GetN(); i < n; ++i) {
      double r   = eff_Data->GetY()[i]/eff_MC->GetY()[i];
      double rup = (eff_Data->GetY()[i] == 0 ? eff_Data->GetErrorYhigh(i)/(eff_MC->GetY()[i]) :
                                             r*TMath::Hypot(eff_Data->GetErrorYhigh(i)/eff_Data->GetY()[i], eff_MC->GetErrorYlow(i)/eff_MC->GetY()[i]));
      double rdn = (eff_Data->GetY()[i] == 0 ? 0 :
                                             r*TMath::Hypot(eff_Data->GetErrorYlow(i)/eff_Data->GetY()[i],  eff_MC->GetErrorYhigh(i)/eff_MC->GetY()[i]));
      max = TMath::Max(max, fabs(r-1+rup));
      max = TMath::Max(max, fabs(r-1-rdn));
      ratio->SetPoint(i, eff_Data->GetX()[i], r);
      ratio->SetPointError(i, eff_Data->GetErrorXlow(i), eff_Data->GetErrorXhigh(i), rdn, rup);
    }

    ratio->SetLineWidth(2);
    ratio->SetLineColor(kBlack);
    ratio->SetMarkerColor(kBlack);
    ratio->SetMarkerStyle(20);
    ratio->SetMarkerSize(1.6);
    ratio->Draw("AP");
    ratio->GetXaxis()->SetTitle("p_{T} [GeV]");
    ratio->GetYaxis()->SetTitle("Data/MC");
    ratio->GetYaxis()->SetRangeUser(0.8, 1.2);

/*
    for(size_t i = 0, n = eff_Data->GetN(); i < n; ++i){
      double a,b;
      eff_Data->GetPoint(i, a, b);
      cout << "x = " << a << ", Data = " << b << endl;
      eff_MC->GetPoint(i, a, b);
      cout << "x = " << a << ", MC = " << b << endl;
      ratio->GetPoint(i, a, b);
      cout << "x = " << a << ", ratio = " << b << endl <<  endl;
    }
*/

    hist_axis(eff_Data, ratio);

    TLine line(ratio->GetX()[0]-ratio->GetErrorXlow(0), 1, ratio->GetX()[ratio->GetN()-1]+ratio->GetErrorXhigh(ratio->GetN()-1), 1);
    line.SetLineWidth(2);
    line.SetLineColor(kRed);
    line.DrawClone("SAME");

    c_eff->cd();
    TLatex latex_CMSPriliminary, latex_Lumi;
    latex_CMSPriliminary.SetNDC();
    latex_Lumi.SetNDC();
    latex_CMSPriliminary.SetTextSize(0.035);
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");
    latex_Lumi.SetTextSize(0.035);
    if(DataPeriod=="BCDEF") latex_Lumi.DrawLatex(0.7, 0.96, "20.1 fb^{-1} (13 TeV)");
    else if(DataPeriod=="GH") latex_Lumi.DrawLatex(0.7, 0.96, "16.3 fb^{-1} (13 TeV)");

    c_eff->SaveAs(plotpath+"ID/pt_PLOT_abseta_bin"+TString::Itoa(i_eta,10)+".pdf");
    
    c_eff->Close();

    delete c_eff;

  }

  //==== Eff vs abseta for each pt region
  for(unsigned int i_pt = 0; i_pt<pt.size()-1; i_pt++){
    
    TGraphAsymmErrors *eff_Data = (TGraphAsymmErrors*)file_ID_Data->Get(dirname+"fit_eff_plots/abseta_PLOT_pt_bin"+TString::Itoa(i_pt,10))->FindObject("hxy_fit_eff");
    TGraphAsymmErrors *eff_MC = (TGraphAsymmErrors*)file_ID_MC->Get(dirname+"fit_eff_plots/abseta_PLOT_pt_bin"+TString::Itoa(i_pt,10))->FindObject("hxy_fit_eff");
    
    TCanvas *c_eff = new TCanvas("c_eff", "", 800, 800);
    c_eff->Draw();
    TPad *c1_up = new TPad("c1", "", 0, 0.25, 1, 1);
    TPad *c1_down = new TPad("c1_down", "", 0, 0, 1, 0.25);
    canvas_margin(c_eff, c1_up, c1_down);
    c1_down->SetGridx();
    c1_down->SetGridy();
    c1_up->Draw();
    c1_down->Draw();
    c1_up->cd();
    
    eff_Data->SetLineColor(kBlack);
    eff_MC->SetLineColor(kBlue);
    eff_Data->SetMarkerColor(kBlack);
    eff_MC->SetMarkerColor(kBlue);
    eff_MC->SetMarkerStyle(21);
    
    eff_Data->Draw("ap");
    eff_MC->Draw("psame");

    TLegend *lg = new TLegend(0.5, 0.5, 0.8, 0.8);
    lg->SetFillStyle(0);
    lg->SetBorderSize(0);
    lg->AddEntry(eff_Data, "Run BCDEF", "lp");
    lg->AddEntry(eff_MC, "MC", "lp");
    lg->Draw();
    
    eff_Data->SetTitle("");
    eff_Data->GetYaxis()->SetTitle("Efficiency");
    eff_Data->GetYaxis()->SetRangeUser(0, 1.1);
    
    c1_down->cd();
    
    if (eff_Data->GetN() == 0 || eff_MC->GetN() == 0) return;
    TGraphAsymmErrors *ratio = new TGraphAsymmErrors(eff_Data->GetN());
    double max = 0;
    for (size_t i = 0, n = eff_Data->GetN(); i < n; ++i) {
      double r   = eff_Data->GetY()[i]/eff_MC->GetY()[i];
      double rup = (eff_Data->GetY()[i] == 0 ? eff_Data->GetErrorYhigh(i)/(eff_MC->GetY()[i]) :
                    r*TMath::Hypot(eff_Data->GetErrorYhigh(i)/eff_Data->GetY()[i], eff_MC->GetErrorYlow(i)/eff_MC->GetY()[i]));
      double rdn = (eff_Data->GetY()[i] == 0 ? 0 :
                    r*TMath::Hypot(eff_Data->GetErrorYlow(i)/eff_Data->GetY()[i],  eff_MC->GetErrorYhigh(i)/eff_MC->GetY()[i]));
      max = TMath::Max(max, fabs(r-1+rup));
      max = TMath::Max(max, fabs(r-1-rdn));
      ratio->SetPoint(i, eff_Data->GetX()[i], r);
      ratio->SetPointError(i, eff_Data->GetErrorXlow(i), eff_Data->GetErrorXhigh(i), rdn, rup);
    }
    
    ratio->SetLineWidth(2);
    ratio->SetLineColor(kBlack);
    ratio->SetMarkerColor(kBlack);
    ratio->SetMarkerStyle(20);
    ratio->SetMarkerSize(1.6);
    ratio->Draw("AP");
    ratio->GetXaxis()->SetTitle("|#eta|");
    ratio->GetYaxis()->SetTitle("Data/MC");
    ratio->GetYaxis()->SetRangeUser(0.8, 1.2);
    
    hist_axis(eff_Data, ratio);
    
    TLine line(ratio->GetX()[0]-ratio->GetErrorXlow(0), 1, ratio->GetX()[ratio->GetN()-1]+ratio->GetErrorXhigh(ratio->GetN()-1), 1);
    line.SetLineWidth(2);
    line.SetLineColor(kRed);
    line.DrawClone("SAME");
    
    c_eff->cd();
    TLatex latex_CMSPriliminary, latex_Lumi;
    latex_CMSPriliminary.SetNDC();
    latex_Lumi.SetNDC();
    latex_CMSPriliminary.SetTextSize(0.035);
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");
    latex_Lumi.SetTextSize(0.035);
    if(DataPeriod=="BCDEF") latex_Lumi.DrawLatex(0.7, 0.96, "20.1 fb^{-1} (13 TeV)");
    else if(DataPeriod=="GH") latex_Lumi.DrawLatex(0.7, 0.96, "16.3 fb^{-1} (13 TeV)");
    
    
    c_eff->SaveAs(plotpath+"ID/abseta_PLOT_pt_bin"+TString::Itoa(i_pt,10)+".pdf");
    
    c_eff->Close();
    delete c_eff;
    
  }

  //==== make SF root file

  TString outputIDSFname = "MuonIDSF_TRILEP_Run"+DataPeriod+".root";
  TFile *file_IDSF = new TFile(plotpath+"ID/"+outputIDSFname, "RECREATE");

  TCanvas *c_IDeff_Data = (TCanvas*)file_ID_Data->Get(dirname+"fit_eff_plots/abseta_pt_PLOT");
  TH2F *hist_IDeff_Data = (TH2F*)c_IDeff_Data->GetPrimitive("abseta_pt_PLOT");
  TCanvas *c_IDeff_MC = (TCanvas*)file_ID_MC->Get(dirname+"fit_eff_plots/abseta_pt_PLOT");
  TH2F *hist_IDeff_MC = (TH2F*)c_IDeff_MC->GetPrimitive("abseta_pt_PLOT");

  TH2F *hist_IDSF = (TH2F*)hist_IDeff_Data->Clone();
  hist_IDSF->Divide(hist_IDeff_MC);
  file_IDSF->cd();
  hist_IDSF->Write();
  file_IDSF->Close();

  
  //============
  //==== PR
  //============
  
  TFile *file_PR_Data = new TFile(filepath+"TnP_Muon_PR_Data_"+DataPeriod+".root");
  
  dirname = "tpTree/HN_muonsel_pt_eta/";
  
  //==== FitResult
  if(DrawFitResult){
    for(unsigned int i_eta = 0; i_eta<abseta.size()-1; i_eta++){
      for(unsigned int i_pt = 0; i_pt<pt.size()-1; i_pt++){
        
        TString dirname_fit_result = "abseta_bin"+TString::Itoa(i_eta,10)+"__combRelIsoPF04dBeta_bin0__pt_bin"+TString::Itoa(i_pt,10)+"__";
        
        for(unsigned int i_fn = 0; i_fn<fitftns.size(); i_fn++){
          
          TCanvas *c_data = (TCanvas*)file_PR_Data->Get(dirname+dirname_fit_result+fitftns.at(i_fn)+"/fit_canvas");
          if(c_data) c_data->SaveAs(plotpath+"PR/fitresult/Data/"+dirname_fit_result+fitftns.at(i_fn)+".pdf");
          

        }
        
      }
    }
  }
  
  //==== Eff vs pt for each eta region
  for(unsigned int i_eta = 0; i_eta<abseta.size()-1; i_eta++){
    
    TGraphAsymmErrors *eff_Data = (TGraphAsymmErrors*)file_PR_Data->Get(dirname+"fit_eff_plots/pt_PLOT_abseta_bin"+TString::Itoa(i_eta,10))->FindObject("hxy_fit_eff");
    
    TCanvas *c_eff = new TCanvas("c_eff", "", 800, 800);
    c_eff->Draw();
    canvas_margin(c_eff);
    c_eff->Draw();
    
    eff_Data->SetLineColor(kBlack);
    eff_Data->SetMarkerColor(kBlack);
    
    eff_Data->Draw("ap");
    
    TLegend *lg = new TLegend(0.5, 0.5, 0.8, 0.8);
    lg->SetFillStyle(0);
    lg->SetBorderSize(0);
    lg->AddEntry(eff_Data, "Run "+DataPeriod, "lp");
    lg->Draw();
    
    eff_Data->SetTitle("");
    eff_Data->GetYaxis()->SetTitle("Efficiency");
    eff_Data->GetYaxis()->SetRangeUser(0, 1.1);
    
    hist_axis(eff_Data);
    
    c_eff->cd();
    TLatex latex_CMSPriliminary, latex_Lumi;
    latex_CMSPriliminary.SetNDC();
    latex_Lumi.SetNDC();
    latex_CMSPriliminary.SetTextSize(0.035);
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");
    latex_Lumi.SetTextSize(0.035);
    if(DataPeriod=="BCDEF") latex_Lumi.DrawLatex(0.7, 0.96, "20.1 fb^{-1} (13 TeV)");
    else if(DataPeriod=="GH") latex_Lumi.DrawLatex(0.7, 0.96, "16.3 fb^{-1} (13 TeV)");
    
    c_eff->SaveAs(plotpath+"PR/pt_PLOT_abseta_bin"+TString::Itoa(i_eta,10)+".pdf");
    
    c_eff->Close();
    
    delete c_eff;
    
  }
  
  //==== Eff vs abseta for each pt region
  for(unsigned int i_pt = 0; i_pt<pt.size()-1; i_pt++){
    
    TGraphAsymmErrors *eff_Data = (TGraphAsymmErrors*)file_PR_Data->Get(dirname+"fit_eff_plots/abseta_PLOT_pt_bin"+TString::Itoa(i_pt,10))->FindObject("hxy_fit_eff");
    
    TCanvas *c_eff = new TCanvas("c_eff", "", 800, 800);
    c_eff->Draw();
    canvas_margin(c_eff);
    c_eff->cd();
    
    eff_Data->SetLineColor(kBlack);
    eff_Data->SetMarkerColor(kBlack);
    
    eff_Data->Draw("ap");
    
    TLegend *lg = new TLegend(0.5, 0.5, 0.8, 0.8);
    lg->SetFillStyle(0);
    lg->SetBorderSize(0);
    lg->AddEntry(eff_Data, "Run "+DataPeriod, "lp");
    lg->Draw();
    
    eff_Data->SetTitle("");
    eff_Data->GetYaxis()->SetTitle("Efficiency");
    eff_Data->GetYaxis()->SetRangeUser(0, 1.1);
    
    hist_axis(eff_Data);
    
    c_eff->cd();
    TLatex latex_CMSPriliminary, latex_Lumi;
    latex_CMSPriliminary.SetNDC();
    latex_Lumi.SetNDC();
    latex_CMSPriliminary.SetTextSize(0.035);
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");
    latex_Lumi.SetTextSize(0.035);
    if(DataPeriod=="BCDEF") latex_Lumi.DrawLatex(0.7, 0.96, "20.1 fb^{-1} (13 TeV)");
    else if(DataPeriod=="GH") latex_Lumi.DrawLatex(0.7, 0.96, "16.3 fb^{-1} (13 TeV)");
    
    
    c_eff->SaveAs(plotpath+"PR/abseta_PLOT_pt_bin"+TString::Itoa(i_pt,10)+".pdf");
    
    c_eff->Close();
    delete c_eff;
    
  }
  
  //==== make PR root file

  TString outputPRname = "MuonPR_TRILEP_Run"+DataPeriod+".root";
  TFile *file_PR = new TFile(plotpath+"PR/"+outputPRname, "RECREATE");

  TCanvas *c_PReff_Data = (TCanvas*)file_PR_Data->Get(dirname+"fit_eff_plots/pt_abseta_PLOT_combRelIsoPF04dBeta_bin0");
  TH2F *hist_PReff_Data = (TH2F*)c_PReff_Data->GetPrimitive("pt_abseta_PLOT_combRelIsoPF04dBeta_bin0");
  hist_PReff_Data->SetName("PR_pt_abseta");
  file_PR->cd();
  hist_PReff_Data->Write();
  file_PR->Close();



}











