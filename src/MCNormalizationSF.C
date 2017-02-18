#include "canvas_margin.h"

void MCNormalizationSF(){
  
  TH1::SetDefaultSumw2(true);

  TString cmssw_version = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");
  
  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/CR/";
  
  map<TString, TFile*> map_string_to_file;
  map_string_to_file["data"] = new TFile(filepath+"trilepton_mumumu_CR_data_DoubleMuon_cat_"+cmssw_version+".root");
  vector<TString> MC_all = {"WZTo3LNu_powheg", "ttW", "ttZ", "ttH_nonbb", "WWW", "WWZ", "WZZ", "ZZZ", "ZGTo2LG", "ZZTo4L_powheg", "fake_sfed_HighdXY"};
  vector<TString> MC_nonWZ = {"ttW", "ttZ", "ttH_nonbb", "WWW", "WWZ", "WZZ", "ZZZ", "ZGTo2LG", "ZZTo4L_powheg"};
  vector<TString> MC_nonZZ = {"ttW", "ttZ", "ttH_nonbb", "WWZ", "WZZ", "ZZZ", "ZGTo2LG"};
  for(unsigned int i=0; i<MC_all.size(); i++){
    //cout << MC_all.at(i) << endl;
    map_string_to_file[MC_all.at(i)] = new TFile(filepath+"trilepton_mumumu_CR_SK"+MC_all.at(i)+"_dilep_cat_"+cmssw_version+".root");
  }
  
  cout << "============" << endl;
  cout << "==== WZ ====" << endl;
  cout << "============" << endl;

  //==== data
  TH1D *hist_WZ_n_data_event = (TH1D*)map_string_to_file["data"]->Get("n_events_WZ");
  //==== WZ MC
  TH1D *hist_WZ_n_WZ_MC = (TH1D*)map_string_to_file["WZTo3LNu_powheg"]->Get("n_events_WZ");
  //=== fake error propagation
  TH1D *hist_WZ_n_WZ_fake = (TH1D*)map_string_to_file["fake_sfed_HighdXY"]->Get("n_events_WZ");
  TH1D *hist_WZ_n_WZ_fake_up = (TH1D*)map_string_to_file["fake_sfed_HighdXY"]->Get("n_events_WZ_up");
  double e1_WZ = hist_WZ_n_WZ_fake->GetBinError(1);
  double e2_WZ = hist_WZ_n_WZ_fake_up->GetBinContent(1)-hist_WZ_n_WZ_fake->GetBinContent(1);
  hist_WZ_n_WZ_fake->SetBinError(1, sqrt(e1_WZ*e1_WZ+e2_WZ*e2_WZ));
  cout << "Fake : " << hist_WZ_n_WZ_fake->GetBinContent(1) << " +- " << hist_WZ_n_WZ_fake->GetBinError(1) << endl;
  //==== Add non WZ MC
  for(unsigned int i=0; i<MC_nonWZ.size(); i++){
    TH1D *hist_tmp = (TH1D*)map_string_to_file[MC_nonWZ.at(i)]->Get("n_events_WZ");
    if(hist_tmp){
      cout << MC_nonWZ.at(i) << " : " << hist_tmp->GetBinContent(1) << " +- " << hist_tmp->GetBinError(1) << endl;
      hist_WZ_n_WZ_fake->Add(hist_tmp);
    }
  }

  cout << "--------------------------------------" << endl; 
  cout << "Data : " << hist_WZ_n_data_event->GetBinContent(1) << endl;
  cout << "Non-WZ : " << hist_WZ_n_WZ_fake->GetBinContent(1) << " +- " << hist_WZ_n_WZ_fake->GetBinError(1) << endl;
  cout << "==> Data - (Non-WZ MC) = " << hist_WZ_n_data_event->GetBinContent(1) - hist_WZ_n_WZ_fake->GetBinContent(1) << endl;
  cout << "WZ MC : " << hist_WZ_n_WZ_MC->GetBinContent(1) << " => " << 100.*hist_WZ_n_WZ_MC->GetBinContent(1)/(hist_WZ_n_WZ_fake->GetBinContent(1)+hist_WZ_n_WZ_MC->GetBinContent(1)) << "%" << endl;
  //==== divide to get SF
  hist_WZ_n_data_event->Add(hist_WZ_n_WZ_fake, -1.);
  hist_WZ_n_data_event->Divide(hist_WZ_n_WZ_MC);
  cout << "==> Norm. SF = " << hist_WZ_n_data_event->GetBinContent(1) << " +- " << hist_WZ_n_data_event->GetBinError(1) << endl;
  
  
  
  //=========
  //==== ZZ
  //=========

  cout << endl;
  cout << "============" << endl;
  cout << "==== ZZ ====" << endl;
  cout << "============" << endl;

  //==== data
  TH1D *hist_ZZ_n_data_event = (TH1D*)map_string_to_file["data"]->Get("n_events_ZZ");
  //==== ZZ MC
  TH1D *hist_ZZ_n_ZZ_MC = (TH1D*)map_string_to_file["ZZTo4L_powheg"]->Get("n_events_ZZ");
  //=== fake error propagation
  TH1D *hist_ZZ_n_ZZ_fake = (TH1D*)map_string_to_file["fake_sfed_HighdXY"]->Get("n_events_ZZ");
  TH1D *hist_ZZ_n_ZZ_fake_up = (TH1D*)map_string_to_file["fake_sfed_HighdXY"]->Get("n_events_ZZ_up");
  double e1_ZZ = hist_ZZ_n_ZZ_fake->GetBinError(1);
  double e2_ZZ = hist_ZZ_n_ZZ_fake_up->GetBinContent(1)-hist_ZZ_n_ZZ_fake->GetBinContent(1);
  hist_ZZ_n_ZZ_fake->SetBinError(1, sqrt(e1_ZZ*e1_ZZ+e2_ZZ*e2_ZZ));
  cout << "Fake : " << hist_ZZ_n_ZZ_fake->GetBinContent(1) << " +- " << hist_ZZ_n_ZZ_fake->GetBinError(1) << endl;
  //==== Add non ZZ MC
  for(unsigned int i=0; i<MC_nonZZ.size(); i++){
    TH1D *hist_tmp = (TH1D*)map_string_to_file[MC_nonZZ.at(i)]->Get("n_events_ZZ");
    if(hist_tmp){
      cout << MC_nonZZ.at(i) << " : " << hist_tmp->GetBinContent(1) << " +- " << hist_tmp->GetBinError(1) << endl;
      hist_ZZ_n_ZZ_fake->Add(hist_tmp);
    }
  }
  
  cout << "--------------------------------------" << endl;
  cout << "Data : " << hist_ZZ_n_data_event->GetBinContent(1) << endl;
  cout << "Non-ZZ : " << hist_ZZ_n_ZZ_fake->GetBinContent(1) << " +- " << hist_ZZ_n_ZZ_fake->GetBinError(1) << endl;
  cout << "==> Data - (Non-ZZ MC) = " << hist_ZZ_n_data_event->GetBinContent(1) - hist_ZZ_n_ZZ_fake->GetBinContent(1) << endl;
  cout << "ZZ MC : " << hist_ZZ_n_ZZ_MC->GetBinContent(1) << " => " << 100.*hist_ZZ_n_ZZ_MC->GetBinContent(1)/(hist_ZZ_n_ZZ_fake->GetBinContent(1)+hist_ZZ_n_ZZ_MC->GetBinContent(1)) << "%" << endl;;
  //==== divide to get SF
  hist_ZZ_n_data_event->Add(hist_ZZ_n_ZZ_fake, -1.);
  hist_ZZ_n_data_event->Divide(hist_ZZ_n_ZZ_MC);
  cout << "==> Norm. SF = " << hist_ZZ_n_data_event->GetBinContent(1) << " +- " << hist_ZZ_n_data_event->GetBinError(1) << endl;
  
  
}
