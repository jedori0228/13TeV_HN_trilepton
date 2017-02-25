#include "cutop.cc"
#include "NLimit.cc"

double PunziFunction(double eff_sig, double bkg_tot, double bkg_fake);
void printcurrunttime();
void setCutsForEachSignalMass(int sig_mass, double& cut_first_pt, double& cut_second_pt, double& cut_third_pt, double& cut_W_pri_mass, double& cut_PFMET);
double GetMeanUncert(double a, double b, bool square=false);

NLimit syst_UpDowns(int sig_mass, bool printnumber=true, bool forlatex=false, bool inclusive=false, bool fillNlimit=false){

  TH1::SetDefaultSumw2(true);
  
  double uncert_lumi = 0.026;
  double uncert_fake = 0.28;

  vector<TString> bkg_prompt_list = {
    "WZTo3LNu_powheg",
    //"ZZTo4L_powheg",
    //"Vgamma",
    //"top",
    //"VVV"
  };
  map<TString, double> MCNormSF, MCNormSF_uncert;
  for(unsigned int i=0; i<bkg_prompt_list.size(); i++){
    MCNormSF[bkg_prompt_list.at(i)] = 1.;
    MCNormSF_uncert[bkg_prompt_list.at(i)] = 0.;
  }
  MCNormSF["ZZTo4L_powheg"] = 1.20803;
  MCNormSF_uncert["ZZTo4L_powheg"] = 0.0869206;
  MCNormSF["WZTo3LNu_powheg"] = 0.945281;
  MCNormSF_uncert["WZTo3LNu_powheg"] = 0.153901;

  double N_MC = 100000.;
  if(sig_mass==200) N_MC = 96193.;
  if(sig_mass==400) N_MC = 99070.;

  //TString region = "Preselection";
  TString region = "WZ";
  //TString region = "ZJets";
  //TString region = "ZZ";
  
  int SignalClass;
  if(sig_mass <= 50) SignalClass = 1;
  else if(sig_mass <= 80) SignalClass = 2;
  else if(sig_mass <= 1000) SignalClass = 3;
  else SignalClass = 4;

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");

  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/UpDownSyst/";

  double cut_first_pt, cut_second_pt, cut_third_pt, cut_W_pri_mass, cut_PFMET;
  setCutsForEachSignalMass(sig_mass, cut_first_pt, cut_second_pt, cut_third_pt, cut_W_pri_mass, cut_PFMET);

  if(inclusive){

    if(sig_mass < 80){
      cut_first_pt = 99999999.;
      cut_second_pt = 99999999.;
      cut_third_pt = 99999999.;
      cut_W_pri_mass = 150;
      cut_PFMET = 0.;
    }
    else{
      cut_first_pt = 20.;
      cut_second_pt = 10.;
      cut_third_pt = 10.;
      cut_W_pri_mass = 0.;
      cut_PFMET = 20.;
    }

  }

  if(printnumber && !forlatex){
    if(SignalClass==1||SignalClass==2){
      cout
      << endl
      << "====================================================================" << endl
      << "(first pt) < " << cut_first_pt << " GeV" << endl
      << "(second pt) < " << cut_second_pt << " GeV" << endl
      << "(third pt) < " << cut_third_pt << " GeV" << endl
      << "W_pri_mass < " << cut_W_pri_mass << " GeV" << endl
      << "====================================================================" << endl << endl;
    }
    else{
      cout
      << endl
      << "====================================================================" << endl
      << "(first pt) > " << cut_first_pt << " GeV" << endl
      << "(second pt) > " << cut_second_pt << " GeV" << endl
      << "(third pt) > " << cut_third_pt << " GeV" << endl
      << "W_pri_mass > " << cut_W_pri_mass << " GeV" << endl
      << "PFMET > " << cut_PFMET << " GeV" << endl
      << "====================================================================" << endl << endl;
    }
  }

  vector<TString> systtypes = {"Central", "MuonEn_up", "MuonEn_down", "JetEn_up", "JetEn_down", "JetRes_up", "JetRes_down", "Unclustered_up", "Unclustered_down", "MCxsec_up", "MCxsec_down", "MuonIDSF_up", "MuonIDSF_down"};
  vector<double> yields_prompt, yields_fake, yields_data, yields_signal, yields_signal_weighted;
  vector<double> syst_error_prompt, syst_error_fake, syst_error_data, syst_error_signal;
  vector<double> rel_syst_error_prompt, rel_syst_error_fake, rel_syst_error_data, rel_syst_error_signal;
  double stat_error_prompt, stat_error_fake, stat_error_data, stat_error_signal;

  for(int i=0; i<systtypes.size(); i++){

    TString this_syst = systtypes.at(i);
    if(this_syst.Contains("MCxsec_")) this_syst = "Central";
          
    double n_bkg_prompt(0.), n_bkg_fake(0.), n_data(0.), n_signal(0.), n_signal_weighted(0.);
    
    TH1D *hist_bkg_for_error = NULL;
    if(systtypes.at(i)=="Central") hist_bkg_for_error = new TH1D("hist_bkg_for_error", "", 1, 0., 1.);
    
    for(unsigned int k=0; k<bkg_prompt_list.size(); k++){
      TString this_samplename = bkg_prompt_list.at(k);
      cutop m_bkg_prompt(filepath+"trilepton_mumumu_ntp_SK"+this_samplename+"_dilep_cat_"+catversion+".root", "Ntp_"+this_syst);
      m_bkg_prompt.SearchRegion = region;
      m_bkg_prompt.cut_first_pt = cut_first_pt;
      m_bkg_prompt.cut_second_pt = cut_second_pt;
      m_bkg_prompt.cut_third_pt = cut_third_pt;
      m_bkg_prompt.cut_W_pri_mass = cut_W_pri_mass;
      m_bkg_prompt.cut_PFMET = cut_PFMET;
      m_bkg_prompt.signalclass = SignalClass;
      m_bkg_prompt.MCNormSF = MCNormSF[this_samplename];
      double MCNormDir(0.);
      if(systtypes.at(i) == "MCxsec_up") MCNormDir = 1.;
      else if(systtypes.at(i) == "MCxsec_down") MCNormDir = -1.;
      m_bkg_prompt.MCNormSF_uncert = MCNormDir*MCNormSF_uncert[this_samplename];
      m_bkg_prompt.Loop();

      n_bkg_prompt += m_bkg_prompt.n_weighted;
      
      if(systtypes.at(i)=="Central"){
        hist_bkg_for_error->Add(m_bkg_prompt.hist_for_error);
        if(printnumber && !forlatex) cout << this_samplename << " : " << m_bkg_prompt.n_weighted << ", error = " << m_bkg_prompt.hist_for_error->GetBinError(1) << endl;
      }

    }

    cutop m_bkg_fake(filepath+"trilepton_mumumu_ntp_SKfake_sfed_HighdXY_dilep_cat_"+catversion+".root", "Ntp_"+this_syst);
    m_bkg_fake.SearchRegion = region;
    m_bkg_fake.cut_first_pt = cut_first_pt;
    m_bkg_fake.cut_second_pt = cut_second_pt;
    m_bkg_fake.cut_third_pt = cut_third_pt;
    m_bkg_fake.cut_W_pri_mass = cut_W_pri_mass;
    m_bkg_fake.cut_PFMET = cut_PFMET;
    m_bkg_fake.signalclass = SignalClass;
    m_bkg_fake.Loop();
    n_bkg_fake = m_bkg_fake.n_weighted;

    cutop m_sig(filepath+"trilepton_mumumu_ntp_SKHN_MuMuMu_"+TString::Itoa(sig_mass, 10)+"_cat_"+catversion+".root", "Ntp_"+this_syst);
    m_sig.SearchRegion = region;
    m_sig.cut_first_pt = cut_first_pt;
    m_sig.cut_second_pt = cut_second_pt;
    m_sig.cut_third_pt = cut_third_pt;
    m_sig.cut_W_pri_mass = cut_W_pri_mass;
    m_sig.cut_PFMET = cut_PFMET;
    m_sig.signalclass = SignalClass;
    m_sig.Loop();
    n_signal = m_sig.n_unweighted;
    n_signal_weighted = m_sig.n_weighted;
    
    cutop m_data(filepath+"trilepton_mumumu_ntp_data_DoubleMuon_cat_"+catversion+".root", "Ntp_"+this_syst);
    m_data.SearchRegion = region;
    m_data.cut_first_pt = cut_first_pt;
    m_data.cut_second_pt = cut_second_pt;
    m_data.cut_third_pt = cut_third_pt;
    m_data.cut_W_pri_mass = cut_W_pri_mass;
    m_data.cut_PFMET = cut_PFMET;
    m_data.signalclass = SignalClass;
    m_data.Loop();
    n_data = m_data.n_weighted;

    yields_prompt.push_back(n_bkg_prompt);
    yields_fake.push_back(n_bkg_fake);
    yields_signal.push_back(n_signal);
    yields_signal_weighted.push_back(n_signal_weighted);
    yields_data.push_back(n_data);


    syst_error_prompt.push_back( n_bkg_prompt-yields_prompt.at(0) );
    syst_error_fake.push_back( n_bkg_fake-yields_fake.at(0) );

    //==== For MuonID SF, it only changes weight, not the # of event pass.
    //==== So, in this case, we calculate the difference with WEIGHTED numbers,
    //==== then obtain relative difference.
    //==== Then, multiply this to # of event (UNweighted) to get systematics..
    if( systtypes.at(i).Contains("MuonIDSF_") ){
      double weighted_diff = n_signal_weighted-yields_signal_weighted.at(0);
      double weighted_diff_rel =  weighted_diff/yields_signal_weighted.at(0);
      syst_error_signal.push_back( yields_signal.at(0) * weighted_diff_rel );
    }
    else{
      syst_error_signal.push_back( n_signal-yields_signal.at(0) );
    }

    syst_error_data.push_back( n_data-yields_data.at(0) );

    if( yields_prompt.at(0) != 0) rel_syst_error_prompt.push_back( syst_error_prompt.at(i)/yields_prompt.at(0) );
    else rel_syst_error_prompt.push_back( 0. );
    if( yields_fake.at(0) != 0) rel_syst_error_fake.push_back( syst_error_fake.at(i)/yields_fake.at(0) );
    else rel_syst_error_fake.push_back( 0. );
    if( yields_signal.at(0) != 0) rel_syst_error_signal.push_back( syst_error_signal.at(i)/yields_signal.at(0) );
    else rel_syst_error_signal.push_back( 0. );
    if( yields_data.at(0) != 0) rel_syst_error_data.push_back( syst_error_data.at(i)/yields_data.at(0) );
    else rel_syst_error_data.push_back( 0. );
    
    if(systtypes.at(i)=="Central"){
      stat_error_prompt = hist_bkg_for_error->GetBinError(1);
      
      double err_fake_sumw2 = m_bkg_fake.hist_for_error->GetBinError(1);

      double err_fake_prop = m_bkg_fake.hist_for_error_up->GetBinContent(1) - m_bkg_fake.hist_for_error->GetBinContent(1);
      stat_error_fake = sqrt(err_fake_sumw2*err_fake_sumw2+err_fake_prop*err_fake_prop);
      
      stat_error_data = m_data.hist_for_error->GetBinError(1);
      //stat_error_signal = m_sig.hist_for_error->GetBinError(1);
      stat_error_signal = sqrt(yields_signal.at(0));
    }
          
  }

  double squared_syst_prompt = uncert_lumi*uncert_lumi*yields_prompt.at(0)*yields_prompt.at(0);
  for(unsigned int i=1; i<syst_error_prompt.size(); i++) squared_syst_prompt += 0.5*syst_error_prompt.at(i)*syst_error_prompt.at(i);
  double squared_syst_fake = uncert_fake*uncert_fake*yields_fake.at(0)*yields_fake.at(0);
  for(unsigned int i=1; i<syst_error_fake.size(); i++) squared_syst_fake += 0.5*syst_error_fake.at(i)*syst_error_fake.at(i);
  double squared_syst_signal = uncert_lumi*uncert_lumi*yields_signal.at(0)*yields_signal.at(0);
  for(unsigned int i=1; i<syst_error_signal.size(); i++) squared_syst_signal += 0.5*syst_error_signal.at(i)*syst_error_signal.at(i);
  
  if(printnumber){

    if(forlatex){

      TString str_mass = TString::Itoa(sig_mass, 10);

      double data_yield = yields_data.at(0);
      if(region=="Preselection") data_yield = 9999;

      //==== low mass
      if(sig_mass < 80){
        cout
        <<str_mass<<"\t& $< "<<cut_W_pri_mass<<"$\t& $< "<<cut_first_pt<<"$\t& $< "<<cut_second_pt<<"$\t& $< "<<cut_third_pt << "$\t& "
        <<"$"<<std::fixed<<std::setprecision(2)<<yields_prompt.at(0)
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<stat_error_prompt<<"~\\stat"
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<sqrt(squared_syst_prompt) << "~\\syst$\t& "
        <<"$"<<std::fixed<<std::setprecision(2)<<yields_fake.at(0)
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<stat_error_fake<<"~\\stat"
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<sqrt(squared_syst_fake) << "~\\syst$\t& "
        <<"$"<<std::fixed<<std::setprecision(2)<<yields_prompt.at(0)+yields_fake.at(0)
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<sqrt(stat_error_prompt*stat_error_prompt+stat_error_fake*stat_error_fake)<<"~\\stat"
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<sqrt(squared_syst_prompt+squared_syst_fake) << "~\\syst$\t& "
        <<"$"<<std::fixed<<std::setprecision(2)<<100.*yields_signal.at(0)/N_MC
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<100.*stat_error_signal/N_MC<<"~\\stat"
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<100.*sqrt(squared_syst_signal)/N_MC << "~\\syst$\t& "
        <<"$"<<data_yield<<"$ \\\\" << endl;
      }
      //==== high mass
      else{
        cout
        <<str_mass<<"\t& $> "<<cut_W_pri_mass<<"$\t& $> "<<cut_PFMET<<"$\t& $> "<<cut_first_pt<<"$\t& $> "<<cut_second_pt<<"$\t& $> "<<cut_third_pt << "$\t& "
        <<"$"<<std::fixed<<std::setprecision(2)<<yields_prompt.at(0)
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<stat_error_prompt<<"~\\stat"
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<sqrt(squared_syst_prompt) << "~\\syst$\t& "
        <<"$"<<std::fixed<<std::setprecision(2)<<yields_fake.at(0)
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<stat_error_fake<<"~\\stat"
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<sqrt(squared_syst_fake) << "~\\syst$\t& "
        <<"$"<<std::fixed<<std::setprecision(2)<<yields_prompt.at(0)+yields_fake.at(0)
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<sqrt(stat_error_prompt*stat_error_prompt+stat_error_fake*stat_error_fake)<<"~\\stat"
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<sqrt(squared_syst_prompt+squared_syst_fake) << "~\\syst$\t& "
        <<"$"<<std::fixed<<std::setprecision(2)<<100.*yields_signal.at(0)/N_MC
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<100.*stat_error_signal/N_MC<<"~\\stat"
        <<" \\pm "<<std::fixed<<std::setprecision(2)<<100.*sqrt(squared_syst_signal)/N_MC << "~\\syst$\t& "
        <<"$"<<data_yield<<"$ \\\\" << endl;
      }

    }

    else{
      cout << endl << "######################### Results #########################" << endl;
      //==== Prompt Systematic
      cout
      << "[Prompt]" << endl
      << "Central Value = " << yields_prompt.at(0) << endl
      << "Muon Resolution" << endl
      << "  Up   = " << yields_prompt.at(1) << " ==> Diff = " << syst_error_prompt.at(1) << " ("<<100.*rel_syst_error_prompt.at(1)<<" %)" << endl
      << "  Down = " << yields_prompt.at(2) << " ==> Diff = " << syst_error_prompt.at(2) << " ("<<100.*rel_syst_error_prompt.at(2)<<" %)" << endl
      << "  Mean = " << GetMeanUncert(syst_error_prompt.at(1), syst_error_prompt.at(2)) << " ("<<100.*GetMeanUncert(rel_syst_error_prompt.at(1), rel_syst_error_prompt.at(2)) << " %)" << endl
      << "JES" << endl
      << "  Up   = " << yields_prompt.at(3) << " ==> Diff = " << syst_error_prompt.at(3) << " ("<<100.*rel_syst_error_prompt.at(3)<<" %)" << endl
      << "  Down = " << yields_prompt.at(4) << " ==> Diff = " << syst_error_prompt.at(4) << " ("<<100.*rel_syst_error_prompt.at(4)<<" %)" << endl
      << "  Mean = " << GetMeanUncert(syst_error_prompt.at(3), syst_error_prompt.at(4)) << " ("<<100.*GetMeanUncert(rel_syst_error_prompt.at(3), rel_syst_error_prompt.at(4)) << " %)" << endl
      << "JER" << endl
      << "  Up   = " << yields_prompt.at(5) << " ==> Diff = " << syst_error_prompt.at(5) << " ("<<100.*rel_syst_error_prompt.at(5)<<" %)" << endl
      << "  Down = " << yields_prompt.at(6) << " ==> Diff = " << syst_error_prompt.at(6) << " ("<<100.*rel_syst_error_prompt.at(6)<<" %)" << endl
      << "  Mean = " << GetMeanUncert(syst_error_prompt.at(5), syst_error_prompt.at(6)) << " ("<<100.*GetMeanUncert(rel_syst_error_prompt.at(5), rel_syst_error_prompt.at(6)) << " %)" << endl
      << "Uncluestered" << endl
      << "  Up   = " << yields_prompt.at(7) << " ==> Diff = " << syst_error_prompt.at(7) << " ("<<100.*rel_syst_error_prompt.at(7)<<" %)" << endl
      << "  Down = " << yields_prompt.at(8) << " ==> Diff = " << syst_error_prompt.at(8) << " ("<<100.*rel_syst_error_prompt.at(8)<<" %)" << endl
      << "  Mean = " << GetMeanUncert(syst_error_prompt.at(7), syst_error_prompt.at(8)) << " ("<<100.*GetMeanUncert(rel_syst_error_prompt.at(7), rel_syst_error_prompt.at(8)) << " %)" << endl
      << "MCxsec" << endl
      << "  Up   = " << yields_prompt.at(9) << " ==> Diff = " << syst_error_prompt.at(9) << " ("<<100.*rel_syst_error_prompt.at(9)<<" %)" << endl
      << "  Down = " << yields_prompt.at(10) << " ==> Diff = " << syst_error_prompt.at(10) << " ("<<100.*rel_syst_error_prompt.at(10)<<" %)" << endl
      << "  Mean = " << GetMeanUncert(syst_error_prompt.at(9), syst_error_prompt.at(10)) << " ("<<100.*GetMeanUncert(rel_syst_error_prompt.at(9), rel_syst_error_prompt.at(10)) << " %)" << endl
      << "ID SF" << endl
      << "  Up   = " << yields_prompt.at(11) << " ==> Diff = " << syst_error_prompt.at(11) << " ("<<100.*rel_syst_error_prompt.at(11)<<" %)" << endl
      << "  Down = " << yields_prompt.at(12) << " ==> Diff = " << syst_error_prompt.at(12) << " ("<<100.*rel_syst_error_prompt.at(12)<<" %)" << endl
      << "  Mean = " << GetMeanUncert(syst_error_prompt.at(11), syst_error_prompt.at(12)) << " ("<<100.*GetMeanUncert(rel_syst_error_prompt.at(11), rel_syst_error_prompt.at(12)) << " %)" << endl;
      cout << "=====================> total # = " << sqrt(squared_syst_prompt) << endl;
      cout << "=====================> total % = " << 100.*sqrt(squared_syst_prompt)/yields_prompt.at(0) << endl;
      
      //==== Fake Systematic
      cout
      << "[Fake]" << endl
      << "Central Value = " << yields_fake.at(0) << endl
      << "Muon Resolution" << endl
      << "  Up   = " << yields_fake.at(1) << " ==> Diff = " << syst_error_fake.at(1) << " ("<<100.*rel_syst_error_fake.at(1)<<" %)" << endl
      << "  Down = " << yields_fake.at(2) << " ==> Diff = " << syst_error_fake.at(2) << " ("<<100.*rel_syst_error_fake.at(2)<<" %)" << endl
      << "  Mean = " << GetMeanUncert(syst_error_fake.at(1), syst_error_fake.at(2)) << " ("<<100.*GetMeanUncert(rel_syst_error_fake.at(1), rel_syst_error_fake.at(2)) << " %)" << endl
      << "JES" << endl
      << "  Up   = " << yields_fake.at(3) << " ==> Diff = " << syst_error_fake.at(3) << " ("<<100.*rel_syst_error_fake.at(3)<<" %)" << endl
      << "  Down = " << yields_fake.at(4) << " ==> Diff = " << syst_error_fake.at(4) << " ("<<100.*rel_syst_error_fake.at(4)<<" %)" << endl
      << "  Mean = " << GetMeanUncert(syst_error_fake.at(3), syst_error_fake.at(4)) << " ("<<100.*GetMeanUncert(rel_syst_error_fake.at(3), rel_syst_error_fake.at(4)) << " %)" << endl
      << "JER" << endl
      << "  Up   = " << yields_fake.at(5) << " ==> Diff = " << syst_error_fake.at(5) << " ("<<100.*rel_syst_error_fake.at(1)<<" %)" << endl
      << "  Down = " << yields_fake.at(6) << " ==> Diff = " << syst_error_fake.at(6) << " ("<<100.*rel_syst_error_fake.at(2)<<" %)" << endl
      << "  Mean = " << GetMeanUncert(syst_error_fake.at(5), syst_error_fake.at(6)) << " ("<<100.*GetMeanUncert(rel_syst_error_fake.at(5), rel_syst_error_fake.at(6)) << " %)" << endl
      << "Uncluestered" << endl
      << "  Up   = " << yields_fake.at(7) << " ==> Diff = " << syst_error_fake.at(7) << " ("<<100.*rel_syst_error_fake.at(7)<<" %)" << endl
      << "  Down = " << yields_fake.at(8) << " ==> Diff = " << syst_error_fake.at(8) << " ("<<100.*rel_syst_error_fake.at(8)<<" %)" << endl
      << "  Mean = " << GetMeanUncert(syst_error_fake.at(7), syst_error_fake.at(8)) << " ("<<100.*GetMeanUncert(rel_syst_error_fake.at(7), rel_syst_error_fake.at(8)) << " %)" << endl;
      cout << "=====================> total # = " << sqrt(squared_syst_fake) << endl;
      cout << "=====================> total % = " << 100.*sqrt(squared_syst_fake)/yields_fake.at(0) << endl;

      //==== signal Systematic
      cout
      << "[signal]" << endl
      << "Central Value = " << yields_signal.at(0) << endl
      << "Muon Resolution" << endl
      << "  Up   = " << yields_signal.at(1) << " ==> Diff = " << syst_error_signal.at(1) << " ("<<100.*rel_syst_error_signal.at(1)<<" %)" << endl
      << "  Down = " << yields_signal.at(2) << " ==> Diff = " << syst_error_signal.at(2) << " ("<<100.*rel_syst_error_signal.at(2)<<" %)" << endl
      << "  Mean = " << GetMeanUncert(syst_error_signal.at(1), syst_error_signal.at(2)) << " ("<<100.*GetMeanUncert(rel_syst_error_signal.at(1), rel_syst_error_signal.at(2)) << " %)" << endl
      << "JES" << endl
      << "  Up   = " << yields_signal.at(3) << " ==> Diff = " << syst_error_signal.at(3) << " ("<<100.*rel_syst_error_signal.at(3)<<" %)" << endl
      << "  Down = " << yields_signal.at(4) << " ==> Diff = " << syst_error_signal.at(4) << " ("<<100.*rel_syst_error_signal.at(4)<<" %)" << endl
      << "  Mean = " << GetMeanUncert(syst_error_signal.at(3), syst_error_signal.at(4)) << " ("<<100.*GetMeanUncert(rel_syst_error_signal.at(3), rel_syst_error_signal.at(4)) << " %)" << endl
      << "JER" << endl
      << "  Up   = " << yields_signal.at(5) << " ==> Diff = " << syst_error_signal.at(5) << " ("<<100.*rel_syst_error_signal.at(5)<<" %)" << endl
      << "  Down = " << yields_signal.at(6) << " ==> Diff = " << syst_error_signal.at(6) << " ("<<100.*rel_syst_error_signal.at(6)<<" %)" << endl
      << "  Mean = " << GetMeanUncert(syst_error_signal.at(5), syst_error_signal.at(6)) << " ("<<100.*GetMeanUncert(rel_syst_error_signal.at(5), rel_syst_error_signal.at(6)) << " %)" << endl
      << "Uncluestered" << endl
      << "  Up   = " << yields_signal.at(7) << " ==> Diff = " << syst_error_signal.at(7) << " ("<<100.*rel_syst_error_signal.at(7)<<" %)" << endl
      << "  Down = " << yields_signal.at(8) << " ==> Diff = " << syst_error_signal.at(8) << " ("<<100.*rel_syst_error_signal.at(8)<<" %)" << endl
      << "  Mean = " << GetMeanUncert(syst_error_signal.at(7), syst_error_signal.at(8)) << " ("<<100.*GetMeanUncert(rel_syst_error_signal.at(7), rel_syst_error_signal.at(8)) << " %)" << endl
      << "ID SF" << endl
      << "  Up   = " << yields_signal.at(11) << " ==> Diff = " << syst_error_signal.at(11) << " ("<<100.*rel_syst_error_signal.at(11)<<" %)" << endl
      << "  Down = " << yields_signal.at(12) << " ==> Diff = " << syst_error_signal.at(12) << " ("<<100.*rel_syst_error_signal.at(12)<<" %)" << endl
      << "  Mean = " << GetMeanUncert(syst_error_signal.at(11), syst_error_signal.at(12)) << " ("<<100.*GetMeanUncert(rel_syst_error_signal.at(11), rel_syst_error_signal.at(12)) << " %)" << endl;
      cout << "=====================> total # = " << sqrt(squared_syst_signal) << endl;
      cout << "=====================> total % = " << 100.*sqrt(squared_syst_signal)/yields_signal.at(0) << endl;
    }

  }

  //==== Fill NLimit

  NLimit n_limit;

  if(fillNlimit){

/*
  enum systsource{
    lumi = 0,
    PDF = 1,
    Q2scale = 2,
    MuonID = 3,
    MuonPt = 4,
    JES = 5,
    JER = 6,
    Uncl = 7,
    btag = 8,
    PU = 9,
    Norm = 10
  }
*/

    n_limit.mass = sig_mass;

    n_limit.n_fake = yields_fake.at(0);
    n_limit.n_stat_fake = stat_error_fake;
    n_limit.n_syst_fake = sqrt(squared_syst_fake);
    n_limit.fake_systs[NLimit::MuonPt] = GetMeanUncert(syst_error_fake.at(1), syst_error_fake.at(2));
    n_limit.fake_systs[NLimit::JES] = GetMeanUncert(syst_error_fake.at(3), syst_error_fake.at(4));
    n_limit.fake_systs[NLimit::JER] = GetMeanUncert(syst_error_fake.at(5), syst_error_fake.at(6));
    n_limit.fake_systs[NLimit::Uncl] = GetMeanUncert(syst_error_fake.at(7), syst_error_fake.at(8));

    n_limit.n_prompt = yields_prompt.at(0);
    n_limit.n_stat_prompt = stat_error_prompt;
    n_limit.n_syst_prompt = sqrt(squared_syst_prompt);
    n_limit.prompt_systs[NLimit::MuonPt] = GetMeanUncert(syst_error_prompt.at(1), syst_error_prompt.at(2));
    n_limit.prompt_systs[NLimit::JES] = GetMeanUncert(syst_error_prompt.at(3), syst_error_prompt.at(4));
    n_limit.prompt_systs[NLimit::JER] = GetMeanUncert(syst_error_prompt.at(5), syst_error_prompt.at(6));
    n_limit.prompt_systs[NLimit::Uncl] = GetMeanUncert(syst_error_prompt.at(7), syst_error_prompt.at(8));
    n_limit.prompt_systs[NLimit::Norm] = GetMeanUncert(syst_error_prompt.at(9), syst_error_prompt.at(10));
    n_limit.prompt_systs[NLimit::MuonID] = GetMeanUncert(syst_error_prompt.at(11), syst_error_prompt.at(12));

    n_limit.n_signal = yields_signal.at(0);
    n_limit.n_stat_signal = stat_error_signal;
    n_limit.n_syst_signal = sqrt(squared_syst_signal);
    n_limit.signal_systs[NLimit::MuonPt] = GetMeanUncert(syst_error_signal.at(1), syst_error_signal.at(2));
    n_limit.signal_systs[NLimit::JES] = GetMeanUncert(syst_error_signal.at(3), syst_error_signal.at(4));
    n_limit.signal_systs[NLimit::JER] = GetMeanUncert(syst_error_signal.at(5), syst_error_signal.at(6));
    n_limit.signal_systs[NLimit::Uncl] = GetMeanUncert(syst_error_signal.at(7), syst_error_signal.at(8));
    n_limit.signal_systs[NLimit::MuonID] = GetMeanUncert(syst_error_signal.at(11), syst_error_signal.at(12));

    n_limit.n_data = yields_data.at(0);
  }  

  return n_limit;

}


