#include "canvas_margin.h"

void draw_MC_Closure(){

  gStyle->SetOptStat(0);

  TString dataset = getenv("CATANVERSION");
  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/FR_MC_Closure/";
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/FR_MC_Closure/";
  
  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
  
  vector<TString> samples = {"TTJets_aMC", "DYJets", "WJets", "QCD"};
  vector<double> ymaxs = {120, 50, 15, 10};

  TString channel = "SSDiMuon";
  
  for(unsigned int i=0; i<samples.size(); i++){
    TString this_sample = samples.at(i);
    
    TCanvas *c1 = new TCanvas("c1", "", 800, 800);
    TPad *c1_up =  new TPad("c1_up", "", 0, 0.25, 1, 1);
    TPad *c1_down = new TPad("c1_down", "", 0, 0, 1, 0.25);
    
    TLegend *lg = new TLegend(0.7, 0.7, 0.9, 0.9);
    lg->SetFillStyle(0);
    lg->SetBorderSize(0);

    canvas_margin(c1, c1_up, c1_down);
    c1_down->SetGridx();
    c1_down->SetGridy();
    c1_up->Draw();
    c1_down->Draw();
    c1_up->cd();
    
    TFile *file_tt = new TFile(filepath+"trilepton_mumumu_CR_SK"+this_sample+"_dilep_cat_v8-0-2.root");
    TFile *file_tt_FR = new TFile(filepath+"trilepton_mumumu_CR_FR_method_SK"+this_sample+"_dilep_cat_v8-0-2.root");
    
    TH1D *hist_tt = (TH1D*)file_tt->Get("leadingLepton_Pt_"+channel);
    TH1D *hist_tt_FR = (TH1D*)file_tt_FR->Get("leadingLepton_Pt_"+channel);
    TH1D *hist_error = (TH1D*)hist_tt_FR->Clone();

    TH1D *hist_tt_n = (TH1D*)file_tt->Get("n_events_"+channel);
    TH1D *hist_tt_FR_n = (TH1D*)file_tt_FR->Get("n_events_"+channel);
    cout << "["<<this_sample<<"]"<<endl;
    cout << "FR prediction = " << hist_tt_FR_n->GetBinContent(1) << " +- " <<  hist_tt_FR_n->GetBinError(1) << endl;
    cout << "MC event = " << hist_tt_n->GetBinContent(1) << endl;
    cout << "==> % difference = " << 100.*fabs(hist_tt_FR_n->GetBinContent(1)-hist_tt_n->GetBinContent(1))/hist_tt_FR_n->GetBinContent(1) << " +- " << 100.*hist_tt_FR_n->GetBinError(1)/hist_tt_FR_n->GetBinContent(1) << endl;
    
    hist_tt->Rebin(10);
    hist_tt_FR->Rebin(10);
    hist_error->Rebin(10);
    
    hist_tt_FR->SetFillColor(kAzure+10);
    hist_tt_FR->SetLineColor(kAzure+10);
    hist_tt->SetMarkerStyle(3);
    hist_tt->SetMarkerSize(1);
    hist_tt->SetMarkerColor(kBlack);
    hist_error->SetMarkerColorAlpha(kAzure-9, 0);
    hist_error->SetFillStyle(3004);
    hist_error->SetFillColor(kBlue);

    lg->AddEntry(hist_tt, "MC event", "p");
    lg->AddEntry(hist_tt_FR, "FR prediction", "f");

    hist_tt_FR->Draw("histsame");
    hist_error->Draw("sameE2");
    hist_axis(hist_tt_FR);
    hist_tt_FR->SetTitle("");
    hist_tt_FR->GetYaxis()->SetRangeUser(0,ymaxs.at(i));
    hist_tt_FR->SetYTitle("Events");
    hist_tt->Draw("psame");
    
    c1_down->cd();
    TH1D *hist_compare = (TH1D*)hist_tt->Clone();
    hist_compare->SetLineColor(kBlack);
    hist_compare->SetMarkerColor(kBlack);
    hist_compare->Divide(hist_tt_FR);
    hist_compare->SetMaximum(2);
    hist_compare->SetMinimum(0);
    hist_compare->GetXaxis()->SetTitle("p_{T} [GeV]");
    hist_compare->SetYTitle("#frac{DATA}{MC}");
    hist_compare->Draw("PE1same");
    hist_axis(hist_tt_FR, hist_compare);

    c1->cd();
    lg->Draw();
    c1->SaveAs(plotpath+this_sample+"_"+channel+"_leadingLepton_Pt.pdf");
    c1->Close();
  }
}
