#include "syst_UpDowns.C"

void make_rootfile_for_limit(bool newfile=true, bool printsyst=false){

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString LIMIT_PATH = WORKING_DIR+"/plots/"+dataset+"/Limit/";
  TString WhichDirectoryInCutop = "170524_v8-0-7.11";

  LIMIT_PATH = LIMIT_PATH+WhichDirectoryInCutop+"/";

  if( !gSystem->mkdir(LIMIT_PATH, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << LIMIT_PATH << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

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
    cout << source << " : " << value << endl;
    CalculatedSysts[source] = value;
  }
  double uncert_lumi = CalculatedSysts["Luminosity"];
  double uncert_fake = CalculatedSysts["FakeLooseID"];

  vector<double> masses = {5, 10, 20, 30, 40, 50, 60, 70, 90, 100, 150, 200, 300, 400, 500, 700, 1000};
  vector<double> xsec = {4.046, 3.982, 3.711, 3.241, 2.619, 1.875, 1.07, 0.3828, 0.02333, 0.01082, 0.001488, 0.0004567, 9.52E-05, 3.14E-05, 1.29E-05, 3.21E-06, 6.48E-07};
  if(printsyst){
    masses = {40, 60, 150, 700};
    xsec = {2.619, 1.07, 0.001488, 3.21E-06};
  }
  const int n_mass = masses.size();

  TH1D *hist_mass = new TH1D("hist_mass", "", n_mass, 0, n_mass);
  TH1D *hist_xsec = new TH1D("hist_xsec", "", n_mass, 0, n_mass);
  TH1D *hist_fake = new TH1D("hist_fake", "", n_mass, 0, n_mass);
  TH1D *hist_prompt = new TH1D("hist_prompt", "", n_mass, 0, n_mass);
  TH1D *hist_signal = new TH1D("hist_signal", "", n_mass, 0, n_mass);
  TH1D *hist_signal_weighted = new TH1D("hist_signal_weighted", "", n_mass, 0, n_mass);

  vector<double> signal_syst_Lumi, signal_syst_MuonPt, signal_syst_JES, signal_syst_Uncl, signal_syst_MuonID, signal_syst_PU, signal_syst_TriggerSF;
  vector<double> prompt_syst_Lumi, prompt_syst_MuonPt, prompt_syst_JES, prompt_syst_Uncl, prompt_syst_MuonID, prompt_syst_Norm, prompt_syst_PU, prompt_syst_TriggerSF;
  vector<double> fake_syst_MuonPt, fake_syst_JES, fake_syst_Uncl, fake_syst_FRHalfSample;
  
  for(unsigned int i=0; i<masses.size(); i++){

    cout << "Writing m(HN) = " << masses.at(i) << endl;

    double mass = masses.at(i);

    double N_MC = 100000.;
    if(mass==200) N_MC = 96193.;
    if(mass==400) N_MC = 99070.;

    // syst_UpDowns(int sig_mass, bool printnumber=true, bool forlatex=false, bool inclusive=false, bool makeroot=false)
    NLimit n_limit = syst_UpDowns(mass, false, false, false, true);
    //NLimit n_limit = syst_UpDowns(mass, false, false, true, true); //inclusive

    //double mass;
    //double n_fake,   n_stat_fake,   n_syst_fake;
    //double n_prompt, n_stat_prompt, n_syst_prompt;
    //double n_signal_weighted, n_signal,  n_stat_signal,  n_syst_signal;
    //double n_data;

    hist_mass->SetBinContent(i+1, n_limit.mass);
    hist_xsec->SetBinContent(i+1, 2.*100.*xsec.at(i)); // X 2.0 is for setting VeN = 0

    hist_fake->SetBinContent(i+1, n_limit.n_fake);
    hist_fake->SetBinError(i+1, n_limit.err_total(NLimit::fake));

    hist_prompt->SetBinContent(i+1, n_limit.n_prompt);
    hist_prompt->SetBinError(i+1, n_limit.err_total(NLimit::prompt));

    hist_signal->SetBinContent(i+1, n_limit.n_signal/N_MC);
    hist_signal->SetBinError(i+1, n_limit.err_total(NLimit::signal)/N_MC);

    hist_signal_weighted->SetBinContent(i+1, n_limit.n_signal_weighted);
    hist_signal_weighted->SetBinError(i+1, n_limit.err_total(NLimit::signal)/N_MC*n_limit.n_signal_weighted);

    signal_syst_Lumi.push_back( uncert_lumi*100. );
    signal_syst_MuonPt.push_back( 100.*n_limit.signal_systs[NLimit::MuonPt]/n_limit.n_signal );
    signal_syst_JES.push_back( 100.*n_limit.signal_systs[NLimit::JES]/n_limit.n_signal );
    signal_syst_Uncl.push_back( 100.*n_limit.signal_systs[NLimit::Uncl]/n_limit.n_signal );
    signal_syst_MuonID.push_back( 100.*n_limit.signal_systs[NLimit::MuonID]/n_limit.n_signal );
    signal_syst_PU.push_back( 100.*n_limit.signal_systs[NLimit::PU]/n_limit.n_signal );
    signal_syst_TriggerSF.push_back( 100.*n_limit.signal_systs[NLimit::TriggerSF]/n_limit.n_signal );

    prompt_syst_Lumi.push_back( uncert_lumi*100. );
    prompt_syst_MuonPt.push_back( 100.*n_limit.prompt_systs[NLimit::MuonPt]/n_limit.n_prompt );
    prompt_syst_JES.push_back( 100.*n_limit.prompt_systs[NLimit::JES]/n_limit.n_prompt );
    prompt_syst_Uncl.push_back( 100.*n_limit.prompt_systs[NLimit::Uncl]/n_limit.n_prompt );
    prompt_syst_MuonID.push_back( 100.*n_limit.prompt_systs[NLimit::MuonID]/n_limit.n_prompt );
    prompt_syst_Norm.push_back( 100.*n_limit.prompt_systs[NLimit::Norm]/n_limit.n_prompt );
    prompt_syst_PU.push_back( 100.*n_limit.prompt_systs[NLimit::PU]/n_limit.n_prompt );
    prompt_syst_TriggerSF.push_back( 100.*n_limit.prompt_systs[NLimit::TriggerSF]/n_limit.n_prompt );
    
    fake_syst_MuonPt.push_back( 100.*n_limit.fake_systs[NLimit::MuonPt]/n_limit.n_fake );
    fake_syst_JES.push_back( 100.*n_limit.fake_systs[NLimit::JES]/n_limit.n_fake );
    fake_syst_Uncl.push_back( 100.*n_limit.fake_systs[NLimit::Uncl]/n_limit.n_fake );
    fake_syst_FRHalfSample.push_back( 100.*n_limit.fake_systs[NLimit::FRHalfSample]/n_limit.n_fake );
    
    //cout << "mass = " << n_limit.mass << ", fake = " << n_limit.n_fake << ", fakeerr = " << n_limit.err_total(NLimit::fake) << ", prompt = " << n_limit.n_prompt << ", prompterr = " << n_limit.err_total(NLimit::prompt) << ", sigeff = " << n_limit.n_signal/N_MC << ", sigefferr = " << n_limit.err_total(NLimit::signal)/N_MC << endl;

  }

  
  if(newfile){
    TFile *file = new TFile(LIMIT_PATH+"/hist.root", "RECREATE");
    file->cd();
    hist_mass->Write();
    hist_xsec->Write();
    hist_fake->Write();
    hist_prompt->Write();
    hist_signal->Write();
    hist_signal_weighted->Write();
    file->Close();
  }

  if(printsyst){

    //===================
    //==== SIGNAL Syst.
    //===================

    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << "@@@@@@@@@@@@@ Signal @@@@@@@@@@@@@" << endl;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl << endl;
    
    cout << "###################### FOR LATEX ######################" << endl;

    cout << "~~~Integrated Luminosity"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<signal_syst_Lumi.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;

    cout << "~~~Muon Momentum Scale"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<signal_syst_MuonPt.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;

    cout << "~~~Muon ID"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<signal_syst_MuonID.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;

    cout << "~~~Jet Energy Scale"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<signal_syst_JES.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;

    cout << "~~~Unclustered Energy"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<signal_syst_Uncl.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;

    cout << "~~~Pileup Model"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<signal_syst_PU.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;

    cout << "~~~Trigger Efficiency"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<signal_syst_TriggerSF.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;

    cout << "###################### FOR SPREADSHEET ######################" << endl;

    cout << "Signal Mass"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << masses.at(i) << "\t";
    }
    cout << endl;

    cout << "Integrated Luminosity"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<signal_syst_Lumi.at(i)<<"\t";
    }
    cout << endl;

    cout << "Muon Momentum Scale"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<signal_syst_MuonPt.at(i)<<"\t";
    }
    cout << endl;

    cout << "Muon ID"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<signal_syst_MuonID.at(i)<<"\t";
    }
    cout << endl;

    cout << "Jet Energy Scale"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<signal_syst_JES.at(i)<<"\t";
    }
    cout << endl;

    cout << "Unclustered Energy"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<signal_syst_Uncl.at(i)<<"\t";
    }
    cout << endl;

    cout << "Pileup Model"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<signal_syst_PU.at(i)<<"\t";
    }
    cout << endl;

    cout << "Trigger Efficiency"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<signal_syst_TriggerSF.at(i)<<"\t";
    }
    cout << endl << endl;

    //===================
    //==== Prompt Syst.
    //===================

    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << "@@@@@@@@@@@@@ Prompt @@@@@@@@@@@@@" << endl;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl << endl;
    
    cout << "###################### FOR LATEX ######################" << endl;
    
    cout << "~~~Integrated Luminosity"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<prompt_syst_Lumi.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;
    
    cout << "~~~Muon Momentum Scale"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<prompt_syst_MuonPt.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;
    
    cout << "~~~Muon ID"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<prompt_syst_MuonID.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;
    
    cout << "~~~Jet Energy Scale"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<prompt_syst_JES.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;
    
    cout << "~~~Unclustered Energy"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<prompt_syst_Uncl.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;
    
    cout << "~~~Normalization"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<prompt_syst_Norm.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;

    cout << "~~~Pileup Model"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<prompt_syst_PU.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;

    cout << "~~~Trigger"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<prompt_syst_TriggerSF.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;
    
    cout << "###################### FOR SPREADSHEET ######################" << endl;
    
    cout << "prompt Mass"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << masses.at(i) << "\t";
    }
    cout << endl;
    
    cout << "Integrated Luminosity"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<prompt_syst_Lumi.at(i)<<"\t";
    }
    cout << endl;
    
    cout << "Muon Momentum Scale"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<prompt_syst_MuonPt.at(i)<<"\t";
    }
    cout << endl;
    
    cout << "Muon ID"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<prompt_syst_MuonID.at(i)<<"\t";
    }
    cout << endl;
    
    cout << "Jet Energy Scale"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<prompt_syst_JES.at(i)<<"\t";
    }
    cout << endl;
    
    cout << "Unclustered Energy"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<prompt_syst_Uncl.at(i)<<"\t";
    }
    cout << endl;
    
    cout << "Normalization"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<prompt_syst_Norm.at(i)<<"\t";
    }
    cout << endl;

    cout << "Pileup Model"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<prompt_syst_PU.at(i)<<"\t";
    }
    cout << endl;

    cout << "Trigger"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<prompt_syst_TriggerSF.at(i)<<"\t";
    }
    cout << endl << endl;

    //===================
    //==== Fake Syst.
    //===================
    
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << "@@@@@@@@@@@@@ Fake @@@@@@@@@@@@@" << endl;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl << endl;
    
    cout << "###################### FOR LATEX ######################" << endl;
    
    cout << "~~~Muon Momentum Scale"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<fake_syst_MuonPt.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;
    
    cout << "~~~Jet Energy Scale"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<fake_syst_JES.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;
    
    cout << "~~~Unclustered energy"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<fake_syst_Uncl.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;

    cout << "~~~Fake Rate Half Sample Test"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << "& "<<std::fixed<<std::setprecision(2)<<fake_syst_FRHalfSample.at(i)<<"\\% ";
    }
    cout << " \\\\" << endl;

    cout << "###################### FOR SPREADSHEET ######################" << endl;
    
    cout << "fake Mass"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout << masses.at(i) << "\t";
    }
    cout << endl;
    
    cout << "Muon Momentum Scale"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<fake_syst_MuonPt.at(i)<<"\t";
    }
    cout << endl;
    
    cout << "Jet Energy Scale"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<fake_syst_JES.at(i)<<"\t";
    }
    cout << endl;
    
    cout << "Unclustered energy"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<fake_syst_Uncl.at(i)<<"\t";
    }
    cout << endl;

    cout << "Fake Rate Half Sample Test"<<"\t";
    for(unsigned int i=0; i<masses.size(); i++){
      cout <<std::fixed<<std::setprecision(2)<<fake_syst_FRHalfSample.at(i)<<"\t";
    }
    cout << endl << endl;

    

  }





}
