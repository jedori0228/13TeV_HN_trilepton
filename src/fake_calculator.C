#include "mylib.h"
#include "canvas_margin.h"
#include "TSystem.h"

//==== MC list = {first, second, third ...}
//==== then, first will be filled at the bottom, so we want them in the bottom of legend
void make_legend(TLegend *lg, TString MCtype, vector<TH1D*> hists, vector<TString> alias);

void fake_calculator(double dXYMin, double RelIsoMax){

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
 
  TString cmssw_version = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/FakeRateCalculator/";
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/FakeRateCalculator/"+str_dXYCut+"/";
  
  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  map< TString, vector<TString> > map_string_to_MC_list;
  map< TString, vector<TString> > map_string_to_MC_alias;
  map< TString, vector<Color_t> > map_string_to_MC_color;
  vector<TString> all_MC_list;
  
  all_MC_list = {"singletop", "TTJets_aMC", "DY", "WJets", "QCD_mu", "HN_MuMuMu_40", "VV"};
  map_string_to_MC_list["SingleMuon"] = {"singletop", "TTJets_aMC", "DY", "WJets"};
  map_string_to_MC_alias["SingleMuon"] = {"singletop", "ttbar", "DY", "WJets"};
  map_string_to_MC_color["SingleMuon"] = {kOrange, kRed, kYellow, kGreen};
  map_string_to_MC_list["DiMuon"] = {"DY", "VV"};
  map_string_to_MC_alias["DiMuon"] = {"DY", "VV"};
  map_string_to_MC_color["DiMuon"] = {kYellow, kGreen};
  
  //==== get all files here
  map< TString, TFile* > map_string_to_file;
  map_string_to_file["data"] = new TFile(filepath+"/FakeRateCalculator_Mu_data_DoubleMuon_cat_"+cmssw_version+".root");
  for(unsigned int i=0; i<all_MC_list.size(); i++){
    TString this_samplename = all_MC_list.at(i);
    map_string_to_file[this_samplename] = new TFile(filepath+"/FakeRateCalculator_Mu_SK"+this_samplename+"_cat_"+cmssw_version+".root");
    if(!map_string_to_file[this_samplename]) cout << "[No file] :" << this_samplename << endl;
  }
  
  //======================
  //==== LooseMuon study
  //======================
  
  cout << "######################## Loose Muon study ########################" << endl;
  
  vector<TString> type_Loose_study = {"SingleMuonTrigger", "SingleMuonTrigger_HighdXY"};
  vector<TString> var_Loose_study = {"RelIso", "Chi2", "dXY", "dXYSig"};
  for(unsigned int it_type_Loose_study=0; it_type_Loose_study<type_Loose_study.size(); it_type_Loose_study++){

    //==== e.g., "SingleMuon"
    TString this_type_Loose_study = type_Loose_study.at(it_type_Loose_study);
    cout << this_type_Loose_study << endl;
    
    //==== prepare MC iterator end point
    vector<TString>::iterator it_MC_END;
    TString this_MC_type;
    if(this_type_Loose_study.Contains("SingleMuon")) this_MC_type = "SingleMuon";
    if(this_type_Loose_study.Contains("DiMuon")) this_MC_type = "DiMuon";
    it_MC_END = map_string_to_MC_list[this_MC_type].end();
    
    for(unsigned int it_var_Loose_study=0; it_var_Loose_study<var_Loose_study.size(); it_var_Loose_study++){
    
      //==== e.g., "RelIso"
      TString this_var_Loose_study = var_Loose_study.at(it_var_Loose_study);
      
      //==== data
      TH1D* hist_data = (TH1D*)map_string_to_file["data"]->Get(str_dXYCut+"_"+this_type_Loose_study+"_LooseMuon_"+this_var_Loose_study);
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
        TH1D* MC_temp = (TH1D*)map_string_to_file[this_samplename]->Get(str_dXYCut+"_"+this_type_Loose_study+"_LooseMuon_"+this_var_Loose_study);
        
        if( !MC_temp ){
          cout << "No Histogram : " << this_samplename << endl;
          continue;
        }
        else{
          MC_temp->SetFillColor( map_string_to_MC_color[this_MC_type].at(aaa) );
          MC_temp->SetLineColor( map_string_to_MC_color[this_MC_type].at(aaa) );
        }
        
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
      c_Loose_study->SaveAs(plotpath+"/LooseMuon_Study_"+this_type_Loose_study+"_"+this_var_Loose_study+".pdf");
      c_Loose_study->Close();
      delete c_Loose_study;
      
    }
  }
  
  //======================================================
  //==== dXYSig cut optimization plots using DY, QCD
  //======================================================

  cout << "######################## dXYSig cut optimization study ########################" << endl;
  vector<TString> MuonId = {"Loose", "Tight"};
  for(unsigned int i=0; i<MuonId.size(); i++){
      
    TH1D* hist_DY = (TH1D*)map_string_to_file["DY"]->Get(str_dXYCut+"_prompt_"+MuonId.at(i)+"_dXYSig");
    TH1D* hist_signal = (TH1D*)map_string_to_file["HN_MuMuMu_40"]->Get(str_dXYCut+"_prompt_"+MuonId.at(i)+"_dXYSig");
    TH1D* hist_ttbar = (TH1D*)map_string_to_file["TTJets_aMC"]->Get(str_dXYCut+"_fake_"+MuonId.at(i)+"_dXYSig");
    TH1D* hist_QCD_mumu = (TH1D*)map_string_to_file["QCD_mu"]->Get(str_dXYCut+"_fake_"+MuonId.at(i)+"_dXYSig");
    
    hist_DY->SetLineColor(kBlue);
    hist_signal->SetLineColor(kBlack);
    hist_ttbar->SetLineColor(kRed);
    hist_QCD_mumu->SetLineColor(kOrange-5);
    
    hist_DY->SetLineWidth(2);
    hist_signal->SetLineWidth(2);
    hist_ttbar->SetLineWidth(2);
    hist_QCD_mumu->SetLineWidth(2);

    int n_xbin = hist_DY->GetXaxis()->GetNbins();
    cout
    << "["<<MuonId.at(i)<<"]" << endl
    << "DY :" << endl
    << "  Sig(dXY) < 3.0 : " << hist_DY->Integral(0, 30) / hist_DY->Integral(0, n_xbin+1) << " = 1 - " << 1.-hist_DY->Integral(0, 30) / hist_DY->Integral(0, n_xbin+1) << endl
    << "  Sig(dXY) > 4.0 : " << hist_DY->Integral(41, n_xbin+1) / hist_DY->Integral(0, n_xbin+1) << " = 1 - " << 1.-hist_DY->Integral(41, n_xbin+1) / hist_DY->Integral(0, n_xbin+1) << endl
    << "HN40 :" << endl
    << "  Sig(dXY) < 3.0 : " << hist_signal->Integral(0, 30) / hist_signal->Integral(0, n_xbin+1) << " = 1 - " << 1.-hist_signal->Integral(0, 30) / hist_signal->Integral(0, n_xbin+1) << endl
    << "  Sig(dXY) > 4.0 : " << hist_signal->Integral(41, n_xbin+1) / hist_signal->Integral(0, n_xbin+1) << " = 1 - " << 1.-hist_signal->Integral(41, n_xbin+1) / hist_signal->Integral(0, n_xbin+1) << endl
    << "ttbar :" << endl
    << "  Sig(dXY) < 3.0 : " << hist_ttbar->Integral(0, 30) / hist_ttbar->Integral(0, n_xbin+1) << " = 1 - " << 1.-hist_ttbar->Integral(0, 30) / hist_ttbar->Integral(0, n_xbin+1) << endl
    << "  Sig(dXY) > 4.0 : " << hist_ttbar->Integral(41, n_xbin+1) / hist_ttbar->Integral(0, n_xbin+1) << " = 1 - " << 1.-hist_ttbar->Integral(41, n_xbin+1) / hist_ttbar->Integral(0, n_xbin+1) << endl
    << "QCD :" << endl
    << "  Sig(dXY) < 3.0 : " << hist_QCD_mumu->Integral(0, 30) / hist_QCD_mumu->Integral(0, n_xbin+1) << " = 1 - " << 1.-hist_QCD_mumu->Integral(0, 30) / hist_QCD_mumu->Integral(0, n_xbin+1) << endl
    << "  Sig(dXY) > 4.0 : " << hist_QCD_mumu->Integral(41, n_xbin+1) / hist_QCD_mumu->Integral(0, n_xbin+1) << " = 1 - " << 1.-hist_QCD_mumu->Integral(41, n_xbin+1) / hist_QCD_mumu->Integral(0, n_xbin+1) << endl;

    
    hist_DY->Scale(1./hist_DY->Integral());
    hist_signal->Scale(1./hist_signal->Integral());
    hist_ttbar->Scale(1./hist_ttbar->Integral());
    hist_QCD_mumu->Scale(1./hist_QCD_mumu->Integral());
    
    TLegend* lg_dXY = new TLegend(0.6, 0.7, 0.9, 0.95);
    lg_dXY->SetFillStyle(0);
    lg_dXY->SetBorderSize(0);
    lg_dXY->AddEntry(hist_DY, "Drell-Yan (prompt)", "l");
    lg_dXY->AddEntry(hist_signal, "m(HN) = 40 GeV/c^{2} (prompt)", "l");
    lg_dXY->AddEntry(hist_ttbar, "ttbar (fake)", "l");
    lg_dXY->AddEntry(hist_QCD_mumu, "QCD (fake)", "l");
    
    TCanvas* c_dXY = new TCanvas("c_dXY", "", 800, 800);
    canvas_margin(c_dXY);
    c_dXY->cd();
    c_dXY->SetLogy(kTRUE);
    
    hist_DY->Draw("histsame");
    hist_DY->SetTitle("");
    hist_DY->SetYTitle("AU");
    hist_DY->SetXTitle("|Sig(d_{xy})|"); 
    hist_axis(hist_DY);
    hist_signal->Draw("histsame");
    hist_ttbar->Draw("histsame");
    hist_QCD_mumu->Draw("histsame");
    lg_dXY->Draw();
    c_dXY->SaveAs(plotpath+"/dXYSig_Study_"+MuonId.at(i)+"IsoMuon.pdf");
    
    c_dXY->Close();
    delete c_dXY;
    delete lg_dXY;

      
  }

  //=============================================
  //==== FR plots (Num/Den, 2D FR, 1D FR curve)
  //=============================================
  
  cout << "######################## FR plots ########################" << endl;
  
  vector<TString> FR_method = {
    "SingleMuonTrigger_HighdXY",
    "SingleMuonTrigger_HighdXY_0jet",
    "SingleMuonTrigger_HighdXY_withjet",
    "SingleMuonTrigger_HighdXY_withjet_0bjet",
    "SingleMuonTrigger_HighdXY_withjet_withbjet",
    "DiMuonTrigger_ZTag_Small",
  };
  vector<TString> FR_method_alias = {
    "HighdXY",
    "HighdXY no jet",
    "HighdXY with jet",
    "HighdXY with jet but no bjet",
    "HighdXY with bjet"
  };
  vector<Color_t> FR_method_color = {
    kBlack,
    kBlue,
    kRed,
    kOrange,
    kOrange+8,
  };
  vector<TString> var_FR = {"eta", "pt", "RelIso", "Chi2", "dXY", "dXYSig"};
  vector<TString> x_title_FR = {"#eta", "p_{T}", "RelIso", "#chi^{2}", "|dXY|", "|dXY/#sigma(dXY)|"};
  
  //==== jet dependency test
  //==== use barrel (0<|eta|<0.8)
  vector<TGraphAsymmErrors*> grs_FRcurvesBarrel;
  
  //==== now, loop over FR methods!

  for(unsigned int it_FR_method=0; it_FR_method<FR_method.size(); it_FR_method++){
    
    TString this_FR_method = FR_method.at(it_FR_method);
    cout << "######################## " << this_FR_method << endl;
    
    //==== store # of (data-prompt MC), to optimize which dXYSig min to use
    TH1F *hist_n_data_prompt_subtraction_num = new TH1F("hist_n_data_prompt_subtraction_num", "", 1, 0, 1);
    TH1F *hist_n_data_prompt_subtraction_den = new TH1F("hist_n_data_prompt_subtraction_den", "", 1, 0, 1);
    
    //==== prepare MC iterator end point
    vector<TString>::iterator it_MC_END;
    TString this_MC_type;
    if(this_FR_method.Contains("SingleMuon")) this_MC_type = "SingleMuon";
    if(this_FR_method.Contains("DiMuon")) this_MC_type = "DiMuon";
    
    it_MC_END = map_string_to_MC_list[this_MC_type].end();
    
    //==== Num/Den
    for(unsigned int it_var_FR=0; it_var_FR<var_FR.size(); it_var_FR++){
    
      TString this_var_FR = var_FR.at(it_var_FR);
      TString this_x_title_FR = x_title_FR.at(it_var_FR);
      
      //==== data
      TH1D *num_data = (TH1D*)map_string_to_file["data"]->Get(str_dXYCut+"_"+this_FR_method+"_"+this_var_FR+"_F");
      TH1D *den_data = (TH1D*)map_string_to_file["data"]->Get(str_dXYCut+"_"+this_FR_method+"_"+this_var_FR+"_F0");
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
        TH1D* num_MC_temp = (TH1D*)map_string_to_file[this_samplename]->Get(str_dXYCut+"_"+this_FR_method+"_"+this_var_FR+"_F");
        TH1D* den_MC_temp = (TH1D*)map_string_to_file[this_samplename]->Get(str_dXYCut+"_"+this_FR_method+"_"+this_var_FR+"_F0");
        
        if( !num_MC_temp || !den_MC_temp ){
          cout << "No Histogram : " << this_samplename << endl;
          continue;
        }
        else{
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
      if(this_FR_method == "SingleMuonTrigger_HighdXY"){
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
      c_den->SaveAs(plotpath+"/den_"+this_FR_method+"_"+this_var_FR+".pdf");
      c_den->Close();
      delete c_den;
      
      
    } // END var_FR loop
    
    //==== 2D FR
    
    //==== data
    TH2F* num_data = (TH2F*)map_string_to_file["data"]->Get(str_dXYCut+"_"+this_FR_method+"_events_F");
    TH2F* den_data = (TH2F*)map_string_to_file["data"]->Get(str_dXYCut+"_"+this_FR_method+"_events_F0");
    TH2F* num_data_subtracted = (TH2F*)num_data->Clone();
    TH2F* den_data_subtracted = (TH2F*)den_data->Clone();
    
    //==== MC iterator
    vector<TString>::iterator it_MC;
    it_MC = map_string_to_MC_list[this_MC_type].begin();
    
    //==== MC loop
    for(int aaa=0; it_MC != it_MC_END; ++it_MC, aaa++ ){
      TString this_samplename = *it_MC;
      TH2F *num_MC_temp = (TH2F*)map_string_to_file[this_samplename]->Get(str_dXYCut+"_"+this_FR_method+"_events_F");
      TH2F *den_MC_temp = (TH2F*)map_string_to_file[this_samplename]->Get(str_dXYCut+"_"+this_FR_method+"_events_F0");
      if( !num_MC_temp || !den_MC_temp ){
        cout << "No Histogram : " << this_samplename << endl;
        continue;
      }
      num_data_subtracted->Add(num_MC_temp, -1.);
      den_data_subtracted->Add(den_MC_temp, -1.);
    }

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
    num_data->GetXaxis()->SetRangeUser(10, 60);
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
    num_data_subtracted->GetXaxis()->SetRangeUser(10, 60);
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
    //==== Edge bin numbers
    TH1I* hist_bins = new TH1I("hist_bins", "", 2, 0, 2);
    int n_pt_bins = n_pt_bins = 7; // pt : 10-15-20-25-30-35-45-60
    int n_eta_bins = 4; // eta : 0.0-0.8-1.479-2.0-2.5
    hist_bins->SetBinContent(1, n_pt_bins);
    hist_bins->SetBinContent(2, n_eta_bins);
    hist_bins->Write();
    file_FR->Close();

    //==== draw FR curve for each eta region
    TCanvas *c_FR_curve = new TCanvas("c_FR_curve", "", 800, 800);
    canvas_margin(c_FR_curve);
    c_FR_curve->cd();
    TLegend* lg_FR_curve = new TLegend(0.6, 0.6, 0.9, 0.9);
    lg_FR_curve->SetFillStyle(0);
    lg_FR_curve->SetBorderSize(0);

    TGraphAsymmErrors *gr_FR_curve[n_eta_bins];
    Color_t colors[n_eta_bins];
    double x_bins[n_pt_bins+1];
    //==== fill pt(x) bins
    x_bins[0] = 10.;
    x_bins[1] = 15.;
    x_bins[2] = 20.;
    x_bins[3] = 25.;
    x_bins[4] = 30.;
    x_bins[5] = 35.;
    x_bins[6] = 45.;
    x_bins[7] = 60.;
    //==== fill colors for each eta regions
    colors[0] = kBlack;
    colors[1] = kRed;
    colors[2] = kBlue;
    colors[3] = kViolet;
    
    for(int j=0; j<n_eta_bins; j++){
      TH1D *FR_curve = new TH1D("FR_eta_"+TString::Itoa(j,10), "", n_pt_bins, x_bins);
      for(int k=0; k<n_pt_bins; k++){
        FR_curve->SetBinContent(k+1, num_data_subtracted->GetBinContent(k+1, j+1) );
        FR_curve->SetBinError(k+1, num_data_subtracted->GetBinError(k+1, j+1) );
      }
      gr_FR_curve[j] = hist_to_graph(FR_curve);
      gr_FR_curve[j]->SetLineColor(colors[j]);
      gr_FR_curve[j]->SetLineWidth(2);
      gr_FR_curve[j]->GetYaxis()->SetTitle("Fake Rate");
      gr_FR_curve[j]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
      gr_FR_curve[j]->SetTitle("");
      hist_axis(gr_FR_curve[j]);  
      if(j==0){
        gr_FR_curve[j]->Draw("alp");
        gr_FR_curve[j]->GetYaxis()->SetRangeUser(0, 0.5);
      }
      else gr_FR_curve[j]->Draw("e1lpsame");
    }
    lg_FR_curve->AddEntry(gr_FR_curve[0], "0 < |#eta| < 0.8", "l");
    lg_FR_curve->AddEntry(gr_FR_curve[1], "0.8 < |#eta| < 1.479", "l");
    lg_FR_curve->AddEntry(gr_FR_curve[2], "1.479 < |#eta| < 2.0", "l");
    lg_FR_curve->AddEntry(gr_FR_curve[3], "2.0 < |#eta| < 2.5", "l");
    grs_FRcurvesBarrel.push_back( (TGraphAsymmErrors*)gr_FR_curve[0]->Clone() );
    lg_FR_curve->Draw();
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
    grs_FRcurvesBarrel.at(i)->SetLineWidth(2);
    hist_axis(grs_FRcurvesBarrel.at(i));
    if(i==0){
      grs_FRcurvesBarrel.at(i)->Draw("alp");
      grs_FRcurvesBarrel.at(i)->GetYaxis()->SetRangeUser(0, 0.5);
    }
    else grs_FRcurvesBarrel.at(i)->Draw("lpsame");
    lg_FRcurves->AddEntry(grs_FRcurvesBarrel.at(i), FR_method_alias.at(i), "l");
  }
  lg_FRcurves->Draw();
  c_FRcurves->SaveAs(plotpath+"/1D_pt_each_eta_FR_Barrels.pdf");

  //=============================
  //==== MCTruth (scale factor)
  //=============================
  
  cout << "######################## MC Truth ########################" << endl;
  
  vector<TString> MCTruth_trigger = {"SingleMuonTrigger"};
  //vector<TString> MC_sample_MCTruth = {"QCD_DoubleEM", "QCD_mu"};
  vector<TString> MC_sample_MCTruth = {"QCD_mu", "TTJets_aMC", "DY", "WJets"}; //FIXME no bb sample in v8-0-2

  vector<TString> sig_region = {"HighdXY_", ""};
  
  for(unsigned int it_MCTruth_trigger=0; it_MCTruth_trigger<MCTruth_trigger.size(); it_MCTruth_trigger++){

    TString this_MCTruth_trigger = MCTruth_trigger.at(it_MCTruth_trigger);
    
    for(unsigned int it_MC_sample_MCTruth=0; it_MC_sample_MCTruth<MC_sample_MCTruth.size(); it_MC_sample_MCTruth++){
    
      TString this_MC_sample_MCTruth = MC_sample_MCTruth.at(it_MC_sample_MCTruth);
      
      //==== 2D
      
      TH2F *large_2D, *small_2D;

      //==== FR for each significanve region
      //==== and save FR for SF
      for(unsigned int it_sig_region=0; it_sig_region<sig_region.size(); it_sig_region++){
        TH2F* hist_num = (TH2F*)map_string_to_file[this_MC_sample_MCTruth]->Get(str_dXYCut+"_"+this_MCTruth_trigger+"_MCTruth_"+sig_region.at(it_sig_region)+"events_F");
        TH2F* hist_den = (TH2F*)map_string_to_file[this_MC_sample_MCTruth]->Get(str_dXYCut+"_"+this_MCTruth_trigger+"_MCTruth_"+sig_region.at(it_sig_region)+"events_F0");
        if( !hist_num || !hist_den ) continue;
        hist_num->Divide(hist_den);
        if(sig_region.at(it_sig_region)=="HighdXY_") large_2D = (TH2F*)hist_num->Clone();
        if(sig_region.at(it_sig_region)=="")         small_2D = (TH2F*)hist_num->Clone();
        TCanvas* c_MCTruth = new TCanvas("c_MCTruth", "", 1600, 1100);
        //canvas_margin(c_MCTruth);
        c_MCTruth->SetLeftMargin(0.07);
        c_MCTruth->SetRightMargin( 0.1 );
        gStyle->SetPaintTextFormat("0.4f");
        c_MCTruth->cd();
        hist_num->Draw("coltexte1");
        hist_num->GetXaxis()->SetRangeUser(10, 60);
        hist_num->SetXTitle("p_{T} [GeV/c]");
        hist_num->SetYTitle("|#eta|");
        hist_num->SetTitle("");
        hist_num->SetMarkerSize(1.3);
        TString histname_suffix("");
        if(sig_region.at(it_sig_region) == "HighdXY_") histname_suffix = "Large";
        if(sig_region.at(it_sig_region) == "") histname_suffix = "Small";
        c_MCTruth->SaveAs(plotpath+"/2D_FR_MCTruth_"+this_MCTruth_trigger+"_"+this_MC_sample_MCTruth+"_"+histname_suffix+".pdf");
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
      TString filename_FR = plotpath+"/13TeV_trimuon_FR_"+this_MCTruth_trigger+"_"+this_MC_sample_MCTruth+".root";
      TFile* file_FR = new TFile(filename_FR, "RECREATE");
      file_FR->cd();
      small_2D->SetName("FR_Small_dXYSig"); 
      small_2D->Write();
      //==== Edge bin numbers
      TH1I* hist_bins = new TH1I("hist_bins", "", 2, 0, 2);
      int n_pt_bins, n_eta_bins;
      n_pt_bins = 7; // pt : 10-15-20-25-30-35-45-60
      n_eta_bins = 4; // eta : 0.0-0.8-1.479-2.0-2.5
      hist_bins->SetBinContent(1, n_pt_bins);
      hist_bins->SetBinContent(2, n_eta_bins);
      hist_bins->Write();

      small_2D->Divide(large_2D);
      small_2D->SetName("FRSF");
      small_2D->Draw("coltexte1");
      small_2D->GetXaxis()->SetRangeUser(10, 60);
      small_2D->SetXTitle("p_{T} [GeV/c]");
      small_2D->SetYTitle("|#eta|");
      small_2D->SetTitle("");
      small_2D->SetMarkerSize(1.3);
      c_2D_FR_SF->SaveAs(plotpath+"/2D_FRSF_MCTruth_"+this_MCTruth_trigger+"_"+this_MC_sample_MCTruth+".pdf");
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

      double x_bins[8] = {10, 15, 20, 25, 30, 35, 45, 60};
      double y_bins[5] = {0.0, 0.8, 1.479, 2.0, 2.5};
      Color_t colors[4] = {kBlack, kRed, kBlue, kViolet};
      for(int j=0; j<4; j++){
        TH1D *SF_curve = new TH1D("FR_eta_"+TString::Itoa(j,10), "", 7, x_bins);
        for(int k=0; k<7; k++){
          SF_curve->SetBinContent(k+1, small_2D->GetBinContent(k+1, j+1) );
          SF_curve->SetBinError(k+1, small_2D->GetBinError(k+1, j+1) );
        }
        gr_SF_curve[j] = hist_to_graph(SF_curve);
        gr_SF_curve[j]->SetLineColor(colors[j]);
        gr_SF_curve[j]->GetYaxis()->SetTitle("Fake Rate Scale Factor");
        gr_SF_curve[j]->GetXaxis()->SetTitle("p_{T} [GeV/c]");
        hist_axis(gr_SF_curve[j]);
        if(j==0){
          gr_SF_curve[j]->Draw("e1alp");
          gr_SF_curve[j]->GetYaxis()->SetRangeUser(0.8, 2.0);
        }
        else gr_SF_curve[j]->Draw("e1lpsame");
      }
      lg_SF_curve->AddEntry(gr_SF_curve[0], "0 < |#eta| < 0.8", "l");
      lg_SF_curve->AddEntry(gr_SF_curve[1], "0.8 < |#eta| < 1.479", "l");
      lg_SF_curve->AddEntry(gr_SF_curve[2], "1.479 < |#eta| < 2.0", "l");
      lg_SF_curve->AddEntry(gr_SF_curve[3], "2.0 < |#eta| < 2.5", "l");
      lg_SF_curve->Draw();
      c_1D_SF_curve->SaveAs(plotpath+"/1D_pt_each_eta_FRSF_MCTruth_"+this_MCTruth_trigger+"_"+this_MC_sample_MCTruth+".pdf");
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
  TH1D *hist_qcdlarge_iso = (TH1D*)map_string_to_file["QCD_mu"]->Get(str_dXYCut+"_SingleMuonTrigger_MCTruth_HighdXY_RelIso_F0");
  TH1D *hist_qcdsmall_iso = (TH1D*)map_string_to_file["QCD_mu"]->Get(str_dXYCut+"_SingleMuonTrigger_MCTruth_RelIso_F0");
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
  TFile *file_FR_QCD = new TFile(plotpath+"/13TeV_trimuon_FR_SingleMuonTrigger_QCD_mu.root", "UPDATE");
  TH1D *hist_FR_QCD = new TH1D("hist_FR_QCD", "", 3, 0, 3);
  hist_FR_QCD->SetBinContent(1, FR_Large);
  hist_FR_QCD->SetBinContent(2, FR_Small);
  hist_FR_QCD->SetBinContent(3, FR_Small/FR_Large);
  file_FR_QCD->cd();
  hist_FR_QCD->Write();
  file_FR_QCD->Close();

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

  for(unsigned int i=0; i<map_string_to_MC_list["TagZ"].size(); i++){
    TString this_sample = map_string_to_MC_list["TagZ"].at(i);
    cout << this_sample << endl;
    TH1D *hist_Large_tmp = (TH1D*)map_string_to_file[this_sample]->Get(str_dXYCut+"_DiMuonTrigger_ZTag_Large_dXYSig");
    TH1D *hist_Small_tmp = (TH1D*)map_string_to_file[this_sample]->Get(str_dXYCut+"_DiMuonTrigger_ZTag_Small_dXYSig");
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




