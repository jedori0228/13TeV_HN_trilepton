#include "canvas_margin.h"

void TestHalfSample(){

  gStyle->SetOptStat(0);
  TH2::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");
  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");

  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/FakeRateCalculator/HalfSample/DiMuonTrkVVL/";
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/FakeRateCalculator/HalfSample/DiMuonTrkVVL/";

  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  vector<TString> samples = {"singletop", "TTJets_aMC", "VGamma", "DY", "WJets"};

  //==== SampleA

  TFile *file_data = new TFile(filepath+"/FakeRateCalculator_Mu_dxysig_data_DoubleMuon_cat_"+catversion+".root");
  TH2D *hist_A_events_F = (TH2D*)file_data->Get("dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleA_events_F");
  TH2D *hist_A_events_F0 = (TH2D*)file_data->Get("dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleA_events_F0");

  for(unsigned int i=0; i<samples.size(); i++){
    TFile *file_tmp = new TFile(filepath+"/FakeRateCalculator_Mu_dxysig_SK"+samples.at(i)+"_cat_"+catversion+".root");
    TH2D *tmp_A_events_F = (TH2D*)file_tmp->Get("dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleA_events_F");
    TH2D *tmp_A_events_F0 = (TH2D*)file_tmp->Get("dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleA_events_F0");

    if(tmp_A_events_F) hist_A_events_F->Add(tmp_A_events_F, -1.);
    if(tmp_A_events_F0) hist_A_events_F0->Add(tmp_A_events_F0, -1.);

    file_tmp->Close();
  }

  TCanvas* c_sampleA = new TCanvas("c_sampleA", "", 1600, 1100);
  c_sampleA->SetLeftMargin(0.07);
  c_sampleA->SetRightMargin( 0.1 );
  gStyle->SetPaintTextFormat("0.4f");
  hist_A_events_F->Divide(hist_A_events_F0);
  hist_A_events_F->Draw("colztexte1");
  hist_A_events_F->GetXaxis()->SetRangeUser(10, 60);
  hist_A_events_F->SetXTitle("p_{T} [GeV/c]");
  hist_A_events_F->SetYTitle("|#eta|");
  hist_A_events_F->SetTitle("");
  hist_A_events_F->SetMarkerSize(1.3);
  hist_A_events_F->SetName("FR_sampleA");
  c_sampleA->SaveAs(plotpath+"/FR_sampleA.pdf");

  TFile *file_FR_sampleA = new TFile(plotpath+"/FR_sampleA.root", "RECREATE");
  file_FR_sampleA->cd();
  hist_A_events_F->Write();
  file_FR_sampleA->Close();

  c_sampleA->Close();

  vector<TString> B_vars = {"PFMET", "njets"};
  for(unsigned int i=0; i<B_vars.size(); i++){
    TString this_var = B_vars.at(i);

    TH1D *hist_A_Predicted = (TH1D*)file_data->Get("dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleA_"+this_var+"_Predicted");
    TH1D *hist_A_F0 = (TH1D*)file_data->Get("dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleA_"+this_var+"_F0");
    TH1D *hist_B_F = (TH1D*)file_data->Get("dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleB_"+this_var+"_F");
    TH1D *hist_B_F0 = (TH1D*)file_data->Get("dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleB_"+this_var+"_F0");   

    for(unsigned int j=0; j<samples.size(); j++){
      TFile *file_tmp = new TFile(filepath+"/FakeRateCalculator_Mu_dxysig_SK"+samples.at(j)+"_cat_"+catversion+".root");


      TH1D *tmp_hist_A_Predicted = (TH1D*)file_tmp->Get("dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleA_"+this_var+"_Predicted");
      TH1D *tmp_hist_A_F0 = (TH1D*)file_tmp->Get("dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleA_"+this_var+"_F0");

      TH1D *tmp_hist_B_F = (TH1D*)file_tmp->Get("dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleB_"+this_var+"_F");
      TH1D *tmp_hist_B_F0 = (TH1D*)file_tmp->Get("dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleB_"+this_var+"_F0");

      if(tmp_hist_A_Predicted) hist_A_Predicted->Add(tmp_hist_A_Predicted, -1.);
      if(tmp_hist_A_F0) hist_A_F0->Add(tmp_hist_A_F0, -1.);
      if(tmp_hist_B_F) hist_B_F->Add(tmp_hist_B_F, -1.);
      if(tmp_hist_B_F0) hist_B_F0->Add(tmp_hist_B_F0, -1.);

      file_tmp->Close();
    }

    //hist_A_Predicted
    //hist_A_F0
    //hist_B_F
    //hist_B_F0

    if(this_var=="PFMET"){
      int n_rebin = 10;
      hist_A_Predicted->Rebin(n_rebin);
      hist_A_F0->Rebin(n_rebin);
      hist_B_F->Rebin(n_rebin);
      hist_B_F0->Rebin(n_rebin);

      hist_A_Predicted->GetXaxis()->SetTitle("PFMET [GeV]");
    }
    if(this_var=="njets"){
      Double_t xbins[4] = {0., 1., 5., 10.};
      hist_A_Predicted = (TH1D*)hist_A_Predicted->Rebin(3,"hnew1",xbins);
      hist_A_F0 = (TH1D*)hist_A_F0->Rebin(3,"hnew2",xbins);
      hist_B_F = (TH1D*)hist_B_F->Rebin(3,"hnew3",xbins);
      hist_B_F0 = (TH1D*)hist_B_F0->Rebin(3,"hnew4",xbins);

      hist_A_Predicted->GetXaxis()->SetTitle("# of jets");
    }

    hist_A_Predicted->Divide(hist_A_F0);
    hist_B_F->Divide(hist_B_F0);

    TCanvas* c_B_FR = new TCanvas("c_B_FR", "", 800, 800);
    c_B_FR->Draw();
    TPad *c_B_FR_up;
    TPad *c_B_FR_down;

    c_B_FR_up = new TPad("c_B_FR", "", 0, 0.25, 1, 1);
    c_B_FR_down = new TPad("c_B_FR_down", "", 0, 0, 1, 0.25);

    canvas_margin(c_B_FR, c_B_FR_up, c_B_FR_down);

    c_B_FR_up->SetGridx();
    c_B_FR_up->SetGridy();
    c_B_FR_down->SetGridx();
    c_B_FR_down->SetGridy();

    c_B_FR_up->Draw();
    c_B_FR_down->Draw();
    c_B_FR_up->cd();

    hist_A_Predicted->SetLineColor(kRed);
    hist_B_F->SetLineColor(kBlue);
    hist_A_Predicted->SetLineWidth(3);
    hist_B_F->SetLineWidth(3);

    hist_A_Predicted->Draw("histsamee1");
    hist_A_Predicted->GetYaxis()->SetRangeUser(0., 0.5);
    if(this_var=="PFMET") hist_A_Predicted->GetXaxis()->SetRangeUser(0., 100.);
    hist_B_F->Draw("histsamee1");

    hist_A_Predicted->GetYaxis()->SetTitle("Fake Rate");
    hist_A_Predicted->SetTitle("");

    TLegend *lg = new TLegend(0.65, 0.75, 0.97, 0.95);
    lg->SetBorderSize(0);
    lg->SetFillStyle(0);
    lg->AddEntry(hist_A_Predicted, "Predicted Fake Rate (p)", "l");
    lg->AddEntry(hist_B_F, "Measured Fake Rate (m)", "l");
    lg->Draw();

    c_B_FR_down->cd();
    TH1D *ratio = (TH1D*)hist_A_Predicted->Clone();
    ratio->Divide(hist_B_F);
    ratio->Draw("PE1same");
    ratio->GetYaxis()->SetRangeUser(0.8, 1.2);
    ratio->GetYaxis()->SetTitle("p/m");
    ratio->SetLineColor(kBlack);

    hist_axis(hist_A_Predicted, ratio);

    cout << "## Var = " << this_var << " ##" << endl;
    for(int aaa=1; aaa<=ratio->GetXaxis()->GetNbins(); aaa++){
      double left = ratio->GetXaxis()->GetBinLowEdge(aaa);
      double right = ratio->GetXaxis()->GetBinUpEdge(aaa);
      double center = ratio->GetBinContent(aaa);
      double error = ratio->GetBinError(aaa);

      double diff = abs(1.-center);
      double syst = 0;
      if(diff>error){
        syst = sqrt(diff*diff-error*error);
      }
      cout << "["<<left<<", "<<right<<"] : " << ratio->GetBinContent(aaa) << "\t" << ratio->GetBinError(aaa) << " => " << syst << endl;
      
    }

    c_B_FR->SaveAs(plotpath+"/B_"+this_var+".pdf");
    c_B_FR->Close();
    
  }

/*
dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleA_events_F
dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleA_events_F0

dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleB_PFMET_F
dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleB_PFMET_F0
dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleB_njets_F
dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleB_njets_F0
dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleB_events_F
dXYSigMin_4p0_LooseRelIsoMax_0p4_HighdXY_HalfSample_SampleB_events_F0
*/


}
