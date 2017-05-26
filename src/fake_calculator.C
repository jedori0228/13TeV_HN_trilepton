#include "mylib.h"
#include "canvas_margin.h"
#include "TSystem.h"

//==== MC list = {first, second, third ...}
//==== then, first will be filled at the bottom, so we want them in the bottom of legend
void make_legend(TLegend *lg, TString MCtype, vector<TH1D*> hists, vector<TString> alias);

double GetMCRescale(int period);
TString GetPeriod(int period);

void fake_calculator(double dXYMin, double RelIsoMax, int period=0){

  bool Draw5GeV = false;

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);
  gStyle->SetOptStat(0);

  int Digit1 = int(dXYMin);
  int Digit0p1 = 10*dXYMin-10*Digit1;
  TString str_dXYCut = "dXYSigMin_"+TString::Itoa(Digit1,10)+"p"+TString::Itoa(Digit0p1,10);

  int iso_Digit1 = int(RelIsoMax);
  int iso_Digit0p1 = 10*RelIsoMax-10*iso_Digit1;
  TString str_iso = "LooseRelIsoMax_"+TString::Itoa(iso_Digit1,10)+"p"+TString::Itoa(iso_Digit0p1,10);

  str_dXYCut = str_dXYCut+"_"+str_iso;
 
  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");

  TString WhichRootFile = "DiMuonTrkVVL";
  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/FakeRateCalculator/"+WhichRootFile+"/";
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/FakeRateCalculator/"+WhichRootFile+"/"+str_dXYCut+"/";
  
  map< TString, vector<TString> > map_string_to_MC_list;
  map< TString, vector<TString> > map_string_to_MC_alias;
  map< TString, vector<Color_t> > map_string_to_MC_color;
  vector<TString> all_MC_list;
  
  all_MC_list = {"singletop", "TTJets_aMC", "VGamma", "DY", "WJets", "QCD_mu", "HN_MuMuMu_40", "VV"};
  map_string_to_MC_list["SingleMuon"] = {"singletop", "TTJets_aMC", "VGamma", "DY", "WJets"};
  map_string_to_MC_alias["SingleMuon"] = {"singletop", "t#bar{t}", "V#gamma", "DY", "W + Jets"};
  map_string_to_MC_color["SingleMuon"] = {kOrange, kRed, kSpring-7, kYellow, kGreen};
  map_string_to_MC_list["DiMuon"] = {"VGamma", "DY", "VV"};
  map_string_to_MC_alias["DiMuon"] = {"V#gamma", "DY", "VV"};
  map_string_to_MC_color["DiMuon"] = {kSpring-7, kYellow, kGreen};

  //==== Data Period
  //==== 0 (default) : BCDEFGH
  //==== 1 : B
  //==== 2 : C
  //==== 3 : D
  //==== 4 : E
  //==== 5 : F
  //==== 6 : G
  //==== 7 : H (_v2+_v3)
  //==== 8 : BCDEF
  //==== 9 : GH
  
  TString DataFileName = "data_";
  TString DataPeriod = "BCDEFGH";
  double MCRescale = 1.;
  if(period!=0){
    DataPeriod = GetPeriod(period);
    DataFileName = "period"+DataPeriod+"_SK";
    MCRescale = GetMCRescale(period);
    cout << "MCRescale = " << MCRescale << endl;
    plotpath = plotpath+DataPeriod+"/";
  }

  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  TLatex latex_CMSPriliminary, latex_Lumi;
  latex_CMSPriliminary.SetNDC();
  latex_Lumi.SetNDC();
  
  
  //==== get all files here
  map< TString, TFile* > map_string_to_file;
  map_string_to_file["data"] = new TFile(filepath+"/FakeRateCalculator_Mu_dxysig_"+DataFileName+"DoubleMuon_cat_"+catversion+".root");
  for(unsigned int i=0; i<all_MC_list.size(); i++){
    TString this_samplename = all_MC_list.at(i);
    map_string_to_file[this_samplename] = new TFile(filepath+"/FakeRateCalculator_Mu_dxysig_SK"+this_samplename+"_cat_"+catversion+".root");
    if(!map_string_to_file[this_samplename]) cout << "[No file] :" << this_samplename << endl;
  }
  
  //======================
  //==== LooseMuon study
  //======================
  
  cout << "######################## Loose Muon study ########################" << endl;
  
  vector<TString> type_Loose_study = {"", "HighdXY_"};
  vector<TString> var_Loose_study = {"RelIso", "Chi2", "dXY", "dXYSig", "pt", "eta"};
  for(unsigned int it_type_Loose_study=0; it_type_Loose_study<type_Loose_study.size(); it_type_Loose_study++){

    //==== e.g., "SingleMuon"
    TString this_type_Loose_study = type_Loose_study.at(it_type_Loose_study);
    cout << this_type_Loose_study << endl;
    
    //==== prepare MC iterator end point
    vector<TString>::iterator it_MC_END;
    TString this_MC_type = "SingleMuon";
    it_MC_END = map_string_to_MC_list[this_MC_type].end();
    
    for(unsigned int it_var_Loose_study=0; it_var_Loose_study<var_Loose_study.size(); it_var_Loose_study++){
    
      //==== e.g., "RelIso"
      TString this_var_Loose_study = var_Loose_study.at(it_var_Loose_study);

      TString histname = str_dXYCut+"_"+this_type_Loose_study+"LooseMuon_"+this_var_Loose_study;
      cout << "histname = " << histname << endl;
      
      //==== data
      TH1D *hist_data = (TH1D*)map_string_to_file["data"]->Get(histname);
      hist_data->SetMarkerStyle(8);
      hist_data->SetMarkerColor(kBlack);
      
      //==== prepare stack for MC
      THStack* MC_stack = new THStack("MC_Stack", "");
      
      //==== hists for legend
      vector<TH1D*> MChist_for_legend;
      vector<TString> MCalias_for_legend;
      MChist_for_legend.clear();
      MCalias_for_legend.clear();
      
      //==== MC iterator
      vector<TString>::iterator it_MC;
      it_MC = map_string_to_MC_list[this_MC_type].begin();

      //==== MC loop
      for(int aaa=0; it_MC != it_MC_END; ++it_MC, aaa++ ){
        TString this_samplename = *it_MC;
        TH1D* MC_temp = (TH1D*)map_string_to_file[this_samplename]->Get(histname);
        
        if( !MC_temp ){
          cout << "No Histogram : " << this_samplename << endl;
          continue;
        }
        else{
          MC_temp->SetFillColor( map_string_to_MC_color[this_MC_type].at(aaa) );
          MC_temp->SetLineColor( map_string_to_MC_color[this_MC_type].at(aaa) );
        }
        
        //==== MCRescale
        MC_temp->Scale(MCRescale);
        
        MChist_for_legend.push_back( (TH1D*)MC_temp->Clone() );
        MCalias_for_legend.push_back( map_string_to_MC_alias[this_MC_type].at(aaa) );
        MC_stack->Add(MC_temp);
      }
      
      
      //==== legend
      TLegend* lg = new TLegend(0.6, 0.6, 0.9, 0.9);
      lg->SetFillStyle(0);
      lg->SetBorderSize(0);
      lg->AddEntry(hist_data, "Data", "p");
      make_legend(lg, this_MC_type, MChist_for_legend, MCalias_for_legend);
      
      TCanvas* c_Loose_study = new TCanvas("c_Loose_study", "", 800, 800);
      canvas_margin(c_Loose_study);
      c_Loose_study->cd();
      gPad->SetLogy();
      //==== MC
      MC_stack->Draw("hist");
      MC_stack->GetXaxis()->SetTitle(this_var_Loose_study);
      MC_stack->GetYaxis()->SetTitle("Events");
      MC_stack->SetTitle("");
      MC_stack->SetMaximum(1000000);
      MC_stack->SetMinimum(1);
      hist_axis(MC_stack);
      //==== data
      hist_data->Draw("psame");
      lg->Draw();

      latex_CMSPriliminary.SetTextSize(0.035);
      latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} Simulation");
      latex_Lumi.SetTextSize(0.035);
      latex_Lumi.DrawLatex(0.7, 0.96, "35.9 fb^{-1} (13 TeV)");

      c_Loose_study->SaveAs(plotpath+"/LooseMuon_Study_"+this_type_Loose_study+this_var_Loose_study+".pdf");
      c_Loose_study->Close();
      delete c_Loose_study;
      
    }
  }
  
  //======================================================
  //==== dXYSig cut optimization plots using DY, QCD
  //======================================================

  cout << "######################## dXYSig cut optimization study ########################" << endl;
  vector<TString> dXYs = {"dXY", "dXYSig"};
  vector<TString> MuonId = {"Loose", "Tight"};

  for(unsigned int i=0; i<MuonId.size(); i++){
    for(unsigned int j=0; j<dXYs.size(); j++){
      
      TH1D* hist_DY = (TH1D*)map_string_to_file["DY"]->Get(str_dXYCut+"_prompt_"+MuonId.at(i)+"_"+dXYs.at(j));
      TH1D* hist_signal = (TH1D*)map_string_to_file["HN_MuMuMu_40"]->Get(str_dXYCut+"_prompt_"+MuonId.at(i)+"_"+dXYs.at(j));
      TH1D* hist_ttbar = (TH1D*)map_string_to_file["TTJets_aMC"]->Get(str_dXYCut+"_fake_"+MuonId.at(i)+"_"+dXYs.at(j));
      TH1D* hist_QCD_mumu = (TH1D*)map_string_to_file["QCD_mu"]->Get(str_dXYCut+"_fake_"+MuonId.at(i)+"_"+dXYs.at(j));
      
      hist_DY->SetLineColor(kBlue);
      hist_signal->SetLineColor(kBlack);
      hist_ttbar->SetLineColor(kRed);
      hist_QCD_mumu->SetLineColor(kOrange-5);
      
      hist_DY->SetLineWidth(2);
      hist_signal->SetLineWidth(2);
      hist_ttbar->SetLineWidth(2);
      hist_QCD_mumu->SetLineWidth(2);

      int n_xbin = hist_DY->GetXaxis()->GetNbins();

      TCanvas *c_dXY_eff = new TCanvas("c_dXY_eff", "", 800, 800);
      canvas_margin(c_dXY_eff);
      c_dXY_eff->cd();
      double xbins[n_xbin];
      double eff_DY_small[n_xbin], eff_signal_small[n_xbin], eff_ttbar_small[n_xbin], eff_QCD_mumu_small[n_xbin];
      double eff_DY_large[n_xbin], eff_signal_large[n_xbin], eff_ttbar_large[n_xbin], eff_QCD_mumu_large[n_xbin];

      cout << "dXYSig" << "\t" << "DY_small" << "\t" << "DY_large" << "\t" << "signal_small" << "\t" << "signal_large" << "\t" << "ttbar_small" << "\t" << "ttbra_large" << "\t" << "QCD_small" << "\t" << "QCD_large" << endl;
      for(unsigned int k=1; k<=n_xbin; k++){

        xbins[k-1] = hist_DY->GetXaxis()->GetBinUpEdge(k);

        eff_DY_small[k-1] = hist_DY->Integral(0, k) / hist_DY->Integral(0, n_xbin+1);
        eff_signal_small[k-1] = hist_signal->Integral(0, k) / hist_signal->Integral(0, n_xbin+1);
        eff_ttbar_small[k-1] = hist_ttbar->Integral(0, k) / hist_ttbar->Integral(0, n_xbin+1);
        eff_QCD_mumu_small[k-1] = hist_QCD_mumu->Integral(0, k) / hist_QCD_mumu->Integral(0, n_xbin+1);

        eff_DY_large[k-1] = hist_DY->Integral(k+1, n_xbin+1) / hist_DY->Integral(0, n_xbin+1);
        eff_signal_large[k-1] = hist_signal->Integral(k+1, n_xbin+1) / hist_signal->Integral(0, n_xbin+1);
        eff_ttbar_large[k-1] = hist_ttbar->Integral(k+1, n_xbin+1) / hist_ttbar->Integral(0, n_xbin+1);
        eff_QCD_mumu_large[k-1] = hist_QCD_mumu->Integral(k+1, n_xbin+1) / hist_QCD_mumu->Integral(0, n_xbin+1);

        cout << hist_DY->GetXaxis()->GetBinUpEdge(k) << "\t"
             << eff_DY_small[k-1] << "\t" << eff_DY_large[k-1] << "\t"
             << eff_signal_small[k-1] << "\t" << eff_signal_large[k-1] << "\t"
             << eff_ttbar_small[k-1] << "\t" << eff_ttbar_large[k-1] << "\t"
             << eff_QCD_mumu_small[k-1] << "\t" << eff_QCD_mumu_large[k-1] << "\t" << endl;
      }

      TGraph *gr_eff_DY_small = new TGraph(n_xbin, xbins, eff_DY_small);
      TGraph *gr_eff_DY_large = new TGraph(n_xbin, xbins, eff_DY_large);
      gr_eff_DY_small->SetLineColor(kBlue);
      gr_eff_DY_large->SetLineColor(kBlue);
      gr_eff_DY_small->SetLineWidth(2);
      gr_eff_DY_large->SetLineWidth(2);
      gr_eff_DY_large->SetLineStyle(3);

      TGraph *gr_eff_signal_small = new TGraph(n_xbin, xbins, eff_signal_small);
      TGraph *gr_eff_signal_large = new TGraph(n_xbin, xbins, eff_signal_large);
      gr_eff_signal_small->SetLineColor(kBlack);
      gr_eff_signal_large->SetLineColor(kBlack);
      gr_eff_signal_small->SetLineWidth(2);
      gr_eff_signal_large->SetLineWidth(2);
      gr_eff_signal_large->SetLineStyle(3);

      TGraph *gr_eff_ttbar_small = new TGraph(n_xbin, xbins, eff_ttbar_small);
      TGraph *gr_eff_ttbar_large = new TGraph(n_xbin, xbins, eff_ttbar_large);
      gr_eff_ttbar_small->SetLineColor(kRed);
      gr_eff_ttbar_large->SetLineColor(kRed);
      gr_eff_ttbar_small->SetLineWidth(2);
      gr_eff_ttbar_large->SetLineWidth(2);
      gr_eff_ttbar_large->SetLineStyle(3);

      TGraph *gr_eff_QCD_mumu_small = new TGraph(n_xbin, xbins, eff_QCD_mumu_small);
      TGraph *gr_eff_QCD_mumu_large = new TGraph(n_xbin, xbins, eff_QCD_mumu_large);
      gr_eff_QCD_mumu_small->SetLineColor(kOrange-5);
      gr_eff_QCD_mumu_large->SetLineColor(kOrange-5);
      gr_eff_QCD_mumu_small->SetLineWidth(2);
      gr_eff_QCD_mumu_large->SetLineWidth(2);
      gr_eff_QCD_mumu_large->SetLineStyle(3);

      TLegend* lg_eff_dXY = new TLegend(0.4, 0.3, 0.9, 0.7);
      lg_eff_dXY->SetFillStyle(0);
      lg_eff_dXY->SetBorderSize(0);
      lg_eff_dXY->AddEntry(gr_eff_DY_small, "Small, Drell-Yan (prompt)", "l");
      lg_eff_dXY->AddEntry(gr_eff_signal_small, "Small, m(HN) = 40 GeV (prompt)", "l");
      lg_eff_dXY->AddEntry(gr_eff_ttbar_small, "Small, t#bar{t} (fake)", "l");
      lg_eff_dXY->AddEntry(gr_eff_QCD_mumu_small, "Small, QCD (fake)", "l");
      lg_eff_dXY->AddEntry(gr_eff_DY_large, "Large, Drell-Yan (prompt)", "l");
      lg_eff_dXY->AddEntry(gr_eff_signal_large, "Large, m(HN) = 40 GeV (prompt)", "l");
      lg_eff_dXY->AddEntry(gr_eff_ttbar_large, "Large, t#bar{t} (fake)", "l");
      lg_eff_dXY->AddEntry(gr_eff_QCD_mumu_large, "Large, QCD (fake)", "l");

      gr_eff_DY_small->SetTitle("");
      gr_eff_DY_small->Draw("al");
      gr_eff_DY_small->GetYaxis()->SetTitle("Efficiency");
      gr_eff_DY_small->GetYaxis()->SetRangeUser(0.00001, 2.);
      if(dXYs.at(j)=="dXYSig") gr_eff_DY_small->GetXaxis()->SetTitle("|SIP|");
      if(dXYs.at(j)=="dXY") gr_eff_DY_small->GetXaxis()->SetTitle("|d_{xy}| [cm]");
      hist_axis(gr_eff_DY_small);
      gr_eff_DY_large->Draw("lsame");
      gr_eff_signal_small->Draw("lsame");
      gr_eff_signal_large->Draw("lsame");
      gr_eff_ttbar_small->Draw("lsame");
      gr_eff_ttbar_large->Draw("lsame");
      gr_eff_QCD_mumu_small->Draw("lsame");
      gr_eff_QCD_mumu_large->Draw("lsame");
      lg_eff_dXY->Draw();

      latex_CMSPriliminary.SetTextSize(0.035);
      latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} Simulation");
      latex_Lumi.SetTextSize(0.035);
      latex_Lumi.DrawLatex(0.7, 0.96, "35.9 fb^{-1} (13 TeV)");

      c_dXY_eff->SetLogy();
      c_dXY_eff->SaveAs(plotpath+"/dXYSig_Study_Eff_"+dXYs.at(j)+"_"+MuonId.at(i)+"IsoMuon.pdf");
      c_dXY_eff->Close();
      delete c_dXY_eff;

      hist_DY->Scale(1./hist_DY->Integral());
      hist_signal->Scale(1./hist_signal->Integral());
      hist_ttbar->Scale(1./hist_ttbar->Integral());
      hist_QCD_mumu->Scale(1./hist_QCD_mumu->Integral());
      
      TLegend* lg_dXY = new TLegend(0.55, 0.7, 0.9, 0.95);
      lg_dXY->SetFillStyle(0);
      lg_dXY->SetBorderSize(0);
      lg_dXY->AddEntry(hist_DY, "Drell-Yan (prompt)", "l");
      lg_dXY->AddEntry(hist_signal, "m(HN) = 40 GeV (prompt)", "l");
      lg_dXY->AddEntry(hist_ttbar, "t#bar{t} (fake)", "l");
      lg_dXY->AddEntry(hist_QCD_mumu, "QCD (fake)", "l");
      
      TCanvas* c_dXY = new TCanvas("c_dXY", "", 800, 800);
      canvas_margin(c_dXY);
      c_dXY->cd();
      c_dXY->SetLogy(kTRUE);
      
      hist_DY->Draw("histsame");
      hist_DY->SetTitle("");
      hist_DY->SetYTitle("A. U.");
      if(dXYs.at(j)=="dXYSig") hist_DY->SetXTitle("|SIP|"); 
      if(dXYs.at(j)=="dXY") hist_DY->SetXTitle("|d_{xy}| [cm]");
      hist_axis(hist_DY);
      hist_signal->Draw("histsame");
      hist_ttbar->Draw("histsame");
      hist_QCD_mumu->Draw("histsame");
      lg_dXY->Draw();

      latex_CMSPriliminary.SetTextSize(0.035);
      latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} Simulation");
      latex_Lumi.SetTextSize(0.035);
      latex_Lumi.DrawLatex(0.7, 0.96, "35.9 fb^{-1} (13 TeV)");

      c_dXY->SaveAs(plotpath+"/dXYSig_Study_"+dXYs.at(j)+"_"+MuonId.at(i)+"IsoMuon.pdf");

      c_dXY->Close();
      delete c_dXY;
      delete lg_dXY;

    }
  }

  //=============================================
  //==== FR plots (Num/Den, 2D FR, 1D FR curve)
  //=============================================
  
  cout << "######################## FR plots ########################" << endl;
  
  vector<TString> FR_method = {
    "HighdXY_alljet",
    "HighdXY_0jet",
    "HighdXY_withjet",
    "HighdXY_0bjet",
    "HighdXY_withbjet",
    "DiMuonTrigger_ZTag_Small",
    "SingleMuonTrigger_Dijet"
  };
  vector<TString> FR_method_alias = {
    "HighdXY all jet",
    "HighdXY no jet",
    "HighdXY with jet",
    "HighdXY no bjet",
    "HighdXY with bjet"
  };
  vector<Color_t> FR_method_color = {
    kBlack,
    kBlue,
    kRed,
    kBlue,
    kRed,
  };
  vector<Color_t> FR_method_style = {
    1,
    1,
    1,
    3,
    3,
  };

  vector<TString> var_FR = {"eta", "pt", "RelIso", "Chi2", "dXY", "dXYSig"};
  vector<TString> x_title_FR = {"#eta", "p_{T} [GeV]", "RelIso", "#chi^{2}", "|d_{XY}| [cm]", "|SIP|"};
  
  //==== jet dependency test
  //==== use barrel (0<|eta|<0.8)
  vector<TGraphAsymmErrors*> grs_FRcurvesBarrel;
  
  //==== now, loop over FR methods!

  for(unsigned int it_FR_method=0; it_FR_method<FR_method.size(); it_FR_method++){
    
    TString this_FR_method = FR_method.at(it_FR_method);
    cout << "######################## " << this_FR_method << endl;
    
    TString hist_prefix = str_dXYCut+"_";
    if(this_FR_method=="SingleMuonTrigger_Dijet") hist_prefix = "";
    
    //==== store # of (data-prompt MC), to optimize which dXYSig min to use
    TH1D *hist_n_data_prompt_subtraction_num = new TH1D("hist_n_data_prompt_subtraction_num", "", 1, 0, 1);
    TH1D *hist_n_data_prompt_subtraction_den = new TH1D("hist_n_data_prompt_subtraction_den", "", 1, 0, 1);
    
    //==== prepare MC iterator end point
    vector<TString>::iterator it_MC_END;
    TString this_MC_type = "SingleMuon";
    if(this_FR_method.Contains("DiMuon")) this_MC_type = "DiMuon";
    
    it_MC_END = map_string_to_MC_list[this_MC_type].end();
    
    //==== Num/Den
    for(unsigned int it_var_FR=0; it_var_FR<var_FR.size(); it_var_FR++){
    
      TString this_var_FR = var_FR.at(it_var_FR);
      TString this_x_title_FR = x_title_FR.at(it_var_FR);
      
      //==== data
      TH1D *num_data = (TH1D*)map_string_to_file["data"]->Get(hist_prefix+this_FR_method+"_"+this_var_FR+"_F");
      TH1D *den_data = (TH1D*)map_string_to_file["data"]->Get(hist_prefix+this_FR_method+"_"+this_var_FR+"_F0");
      num_data->SetMarkerStyle(8);
      num_data->SetMarkerColor(kBlack);
      den_data->SetMarkerStyle(8);
      den_data->SetMarkerColor(kBlack);
      
      //==== Fill # of event
      if(it_var_FR==0){
        hist_n_data_prompt_subtraction_num->Fill(0., num_data->Integral(0,num_data->GetXaxis()->GetNbins()+1));
        hist_n_data_prompt_subtraction_den->Fill(0., den_data->Integral(0,den_data->GetXaxis()->GetNbins()+1));
      }
      
      //==== prepare stack for prompt MC
      THStack* num_MC_stack = new THStack("num_MC_stack", "");
      THStack* den_MC_stack = new THStack("den_MC_stack", "");
      
      //==== rebin pT..
      if(this_var_FR == "pt"){
        num_data->Rebin(5);
        den_data->Rebin(5);
      }
      
      //==== hists for legend
      vector<TH1D*> MChist_for_legend;
      vector<TString> MCalias_for_legend;
      MChist_for_legend.clear();
      MCalias_for_legend.clear();
      
      //==== MC iterator
      vector<TString>::iterator it_MC;
      it_MC = map_string_to_MC_list[this_MC_type].begin();
      
      //==== MC loop
      for(int aaa=0; it_MC != it_MC_END; ++it_MC, aaa++ ){
        TString this_samplename = *it_MC;
        TH1D* num_MC_temp = (TH1D*)map_string_to_file[this_samplename]->Get(hist_prefix+this_FR_method+"_"+this_var_FR+"_F");
        TH1D* den_MC_temp = (TH1D*)map_string_to_file[this_samplename]->Get(hist_prefix+this_FR_method+"_"+this_var_FR+"_F0");
        
        if( !num_MC_temp || !den_MC_temp ){
          cout << "No Histogram : " << this_samplename << endl;
          continue;
        }
        else{
          
          //==== MCRescale
          num_MC_temp->Scale(MCRescale);
          den_MC_temp->Scale(MCRescale);
          
          if(this_var_FR == "pt"){
            num_MC_temp->Rebin(5);
            den_MC_temp->Rebin(5);
          }
          
          num_MC_temp->SetFillColor( map_string_to_MC_color[this_MC_type].at(aaa) );
          num_MC_temp->SetLineColor( map_string_to_MC_color[this_MC_type].at(aaa) );
          den_MC_temp->SetFillColor( map_string_to_MC_color[this_MC_type].at(aaa) );
          den_MC_temp->SetLineColor( map_string_to_MC_color[this_MC_type].at(aaa) );
        }
        
        //==== Prompt subtraction to # of event
        if(it_var_FR==0){
          hist_n_data_prompt_subtraction_num->Fill(0., num_MC_temp->Integral(0,-1.*num_MC_temp->GetXaxis()->GetNbins()+1));
          hist_n_data_prompt_subtraction_den->Fill(0., den_MC_temp->Integral(0,-1.*den_MC_temp->GetXaxis()->GetNbins()+1));
        }
        
        MChist_for_legend.push_back( (TH1D*)num_MC_temp->Clone() );
        MCalias_for_legend.push_back( map_string_to_MC_alias[this_MC_type].at(aaa) );
        num_MC_stack->Add(num_MC_temp);
        den_MC_stack->Add(den_MC_temp);
      }
      
      //==== legend
      TLegend *lg = new TLegend(0.6, 0.6, 0.9, 0.9);
      lg->SetFillStyle(0);
      lg->SetBorderSize(0);
      lg->AddEntry(num_data, "Data", "p");
      make_legend(lg, this_MC_type, MChist_for_legend, MCalias_for_legend);
      
      //==== Y axis range
      if(this_FR_method.Contains("HighdXY")){
        if(this_var_FR == "pt"){
          num_MC_stack->SetMaximum(10000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(10000000);
          den_MC_stack->SetMinimum(1);
        }
        if(this_var_FR == "eta"){
          num_MC_stack->SetMaximum(10000000);
          num_MC_stack->SetMinimum(10);
          den_MC_stack->SetMaximum(100000000);
          den_MC_stack->SetMinimum(10);
        }
        if(this_var_FR == "RelIso"){
          num_MC_stack->SetMaximum(1000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(1000000);
          den_MC_stack->SetMinimum(1);
        }
        if(this_var_FR == "Chi2"){
          num_MC_stack->SetMaximum(1000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(1000000);
          den_MC_stack->SetMinimum(1);
        }
        if(this_var_FR == "dXY"){
          num_MC_stack->SetMaximum(1000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(1000000);
          den_MC_stack->SetMinimum(1);
        }
        if(this_var_FR == "dXYSig"){
          num_MC_stack->SetMaximum(10000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(10000000);
          den_MC_stack->SetMinimum(1);
        }
      }
      if(this_FR_method == "DiMuonTrigger_ZTag_Small"){
        if(this_var_FR == "pt"){
          num_MC_stack->SetMaximum(10000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(10000000);
          den_MC_stack->SetMinimum(1);
        }
        if(this_var_FR == "eta"){
          num_MC_stack->SetMaximum(1000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(1000000);
          den_MC_stack->SetMinimum(1);
        }
        if(this_var_FR == "RelIso"){
          num_MC_stack->SetMaximum(1000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(1000000);
          den_MC_stack->SetMinimum(1);
        }
        if(this_var_FR == "Chi2"){
          num_MC_stack->SetMaximum(1000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(1000000);
          den_MC_stack->SetMinimum(1);
        }
        if(this_var_FR == "dXY"){
          num_MC_stack->SetMaximum(1000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(1000000);
          den_MC_stack->SetMinimum(1);
        }
        if(this_var_FR == "dXYSig"){
          num_MC_stack->SetMaximum(10000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(10000000);
          den_MC_stack->SetMinimum(1);
        }
      }
      if(this_FR_method == "SingleMuonTrigger_Dijet"){
        if(this_var_FR == "pt"){
          num_MC_stack->SetMaximum(10000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(10000000);
          den_MC_stack->SetMinimum(1);
        }
        if(this_var_FR == "eta"){
          num_MC_stack->SetMaximum(1000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(1000000);
          den_MC_stack->SetMinimum(1);
        }
        if(this_var_FR == "RelIso"){
          num_MC_stack->SetMaximum(1000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(1000000);
          den_MC_stack->SetMinimum(1);
        }
        if(this_var_FR == "Chi2"){
          num_MC_stack->SetMaximum(1000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(1000000);
          den_MC_stack->SetMinimum(1);
        }
        if(this_var_FR == "dXY"){
          num_MC_stack->SetMaximum(1000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(1000000);
          den_MC_stack->SetMinimum(1);
        }
        if(this_var_FR == "dXYSig"){
          num_MC_stack->SetMaximum(10000000);
          num_MC_stack->SetMinimum(1);
          den_MC_stack->SetMaximum(10000000);
          den_MC_stack->SetMinimum(1);
        }
      }
      
      //==== draw numerator
      TCanvas* c_num = new TCanvas("c_num", "", 800, 800);
      canvas_margin(c_num);
      c_num->cd();
      gPad->SetLogy();
      //==== MC
      num_MC_stack->Draw("hist");
      num_MC_stack->GetXaxis()->SetTitle(this_x_title_FR);
      num_MC_stack->GetYaxis()->SetTitle("Numerator Events");
      num_MC_stack->SetTitle("");
      hist_axis(num_MC_stack);
      //==== data
      num_data->Draw("psame");
      lg->Draw();

      latex_CMSPriliminary.SetTextSize(0.035);
      latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} Simulation");
      latex_Lumi.SetTextSize(0.035);
      latex_Lumi.DrawLatex(0.7, 0.96, "35.9 fb^{-1} (13 TeV)");

      c_num->SaveAs(plotpath+"/num_"+this_FR_method+"_"+this_var_FR+".pdf");
      c_num->Close();
      delete c_num;
      
      //==== draw denominator
      TCanvas* c_den = new TCanvas("c_den", "", 800, 800);
      canvas_margin(c_den);
      c_den->cd();
      gPad->SetLogy();
      //==== MC
      den_MC_stack->Draw("hist");
      den_MC_stack->GetXaxis()->SetTitle(this_x_title_FR);
      den_MC_stack->GetYaxis()->SetTitle("Denominator Events");
      den_MC_stack->SetTitle("");
      hist_axis(den_MC_stack);
      //==== data
      den_data->Draw("psame");
      lg->Draw();

      latex_CMSPriliminary.SetTextSize(0.035);
      latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} Simulation");
      latex_Lumi.SetTextSize(0.035);
      latex_Lumi.DrawLatex(0.7, 0.96, "35.9 fb^{-1} (13 TeV)");

      c_den->SaveAs(plotpath+"/den_"+this_FR_method+"_"+this_var_FR+".pdf");
      c_den->Close();
      delete c_den;
      
      
    } // END var_FR loop
    
    //==== 2D FR
    
    //==== data
    cout << hist_prefix+this_FR_method+"_events_F" << endl;
    TH2D* num_data = (TH2D*)map_string_to_file["data"]->Get(hist_prefix+this_FR_method+"_events_F");
    TH2D* den_data = (TH2D*)map_string_to_file["data"]->Get(hist_prefix+this_FR_method+"_events_F0");
    TH2D* num_data_subtracted = (TH2D*)num_data->Clone();
    TH2D* den_data_subtracted = (TH2D*)den_data->Clone();
    
    //==== MC iterator
    vector<TString>::iterator it_MC;
    it_MC = map_string_to_MC_list[this_MC_type].begin();
    
    //==== MC loop
    for(int aaa=0; it_MC != it_MC_END; ++it_MC, aaa++ ){
      TString this_samplename = *it_MC;
      TH2D *num_MC_temp = (TH2D*)map_string_to_file[this_samplename]->Get(hist_prefix+this_FR_method+"_events_F");
      TH2D *den_MC_temp = (TH2D*)map_string_to_file[this_samplename]->Get(hist_prefix+this_FR_method+"_events_F0");
      if( !num_MC_temp || !den_MC_temp ){
        cout << "No Histogram : " << this_samplename << endl;
        continue;
      }
      
      //==== MCRescale
      num_MC_temp->Scale(MCRescale);
      den_MC_temp->Scale(MCRescale);
      
      num_data_subtracted->Add(num_MC_temp, -1.);
      den_data_subtracted->Add(den_MC_temp, -1.);
    }

    TH2D *num_prompt_ratio = new TH2D("num_prompt_ratio", "", num_data->GetXaxis()->GetNbins(), num_data->GetXaxis()->GetXbins()->GetArray(), num_data->GetYaxis()->GetNbins(), num_data->GetYaxis()->GetXbins()->GetArray());
    TH2D *den_prompt_ratio = new TH2D("den_prompt_ratio", "", num_data->GetXaxis()->GetNbins(), num_data->GetXaxis()->GetXbins()->GetArray(), num_data->GetYaxis()->GetNbins(), num_data->GetYaxis()->GetXbins()->GetArray());
    //==== check numbers
    for(int aaa=1;aaa<=num_data->GetXaxis()->GetNbins();aaa++){
      for(int bbb=1;bbb<=num_data->GetYaxis()->GetNbins();bbb++){
        if(num_data->GetXaxis()->GetBinLowEdge(aaa)>=60) continue;
        //cout << "x : ["<<num_data->GetXaxis()->GetBinLowEdge(aaa)<<","<<num_data->GetXaxis()->GetBinUpEdge(aaa)<<"], y : ["<<num_data->GetYaxis()->GetBinLowEdge(bbb)<<","<<num_data->GetYaxis()->GetBinUpEdge(bbb)<<"]"<<endl;
        //cout << "num, before : " << num_data->GetBinContent(aaa,bbb) << endl;
        //cout << "num, after : " << num_data_subtracted->GetBinContent(aaa,bbb) << endl;
        //cout << "==> " << (num_data->GetBinContent(aaa,bbb)-num_data_subtracted->GetBinContent(aaa,bbb))/num_data->GetBinContent(aaa,bbb) << endl;
        num_prompt_ratio->SetBinContent(aaa,bbb, (num_data->GetBinContent(aaa,bbb)-num_data_subtracted->GetBinContent(aaa,bbb))/num_data->GetBinContent(aaa,bbb));
        //cout << "den, before : " << den_data->GetBinContent(aaa,bbb) << endl;
        //cout << "den, after : " << den_data_subtracted->GetBinContent(aaa,bbb) << endl;
        //cout << "==> " << (den_data->GetBinContent(aaa,bbb)-den_data_subtracted->GetBinContent(aaa,bbb))/den_data->GetBinContent(aaa,bbb) << endl;
        den_prompt_ratio->SetBinContent(aaa,bbb, (den_data->GetBinContent(aaa,bbb)-den_data_subtracted->GetBinContent(aaa,bbb))/den_data->GetBinContent(aaa,bbb));
      }
    }
    TCanvas* c_num_prompt_ratio = new TCanvas("c_num_prompt_ratio", "", 1600, 1100);
    //canvas_margin(c_data);
    c_num_prompt_ratio->SetLeftMargin(0.07);
    c_num_prompt_ratio->SetRightMargin( 0.1 );
    gStyle->SetPaintTextFormat("0.4f");
    num_prompt_ratio->Draw("colztexte1");
    double minpt = 10;
    if(Draw5GeV) minpt = 5;
    num_prompt_ratio->GetXaxis()->SetRangeUser(minpt, 60);
    num_prompt_ratio->SetXTitle("p_{T} [GeV/c]");
    num_prompt_ratio->SetYTitle("|#eta|");
    num_prompt_ratio->SetTitle("");
    num_prompt_ratio->SetMarkerSize(1.3);
    c_num_prompt_ratio->SaveAs(plotpath+"/2D_fakerate_"+this_FR_method+"_num_prompt_ratio.pdf");
    c_num_prompt_ratio->Close();
    delete c_num_prompt_ratio;
    
    TCanvas* c_den_prompt_ratio = new TCanvas("c_den_prompt_ratio", "", 1600, 1100);
    //canvas_margin(c_data);
    c_den_prompt_ratio->SetLeftMargin(0.07);
    c_den_prompt_ratio->SetRightMargin( 0.1 );
    gStyle->SetPaintTextFormat("0.4f");
    den_prompt_ratio->Draw("colztexte1");
    den_prompt_ratio->GetXaxis()->SetRangeUser(minpt, 60);
    den_prompt_ratio->SetXTitle("p_{T} [GeV/c]");
    den_prompt_ratio->SetYTitle("|#eta|");
    den_prompt_ratio->SetTitle("");
    den_prompt_ratio->SetMarkerSize(1.3);
    c_den_prompt_ratio->SaveAs(plotpath+"/2D_fakerate_"+this_FR_method+"_den_prompt_ratio.pdf");
    c_den_prompt_ratio->Close();
    delete c_den_prompt_ratio;
    
    int n_xbins = num_data->GetXaxis()->GetNbins();
    int n_ybins = num_data->GetYaxis()->GetNbins();

    //==== before subtraction (= data only)
    TCanvas* c_data = new TCanvas("c_data", "", 1600, 1100);
    //canvas_margin(c_data);
    c_data->SetLeftMargin(0.07);
    c_data->SetRightMargin( 0.1 );
    gStyle->SetPaintTextFormat("0.4f");
    num_data->Divide(den_data);
    num_data->Draw("colztexte1");
    num_data->GetXaxis()->SetRangeUser(minpt, 60);
    num_data->SetXTitle("p_{T} [GeV/c]");
    num_data->SetYTitle("|#eta|");
    num_data->SetTitle("");
    num_data->SetMarkerSize(1.3);
    c_data->SaveAs(plotpath+"/2D_fakerate_"+this_FR_method+"_before_Prompt_subtraction.pdf");
    c_data->Close();
    delete c_data;
    
    //==== after subtraction
    TCanvas* c_subtracted = new TCanvas("c_subtracted", "", 1600, 1100);
    //canvas_margin(c_subtracted);
    c_subtracted->SetLeftMargin(0.07);
    c_subtracted->SetRightMargin( 0.1 );
    gStyle->SetPaintTextFormat("0.4f");
    //==== check negative values
    for(int i_x=0; i_x<n_xbins; i_x++){
      for(int i_y=0; i_y<n_ybins; i_y++){
        if(num_data_subtracted->GetBinContent(i_x+1, i_y+1) <= 0 || den_data_subtracted->GetBinContent(i_x+1, i_y+1) <= 0 ){
          num_data_subtracted->SetBinContent(i_x+1, i_y+1, 0);
        }
      }
    }
    num_data_subtracted->Divide(den_data_subtracted);
    num_data_subtracted->Draw("colztexte1");
    num_data_subtracted->GetXaxis()->SetRangeUser(minpt, 60);
    num_data_subtracted->SetXTitle("p_{T} [GeV/c]");
    num_data_subtracted->SetYTitle("|#eta|");
    num_data_subtracted->SetTitle("");
    num_data_subtracted->SetMarkerSize(1.3);
    c_subtracted->SaveAs(plotpath+"/2D_fakerate_"+this_FR_method+"_after_Prompt_subtraction.pdf");
    c_subtracted->Close();
    delete c_subtracted;
    //==== write rootfile
    TString filename = plotpath+"/13TeV_trimuon_FR_"+this_FR_method+".root";
    TFile* file_FR = new TFile(filename, "RECREATE");
    file_FR->cd();
    num_data_subtracted->Write();
    hist_n_data_prompt_subtraction_num->Write();
    hist_n_data_prompt_subtraction_den->Write();
    file_FR->Close();

    //==== draw FR curve for each eta region
    TCanvas *c_FR_curve = new TCanvas("c_FR_curve", "", 800, 800);
    canvas_margin(c_FR_curve);
    c_FR_curve->cd();
    TLegend* lg_FR_curve = new TLegend(0.6, 0.6, 0.9, 0.9);
    lg_FR_curve->SetFillStyle(0);
    lg_FR_curve->SetBorderSize(0);

    int n_pt_bins = 7;
    vector<double> vec_pt_bins = {10., 15., 20., 25., 30., 35., 45., 60.};
    if(Draw5GeV){
      n_pt_bins = 8;
      vec_pt_bins = {5., 10., 15., 20., 25., 30., 35., 45., 60.};
    }
    int n_eta_bins = 4; // eta : 0.0-0.8-1.479-2.0-2.5

    TGraphAsymmErrors *gr_FR_curve[n_eta_bins];

    //==== fill pt(x) bins
    double x_bins[n_pt_bins+1];
    for(int aaa=0;aaa<n_pt_bins+1;aaa++){
      x_bins[aaa] = vec_pt_bins.at(aaa);
    }

    //==== fill colors for each eta regions
    Color_t colors[n_eta_bins];
    colors[0] = kBlack;
    colors[1] = kRed;
    colors[2] = kBlue;
    colors[3] = kViolet;

    //==== 2D FR Histogram starts from 0 GeV bin
    //==== [5,10] GeV is bin2
    //==== [10,15] GeV is bin3
    int PtBinToSkip = 3;
    if(Draw5GeV) PtBinToSkip = 2;
    for(int j=0; j<n_eta_bins; j++){
      TH1D *FR_curve = new TH1D("FR_eta_"+TString::Itoa(j,10), "", n_pt_bins, x_bins);
      for(int k=0; k<n_pt_bins; k++){
        FR_curve->SetBinContent(k+1, num_data_subtracted->GetBinContent(k+PtBinToSkip, j+1) );
        FR_curve->SetBinError(k+1, num_data_subtracted->GetBinError(k+PtBinToSkip, j+1) );
      }
      gr_FR_curve[j] = hist_to_graph(FR_curve);
      gr_FR_curve[j]->SetLineColor(colors[j]);
      gr_FR_curve[j]->SetLineWidth(2);
      gr_FR_curve[j]->GetYaxis()->SetTitle("Fake Rate");
      gr_FR_curve[j]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
      gr_FR_curve[j]->SetTitle("");
      hist_axis(gr_FR_curve[j]);  
      if(j==0){
        gr_FR_curve[j]->Draw("e1al");
        gr_FR_curve[j]->GetYaxis()->SetRangeUser(0, 0.5);
        hist_axis(gr_FR_curve[j]);
      }
      else gr_FR_curve[j]->Draw("e1lsame");
    }
    lg_FR_curve->AddEntry(gr_FR_curve[0], "0 < |#eta| < 0.8", "l");
    lg_FR_curve->AddEntry(gr_FR_curve[1], "0.8 < |#eta| < 1.479", "l");
    lg_FR_curve->AddEntry(gr_FR_curve[2], "1.479 < |#eta| < 2.0", "l");
    lg_FR_curve->AddEntry(gr_FR_curve[3], "2.0 < |#eta| < 2.5", "l");
    grs_FRcurvesBarrel.push_back( (TGraphAsymmErrors*)gr_FR_curve[0]->Clone() );
    lg_FR_curve->Draw();

    latex_CMSPriliminary.SetTextSize(0.035);
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} Simulation");
    latex_Lumi.SetTextSize(0.035);
    latex_Lumi.DrawLatex(0.7, 0.96, "35.9 fb^{-1} (13 TeV)");

    c_FR_curve->SaveAs(plotpath+"/1D_pt_each_eta_FR_"+this_FR_method+".pdf");
    c_FR_curve->Close();
    delete c_FR_curve;
    
  } // END FR_method loop
  TCanvas *c_FRcurves = new TCanvas("c_FRcurves", "", 800, 800);
  canvas_margin(c_FRcurves);
  TLegend *lg_FRcurves = new TLegend(0.5, 0.45, 0.95, 0.88);
  lg_FRcurves->SetFillStyle(0);
  lg_FRcurves->SetBorderSize(0);
  c_FRcurves->cd();
  for(unsigned int i=0;i<FR_method.size();i++){
    if( FR_method.at(i).Contains("Dijet") || FR_method.at(i).Contains("DiMuonTrigger") ) continue;
    grs_FRcurvesBarrel.at(i)->GetYaxis()->SetRangeUser(0, 0.3);
    grs_FRcurvesBarrel.at(i)->GetYaxis()->SetTitle("Fake Rate");
    grs_FRcurvesBarrel.at(i)->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    grs_FRcurvesBarrel.at(i)->SetLineColor(FR_method_color.at(i));
    grs_FRcurvesBarrel.at(i)->SetLineStyle(FR_method_style.at(i));
    grs_FRcurvesBarrel.at(i)->SetLineWidth(2);
    hist_axis(grs_FRcurvesBarrel.at(i));
    if(i==0){
      grs_FRcurvesBarrel.at(i)->Draw("alp");
      grs_FRcurvesBarrel.at(i)->GetYaxis()->SetRangeUser(0, 0.5);
      hist_axis(grs_FRcurvesBarrel.at(i));
    }
    else grs_FRcurvesBarrel.at(i)->Draw("lpsame");
    lg_FRcurves->AddEntry(grs_FRcurvesBarrel.at(i), FR_method_alias.at(i), "l");
  }
  lg_FRcurves->Draw();

  latex_CMSPriliminary.SetTextSize(0.035);
  latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} Simulation");
  latex_Lumi.SetTextSize(0.035);
  latex_Lumi.DrawLatex(0.7, 0.96, "35.9 fb^{-1} (13 TeV)");

  c_FRcurves->SaveAs(plotpath+"/1D_pt_each_eta_FR_Barrels.pdf");

  //=============================
  //==== MCTruth (scale factor)
  //=============================
  
  cout << "######################## MC Truth ########################" << endl;

  vector<TString> MCTruth_njet = {"alljet_", "0jet_", "withjet_", "0bjet_", "withbjet_"};
  vector<TString> MC_sample_MCTruth = {"QCD_mu", "TTJets_aMC", "DY", "WJets"}; //FIXME no bb sample in v8-0-2

  vector<TString> sig_region = {"HighdXY_", ""};
  
  for(unsigned int it_MCTruth_njet=0; it_MCTruth_njet<MCTruth_njet.size(); it_MCTruth_njet++){

    TString this_MCTruth_njet = MCTruth_njet.at(it_MCTruth_njet); 

    for(unsigned int it_MC_sample_MCTruth=0; it_MC_sample_MCTruth<MC_sample_MCTruth.size(); it_MC_sample_MCTruth++){
    
      TString this_MC_sample_MCTruth = MC_sample_MCTruth.at(it_MC_sample_MCTruth);
      
      //==== 2D
      
      TH2D *large_2D, *small_2D;

      //==== FR for each significance region
      //==== and save FR for SF
      for(unsigned int it_sig_region=0; it_sig_region<sig_region.size(); it_sig_region++){
        TH2D* hist_num = (TH2D*)map_string_to_file[this_MC_sample_MCTruth]->Get(str_dXYCut+"_MCTruth_"+sig_region.at(it_sig_region)+this_MCTruth_njet+"events_F");
        TH2D* hist_den = (TH2D*)map_string_to_file[this_MC_sample_MCTruth]->Get(str_dXYCut+"_MCTruth_"+sig_region.at(it_sig_region)+this_MCTruth_njet+"events_F0");
        if( !hist_num || !hist_den ) continue;
        hist_num->Divide(hist_den);
        if(sig_region.at(it_sig_region)=="HighdXY_") large_2D = (TH2D*)hist_num->Clone();
        if(sig_region.at(it_sig_region)=="")         small_2D = (TH2D*)hist_num->Clone();
        TCanvas* c_MCTruth = new TCanvas("c_MCTruth", "", 1600, 1100);
        //canvas_margin(c_MCTruth);
        c_MCTruth->SetLeftMargin(0.07);
        c_MCTruth->SetRightMargin( 0.1 );
        gStyle->SetPaintTextFormat("0.4f");
        c_MCTruth->cd();
        hist_num->Draw("colztexte1");
        hist_num->GetXaxis()->SetRangeUser(5, 60);
        hist_num->SetXTitle("p_{T} [GeV/c]");
        hist_num->SetYTitle("|#eta|");
        hist_num->SetTitle("");
        hist_num->SetMarkerSize(1.3);
        TString histname_suffix("");
        if(sig_region.at(it_sig_region) == "HighdXY_") histname_suffix = "Large";
        if(sig_region.at(it_sig_region) == "") histname_suffix = "Small";
        c_MCTruth->SaveAs(plotpath+"/2D_FR_MCTruth_"+this_MCTruth_njet+this_MC_sample_MCTruth+"_"+histname_suffix+".pdf");
        c_MCTruth->Close();
        delete c_MCTruth;
      }
      
      //==== draw 2D SF
      TCanvas* c_2D_FR_SF = new TCanvas("c_2D_FR_SF", "", 1600, 1100);
      //canvas_margin(c_2D_FR_SF);
      c_2D_FR_SF->SetLeftMargin(0.07);
      c_2D_FR_SF->SetRightMargin( 0.1 );
      gStyle->SetPaintTextFormat("0.4f");
      c_2D_FR_SF->cd();
      //==== before dividing, save dXYSig small FR for MC-Closure test
      TString filename_FR = plotpath+"/13TeV_trimuon_FR_"+this_MCTruth_njet+this_MC_sample_MCTruth+".root";
      TFile* file_FR = new TFile(filename_FR, "RECREATE");
      file_FR->cd();
      small_2D->SetName("FR_Small_dXYSig"); 
      small_2D->Write();

      small_2D->Divide(large_2D);
      small_2D->SetName("FRSF");
      small_2D->Draw("colztexte1");
      small_2D->GetXaxis()->SetRangeUser(5, 60);
      small_2D->SetXTitle("p_{T} [GeV/c]");
      small_2D->SetYTitle("|#eta|");
      small_2D->SetTitle("");
      small_2D->SetMarkerSize(1.3);
      c_2D_FR_SF->SaveAs(plotpath+"/2D_FRSF_MCTruth_"+this_MCTruth_njet+this_MC_sample_MCTruth+".pdf");
      c_2D_FR_SF->Close();
      delete c_2D_FR_SF;
      
      //==== draw SF curve for each eta region
      TCanvas *c_1D_SF_curve = new TCanvas("c_1D_SF_curve", "", 800, 800);
      canvas_margin(c_1D_SF_curve);
      c_1D_SF_curve->cd();
      TLegend* lg_SF_curve = new TLegend(0.6, 0.6, 0.9, 0.9);
      lg_SF_curve->SetFillStyle(0);
      lg_SF_curve->SetBorderSize(0);

      TGraphAsymmErrors *gr_SF_curve[4];

      int n_pt_bins = 7;
      vector<double> vec_pt_bins = {10., 15., 20., 25., 30., 35., 45., 60.};
      if(Draw5GeV){
        n_pt_bins = 8;
        vec_pt_bins = {5., 10., 15., 20., 25., 30., 35., 45., 60.};
      }
      int n_eta_bins = 4; // eta : 0.0-0.8-1.479-2.0-2.5

      //==== fill pt(x) bins
      double x_bins[n_pt_bins+1];
      for(int aaa=0;aaa<n_pt_bins+1;aaa++){
        x_bins[aaa] = vec_pt_bins.at(aaa);
      }

      double y_bins[5] = {0.0, 0.8, 1.479, 2.0, 2.5};
      Color_t colors[4] = {kBlack, kRed, kBlue, kViolet};

      //==== 2D FR Histogram starts from 0 GeV bin
      //==== [5,10] GeV is bin2
      //==== [10,15] GeV is bin3
      int PtBinToSkip = 3;
      if(Draw5GeV) PtBinToSkip = 2;

      for(int j=0; j<4; j++){
        TH1D *SF_curve = new TH1D("FR_eta_"+TString::Itoa(j,10), "", 7, x_bins);
        for(int k=0; k<8; k++){
          SF_curve->SetBinContent(k+1, small_2D->GetBinContent(k+PtBinToSkip, j+1) );
          SF_curve->SetBinError(k+1, small_2D->GetBinError(k+PtBinToSkip, j+1) );
        }
        gr_SF_curve[j] = hist_to_graph(SF_curve);
        gr_SF_curve[j]->SetLineColor(colors[j]);
        gr_SF_curve[j]->GetYaxis()->SetTitle("Fake Rate Scale Factor");
        gr_SF_curve[j]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
        hist_axis(gr_SF_curve[j]);
        if(j==0){
          gr_SF_curve[j]->Draw("e1al");
          gr_SF_curve[j]->GetYaxis()->SetRangeUser(0.8, 2.0);
          hist_axis(gr_SF_curve[j]);
        }
        else gr_SF_curve[j]->Draw("e1lsame");
      }
      lg_SF_curve->AddEntry(gr_SF_curve[0], "0 < |#eta| < 0.8", "l");
      lg_SF_curve->AddEntry(gr_SF_curve[1], "0.8 < |#eta| < 1.479", "l");
      lg_SF_curve->AddEntry(gr_SF_curve[2], "1.479 < |#eta| < 2.0", "l");
      lg_SF_curve->AddEntry(gr_SF_curve[3], "2.0 < |#eta| < 2.5", "l");
      lg_SF_curve->Draw();

      latex_CMSPriliminary.SetTextSize(0.035);
      latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} Simulation");
      latex_Lumi.SetTextSize(0.035);
      latex_Lumi.DrawLatex(0.7, 0.96, "35.9 fb^{-1} (13 TeV)");

      c_1D_SF_curve->SaveAs(plotpath+"/1D_pt_each_eta_FRSF_MCTruth_"+this_MCTruth_njet+this_MC_sample_MCTruth+".pdf");
      c_1D_SF_curve->Close();
      delete c_1D_SF_curve;
      
      //==== write rootfile
      file_FR->cd();
      small_2D->Write();

      file_FR->Close();
      
      
    } // END MC sample loop
    
    
  } // END Trigger loop
  

  //==== Large and Small dXYSig Isolation distribution with QCD MC

  cout << "######################## Large/Small dXYSig Isolation distribution using QCD MC ########################" << endl;

  TCanvas *c_QCD_isodist = new TCanvas("c_QCD_isodist", "", 800, 800);
  canvas_margin(c_QCD_isodist);
  c_QCD_isodist->cd();
  TH1D *hist_qcdlarge_iso = (TH1D*)map_string_to_file["QCD_mu"]->Get(str_dXYCut+"_MCTruth_HighdXY_alljet_RelIso_F0");
  TH1D *hist_qcdsmall_iso = (TH1D*)map_string_to_file["QCD_mu"]->Get(str_dXYCut+"_MCTruth_alljet_RelIso_F0");
  TLegend *lg_QCD_isodist = new TLegend(0.6, 0.7, 0.9, 0.9);
  lg_QCD_isodist->SetBorderSize(0);
  lg_QCD_isodist->SetFillStyle(0);
  hist_qcdlarge_iso->SetLineColor(kRed);
  hist_qcdsmall_iso->SetLineColor(kBlack);
  hist_qcdlarge_iso->SetLineWidth(2);
  hist_qcdsmall_iso->SetLineWidth(2);
  lg_QCD_isodist->AddEntry(hist_qcdlarge_iso, "Sig(d_{xy}) > 4", "l");
  lg_QCD_isodist->AddEntry(hist_qcdsmall_iso, "Sig(d_{xy}) < 3", "l");
  hist_qcdlarge_iso->Scale(1./hist_qcdlarge_iso->Integral(0, hist_qcdlarge_iso->GetXaxis()->GetNbins()+1));
  hist_qcdsmall_iso->Scale(1./hist_qcdsmall_iso->Integral(0, hist_qcdsmall_iso->GetXaxis()->GetNbins()+1));
  hist_qcdlarge_iso->Draw("histsame");
  hist_axis(hist_qcdlarge_iso);
  hist_qcdlarge_iso->GetXaxis()->SetRangeUser(0, 0.6);
  hist_qcdlarge_iso->GetYaxis()->SetRangeUser(0, 0.07);
  hist_qcdsmall_iso->Draw("histsame");
  hist_qcdlarge_iso->SetTitle("");
  hist_qcdlarge_iso->SetXTitle("RelIso04");
  hist_qcdlarge_iso->SetYTitle("");
  lg_QCD_isodist->Draw();
  double FR_Large = hist_qcdlarge_iso->Integral(1,10)/hist_qcdlarge_iso->Integral(0, hist_qcdlarge_iso->GetXaxis()->GetNbins()+1);
  double FR_Small = hist_qcdsmall_iso->Integral(1,10)/hist_qcdsmall_iso->Integral(0, hist_qcdsmall_iso->GetXaxis()->GetNbins()+1);
  cout << "FR with dXYSig Large : " << FR_Large << endl;
  cout << "FR with dXYSig Small : " << FR_Small << endl;
  cout << "==> SF = (small/large) = " << FR_Small/FR_Large << endl;
  //==== Write rootfile
  TFile *file_FR_QCD = new TFile(plotpath+"/13TeV_trimuon_FR_alljet_QCD_mu.root", "UPDATE");
  TH1D *hist_FR_QCD = new TH1D("hist_FR_QCD", "", 3, 0, 3);
  hist_FR_QCD->SetBinContent(1, FR_Large);
  hist_FR_QCD->SetBinContent(2, FR_Small);
  hist_FR_QCD->SetBinContent(3, FR_Small/FR_Large);
  file_FR_QCD->cd();
  hist_FR_QCD->Write();
  file_FR_QCD->Close();

  latex_CMSPriliminary.SetTextSize(0.035);
  latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} Simulation");
  latex_Lumi.SetTextSize(0.035);
  latex_Lumi.DrawLatex(0.7, 0.96, "35.9 fb^{-1} (13 TeV)");

  c_QCD_isodist->SaveAs(plotpath+"/QCD_mu_RelIso_dXYSigs.pdf");
  c_QCD_isodist->Close();

  //===========================
  //==== SF using Data (TagZ)
  //===========================

  cout << "######################## FR using ZTag ########################" << endl;

  TH1D *hist_FR_ZTag_Large = (TH1D*)map_string_to_file["data"]->Get(str_dXYCut+"_DiMuonTrigger_ZTag_Large_dXYSig");
  TH1D *hist_FR_ZTag_Small = (TH1D*)map_string_to_file["data"]->Get(str_dXYCut+"_DiMuonTrigger_ZTag_Small_dXYSig");

  cout << hist_FR_ZTag_Large->GetBinContent(2)<<" / " << hist_FR_ZTag_Large->GetBinContent(1) << endl;
  cout << hist_FR_ZTag_Small->GetBinContent(2)<<" / " << hist_FR_ZTag_Small->GetBinContent(1) << endl;
  cout << endl;

  for(unsigned int i=0; i<map_string_to_MC_list["DiMuon"].size(); i++){
    TString this_sample = map_string_to_MC_list["DiMuon"].at(i);
    cout << this_sample << endl;
    TH1D *hist_Large_tmp = (TH1D*)map_string_to_file[this_sample]->Get(str_dXYCut+"_DiMuonTrigger_ZTag_Large_dXYSig");
    TH1D *hist_Small_tmp = (TH1D*)map_string_to_file[this_sample]->Get(str_dXYCut+"_DiMuonTrigger_ZTag_Small_dXYSig");
    
    hist_Large_tmp->Scale(MCRescale);
    hist_Small_tmp->Scale(MCRescale);
    
    hist_FR_ZTag_Large->Add(hist_Large_tmp, -1.);
    hist_FR_ZTag_Small->Add(hist_Small_tmp, -1.);

  cout << hist_FR_ZTag_Large->GetBinContent(2)<<" / " << hist_FR_ZTag_Large->GetBinContent(1) << endl;
  cout << hist_FR_ZTag_Small->GetBinContent(2)<<" / " << hist_FR_ZTag_Small->GetBinContent(1) << endl;
  cout << endl;

  }

  double FR_ZTag_Large = hist_FR_ZTag_Large->GetBinContent(2)/hist_FR_ZTag_Large->GetBinContent(1);
  double FR_ZTag_Small = hist_FR_ZTag_Small->GetBinContent(2)/hist_FR_ZTag_Small->GetBinContent(1);

  cout << "FR_ZTag_Large = " << hist_FR_ZTag_Large->GetBinContent(2)<<" / " << hist_FR_ZTag_Large->GetBinContent(1) << " = " << FR_ZTag_Large << endl;
  cout << "FR_ZTag_Small = " << hist_FR_ZTag_Small->GetBinContent(2)<<" / " << hist_FR_ZTag_Small->GetBinContent(1) << " = " << FR_ZTag_Small << endl;
  cout << "==> SF = Small/Large = " << FR_ZTag_Small/FR_ZTag_Large << endl;

  TFile *file_FR_TagZ = new TFile(plotpath+"/13TeV_trimuon_FR_DiMuonTrigger_TagZ.root", "RECREATE");
  TH1D *hist_FR_TagZ = new TH1D("hist_FR_TagZ", "", 3, 0, 3);
  hist_FR_TagZ->SetBinContent(1, FR_ZTag_Large);
  hist_FR_TagZ->SetBinContent(2, FR_ZTag_Small);
  hist_FR_TagZ->SetBinContent(3, FR_ZTag_Small/FR_ZTag_Large);
  file_FR_TagZ->cd();
  hist_FR_TagZ->Write();
  file_FR_TagZ->Close();

}



void make_legend(TLegend *lg, TString MCtype, vector<TH1D*> hists, vector<TString> alias){
  
  for(int i=hists.size()-1; i>=0; i--){
    lg->AddEntry(hists.at(i), alias.at(i), "f");
  }
  
}


double GetMCRescale(int period){
  
  //==== 0 (default) : BCDEFGH
  //==== 1 : B
  //==== 2 : C
  //==== 3 : D
  //==== 4 : E
  //==== 5 : F
  //==== 6 : G
  //==== 7 : H (_v2+_v3)
  //==== 8 : BCDEF
  //==== 9 : GH
  
  double lumi_periodB = 5.929001722;
  double lumi_periodC = 2.645968083;
  double lumi_periodD = 4.35344881;
  double lumi_periodE = 4.049732039;
  double lumi_periodF = 3.157020934;
  double lumi_periodG = 7.549615806;
  double lumi_periodH = 8.545039549 + 0.216782873;
  double total_lumi = (lumi_periodB+lumi_periodC+lumi_periodD+lumi_periodE+lumi_periodF+lumi_periodG+lumi_periodH) ;

  if(period==1) return lumi_periodB/total_lumi;
  else if(period==2) return lumi_periodC/total_lumi;
  else if(period==3) return lumi_periodD/total_lumi;
  else if(period==4) return lumi_periodE/total_lumi;
  else if(period==5) return lumi_periodF/total_lumi;
  else if(period==6) return lumi_periodG/total_lumi;
  else if(period==7) return lumi_periodH/total_lumi;
  else if(period==8) return (lumi_periodB+lumi_periodC+lumi_periodD+lumi_periodE+lumi_periodF)/total_lumi;
  else if(period==9) return (lumi_periodG+lumi_periodH)/total_lumi;
  else{
    cout << "[Error] Period Wrong" << endl;
    return 0.;
  }
  
}

TString GetPeriod(int period){
  //==== 0 (default) : BCDEFGH
  //==== 1 : B
  //==== 2 : C
  //==== 3 : D
  //==== 4 : E
  //==== 5 : F
  //==== 6 : G
  //==== 7 : H (_v2+_v3)
  //==== 8 : BCDEF
  //==== 9 : GH
  
  if(period==1) return "B";
  else if(period==2) return "C";
  else if(period==3) return "D";
  else if(period==4) return "E";
  else if(period==5) return "F";
  else if(period==6) return "G";
  else if(period==7) return "H";
  else if(period==8) return "BCDEF";
  else if(period==9) return "GH";
  else{
    cout << "[Error] Period Wrong" << endl;
    return "ERROR_PERIOD_WRONG";
  }
}











