#include "cutop.cc"
#include "NLimit.cc"

void MCNormalizationSF(bool UpdateSF=true){
  
  TH1::SetDefaultSumw2(true);

  bool DoBVeto = true;
  
  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");
  
  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/UpDownSyst/";
  
  //=====================================
  //==== Setting calculated systematics
  //=====================================

  map<TString, double> CalculatedSysts;

  string elline_syst;
  ifstream in_syst(WORKING_DIR+"/data/"+dataset+"/Syst.txt");
  //cout << "#### Setting calculated systematics ####" << endl;
  while(getline(in_syst,elline_syst)){
    std::istringstream is( elline_syst );
    TString source;
    double value;
    is >> source;
    is >> value;
    //cout << source << " : " << value << endl;
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
  vector<TString> NormCalcOrder_region = {
    //"ZZ_4mu0el",
    //"WZ_3mu0el",
    //"ZGamma_3mu0el"
    "ZZ",
    "WZ",
    "ZGamma"
    //"ZZ_2mu2el",
    //"WZ_2mu1el",
    //"ZGamma_2mu1el"
  };
  vector<TString> NormCalcOrder = {
    "ZZTo4L_powheg",
    "WZTo3LNu_powheg",
    "ZGto2LG"
  };
  
  //==== Initialise
  map<TString, double> MCNormSF, MCNormSF_uncert;
  for(unsigned int i=0; i<bkg_prompt_list.size(); i++){
    MCNormSF[bkg_prompt_list.at(i)] = 1.;
    MCNormSF_uncert[bkg_prompt_list.at(i)] = 0.;
  }

  //==== MCSF from EXO-17-006
  vector<TString> VVVandttV = {
    "VVV", "WWW", "WWZ", "WZZ", "ZZZ",
    "ttV", "ttW", "ttZ", "ttH_nonbb",
    "top"
  };
  for(unsigned int i=0; i<VVVandttV.size(); i++){
    MCNormSF[VVVandttV.at(i)] = 1.;
    MCNormSF_uncert[VVVandttV.at(i)] = 0.50;
  }

  int n_signal = NormCalcOrder_region.size();
  TMatrix matrixA(n_signal, n_signal);
  TMatrix matrixC(n_signal, 1);
  
  
  for(unsigned int it_region=0; it_region<NormCalcOrder_region.size(); it_region++){
    
    TString region = NormCalcOrder_region.at(it_region);
    TString this_signal = NormCalcOrder.at(it_region);
    if(UpdateSF){
      cout << endl;
      cout << "#### Calculating " << this_signal << " MC Normalisation Scale Factor ####" << endl << endl;
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

      for(unsigned int k=0; k<bkg_prompt_list.size(); k++){
        TString this_samplename = bkg_prompt_list.at(k);

        cutop m_bkg_prompt(filepath+"trilepton_mumumu_ntp_SK"+this_samplename+"_trilep_cat_"+catversion+".root", "Ntp_"+this_syst);
        m_bkg_prompt.SearchRegion = region;
        m_bkg_prompt.MCNormSF = MCNormSF[this_samplename];
        double MCNormDir(0.);
        if(systtypes.at(i) == "MCxsec_up") MCNormDir = 1.;
        else if(systtypes.at(i) == "MCxsec_down") MCNormDir = -1.;
        m_bkg_prompt.MCNormSF_uncert = MCNormDir*MCNormSF_uncert[this_samplename];
        m_bkg_prompt.BVeto = DoBVeto;
        m_bkg_prompt.Loop();

        //==== make matrix
        if(i==0){
          bool isMatrixEl = false;
          for(unsigned int j=0; j<NormCalcOrder.size(); j++){
            if(this_samplename==NormCalcOrder.at(j)){
              matrixA(it_region, j) = m_bkg_prompt.n_weighted;
              isMatrixEl = true;
            }
          }
          if(!isMatrixEl){
            n_non_matrix_el += m_bkg_prompt.n_weighted;
          }
        }

        if(this_samplename==this_signal) continue;
        n_bkg_prompt += m_bkg_prompt.n_weighted;
        
        if(systtypes.at(i)=="Central"){
          hist_bkg_for_error->Add(m_bkg_prompt.hist_for_error);
          //cout << this_samplename << " : " << m_bkg_prompt.n_weighted << ", error = " << m_bkg_prompt.hist_for_error->GetBinError(1) << endl;
        }
        
      }
      
      cutop m_bkg_fake(filepath+"trilepton_mumumu_ntp_SKfake_sfed_HighdXY_trilep_cat_"+catversion+".root", "Ntp_"+this_syst);
      m_bkg_fake.SearchRegion = region;
      m_bkg_fake.BVeto = DoBVeto;
      m_bkg_fake.Loop();
      n_bkg_fake = m_bkg_fake.n_weighted;
      
      cutop m_data(filepath+"trilepton_mumumu_ntp_data_DoubleMuon_cat_"+catversion+".root", "Ntp_"+this_syst);
      m_data.SearchRegion = region;
      m_data.BVeto = DoBVeto;
      m_data.Loop();
      n_data = m_data.n_weighted;

      cutop m_sig(filepath+"trilepton_mumumu_ntp_SK"+this_signal+"_trilep_cat_"+catversion+".root", "Ntp_"+this_syst);
      m_sig.SearchRegion = region;
      m_sig.MCNormSF = MCNormSF[this_signal];
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

      //==== make matrix
      if(i==0){
        for(unsigned int j=0; j<NormCalcOrder.size(); j++){
          if(this_signal==NormCalcOrder.at(j)){
            matrixC(it_region, 0) = n_data - n_bkg_fake - n_non_matrix_el;
          }
        }
      }
      
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

    double total_err_ratio = total_err/(data-total);
    double signal_err_ratio = signal_err/signal;
    double SF_err_ratio = total_err_ratio+signal_err_ratio;

    double SF = (data-total)/signal;
    double SF_err = SF*SF_err_ratio;

    if(UpdateSF){
      cout << endl;
      cout << "data = " << data << endl;
      cout << "total bkg = " << total << " +- " << total_err << endl;
      cout << "==> data - total bkg = " << data-total << " +- " << total_err << endl;
      cout << "signal = " << signal << " +- " << signal_err << " ( " << 100.*signal/data << " % )" << endl;
      cout << "==> SF = " << SF << " +- " << SF_err << endl;

      MCNormSF[this_signal] = SF;
      MCNormSF_uncert[this_signal] = SF_err;
    }
    
  }

  ofstream outputFile;
  cout << WORKING_DIR+"/data/"+dataset+"/MCSF.txt" << endl;
  outputFile.open(WORKING_DIR+"/data/"+dataset+"/MCSF.txt");
  for(unsigned int i=0; i<NormCalcOrder.size(); i++){
    outputFile<<NormCalcOrder.at(i)<<"\t"<<MCNormSF[NormCalcOrder.at(i)]<<"\t"<<MCNormSF_uncert[NormCalcOrder.at(i)]<<endl;
  }
  for(unsigned int i=0; i<VVVandttV.size(); i++){
    outputFile<<VVVandttV.at(i)<<"\t"<<MCNormSF[VVVandttV.at(i)]<<"\t"<<MCNormSF_uncert[VVVandttV.at(i)]<<endl;
  }


  if(UpdateSF) return;

  cout << endl;
  cout << "#######################" << endl;
  cout << "#### Matrix method ####" << endl;
  cout << "#######################" << endl;

  //matrixA.Print();

  TMatrix inverse(TMatrix::kInverted, matrixA);

  //inverse.Print();

  //matrixC.Print();

  TMatrix Cs(n_signal, 1);
  for(unsigned int i=0; i<n_signal; i++){
    Cs(i,0) = 0.;;
  }
  for(unsigned int i=0; i<n_signal; i++){
    for(unsigned int j=0; j<n_signal; j++){
      Cs(i,0) += inverse(i,j)*matrixC(j,0);
    }
  }

  Cs.Print();
  

}




