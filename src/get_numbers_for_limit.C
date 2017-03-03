void get_numbers_for_limit(){

  TH1::SetDefaultSumw2(true);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");

  TString SearchRegion = "SR";
  TString cut = "_cut0";

  vector<TString> bkgs = {"DYJets_10to50", "DYJets", "WJets", "TTJets_aMC", "WW", "WZ", "ZZ"};
  vector<double> n_events;
  double n_total;
  for(unsigned int i=0; i<bkgs.size(); i++){
    TFile* file_sig = new TFile(WORKING_DIR+"/rootfiles/"+dataset+"/"+SearchRegion+"/non_prompt_MC/trilepton_mumumu_SK"+bkgs.at(i)+"_dilep_cat_"+catversion+".root");
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
