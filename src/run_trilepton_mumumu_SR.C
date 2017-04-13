#include "trilepton_mumumu.cc"
#include <fstream>

void run_trilepton_mumumu_SR(int XXX){
 
  //==============
  //==== get env
  //==============

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");

  //====================
  //==== decalre class
  //====================

  trilepton_mumumu m;

  //=====================
  //==== set data class
  //=====================

  m.data_class = dataset+"/SR/";

  //================================
  //==== set prefixes and suffixes
  //================================

  m.filename_prefix = "trilepton_mumumu";
  m.filename_suffix = "_cat_"+catversion+".root";
  m.histname_suffix = {"_cut0", "_cutWlow", "_cutWhigh"};
  m.drawdata = {false, false, false};
  m.ApplyMCNormSF = {true, true, true};
  //m.ApplyMCNormSF = {false, false, false};

  //==========================
  //==== Signal plot setting
  //==========================

  m.signal_mass = {
    5, 10, 20,
    30, 40, 50,
    60, 70,
    90, 100, 150,
    200, 300, 400,
    500, 700, 1000
  };
  for(unsigned int i=0; i<m.signal_mass.size(); i++){
    TString thiscut = "_cutHN"+TString::Itoa(m.signal_mass.at(i),10);
    Color_t this_color;
    if(i<=5) this_color = kAzure+(-9+2*i);
    else if(i<=10) this_color = kRed+(-10+2*(i-6));
    else this_color = kGreen+(-15+3*(i-11));
    cout << thiscut << " : " << this_color << endl;

    m.histname_suffix.push_back(thiscut);
    m.drawdata.push_back(false);
    m.ApplyMCNormSF.push_back(true);
    m.signal_color.push_back(this_color);
    
  }
  m.signal_draw = {
    false, false, true,
    false, false, false,
    true, false,
    true, false, false,
    true, false, false,
    false, true, true
  };

  //=========================
  //==== set sample mapping
  //=========================

  m.map_sample_string_to_list["DY"] = {"DYJets_10to50", "DYJets"};
  m.map_sample_string_to_list["WJets"] = {"WJets"};
  m.map_sample_string_to_list["VV_excl"] = {"WZTo3LNu_powheg", "ZZTo4L_powheg"};
  m.map_sample_string_to_list["VV_incl"] = {"WZ", "ZZ"};
  m.map_sample_string_to_list["WZ_excl"] = {"WZTo3LNu_powheg"};
  m.map_sample_string_to_list["ZZ_excl"] = {"ZZTo4L_powheg"};
  m.map_sample_string_to_list["VVV"] = {"WWW", "WWZ", "WZZ", "ZZZ"};
  m.map_sample_string_to_list["ttbar"] = {"TTJets_aMC"};
  m.map_sample_string_to_list["ttV"] = {"ttW", "ttZ", "ttH_nonbb"}; //FIXME ttH into ttV
  m.map_sample_string_to_list["ttH"] = {"ttH_nonbb"};
  m.map_sample_string_to_list["top"] = {"TTJets_aMC", "ttW", "ttZ", "ttH_nonbb"};
  m.map_sample_string_to_list["Wgamma"] = {"WGtoLNuG", "WGtoLNuMM"};
  m.map_sample_string_to_list["Zgamma"] = {"ZGto2LG"};
  m.map_sample_string_to_list["Vgamma"] = {"WGtoLNuG", "WGtoLNuMM", "ZGto2LG"};
  m.map_sample_string_to_list["fake_HighdXY"] = {"fake_HighdXY"};
  m.map_sample_string_to_list["fake_sfed_HighdXY"] = {"fake_sfed_HighdXY"};
  m.map_sample_string_to_list["fake_DiMuon_HighdXY"] = {"fake_HighdXY"};

  m.map_sample_string_to_legendinfo["DY"] = make_pair("DY", kYellow);
  m.map_sample_string_to_legendinfo["WJets"] = make_pair("WJets", kGreen);
  m.map_sample_string_to_legendinfo["VV_excl"] = make_pair("VV", kSpring-1);
  m.map_sample_string_to_legendinfo["VV_incl"] = make_pair("VV", kSpring-1);
  m.map_sample_string_to_legendinfo["WZ_excl"] = make_pair("WZ", kGreen);
  m.map_sample_string_to_legendinfo["ZZ_excl"] = make_pair("ZZ", kRed-7);
  m.map_sample_string_to_legendinfo["VVV"] = make_pair("VVV", kSpring+10);
  m.map_sample_string_to_legendinfo["ttbar"] = make_pair("ttbar", kRed);
  m.map_sample_string_to_legendinfo["ttV"] = make_pair("ttV", kOrange);
  m.map_sample_string_to_legendinfo["ttH"] = make_pair("ttH", kOrange);
  m.map_sample_string_to_legendinfo["top"] = make_pair("top", kOrange);
  m.map_sample_string_to_legendinfo["Wgamma"] = make_pair("W#gamma", kSpring-7);
  m.map_sample_string_to_legendinfo["Zgamma"] = make_pair("Z#gamma", kSpring-7);
  m.map_sample_string_to_legendinfo["Vgamma"] = make_pair("V#gamma", kSpring-7);
  m.map_sample_string_to_legendinfo["fake_HighdXY"] = make_pair("Misd", kAzure+10);
  m.map_sample_string_to_legendinfo["fake_sfed_HighdXY"] = make_pair("Misd", kAzure+10);
  m.map_sample_string_to_legendinfo["fake_DiMuon_HighdXY"] = make_pair("Misd", kAzure+10);

  //===============================  
  //==== set and make sample list
  //===============================

  if(XXX==0){
    m.data_class = m.data_class+"/before_prompt_matching";
    m.samples_to_use = {"DY_MCatNLO", "WJets_MCatNLO", "VV_excl_MCatNLO", "t", "ttV", "VVV_MCatNLO"};
  }
  if(XXX==1) m.samples_to_use = {"fake_HighdXY", "VV_excl", "Zgamma", "ttV", "VVV"};
  if(XXX==2) m.samples_to_use = {"fake_sfed_HighdXY", "VV_excl", "Zgamma", "ttV", "VVV"};

  m.make_bkglist();

  //=====================
  //==== Fill MCNorm SF
  //=====================

  for(unsigned int i=0; i<m.bkglist.size(); i++){
    m.MCNormSF[m.bkglist.at(i)] = 1.;
    m.MCNormSF_uncert[m.bkglist.at(i)] = 0.;
  }
  m.SetMCSF(WORKING_DIR+"/data/"+dataset+"/MCSF.txt");

  //======================
  //==== Get Systematics
  //======================

  m.SetCalculatedSysts(WORKING_DIR+"/data/"+dataset+"/Syst.txt");


  //============================
  //==== set variables to draw
  //============================

  m.histname = {
    "h_HN_mass_class1", "h_HN_mass_class2", "h_HN_mass_class3", "h_HN_mass_class4",
    "h_W_pri_lowmass_mass", "h_W_pri_highmass_mass",
    "h_deltaR_OS_min", "h_gamma_star_mass", "h_Njets", "h_z_candidate_mass",
    "h_PFMET", "h_PFMET_phi",
    "h_leadingLepton_Pt",
    "h_secondLepton_Pt",
    "h_thirdLepton_Pt",
    "h_leadingLepton_Eta",
    "h_secondLepton_Eta",
    "h_thirdLepton_Eta",
    "h_leadingLepton_LeptonRelIso",
    "h_secondLepton_LeptonRelIso",
    "h_thirdLepton_LeptonRelIso",
    "h_leadingLepton_dXY",
    "h_secondLepton_dXY",
    "h_thirdLepton_dXY",
    "h_leadingLepton_dZ",
    "h_secondLepton_dZ",
    "h_thirdLepton_dZ",
    "h_leadingLepton_GlobalChi2",
    "h_secondLepton_GlobalChi2",
    "h_thirdLepton_GlobalChi2",
    "h_Nevents",
    "h_nVertices",
    "h_osllmass",
    "h_Nbjets",
    "h_W_sec_highmass_mass",
    "h_leadingLepton_dXYSig",
    "h_secondLepton_dXYSig",
    "h_thirdLepton_dXYSig",
    "h_mlll",
    "h_mT"
  };
  m.x_title = {
    "m(#mu#mu#nu) [GeV]", "m(#mu#mu#nu) [GeV]", "m(#mu#mu#nu) [GeV]", "m(#mu#mu#nu) [GeV]",
    "m(#mu#mu#mu#nu) [GeV]", "m(#mu#mu#mu#nu) [GeV]",
    "#DeltaR(OS)_{min}", "m(#mu+#mu-) [GeV]", "# of jets", "m(#mu+#mu-) [GeV]",
    "#slash{E}_{T}^{miss} [GeV]", "#phi of #slash{E}_{T}^{miss}",
    "p_{T} [GeV]",
    "p_{T} [GeV]",
    "p_{T} [GeV]",
    "#eta",
    "#eta",
    "#eta",
    "LepRelIso04",
    "LepRelIso04",
    "LepRelIso04",
    "dXY [cm]",
    "dXY [cm]",
    "dXY [cm]",
    "dZ [cm]",
    "dZ [cm]",
    "dZ [cm]",
    "GlobalChi2",
    "GlobalChi2",
    "GlobalChi2",
    "onebin",
    "# of Vertices",
    "m(OS) [GeV]",
    "# of b-tagged jets",
    "m(#mu#nu) [GeV]",
    "#sigma(dXY)",
    "#sigma(dXY)",
    "#sigma(dXY)",
    "m(#mu#mu#mu) [GeV]",
    "m_{T}(#mu, #slash{E}_{T}^{miss}) [GeV]"
  };
  m.units = {
    "GeV", "GeV", "GeV", "GeV",
    "GeV", "GeV",
    "", "GeV", "int", "GeV", "GeV", "radian",
    "GeV",
    "GeV",
    "GeV",
    "",
    "",
    "",
    "",
    "",
    "",
    "cm",
    "cm",
    "cm",
    "cm",
    "cm",
    "cm",
    "",
    "",
    "",
    "int",
    "int",
    "GeV",
    "int",
    "GeV",
    "",
    "",
    "",
    "GeV"
  };

  //====================
  //==== cut-var skips
  //====================

/*
  for(unsigned int i=0; i<m.histname.size(); i++){
    TString this_var = m.histname.at(i);
    if( this_var.Contains("class3") || this_var.Contains("class4") || this_var.Contains("highmass") ) m.CutVarSkips.push_back( make_pair("_cutWlow", this_var) );
    if( this_var.Contains("class1") || this_var.Contains("class2") || this_var.Contains("lowmass") ) m.CutVarSkips.push_back( make_pair("_cutWhigh", this_var) );
  }
*/

  //====================================
  //==== set signal coupling constants
  //====================================

  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 5)] = 0.01;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 10)] = 0.001;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 20)] = 0.001;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 30)] = 0.001;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 40)] = 0.001;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 50)] = 0.001;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 60)] = 0.001;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 70)] = 0.001;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 90)] = 0.1;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 100)] = 0.1;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 150)] = 0.001;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 200)] = 1.;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 300)] = 1.;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 400)] = 10.;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 500)] = 100.;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 700)] = 100.;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 1000)] = 100.;
  }

  //=====================================
  //==== set signal mass for each class
  //=====================================

  //m.map_class_to_signal_mass[trilepton_mumumu::class1] = {5, 10, 20, 30, 40, 50};
  //m.map_class_to_signal_mass[trilepton_mumumu::class2] = {60, 70};
  //m.map_class_to_signal_mass[trilepton_mumumu::lowmass] = {5, 10, 20, 30, 40, 50, 60, 70};
  //m.map_class_to_signal_mass[trilepton_mumumu::class3] = {90, 100, 150, 200, 300, 400, 500, 700, 1000};
  //m.map_class_to_signal_mass[trilepton_mumumu::class4] = {2000};
  //m.map_class_to_signal_mass[trilepton_mumumu::highmass] = {90, 100, 150, 200, 300, 400, 500, 700, 1000};

  m.map_class_to_signal_mass[trilepton_mumumu::class1] = {5, 10, 20, 30, 40, 50, 60, 70, 90, 100, 150, 200, 300, 400, 500, 700, 1000};
  m.map_class_to_signal_mass[trilepton_mumumu::class2] = {5, 10, 20, 30, 40, 50, 60, 70, 90, 100, 150, 200, 300, 400, 500, 700, 1000};
  m.map_class_to_signal_mass[trilepton_mumumu::lowmass] = {5, 10, 20, 30, 40, 50, 60, 70, 90, 100, 150, 200, 300, 400, 500, 700, 1000};
  m.map_class_to_signal_mass[trilepton_mumumu::class3] = {5, 10, 20, 30, 40, 50, 60, 70, 90, 100, 150, 200, 300, 400, 500, 700, 1000};
  m.map_class_to_signal_mass[trilepton_mumumu::class4] = {5, 10, 20, 30, 40, 50, 60, 70, 90, 100, 150, 200, 300, 400, 500, 700, 1000};
  m.map_class_to_signal_mass[trilepton_mumumu::highmass] = {5, 10, 20, 30, 40, 50, 60, 70, 90, 100, 150, 200, 300, 400, 500, 700, 1000};

  //=============
  //==== rebins
  //=============

