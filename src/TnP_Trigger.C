#include "canvas_margin.h"

void TnP_Trigger(int period){
  
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

  TString CutsOnDen = "dB_bin0__dzPV_bin0__pair_dPhiPrimeDeg_bin0__pair_deltaR_bin0";
  if(DataPeriod=="GH") CutsOnDen = "dB_bin0__dzPV_bin0__pair_deltaR_bin0";

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString filepath = WORKING_DIR+"/rootfiles/TnP_Results/Run"+DataPeriod+"/";
  TString base_plotpath = WORKING_DIR+"/plots/TnP_Results/Run"+DataPeriod+"/Trigger/";
  
  if( !gSystem->mkdir(base_plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
  
  vector<double> abseta = {0., 0.9, 1.2, 2.1, 2.4};
  
  vector<TString> fitftns = {"vpvPlusExpo", "vpvPlusCheb", "vpvPlusCheb_4th", "gaussPlusExpo", "vpvPlusCMSbeta0p2", "vpvPlusExpo3", "vpvPlusCheb_3rd"};
  
  //vector<TString> triggers = {"DoubleIsoMu17Mu8_IsoMu17leg", "DoubleIsoMu17Mu8_IsoMu8leg", "DoubleIsoMu17TkMu8_IsoMu17leg", "DoubleIsoMu17TkMu8_IsoMu8leg", "Mu8_OR_TkMu8", "test1"};
  vector<TString> triggers = {"DoubleIsoMu17Mu8_IsoMu17leg", "Mu8_OR_TkMu8"};
  
  for(unsigned it_trigger=0; it_trigger<triggers.size(); it_trigger++){
    
    TString trigger = triggers.at(it_trigger);
    TString plotpath = base_plotpath+"/"+trigger+"/";

    vector<double> pt = {10, 20, 25, 30, 40, 50, 60, 120};
    if(trigger=="DoubleIsoMu17Mu8_IsoMu17leg") pt = {10, 15, 16, 17, 18, 19, 20, 25, 30, 40, 50, 60, 120};

    gSystem->mkdir(plotpath, kTRUE);
    gSystem->mkdir(plotpath+"fitresult/", kTRUE);
    gSystem->mkdir(plotpath+"fitresult/Data", kTRUE);
    gSystem->mkdir(plotpath+"fitresult/MC", kTRUE);

    TFile *file_Trigger_Data = new TFile(filepath+trigger+"_Data_"+DataPeriod+".root");
    TFile *file_Trigger_MC = new TFile(filepath+trigger+"_MC_"+DataPeriod+".root");
    
    TString dirname = "tpTree/HN_TRI_TIGHT_Trigger_pt_eta/";
    
    //==== FitResult
    if(DrawFitResult){
      for(unsigned int i_eta = 0; i_eta<abseta.size()-1; i_eta++){
        for(unsigned int i_pt = 0; i_pt<pt.size()-1; i_pt++){
          
          TString dirname_fit_result = "abseta_bin"+TString::Itoa(i_eta,10)+"__"+CutsOnDen+"__pt_bin"+TString::Itoa(i_pt,10)+"__";
          
          for(unsigned int i_fn = 0; i_fn<fitftns.size(); i_fn++){
            TCanvas *c_data = (TCanvas*)file_Trigger_Data->Get(dirname+dirname_fit_result+fitftns.at(i_fn)+"/fit_canvas");
            if(c_data) c_data->SaveAs(plotpath+"/fitresult/Data/"+dirname_fit_result+fitftns.at(i_fn)+".pdf");
            
            TCanvas *c_MC = (TCanvas*)file_Trigger_MC->Get(dirname+dirname_fit_result+fitftns.at(i_fn)+"/fit_canvas");
            if(c_MC) c_MC->SaveAs(plotpath+"/fitresult/MC/"+dirname_fit_result+fitftns.at(i_fn)+".pdf");
            
          }
          
        }
      }
    }
    
    //==== Eff vs pt for each eta region
    for(unsigned int i_eta = 0; i_eta<abseta.size()-1; i_eta++){
      
      TGraphAsymmErrors *eff_Data = (TGraphAsymmErrors*)file_Trigger_Data->Get(dirname+"fit_eff_plots/pt_PLOT_abseta_bin"+TString::Itoa(i_eta,10))->FindObject("hxy_fit_eff");
      TGraphAsymmErrors *eff_MC = (TGraphAsymmErrors*)file_Trigger_MC->Get(dirname+"fit_eff_plots/pt_PLOT_abseta_bin"+TString::Itoa(i_eta,10))->FindObject("hxy_fit_eff");
      
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
      
      c_eff->SaveAs(plotpath+"/pt_PLOT_abseta_bin"+TString::Itoa(i_eta,10)+".pdf");
      
      c_eff->Close();
      
      delete c_eff;
      
    }
    
    //==== Eff vs abseta for each pt region
    for(unsigned int i_pt = 0; i_pt<pt.size()-1; i_pt++){
      
      TGraphAsymmErrors *eff_Data = (TGraphAsymmErrors*)file_Trigger_Data->Get(dirname+"fit_eff_plots/abseta_PLOT_pt_bin"+TString::Itoa(i_pt,10))->FindObject("hxy_fit_eff");
      TGraphAsymmErrors *eff_MC = (TGraphAsymmErrors*)file_Trigger_MC->Get(dirname+"fit_eff_plots/abseta_PLOT_pt_bin"+TString::Itoa(i_pt,10))->FindObject("hxy_fit_eff");
      
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
      if(DataPeriod=="BCDEF") lg->AddEntry(eff_Data, "Run BCDEF", "lp");
      else if(DataPeriod=="GH") lg->AddEntry(eff_Data, "Run GH", "lp");
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
      
      
      c_eff->SaveAs(plotpath+"/abseta_PLOT_pt_bin"+TString::Itoa(i_pt,10)+".pdf");
      
      c_eff->Close();
      delete c_eff;
      
    }
    
    //==== make SF root file
    
    TString outputTriggerEffname = "MuonTriggerEfficiency_HNTRILEP_"+trigger+"_Run"+DataPeriod+".root";
    TFile *file_TriggerEff = new TFile(plotpath+outputTriggerEffname, "RECREATE");

    TString CutsOnDen2 = "dB_bin0_&_dzPV_bin0_&_pair_dPhiPrimeDeg_bin0_&_pair_deltaR_bin0";
    if(DataPeriod=="GH") CutsOnDen2 = "dB_bin0_&_dzPV_bin0_&_pair_deltaR_bin0";

    TCanvas *c_TriggerEff_Data = (TCanvas*)file_Trigger_Data->Get(dirname+"fit_eff_plots/abseta_pt_PLOT_"+CutsOnDen2);
    TCanvas *c_TriggerEff_MC = (TCanvas*)file_Trigger_MC->Get(dirname+"fit_eff_plots/abseta_pt_PLOT_"+CutsOnDen2);

    TH2F *hist_TriggerEff_Data = (TH2F*)c_TriggerEff_Data->GetPrimitive("abseta_pt_PLOT_"+CutsOnDen2)->Clone();
    TH2F *hist_TriggerEff_MC = (TH2F*)c_TriggerEff_MC->GetPrimitive("abseta_pt_PLOT_"+CutsOnDen2)->Clone();

    c_TriggerEff_Data->Close();
    c_TriggerEff_MC->Close();

    hist_TriggerEff_Data->SetName("TriggerEff_HNTrilepton_"+DataPeriod+"_Data");
    hist_TriggerEff_MC->SetName("TriggerEff_HNTrilepton_"+DataPeriod+"_MC");

    file_TriggerEff->cd();
    hist_TriggerEff_Data->Write();
    hist_TriggerEff_MC->Write();
    file_TriggerEff->Close();

    //=== draw MC efficiency 
    TCanvas *c_eff_MC = new TCanvas("c_eff_MC", "", 800, 800);
    //if(trigger=="DoubleIsoMu17Mu8_IsoMu17leg") c_eff_MC = new TCanvas("c_eff_MC", "", 1500, 3000);
    //else c_eff_MC = new TCanvas("c_eff_MC", "", 800, 800);
    canvas_margin(c_eff_MC);
    c_eff_MC->cd();
    gStyle->SetPaintTextFormat("0.4f");
    hist_TriggerEff_MC->Draw("texte1");
    hist_axis(hist_TriggerEff_MC);
    hist_TriggerEff_MC->GetXaxis()->SetTitle("muon |#eta|");
    hist_TriggerEff_MC->GetYaxis()->SetTitle("muon p_{T} [GeV]");
    c_eff_MC->SaveAs(plotpath+"/abseta_pt_MC.pdf");
    if(trigger=="DoubleIsoMu17Mu8_IsoMu17leg"){
      hist_TriggerEff_MC->GetYaxis()->SetRangeUser(10, 25);
      c_eff_MC->SaveAs(plotpath+"/abseta_pt_MC_zoom.pdf");
    }
    c_eff_MC->Close();

    //=== draw Data efficiency 
    TCanvas *c_eff_Data = new TCanvas("c_eff_Data", "", 800, 800);
    //if(trigger=="DoubleIsoMu17Mu8_IsoMu17leg") c_eff_Data = new TCanvas("c_eff_Data", "", 1500, 300);
    //else c_eff_Data = new TCanvas("c_eff_Data", "", 800, 800);
    canvas_margin(c_eff_Data);
    c_eff_Data->cd();
    gStyle->SetPaintTextFormat("0.4f");
    hist_TriggerEff_Data->Draw("texte1");
    hist_axis(hist_TriggerEff_Data);
    hist_TriggerEff_Data->GetXaxis()->SetTitle("muon |#eta|");
    hist_TriggerEff_Data->GetYaxis()->SetTitle("muon p_{T} [GeV]");
    c_eff_Data->SaveAs(plotpath+"/abseta_pt_Data.pdf");
    //==== ratio
    hist_TriggerEff_Data->Divide(hist_TriggerEff_MC);
    hist_TriggerEff_Data->Draw("texte1");
    c_eff_Data->SaveAs(plotpath+"/abseta_pt_SF.pdf");
    if(trigger=="DoubleIsoMu17Mu8_IsoMu17leg"){
      hist_TriggerEff_Data->GetYaxis()->SetRangeUser(10, 25);
      c_eff_Data->SaveAs(plotpath+"/abseta_pt_Data_zoom.pdf");
    }
    c_eff_Data->Close();




  }
    
}











