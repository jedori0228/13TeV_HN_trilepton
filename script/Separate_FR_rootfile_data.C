void Separate_FR_rootfile_data(TString inputdir, TString outdir, int ForWhichTrigger){

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/FakeRateCalculator/"+inputdir+"/";
  TString outpath = WORKING_DIR+"/rootfiles/"+dataset+"/FakeRateCalculator/"+outdir+"/";


  TFile *file_all_DoubleMuon = new TFile(filepath+"FakeRateCalculator_Mu_dxysig_data_DoubleMuon_cat_"+catversion+".root");
  TFile *file_all_SingleMuon = new TFile(filepath+"FakeRateCalculator_Mu_dxysig_data_SingleMuon_cat_"+catversion+".root");

  TList *list_all = file_all_DoubleMuon->GetListOfKeys();

/*
  HLT_ptrange["HLT_Mu3_PFJet40_v"].push_back(5.);
  HLT_ptrange["HLT_Mu3_PFJet40_v"].push_back(10.);
  HLT_ptrange["HLT_Mu8_v"].push_back(10.);
  HLT_ptrange["HLT_Mu8_v"].push_back(20.);
  HLT_ptrange["HLT_Mu8_TrkIsoVVL_v"].push_back(10.);
  HLT_ptrange["HLT_Mu8_TrkIsoVVL_v"].push_back(20.);
  HLT_ptrange["HLT_Mu17_v"].push_back(20.);
  HLT_ptrange["HLT_Mu17_v"].push_back(25.);
  HLT_ptrange["HLT_Mu17_TrkIsoVVL_v"].push_back(20.);
  HLT_ptrange["HLT_Mu17_TrkIsoVVL_v"].push_back(25.);
  HLT_ptrange["HLT_Mu20_v"].push_back(25.);
  HLT_ptrange["HLT_Mu20_v"].push_back(9999.);
  HLT_ptrange["HLT_IsoMu24_v"].push_back(25.);
  HLT_ptrange["HLT_IsoMu24_v"].push_back(9999.);
*/

  vector<TString> HLTsToUse, WhichData, HLTsNotUse;

  //==== For HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v
  //==== Use TrkVVL triggers
  if(ForWhichTrigger==0){
    HLTsToUse = {"HLT_Mu3_PFJet40_v", "HLT_Mu8_TrkIsoVVL_v", "HLT_Mu17_TrkIsoVVL_v", "HLT_Mu20_v"};
    WhichData = {"DoubleMuon", "DoubleMuon", "DoubleMuon", "SingleMuon"};
    HLTsNotUse = {"HLT_Mu8_v", "HLT_Mu17_v", "HLT_IsoMu24_v"};
  }
  //==== For IsoMu24
  //==== Muons firing this trigger will pass TightID
  //==== So, LooseNotTight Muons in this events are not TriggerBiased, so we use non-TrkVVL trigger
  else if(ForWhichTrigger==1){
    HLTsToUse = {"HLT_Mu3_PFJet40_v", "HLT_Mu8_v", "HLT_Mu17_v", "HLT_Mu20_v"};
    WhichData = {"DoubleMuon", "DoubleMuon", "DoubleMuon", "SingleMuon"};
    HLTsNotUse = {"HLT_Mu8_TrkIsoVVL_v", "HLT_Mu17_TrkIsoVVL_v", "HLT_IsoMu24_v"};
  }

  map<TString, TH1D*> map_TH1D;
  map<TString, TH2D*> map_TH2D;

  for(int i=0; i<list_all->Capacity(); i++){
    TObject *this_obj = list_all->At(i);
    TString this_name = this_obj->GetName();

    bool SkipThis = false;
    for(unsigned int j=0;j<HLTsNotUse.size(); j++){
      if(this_name.Contains(HLTsNotUse.at(j))){
        SkipThis = true;
        break;
      }
    }
    if(SkipThis) continue;

    //cout << "this_name = " << this_name << endl;

    bool Used = false;

    for(unsigned int j=0;j<HLTsToUse.size(); j++){

      TString tmp_this_name = this_name;

      TFile *file_all = file_all_DoubleMuon;
      if(WhichData.at(j)=="SingleMuon") file_all = file_all_SingleMuon;

      if(!tmp_this_name.Contains(HLTsToUse.at(j))) continue;

      TString MergedName = tmp_this_name.Remove(0, HLTsToUse.at(j).Length()+1);
      //cout << "  MergedName = " << MergedName << endl;
      Used = true;

      //==== TH2D
      if(this_name.Contains("events")){

        TH2D *this_hist = (TH2D*)file_all->Get(this_name);
        this_hist->SetName( MergedName );
        this_hist->SetTitle("");

        std::map<TString, TH2D*>::iterator mapit = map_TH2D.find(MergedName);
        //==== If not found, clone this one
        if(mapit == map_TH2D.end()){
          map_TH2D[MergedName] = (TH2D*)this_hist->Clone();
        }
        else{
          map_TH2D[MergedName]->Add( this_hist );
        }

      }
      //==== TH1D
      else{

        TH1D *this_hist = (TH1D*)file_all->Get(this_name);
        this_hist->SetName( MergedName );
        this_hist->SetTitle("");

        std::map<TString, TH1D*>::iterator mapit = map_TH1D.find(MergedName);
        //==== If not found, clone this one
        if(mapit == map_TH1D.end()){
          map_TH1D[MergedName] = (TH1D*)this_hist->Clone();
        }
        else{
          map_TH1D[MergedName]->Add( this_hist );
        }

      }

    } // HLT loop

    if(!Used){

      TFile *file_all = file_all_DoubleMuon;

      //cout << "  Genearl Histogram" << endl;

      //==== TH2D
      if(this_name.Contains("events")){
        TH2D *this_hist = (TH2D*)file_all->Get(this_name);
        map_TH2D[this_name] = (TH2D*)this_hist->Clone();
      }
      //==== TH1D
      else{
        TH1D *this_hist = (TH1D*)file_all->Get(this_name);
        map_TH1D[this_name] = (TH1D*)this_hist->Clone();
      }

      continue;

    }

  }


  TString filename = "FakeRateCalculator_Mu_dxysig_data_DoubleMuon_cat_"+catversion+".root";
  TFile *file_out = new TFile(outpath+filename, "RECREATE");
  file_out->cd();
  for(map<TString, TH1D*>::iterator it=map_TH1D.begin(); it!=map_TH1D.end(); it++){
    it->second->Write();
  }
  for(map<TString, TH2D*>::iterator it=map_TH2D.begin(); it!=map_TH2D.end(); it++){
    it->second->Write();
  }

  file_out->Close();





}