/*
  //==== script to generate rebins
  ofstream skeleton_rebins("./data/SR_skeleton_rebins.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_rebins
      //<< "m.rebins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<endl;
    }
  }
  skeleton_rebins.close();
  return;
*/

  m.SetRebins(WORKING_DIR+"/data/"+dataset+"/SR_rebins.txt");

  //=============
  //==== y_maxs
  //=============

/*
  ofstream skeleton_y_maxs("./data/skeleton_y_maxs.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_y_maxs
      //<< "  m.y_maxs[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<endl;
    }
  }
  skeleton_y_maxs.close();
  return;
*/

  //==== default max
  m.default_y_max = 2000.;

  m.SetYAxis(WORKING_DIR+"/data/"+dataset+"/SR_yaxis.txt");

  //=============
  //==== x_mins
  //=============

/*
  //==== script to generate rebins
  ofstream skeleton_x_mins("./data/SR_skeleton_xaxis.txt", ios::trunc);
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
  for(unsigned int j=0; j<m.histname.size(); j++){
      skeleton_x_mins
      //<< "  m.x_mins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
      <<m.histname_suffix.at(i)<<"\t"<<m.histname.at(j)<<"\t"<<-999<<"\t"<<-999<<endl;
    }
  }
  skeleton_x_mins.close();
  return;
*/

  m.SetXAxis(WORKING_DIR+"/data/"+dataset+"/SR_xaxis.txt");

  //===============
  //==== k-factor
  //===============

  m.k_factor = 1.;

  //=================================
  //==== mixing at generation level
  //=================================

  m.log_of_generation_mixing = -2.;

  //===============================
  //==== prepare plot directories
  //===============================

  m.make_plot_directory();

  //===============================
  //==== declare output rootfiles
  //===============================

  m.outputfile = new TFile(m.plotpath+"/hists.root", "RECREATE");

  //==========================
  //==== finally, draw plots
  //==========================

  m.draw_hist();

}






