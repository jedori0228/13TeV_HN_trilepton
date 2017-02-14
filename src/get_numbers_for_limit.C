void get_numbers_for_limit(){

  TH1::SetDefaultSumw2(true);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");

  TString fake = "SFed_HighdXY";

  //=============
  //==== For SR
  //=============
  //TString SearchRegion = "SR";
  //TString signalfileprefix = "trilepton_mumumu_SKHN_MuMuMu_";
  //TString cut = "_cut0";
  //TString cut = "_cutWlow";
  //TString cut = "_cutWhigh";
  
  //=============
  //==== For CR
  //=============
  TString SearchRegion = "CR";
  TString signalfileprefix = "trilepton_mumumu_CR_SKHN_MuMuMu_";
  //TString cut = "_WZ";
  TString cut = "_ZJets";

  TString filepath = WORKING_DIR+"/plots/"+dataset+"/"+SearchRegion+"/use_FR_method/"+fake+"/hists.root";

  TFile* file = new TFile(filepath);
  TDirectory* dir = (TDirectory*)file->Get(cut);
  TCanvas* c1 = (TCanvas*)dir->Get("n_events");
  TPad* pad1 = (TPad*)c1->GetPrimitive("c1");

  if(SearchRegion=="CR" || (SearchRegion=="SR" && cut=="_cut0") ){
    TString data_prifix = "trilepton_mumumu";
    if(SearchRegion=="CR") data_prifix = "trilepton_mumumu_CR";

    TFile *file_data = new TFile(WORKING_DIR+"/rootfiles/"+dataset+"/"+SearchRegion+"/"+data_prifix+"_data_DoubleMuon_cat_v8-0-2.root");
    TH1D *hist_data = (TH1D*)file_data->Get("n_events"+cut);
    cout
    << "============== Data ==============" << endl
    << " (# of event) = " << hist_data->GetBinContent(1) << endl
    <<  " (# of entry) = " << hist_data->GetEntries() << endl
    << " stat uncert. = " << hist_data->GetBinError(1) << endl;
  }


  TH1D* hist_bkg;
  if(pad1) hist_bkg = (TH1D*)pad1->GetPrimitive("MC_stacked_err");
  else hist_bkg = (TH1D*)c1->GetPrimitive("MC_stacked_err");
  cout
  << "============== Bkg ==============" << endl
  << " (# of event) = " << hist_bkg->GetBinContent(1) << endl
  << " (# of entry) = " << hist_bkg->GetEntries() << endl
  << "        error = " << hist_bkg->GetBinError(1) << endl;
  
  THStack *MCstack;
  if(pad1) MCstack = (THStack*)pad1->GetPrimitive("MC_stacked");
  else MCstack = (THStack*)c1->GetPrimitive("MC_stacked");
  TList *listmc = (TList*)MCstack->GetHists();
  TH1D *hist_prompt = NULL;
  for(int i=0; i<listmc->Capacity(); i++){
    TH1D *hist = (TH1D*)listmc->At(i);
    TString histname = hist->GetName();
    if( histname.Contains("fake") ){
      cout
      << "============== Fake ==============" << endl
      << " (# of event) = " << hist->GetBinContent(1) << endl
      << " (# of entry) = " << hist->GetEntries() << endl
      << "        error = " << hist->GetBinError(1) << endl;
    }
    else{
      cout << "["<<histname<<"] " << hist->GetBinContent(1) << endl; 
      if(!hist_prompt) hist_prompt = (TH1D*)hist->Clone();
      else hist_prompt->Add(hist);
    }
  }
  cout
  << "============== Prompt ==============" << endl 
  << " (# of event) = " << hist_prompt->GetBinContent(1) << endl
  << " (# of entry) = " << hist_prompt->GetEntries() << endl
  << "        error = " << hist_prompt->GetBinError(1) << endl;
  

  cout << "============== Signal ==============" << endl;
  vector<TString> sig_mass = {"40", "60", "150", "700"};
  vector<double> n_generated = {100000, 100000, 100000, 100000};
  for(unsigned int i=0; i<sig_mass.size(); i++){

    TFile* file_sig = new TFile(WORKING_DIR+"/rootfiles/"+dataset+"/"+SearchRegion+"/"+signalfileprefix+sig_mass.at(i)+"_cat_v8-0-2.root");
    TH1D* hist_sig = (TH1D*)file_sig->Get("n_events"+cut);

    if(!hist_sig) continue;
    cout
    << "HN"+sig_mass.at(i)+" Sig" << endl
    << " (# of event) = " << hist_sig->GetBinContent(1) << endl
    << " (# of entry) = " << hist_sig->GetEntries() << endl
    << "        error = " << hist_sig->GetBinError(1) << endl
    << "   efficiency = " << hist_sig->GetEntries()/n_generated.at(i) << endl;

  }

  cout << "============== Non prompt ==============" << endl;
  vector<TString> bkgs = {"DYJets_10to50", "DYJets", "WJets", "TTJets_aMC", "WW", "WZ", "ZZ"};
  vector<double> n_events;
  double n_total;
  for(unsigned int i=0; i<bkgs.size(); i++){
    TFile* file_sig = new TFile(WORKING_DIR+"/rootfiles/"+dataset+"/"+SearchRegion+"/non_prompt_MC/trilepton_mumumu_SK"+bkgs.at(i)+"_dilep_cat_v8-0-2.root");
    //TH1D* hist_sig = (TH1D*)file_sig->Get("n_events"+cut);
    TH1D* hist_sig = (TH1D*)file_sig->Get("h_Nbjets"+cut);

    if(!hist_sig){
      n_events.push_back(0.);
      continue;
    }
    n_events.push_back(hist_sig->GetBinContent(1));
    n_total += hist_sig->GetBinContent(1);
    cout
    << bkgs.at(i) << endl
    << " (# of event) = " << hist_sig->GetBinContent(1) << endl
    << " (# of entry) = " << hist_sig->GetEntries() << endl
    << "        error = " << hist_sig->GetBinError(1) << endl;

  }
  

  cout << "==>" << endl;
  cout
  << "DY : " << 100.*(n_events.at(0)+n_events.at(1))/n_total << endl
  << "WJets : " << 100.*n_events.at(2)/n_total << endl
  << "ttbar : " << 100.*n_events.at(3)/n_total << endl
  << "VV : " << 100.*(n_events.at(4)+n_events.at(5)+n_events.at(6))/n_total << endl;


}