double PunziFunction(double eff_sig, double bkg_tot, double bkg_fake){
  
  double den = 1 + sqrt( bkg_tot + (0.30 * bkg_fake)*(0.30 * bkg_fake) );
  //double den = sqrt( bkg_tot );
  
  return eff_sig/den;
  
}


void printcurrunttime(){
  
  TDatime datime;
  cout << datime.GetYear()<<"/"<<datime.GetMonth()<<"/"<<datime.GetDay()<<" "<<datime.GetHour()<<":"<<datime.GetMinute()<<":"<<datime.GetSecond();
  
}

void setCutsForEachSignalMass(int sig_mass, double& cut_first_pt, double& cut_second_pt, double& cut_third_pt, double& cut_W_pri_mass, double& cut_PFMET){

  if(sig_mass == 5){
    cut_first_pt = 75.;
    cut_second_pt = 45.;
    cut_third_pt = 25.;
    cut_W_pri_mass = 105.;
    cut_PFMET = 0.;
  }
  else if(sig_mass == 10){
    cut_first_pt = 60.;
    cut_second_pt = 55.;
    cut_third_pt = 35.;
    cut_W_pri_mass = 110.;
    cut_PFMET = 0.;
  }
  else if(sig_mass == 20){
    cut_first_pt = 80.;
    cut_second_pt = 55.;
    cut_third_pt = 50.;
    cut_W_pri_mass = 100.;
    cut_PFMET = 0.;
  }
  else if(sig_mass == 30){
    cut_first_pt = 40.;
    cut_second_pt = 30.;
    cut_third_pt = 25.;
    cut_W_pri_mass = 115.;
    cut_PFMET = 0.;
  }
  else if(sig_mass == 40){
    cut_first_pt = 35.;
    cut_second_pt = 30.;
    cut_third_pt = 30.;
    cut_W_pri_mass = 95.;
    cut_PFMET = 0.;
  }
  else if(sig_mass == 50){
    cut_first_pt = 30.;
    cut_second_pt = 30.;
    cut_third_pt = 25.;
    cut_W_pri_mass = 95.;
    cut_PFMET = 0.;
  }
  else if(sig_mass == 60){
    cut_first_pt = 30.;
    cut_second_pt = 25.;
    cut_third_pt = 25.;
    cut_W_pri_mass = 95.;
    cut_PFMET = 0.;
  }
  else if(sig_mass == 70){
    cut_first_pt = 100.;
    cut_second_pt = 75.;
    cut_third_pt = 45.;
    cut_W_pri_mass = 85.;
    cut_PFMET = 0.;
  }
  else if(sig_mass == 90){
    cut_first_pt = 50.;
    cut_second_pt = 25.;
    cut_third_pt = 15.;
    cut_W_pri_mass = 100.;
    cut_PFMET = 20.;
  }
  else if(sig_mass == 100){
    cut_first_pt = 25.;
    cut_second_pt = 15.;
    cut_third_pt = 15.;
    cut_W_pri_mass = 130.;
    cut_PFMET = 35.;
  }
  else if(sig_mass == 150){
    cut_first_pt = 25.;
    cut_second_pt = 35.;
    cut_third_pt = 20.;
    cut_W_pri_mass = 200.;
    cut_PFMET = 20.;
  }
  else if(sig_mass == 200){
    cut_first_pt = 25.;
    cut_second_pt = 55.;
    cut_third_pt = 25.;
    cut_W_pri_mass = 260.;
    cut_PFMET = 20.;
  }
  else if(sig_mass == 300){
    cut_first_pt = 100.;
    cut_second_pt = 55.;
    cut_third_pt = 25.;
    cut_W_pri_mass = 360.;
    cut_PFMET = 20.;
  }
  else if(sig_mass == 400){
    cut_first_pt = 110.;
    cut_second_pt = 75.;
    cut_third_pt = 25.;
    cut_W_pri_mass = 500.;
    cut_PFMET = 25.;
  }
  else if(sig_mass == 500){
    cut_first_pt = 130.;
    cut_second_pt = 110.;
    cut_third_pt = 20.;
    cut_W_pri_mass = 590.;
    cut_PFMET = 30.;
  }
  else if(sig_mass == 700){
    cut_first_pt = 190.;
    cut_second_pt = 100.;
    cut_third_pt = 45.;
    cut_W_pri_mass = 800.;
    cut_PFMET = 20.;
  }
  else if(sig_mass == 1000){
    cut_first_pt = 290.;
    cut_second_pt = 120.;
    cut_third_pt = 40.;
    cut_W_pri_mass = 1060.;
    cut_PFMET = 100.;
  }
  else{
    cout << "Wrong Signal Mass" << endl;
    cut_first_pt = 9999999;
    cut_second_pt = 9999999;
    cut_third_pt = 99999999;
    cut_W_pri_mass = 99999999;
  }

}

double GetMeanUncert(double a, double b, bool square){

  if(!square) return sqrt(0.5*(a*a+b*b));
  else return 0.5*(a*a+b*b);

}





