#include "canvas_margin.h"
#define n_xbins 9
#define n_ybins 4

void syst_MC_Closure(){
  
  gStyle->SetOptStat(0);
  TH1::AddDirectory(kFALSE);
  gStyle->SetPaintTextFormat("0.2f");

  TString dataset = getenv("CATANVERSION");

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/FR_MC_Closure/syst_FR/";
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/FR_MC_Closure/";
  
  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
  
  vector<TString> samples = {"TTJets_aMC", "DYJets", "WJets", "QCD"};

  TString channel = "SSDiMuon";
  
  vector<double> dXYMins = {3.0, 4.0, 5.0};
  double bins_dXYMins[] =  {3.0, 4.0, 5.0, 6.0};
  vector<double> RelIsoMaxs = {0.2, 0.3, 0.4, 0.6, 0.8, 1.0};
  double bins_RelIsoMaxs[] =  {0.2, 0.3, 0.4, 0.6, 0.8, 1.0, 1.2};
  
  double dXYMin_cent = 4.0;
  double RelIsoMax_cent = 0.4;
  
  for(unsigned int i=0; i<samples.size(); i++){
    TString this_sample = samples.at(i);
    
    double n_events_cent, error_cent;
    
    TH2D *hist_yield = new TH2D(this_sample+"_hist_yield", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
    
    TFile *file_tmp = new TFile(filepath+"/trilepton_mumumu_syst_FR_SK"+this_sample+"_dilep_cat_v8-0-2.root");
    
    for(int aaa=0; aaa<dXYMins.size(); aaa++){
      for(int bbb=0; bbb<RelIsoMaxs.size(); bbb++){
        
        int Digit1 = int(dXYMins.at(aaa));
        int Digit0p1 = 10*dXYMins.at(aaa)-10*Digit1;
        TString str_dXYCut = "dXYSigMin_"+TString::Itoa(Digit1,10)+"p"+TString::Itoa(Digit0p1,10);
        //cout << str_dXYCut << endl;
        
        int iso_Digit1 = int(RelIsoMaxs.at(bbb));
        int iso_Digit0p1 = 10*RelIsoMaxs.at(bbb)-10*iso_Digit1;
        TString str_iso = "LooseRelIsoMax_"+TString::Itoa(iso_Digit1,10)+"p"+TString::Itoa(iso_Digit0p1,10);
        
        str_dXYCut = str_dXYCut+"_"+str_iso;
        
        //cout << "["<<str_dXYCut<<"]"<< endl;
        
        TH1D *hist_tmp_n_events = (TH1D*)file_tmp->Get(str_dXYCut+"_n_events_"+channel)->Clone();
        TH1D *hist_tmp_n_events_up = (TH1D*)file_tmp->Get(str_dXYCut+"_n_events_"+channel+"_up")->Clone();
        TH1D *hist_tmp_n_events_down = (TH1D*)file_tmp->Get(str_dXYCut+"_n_events_"+channel+"_down")->Clone();

        double n_yield_cent = hist_tmp_n_events->GetBinContent(1);
        double n_yield_up = hist_tmp_n_events_up->GetBinContent(1);
        double n_yield_down = hist_tmp_n_events_down->GetBinContent(1);
        double n_yield_error_propagated = n_yield_up-n_yield_cent;
        double n_yeild_error_sumw2 = hist_tmp_n_events->GetBinError(1);

        hist_yield->SetBinContent(aaa+1, bbb+1, n_yield_cent);
        hist_yield->SetBinError(aaa+1, bbb+1, sqrt( n_yield_error_propagated*n_yield_error_propagated + n_yeild_error_sumw2*n_yeild_error_sumw2 ));
        if(dXYMins.at(aaa)==dXYMin_cent && RelIsoMaxs.at(bbb)==RelIsoMax_cent){
          n_events_cent = n_yield_cent;
          error_cent = sqrt( n_yield_error_propagated*n_yield_error_propagated + n_yeild_error_sumw2*n_yeild_error_sumw2 );
        }
        
      }
    }
    
    //==== yield
    TCanvas *c_yield = new TCanvas("c_yield", "", 800, 800);
    canvas_margin(c_yield);
    c_yield->cd();
    hist_yield->SetMarkerSize(2);
    hist_yield->Draw("coltexte1");
    hist_yield->GetYaxis()->SetTitle("Maximum Loose Isolation");
    hist_yield->GetXaxis()->SetTitle("Minimum |dXYSig|");
    hist_axis(hist_yield);
    //TGaxis::SetMaxDigits(2);
    c_yield->SaveAs(plotpath+this_sample+"_"+channel+"_yield.pdf");
    c_yield->Close();
    
    //==== syst
    TCanvas *c_syst = new TCanvas("c_syst", "", 800, 800);
    canvas_margin(c_syst);
    c_syst->cd();
    TH2D *hist_yield_cent = new TH2D("hist_yield_cent", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
    for(int aaa=0; aaa<dXYMins.size(); aaa++){
      for(int bbb=0; bbb<RelIsoMaxs.size(); bbb++){
        hist_yield_cent->SetBinContent( aaa+1, bbb+1, n_events_cent  );
        hist_yield_cent->SetBinError( aaa+1, bbb+1, error_cent  );
      }
    }
    TH2D *hist_syst = (TH2D*)hist_yield->Clone();
    //==== diff
    hist_syst->Add(hist_yield_cent, -1.);
    hist_syst->SetMarkerSize(2);
    hist_syst->Draw("coltexte1");
    hist_syst->GetYaxis()->SetTitle("Maximum Loose Isolation");
    hist_syst->GetXaxis()->SetTitle("Minimum |dXYSig|");
    c_syst->SaveAs(plotpath+this_sample+"_"+channel+"_diff.pdf");
    //==== syst
    hist_syst->SetMarkerSize(2);
    hist_syst->Draw("coltexte1");
    hist_syst->GetYaxis()->SetTitle("Maximum Loose Isolation");
    hist_syst->GetXaxis()->SetTitle("Minimum |dXYSig|");
    hist_syst->Divide(hist_yield_cent);
    c_syst->SaveAs(plotpath+this_sample+"_"+channel+"_syst.pdf");
    c_syst->Close();

  }
  
  
}


