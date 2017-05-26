#include "canvas_margin.h"

void MCClosure(){

  gStyle->SetOptStat(0);

  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");

  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/MCClosure/";
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/MCClosure/";

  TString region = "";
  region = "ThreeMuon_ZVeto_mllloffZ_bjetveto";
  //region = "ThreeMuon_ZVeto_mllloffZ";
  //region = "TwoMuonOneElectron_bjetveto";

  plotpath += region+"/";

  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  vector<TString> vars = {"", "_leadingLepton_Pt", "_secondLepton_Pt", "_thirdLepton_Pt", "_PFMET"};
  vector<int> rebins = {1, 10, 10, 10, 10};
  vector<double> xmaxs = {1, 100, 100, 50, 100};
  vector<double> ymaxs = {300, 60, 60, 150, 40};
  vector<TString> xtitles = {"", "FirstLepton p_{T} [GeV]", "SecondLepton p_{T} [GeV]", "ThirdLepton p_{T} [GeV]", "PFMET [GeV]"};


//'DYJets_10to50' 'DYJets' 'DYJets_MG_10to50' 'DYJets_MG'
//'TTJets_aMC' 'TTLL_powheg'

  vector<TString> samples = {"DYJets_MG_added", "TT_powheg"};
  //vector<TString> samples = {"DYJets_added", "TTJets_aMC"};
  vector<TString> alias = {"DY", "t#bar{t}"};
  vector<Color_t> colors = {kYellow, kRed};

  for(unsigned int i=0; i<vars.size(); i++){

    TString var = vars.at(i);

    TH1D *Measured, *Predicted;
    THStack *Predicted_stack = new THStack("Predicted_stack", "");

    TLegend *lg = new TLegend(0.7, 0.7, 0.9, 0.9);
    lg->SetFillStyle(0);
    lg->SetBorderSize(0);

    for(unsigned int j=0; j<samples.size(); j++){

      TString sample = samples.at(j);

      TString filename = "trilepton_mumumu_MCClosure_nonprompt_SK"+sample+"_trilep_cat_v8-0-7.root";
      TFile *file = new TFile(filepath+filename);

      TH1D *tmp_Measured = (TH1D*)file->Get("Measured_"+region+var);
      TH1D *tmp_Predicted = (TH1D*)file->Get("Predicted_"+region+var);
      TH1D *tmp_Predicted_up = (TH1D*)file->Get("Predicted_"+region+var+"_up");
      TH1D *tmp_Predicted_down = (TH1D*)file->Get("Predicted_"+region+var+"_down");

      tmp_Measured->Rebin(rebins.at(i));
      tmp_Predicted->Rebin(rebins.at(i));
      tmp_Predicted_up->Rebin(rebins.at(i));
      tmp_Predicted_down->Rebin(rebins.at(i));

      tmp_Measured->SetLineColor(kBlack);
      tmp_Measured->SetFillColor(kBlack);

      tmp_Predicted->SetLineColor(colors.at(j));
      tmp_Predicted->SetFillColor(colors.at(j));

      if(var==""){
        cout << "["<<sample<<"]" << endl;
        cout << "Measured : " << tmp_Measured->GetEntries() << "\t" << tmp_Measured->GetBinContent(1) << endl;
        cout << "Predicted : " << tmp_Predicted->GetEntries() << "\t" << tmp_Predicted->GetBinContent(1) << endl;
      }

      lg->AddEntry(tmp_Predicted, alias.at(j), "f");

      //==== Set Error
      for(int aaa=1; aaa<=tmp_Measured->GetXaxis()->GetNbins(); aaa++){
        double y_Predicted = tmp_Predicted->GetBinContent(aaa);
        double y_Predicted_up = tmp_Predicted_up->GetBinContent(aaa);
        double y_Predicted_down = tmp_Predicted_down->GetBinContent(aaa);

        double y_Predicted_error_sumw2 = tmp_Predicted->GetBinError(aaa);
        double y_Predicted_error_propa = y_Predicted_up-y_Predicted;
        double error = sqrt(y_Predicted_error_sumw2*y_Predicted_error_sumw2+y_Predicted_error_propa*y_Predicted_error_propa); 

        tmp_Predicted->SetBinError(aaa, error);
      }

      if(j==0){
        Measured = (TH1D*)tmp_Measured->Clone();
        Predicted = (TH1D*)tmp_Predicted->Clone();
      }
      else{
        Measured->Add(tmp_Measured);
        Predicted->Add(tmp_Predicted);
      }

      Predicted_stack->Add(tmp_Predicted);

    } // END sample

    //==== print number once
    if(var==""){

      double y_Measured = Measured->GetBinContent(1);
      double y_Predicted = Predicted->GetBinContent(1);
      double error = Predicted->GetBinError(1);

      cout << "##################" << endl;
      cout << "Measured = " << y_Measured << endl;
      cout << "Predicted = " << y_Predicted << " +- " << error << endl;

      cout << 100.*fabs(y_Measured-y_Predicted)/y_Predicted << "\t" << 100.*error/y_Predicted << endl;
      cout << "##################" << endl;

    }

    TCanvas *c1 = new TCanvas("c1", "", 800, 800);
    canvas_margin(c1);

    TH1D *hist_empty = (TH1D*)Predicted_stack->GetHists()->At(0)->Clone();
    hist_empty->SetLineWidth(0);
    hist_empty->SetLineColor(0);
    hist_empty->SetMarkerSize(0);
    hist_empty->SetMarkerColor(0);
    hist_empty->Draw("histsame");
    hist_empty->GetXaxis()->SetRangeUser(0,xmaxs.at(i));
    hist_empty->GetYaxis()->SetRangeUser(0,ymaxs.at(i));
    hist_empty->SetMinimum(0);

    Predicted_stack->Draw("histsame");
    Measured->Draw("Psame");

    Predicted->SetMarkerColorAlpha(kAzure-9, 0);
    Predicted->SetFillStyle(3013);
    Predicted->SetFillColor(kBlack);
    Predicted->SetLineColor(0);
    Predicted->Draw("sameE2");

    hist_axis(hist_empty);
    hist_empty->SetTitle("");
    hist_empty->GetYaxis()->SetTitle("Events");
    hist_empty->GetXaxis()->SetTitle(xtitles.at(i));

    lg->Draw();

    c1->SaveAs(plotpath+"/"+region+var+".pdf");
    c1->Close();

  } // END varialbe

}
