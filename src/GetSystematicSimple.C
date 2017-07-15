#include "cutop.cc"
#include "NLimit.cc"

void GetSystematicSimple(int sig_mass){
  
  TH1::SetDefaultSumw2(true);

  bool DoBVeto = true;
  
  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");
  
  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/UpDownSyst/";

  //TString region = "Preselection";
  TString region = "Preselection_MuMuE";

  //=====================================
  //==== Setting calculated systematics
  //=====================================

  map<TString, double> CalculatedSysts;

  string elline_syst;
  ifstream in_syst(WORKING_DIR+"/data/"+dataset+"/Syst.txt");
  cout << "#### Setting calculated systematics ####" << endl;
  while(getline(in_syst,elline_syst)){
    std::istringstream is( elline_syst );
    TString source;
    double value;
    is >> source;
    is >> value;
    cout << source << " : " << value << endl;
    CalculatedSysts[source] = value;
  }

  double uncert_lumi = CalculatedSysts["Luminosity"];
  double uncert_fake = CalculatedSysts["FakeLooseID"];

  vector<TString> bkg_prompt_list = {
    "WZTo3LNu_powheg",
    "ZZTo4L_powheg",
    "ZGto2LG",
    "top",
    "VVV"
  };
  vector<TString> alias = {
    "WZ",
    "ZZ",
    "Z\\gamma",
    "t \\bar t V",
    "VVV"
  };
  
  //============================================
  //==== Setting MC Normalization Scale Factor
  //============================================

  map<TString, double> MCNormSF, MCNormSF_uncert;
  for(unsigned int i=0; i<bkg_prompt_list.size(); i++){
    MCNormSF[bkg_prompt_list.at(i)] = 1.;
    MCNormSF_uncert[bkg_prompt_list.at(i)] = 0.;
  }

  string elline_MCSF;
  ifstream in_MCSF(WORKING_DIR+"/data/"+dataset+"/MCSF.txt");
  cout << "#### Setting MCSF ####" << endl;
  while(getline(in_MCSF,elline_MCSF)){
    std::istringstream is( elline_MCSF );
    TString sample;
    double MCSF, MCSF_err;
    is >> sample;
    is >> MCSF;
    is >> MCSF_err;

    cout << sample << " : " << "MCSF = " << MCSF << ", MCSF_err = " << MCSF_err << endl;

    MCNormSF[sample] = MCSF;
    MCNormSF_uncert[sample] = MCSF_err;

  }

  int SignalClass;
  if(sig_mass <= 50) SignalClass = 1;
  else if(sig_mass <= 80) SignalClass = 2;
  else if(sig_mass <= 1000) SignalClass = 3;
  else SignalClass = 4;

  double cut_first_pt, cut_second_pt, cut_third_pt, cut_W_pri_mass, cut_PFMET, cut_HN_mass;

  //==== Low Mass
  if(region=="Preselection"){
    if(sig_mass < 80){
      cout << "#### Low Mass ####" << endl;
      cut_first_pt = 99999999.;
      cut_second_pt = 99999999.;
      cut_third_pt = 99999999.;
      cut_W_pri_mass = 150;
      cut_PFMET = 0.;
      cut_HN_mass = 99999999.;
    }
    //==== High Mass
    else{
      cout << "#### High Mass ####" << endl;
      cut_first_pt = 20.;
      cut_second_pt = 10.;
      cut_third_pt = 10.;
      cut_W_pri_mass = 0.;
      cut_PFMET = 20.;
    }
  }

  if(region=="Preselection_MuMuE"){
    if(sig_mass < 80){
      cout << "#### Low Mass ####" << endl;
      cut_first_pt = 99999999.;
      cut_second_pt = 99999999.;
      cut_third_pt = 99999999.;
      cut_W_pri_mass = 99999999.;
      cut_PFMET = 99999999.;
      cut_HN_mass = 99999999.;
    }
    //==== High Mass
    else{
      cout << "#### High Mass ####" << endl;
      cut_first_pt = 20.;
      cut_second_pt = 10.;
      cut_third_pt = 10.;
      cut_W_pri_mass = 0.;
      cut_PFMET = 0.;
      cut_HN_mass = 99999999.;
    }
  }


  //==== Run loop

  for(unsigned int it_prompt=0; it_prompt<bkg_prompt_list.size()+1; it_prompt++){

    TString this_prompt;
    vector<TString> this_bkg_prompt_list;
    if(it_prompt!=bkg_prompt_list.size()){
      this_prompt = alias.at(it_prompt);
      this_bkg_prompt_list.push_back( bkg_prompt_list.at(it_prompt) );
    }
    else{
      this_prompt = "ALL";
      for(unsigned int aaa=0;aaa<bkg_prompt_list.size();aaa++) this_bkg_prompt_list.push_back( bkg_prompt_list.at(aaa) );
    }

    vector<TString> systtypes = {"Central", "MuonEn_up", "MuonEn_down", "JetEn_up", "JetEn_down", "JetRes_up", "JetRes_down", "Unclustered_up", "Unclustered_down", "MCxsec_up", "MCxsec_down", "MuonIDSF_up", "MuonIDSF_down", "PU_up", "PU_down"};
    vector<double> yields_prompt, yields_fake, yields_data, yields_signal, yields_signal_weighted;
    vector<double> syst_error_prompt, syst_error_fake, syst_error_data, syst_error_signal;
    vector<double> rel_syst_error_prompt, rel_syst_error_fake, rel_syst_error_data, rel_syst_error_signal;
    double stat_error_prompt, stat_error_fake, stat_error_data, stat_error_signal;

    for(int i=0; i<systtypes.size(); i++){
      
      TString this_syst = systtypes.at(i);
      if(this_syst.Contains("JetRes")) this_syst = "Central";  //FIXME
      if(this_syst.Contains("MCxsec_")) this_syst = "Central";
      
      double n_bkg_prompt(0.), n_bkg_fake(0.), n_data(0.), n_signal(0.), n_signal_weighted(0.);
      
      TH1D *hist_bkg_for_error = NULL;
      if(systtypes.at(i)=="Central") hist_bkg_for_error = new TH1D("hist_bkg_for_error", "", 1, 0., 1.);
      
      double n_non_matrix_el(0.);

      for(unsigned int k=0; k<this_bkg_prompt_list.size(); k++){
        TString this_samplename = this_bkg_prompt_list.at(k);

        cutop m_bkg_prompt(filepath+"trilepton_mumumu_ntp_SK"+this_samplename+"_trilep_cat_"+catversion+".root", "Ntp_"+this_syst);
        m_bkg_prompt.SearchRegion = region;
        m_bkg_prompt.cut_first_pt = cut_first_pt;
        m_bkg_prompt.cut_second_pt = cut_second_pt;
        m_bkg_prompt.cut_third_pt = cut_third_pt;
        m_bkg_prompt.cut_W_pri_mass = cut_W_pri_mass;
        m_bkg_prompt.cut_PFMET = cut_PFMET;
        m_bkg_prompt.cut_HN_mass = cut_HN_mass;
        m_bkg_prompt.signalclass = SignalClass;
        m_bkg_prompt.MCNormSF = MCNormSF[this_samplename];
        double MCNormDir(0.);
        if(systtypes.at(i) == "MCxsec_up") MCNormDir = 1.;
        else if(systtypes.at(i) == "MCxsec_down") MCNormDir = -1.;
        m_bkg_prompt.MCNormSF_uncert = MCNormDir*MCNormSF_uncert[this_samplename];
        m_bkg_prompt.BVeto = DoBVeto;
        m_bkg_prompt.Loop();

        n_bkg_prompt += m_bkg_prompt.n_weighted;
        
        if(systtypes.at(i)=="Central"){
          hist_bkg_for_error->Add(m_bkg_prompt.hist_for_error);
          //cout << this_samplename << " : " << m_bkg_prompt.n_weighted << ", error = " << m_bkg_prompt.hist_for_error->GetBinError(1) << endl;
        }
        
      }
      
      cutop m_bkg_fake(filepath+"trilepton_mumumu_ntp_SKfake_sfed_HighdXY_trilep_cat_"+catversion+".root", "Ntp_"+this_syst);
      m_bkg_fake.SearchRegion = region;
      m_bkg_fake.cut_first_pt = cut_first_pt;
      m_bkg_fake.cut_second_pt = cut_second_pt;
      m_bkg_fake.cut_third_pt = cut_third_pt;
      m_bkg_fake.cut_W_pri_mass = cut_W_pri_mass;
      m_bkg_fake.cut_PFMET = cut_PFMET;
      m_bkg_fake.cut_HN_mass = cut_HN_mass;
      m_bkg_fake.signalclass = SignalClass;
      m_bkg_fake.BVeto = DoBVeto;
      m_bkg_fake.Loop();
      n_bkg_fake = m_bkg_fake.n_weighted;
      
      cutop m_data(filepath+"trilepton_mumumu_ntp_data_DoubleMuon_cat_"+catversion+".root", "Ntp_"+this_syst);
      m_data.SearchRegion = region;
      m_data.cut_first_pt = cut_first_pt;
      m_data.cut_second_pt = cut_second_pt;
      m_data.cut_third_pt = cut_third_pt;
      m_data.cut_W_pri_mass = cut_W_pri_mass;
      m_data.cut_PFMET = cut_PFMET;
      m_data.cut_HN_mass = cut_HN_mass;
      m_data.signalclass = SignalClass;
      m_data.BVeto = DoBVeto;
      m_data.Loop();
      n_data = m_data.n_weighted;

      TString channel = "MuMuMu";
      if(region=="Preselection_MuMuE") channel = "SSSF_MuMuE";
      cutop m_sig(filepath+"trilepton_mumumu_ntp_SKHN_"+channel+"_"+TString::Itoa(sig_mass, 10)+"_cat_"+catversion+".root", "Ntp_"+this_syst);
      m_sig.SearchRegion = region;
      m_sig.cut_first_pt = cut_first_pt;
      m_sig.cut_second_pt = cut_second_pt;
      m_sig.cut_third_pt = cut_third_pt;
      m_sig.cut_W_pri_mass = cut_W_pri_mass;
      m_sig.cut_PFMET = cut_PFMET;
      m_sig.cut_HN_mass = cut_HN_mass;
      m_sig.signalclass = SignalClass;
      m_sig.BVeto = DoBVeto;
      m_sig.Loop();
      n_signal = m_sig.n_weighted;
      
      yields_prompt.push_back(n_bkg_prompt);
      yields_fake.push_back(n_bkg_fake);
      yields_signal.push_back(n_signal);
      yields_data.push_back(n_data);
      
      syst_error_prompt.push_back( n_bkg_prompt-yields_prompt.at(0) );
      syst_error_fake.push_back( n_bkg_fake-yields_fake.at(0) );
      syst_error_signal.push_back( n_signal-yields_signal.at(0) );
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
        stat_error_signal = m_sig.hist_for_error->GetBinError(1);

      }
      
    }
    
    double squared_syst_prompt = uncert_lumi*uncert_lumi*yields_prompt.at(0)*yields_prompt.at(0);
    for(unsigned int i=1; i<syst_error_prompt.size(); i++) squared_syst_prompt += 0.5*syst_error_prompt.at(i)*syst_error_prompt.at(i);
    double squared_syst_fake = uncert_fake*uncert_fake*yields_fake.at(0)*yields_fake.at(0);
    for(unsigned int i=1; i<syst_error_fake.size(); i++) squared_syst_fake += 0.5*syst_error_fake.at(i)*syst_error_fake.at(i);
    double squared_syst_signal = uncert_lumi*uncert_lumi*yields_signal.at(0)*yields_signal.at(0);
    for(unsigned int i=1; i<syst_error_signal.size(); i++) squared_syst_signal += 0.5*syst_error_signal.at(i)*syst_error_signal.at(i);

    double data = yields_data.at(0);
    double prompt = yields_prompt.at(0);
    double prompt_stat = stat_error_prompt;
    double prompt_syst = sqrt(squared_syst_prompt);
    double fake = yields_fake.at(0);
    double fake_stat = stat_error_fake;
    double fake_syst = sqrt(squared_syst_fake);

    double total = prompt+fake;
    double total_stat = sqrt(prompt_stat*prompt_stat + fake_stat*fake_stat);
    double total_syst = sqrt(prompt_syst*prompt_syst + fake_syst*fake_syst);
    double total_err = sqrt(total_stat*total_stat+total_syst*total_syst);

    double signal = yields_signal.at(0);
    double signal_stat = stat_error_signal;
    double signal_syst = sqrt(squared_syst_signal);
    double signal_err = sqrt(signal_stat*signal_stat+signal_syst*signal_syst);

    if(it_prompt!=bkg_prompt_list.size()){
      cout << "~~$"
      <<std::fixed<<std::setprecision(2)<<this_prompt<<"$\t& $" 
      <<std::fixed<<std::setprecision(2)<<prompt << " \\pm "
      <<std::fixed<<std::setprecision(2)<<prompt_stat << "~\\mathrm{(stat.)}\\pm "
      <<std::fixed<<std::setprecision(2)<<prompt_syst << "~\\mathrm{(syst.)}$ \\\\"
      << endl;
    }
    else{
      cout << "Total prompt backgrounds\t& $"
      <<std::fixed<<std::setprecision(2)<<prompt << " \\pm "
      <<std::fixed<<std::setprecision(2)<<prompt_stat << "~\\mathrm{(stat.)}\\pm "
      <<std::fixed<<std::setprecision(2)<<prompt_syst << "~\\mathrm{(syst.)}$ \\\\" << endl;
      cout << "\\hline" << endl;
      cout << "Non-prompt background\t& $"
      <<std::fixed<<std::setprecision(2)<<fake << " \\pm "
      <<std::fixed<<std::setprecision(2)<<fake_stat << "~\\mathrm{(stat.)}\\pm "
      <<std::fixed<<std::setprecision(2)<<fake_syst << "~\\mathrm{(syst.)}$ \\\\" << endl;
      cout << "\\hline" << endl;
      cout << "Total backgrounds\t& $"
      <<std::fixed<<std::setprecision(2)<<total << " \\pm "
      <<std::fixed<<std::setprecision(2)<<total_stat << "~\\mathrm{(stat.)}\\pm "
      <<std::fixed<<std::setprecision(2)<<total_syst << "~\\mathrm{(syst.)}$ \\\\" << endl;
      cout << "\\hline" << endl;
      cout << "Observed in data (35.9 fb$^{-1}$)\t& $BLIND$ \\\\" << endl;
    }
    

  } // prompt loop


}




