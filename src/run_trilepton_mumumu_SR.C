#include "trilepton_mumumu.cc"
#include <fstream>

void run_trilepton_mumumu_SR(int XXX){
 
  //==== get env
  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");
 
  //==== decalre class
  trilepton_mumumu m;
  
  //==== set data class
  m.data_class = dataset+"/SR";
  
  //==== set prefixes and suffixes
  m.filename_prefix = "trilepton_mumumu";
  m.filename_suffix = "_cat_"+catversion+".root";
  m.histname_suffix = {"_cut0", "_cutWlow", "_cutWhigh"};
  m.drawdata = {false, false, false};

  m.DrawPU = true;

  //==== set sample mapping
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
  
  //==== set and make sample list
  if(XXX==0){
    m.data_class = m.data_class+"/before_prompt_matching";
    m.samples_to_use = {"DY_MCatNLO", "WJets_MCatNLO", "VV_excl_MCatNLO", "t", "ttV", "VVV_MCatNLO"};
  }
  if(XXX==1) m.samples_to_use = {"fake_HighdXY", "VV_excl", "Vgamma", "ttV", "VVV"};
  if(XXX==2) m.samples_to_use = {"fake_sfed_HighdXY", "VV_excl", "Vgamma", "ttV", "VVV"};

  m.make_bkglist();
 
  //==== set variables to draw
  m.histname = {
    "HN_mass_class1", "HN_mass_class2", "HN_mass_class3", "HN_mass_class4",
    "W_pri_lowmass_mass", "W_pri_highmass_mass",
    "deltaR_OS_min", "gamma_star_mass", "h_Njets", "z_candidate_mass", "h_PFMET", "h_PFMET_phi",
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
    "n_events",
    "h_nVertices",
    "h_osllmass",
    "h_Nbjets",
    "W_sec_highmass_mass",
    "h_leadingLepton_dXYSig",
    "h_secondLepton_dXYSig",
    "h_thirdLepton_dXYSig",
  };
  m.x_title = {
    "m(#mu#mu#nu) [GeV]", "m(#mu#mu#nu) [GeV]", "m(#mu#mu#nu) [GeV]", "m(#mu#mu#nu) [GeV]",
    "m(#mu#mu#mu#nu) [GeV]", "m(#mu#mu#mu#nu) [GeV]",
    "#DeltaR(OS)_{min}", "m(#mu+#mu-) [GeV]", "# of jets", "m(#mu+#mu-) [GeV]", "#slash{E}_{T}^{miss} [GeV]", "#phi of #slash{E}_{T}^{miss}",
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
  };

  //==== cut-var skips
  for(unsigned int i=0; i<m.histname.size(); i++){
    TString this_var = m.histname.at(i);
    if( this_var.Contains("class3") || this_var.Contains("class4") || this_var.Contains("highmass") ) m.CutVarSkips.push_back( make_pair("_cutWlow", this_var) );
    if( this_var.Contains("class1") || this_var.Contains("class2") || this_var.Contains("lowmass") ) m.CutVarSkips.push_back( make_pair("_cutWhigh", this_var) );
  }
  
  //==== set signal mass points
  m.signal_mass = {
    20,
    60,
    90,
    200,
    700,
    1000
  };
  m.signal_color = {
    kRed-10, kRed-2, kRed,
    kBlue, kSpring+2,
    kMagenta-7, kMagenta-2, kViolet+1,
    kGray+1, kOrange+10, kBlack
  };
  
  //==== set signal coupling constants
  for(unsigned int i=0; i<m.histname_suffix.size(); i++){
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 5)] = 0.001;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 20)] = 0.001;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 40)] = 0.001;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 60)] = 0.001;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 70)] = 0.001;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 90)] = 0.1;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 100)] = 0.1;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 200)] = 1.;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 400)] = 10.;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 500)] = 100.;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 700)] = 100.;
    m.coupling_constants[make_pair(m.histname_suffix.at(i), 1000)] = 100.;
  }

  //==== set signal mass for each class
  m.map_class_to_signal_mass[trilepton_mumumu::class1] = {5, 10, 20, 30, 40, 50};
  m.map_class_to_signal_mass[trilepton_mumumu::class2] = {60, 70};
  m.map_class_to_signal_mass[trilepton_mumumu::lowmass] = {5, 10, 20, 30, 40, 50, 60, 70};
  m.map_class_to_signal_mass[trilepton_mumumu::class3] = {90, 100, 150, 200, 300, 400, 500, 700, 1000};
  m.map_class_to_signal_mass[trilepton_mumumu::class4] = {2000};
  m.map_class_to_signal_mass[trilepton_mumumu::highmass] = {90, 100, 150, 200, 300, 400, 500, 700, 1000};
  //m.map_class_to_signal_mass[trilepton_mumumu::class1] = {40, 60, 150, 700};
  //m.map_class_to_signal_mass[trilepton_mumumu::class2] = {40, 60, 150, 700};
  //m.map_class_to_signal_mass[trilepton_mumumu::lowmass] = {40, 60, 150, 700};
  //m.map_class_to_signal_mass[trilepton_mumumu::class3] = {40, 60, 150, 700};
  //m.map_class_to_signal_mass[trilepton_mumumu::class4] = {40, 60, 150, 700};
  //m.map_class_to_signal_mass[trilepton_mumumu::highmass] = {40, 60, 150, 700};
  
  //==== rebins
  //ofstream skeleton_rebins("./txt/skeleton_rebins.txt", ios::trunc);
  //for(unsigned int i=0; i<m.histname_suffix.size(); i++){
  //  for(unsigned int j=0; j<m.histname.size(); j++){
  //    skeleton_rebins
  //    << "  m.rebins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
  //  }
  //}
  //skeleton_rebins.close();
  //return;
  m.rebins[make_pair("_cut0", "gamma_star_mass")] = 5;
  m.rebins[make_pair("_cut0", "h_leadingLepton_Eta")] = 5;
  m.rebins[make_pair("_cut0", "h_secondLepton_Eta")] = 5;
  m.rebins[make_pair("_cut0", "h_thirdLepton_Eta")] = 5;
  m.rebins[make_pair("_cut0", "h_PFMET_phi")] = 5;
  m.rebins[make_pair("_cut0", "HN_mass_class1")] = 10;
  m.rebins[make_pair("_cut0", "HN_mass_class2")] = 10;
  m.rebins[make_pair("_cut0", "HN_mass_class3")] = 10;
  m.rebins[make_pair("_cut0", "HN_mass_class4")] = 10;
  m.rebins[make_pair("_cut0", "W_pri_highmass_mass")] = 10;
  m.rebins[make_pair("_cut0", "W_pri_lowmass_mass")] = 10;
  m.rebins[make_pair("_cut0", "W_sec_highmass_mass")] = 10;
  m.rebins[make_pair("_cut0", "z_candidate_mass")] = 5;
  m.rebins[make_pair("_cutWlow", "gamma_star_mass")] = 5;
  m.rebins[make_pair("_cutWlow", "h_leadingLepton_Eta")] = 5;
  m.rebins[make_pair("_cutWlow", "h_secondLepton_Eta")] = 5;
  m.rebins[make_pair("_cutWlow", "h_thirdLepton_Eta")] = 5;
  m.rebins[make_pair("_cutWlow", "h_PFMET_phi")] = 5;
  m.rebins[make_pair("_cutWlow", "HN_mass_class1")] = 10;
  m.rebins[make_pair("_cutWlow", "HN_mass_class2")] = 10;
  m.rebins[make_pair("_cutWlow", "W_pri_highmass_mass")] = 10;
  m.rebins[make_pair("_cutWlow", "W_pri_lowmass_mass")] = 10;
  m.rebins[make_pair("_cutWlow", "z_candidate_mass")] = 5;
  m.rebins[make_pair("_cutWhigh", "gamma_star_mass")] = 5;
  m.rebins[make_pair("_cutWhigh", "h_leadingLepton_Eta")] = 5;
  m.rebins[make_pair("_cutWhigh", "h_secondLepton_Eta")] = 5;
  m.rebins[make_pair("_cutWhigh", "h_thirdLepton_Eta")] = 5;
  m.rebins[make_pair("_cutWhigh", "h_PFMET_phi")] = 5;
  m.rebins[make_pair("_cutWhigh", "HN_mass_class3")] = 10;
  m.rebins[make_pair("_cutWhigh", "HN_mass_class4")] = 10;
  m.rebins[make_pair("_cutWhigh", "W_pri_highmass_mass")] = 10;
  m.rebins[make_pair("_cutWhigh", "W_pri_lowmass_mass")] = 10;
  m.rebins[make_pair("_cutWhigh", "W_sec_highmass_mass")] = 10;
  m.rebins[make_pair("_cutWhigh", "z_candidate_mass")] = 5;

  //==== y_maxs
  //ofstream skeleton_y_maxs("./txt/skeleton_y_maxs.txt", ios::trunc);
  //for(unsigned int i=0; i<m.histname_suffix.size(); i++){
  //  for(unsigned int j=0; j<m.histname.size(); j++){
  //    skeleton_y_maxs
  //    << "  m.y_maxs[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
  //  }
  //}
  //skeleton_y_maxs.close();
  //return;
  m.default_y_max = 1000;
  
  //==== preselection
  m.y_maxs[make_pair("_cut0", "deltaR_OS_min")] = 120;
  m.y_maxs[make_pair("_cut0", "gamma_star_mass")] = 300;
  m.y_maxs[make_pair("_cut0", "h_leadingLepton_dXY")] = 250;
  m.y_maxs[make_pair("_cut0", "h_leadingLepton_dXYSig")] = 250;
  m.y_maxs[make_pair("_cut0", "h_leadingLepton_dZ")] = 30;
  m.y_maxs[make_pair("_cut0", "h_leadingLepton_Eta")] = 250;
  m.y_maxs[make_pair("_cut0", "h_leadingLepton_GlobalChi2")] = 1500;
  m.y_maxs[make_pair("_cut0", "h_leadingLepton_LeptonRelIso")] = 1500;
  m.y_maxs[make_pair("_cut0", "h_leadingLepton_Pt")] = 300;
  m.y_maxs[make_pair("_cut0", "h_Nbjets")] = 2000;
  m.y_maxs[make_pair("_cut0", "h_Njets")] = 1000;
  m.y_maxs[make_pair("_cut0", "h_nVertices")] = 150;
  m.y_maxs[make_pair("_cut0", "h_osllmass")] = 300;
  m.y_maxs[make_pair("_cut0", "h_PFMET")] = 200;
  m.y_maxs[make_pair("_cut0", "h_PFMET_phi")] = 120;
  m.y_maxs[make_pair("_cut0", "h_secondLepton_dXY")] = 250;
  m.y_maxs[make_pair("_cut0", "h_secondLepton_dXYSig")] = 250;
  m.y_maxs[make_pair("_cut0", "h_secondLepton_dZ")] = 30;
  m.y_maxs[make_pair("_cut0", "h_secondLepton_Eta")] = 250;
  m.y_maxs[make_pair("_cut0", "h_secondLepton_GlobalChi2")] = 1500;
  m.y_maxs[make_pair("_cut0", "h_secondLepton_LeptonRelIso")] = 1500;
  m.y_maxs[make_pair("_cut0", "h_secondLepton_Pt")] = 350;
  m.y_maxs[make_pair("_cut0", "h_thirdLepton_dXY")] = 250;
  m.y_maxs[make_pair("_cut0", "h_thirdLepton_dXYSig")] = 250;
  m.y_maxs[make_pair("_cut0", "h_thirdLepton_dZ")] = 30;
  m.y_maxs[make_pair("_cut0", "h_thirdLepton_Eta")] = 250;
  m.y_maxs[make_pair("_cut0", "h_thirdLepton_GlobalChi2")] = 1500;
  m.y_maxs[make_pair("_cut0", "h_thirdLepton_LeptonRelIso")] = 1500;
  m.y_maxs[make_pair("_cut0", "h_thirdLepton_Pt")] = 800;
  m.y_maxs[make_pair("_cut0", "HN_mass_class1")] = 200;
  m.y_maxs[make_pair("_cut0", "HN_mass_class2")] = 200;
  m.y_maxs[make_pair("_cut0", "HN_mass_class3")] = 350;
  m.y_maxs[make_pair("_cut0", "HN_mass_class4")] = 350;
  m.y_maxs[make_pair("_cut0", "n_events")] = 2000;
  m.y_maxs[make_pair("_cut0", "W_pri_highmass_mass")] = 250;
  m.y_maxs[make_pair("_cut0", "W_pri_lowmass_mass")] = 200;
  m.y_maxs[make_pair("_cut0", "W_sec_highmass_mass")] = 1500;
  m.y_maxs[make_pair("_cut0", "z_candidate_mass")] = 300;
  //==== _cutWlow
  m.y_maxs[make_pair("_cutWlow", "deltaR_OS_min")] = 80;
  m.y_maxs[make_pair("_cutWlow", "gamma_star_mass")] = 200;
  m.y_maxs[make_pair("_cutWlow", "h_leadingLepton_dXY")] = 150;
  m.y_maxs[make_pair("_cutWlow", "h_leadingLepton_dXYSig")] = 150;
  m.y_maxs[make_pair("_cutWlow", "h_leadingLepton_dZ")] = 20;
  m.y_maxs[make_pair("_cutWlow", "h_leadingLepton_Eta")] = 200;
  m.y_maxs[make_pair("_cutWlow", "h_leadingLepton_GlobalChi2")] = 600;
  m.y_maxs[make_pair("_cutWlow", "h_leadingLepton_LeptonRelIso")] = 600;
  m.y_maxs[make_pair("_cutWlow", "h_leadingLepton_Pt")] = 200;
  m.y_maxs[make_pair("_cutWlow", "h_Nbjets")] = 1000;
  m.y_maxs[make_pair("_cutWlow", "h_Njets")] = 600;
  m.y_maxs[make_pair("_cutWlow", "h_nVertices")] = 80;
  m.y_maxs[make_pair("_cutWlow", "h_osllmass")] = 250;
  m.y_maxs[make_pair("_cutWlow", "h_PFMET")] = 150;
  m.y_maxs[make_pair("_cutWlow", "h_PFMET_phi")] = 100;
  m.y_maxs[make_pair("_cutWlow", "h_secondLepton_dXY")] = 150;
  m.y_maxs[make_pair("_cutWlow", "h_secondLepton_dXYSig")] = 150;
  m.y_maxs[make_pair("_cutWlow", "h_secondLepton_dZ")] = 20;
  m.y_maxs[make_pair("_cutWlow", "h_secondLepton_Eta")] = 200;
  m.y_maxs[make_pair("_cutWlow", "h_secondLepton_GlobalChi2")] = 600;
  m.y_maxs[make_pair("_cutWlow", "h_secondLepton_LeptonRelIso")] = 600;
  m.y_maxs[make_pair("_cutWlow", "h_secondLepton_Pt")] = 300;
  m.y_maxs[make_pair("_cutWlow", "h_thirdLepton_dXY")] = 150;
  m.y_maxs[make_pair("_cutWlow", "h_thirdLepton_dXYSig")] = 150;
  m.y_maxs[make_pair("_cutWlow", "h_thirdLepton_dZ")] = 20;
  m.y_maxs[make_pair("_cutWlow", "h_thirdLepton_Eta")] = 200;
  m.y_maxs[make_pair("_cutWlow", "h_thirdLepton_GlobalChi2")] = 600;
  m.y_maxs[make_pair("_cutWlow", "h_thirdLepton_LeptonRelIso")] = 600;
  m.y_maxs[make_pair("_cutWlow", "h_thirdLepton_Pt")] = 600;
  m.y_maxs[make_pair("_cutWlow", "HN_mass_class1")] = 200;
  m.y_maxs[make_pair("_cutWlow", "HN_mass_class2")] = 200;
  m.y_maxs[make_pair("_cutWlow", "n_events")] = 1000;
  m.y_maxs[make_pair("_cutWlow", "W_pri_lowmass_mass")] = 200;
  m.y_maxs[make_pair("_cutWlow", "z_candidate_mass")] = 200;
  //==== _cutWhigh
  m.y_maxs[make_pair("_cutWhigh", "deltaR_OS_min")] = 80;
  m.y_maxs[make_pair("_cutWhigh", "gamma_star_mass")] = 200;
  m.y_maxs[make_pair("_cutWhigh", "h_leadingLepton_dXY")] = 150;
  m.y_maxs[make_pair("_cutWhigh", "h_leadingLepton_dXYSig")] = 150;
  m.y_maxs[make_pair("_cutWhigh", "h_leadingLepton_dZ")] = 20;
  m.y_maxs[make_pair("_cutWhigh", "h_leadingLepton_Eta")] = 200;
  m.y_maxs[make_pair("_cutWhigh", "h_leadingLepton_GlobalChi2")] = 600;
  m.y_maxs[make_pair("_cutWhigh", "h_leadingLepton_LeptonRelIso")] = 800;
  m.y_maxs[make_pair("_cutWhigh", "h_leadingLepton_Pt")] = 150;
  m.y_maxs[make_pair("_cutWhigh", "h_Nbjets")] = 1500;
  m.y_maxs[make_pair("_cutWhigh", "h_Njets")] = 1000;
  m.y_maxs[make_pair("_cutWhigh", "h_nVertices")] = 80;
  m.y_maxs[make_pair("_cutWhigh", "h_osllmass")] = 200;
  m.y_maxs[make_pair("_cutWhigh", "h_PFMET")] = 200;
  m.y_maxs[make_pair("_cutWhigh", "h_PFMET_phi")] = 100;
  m.y_maxs[make_pair("_cutWhigh", "h_secondLepton_dXY")] = 150;
  m.y_maxs[make_pair("_cutWhigh", "h_secondLepton_dXYSig")] = 150;
  m.y_maxs[make_pair("_cutWhigh", "h_secondLepton_dZ")] = 20;
  m.y_maxs[make_pair("_cutWhigh", "h_secondLepton_Eta")] = 200;
  m.y_maxs[make_pair("_cutWhigh", "h_secondLepton_GlobalChi2")] = 600;
  m.y_maxs[make_pair("_cutWhigh", "h_secondLepton_LeptonRelIso")] = 800;
  m.y_maxs[make_pair("_cutWhigh", "h_secondLepton_Pt")] = 250;
  m.y_maxs[make_pair("_cutWhigh", "h_thirdLepton_dXY")] = 150;
  m.y_maxs[make_pair("_cutWhigh", "h_thirdLepton_dXYSig")] = 150;
  m.y_maxs[make_pair("_cutWhigh", "h_thirdLepton_dZ")] = 20;
  m.y_maxs[make_pair("_cutWhigh", "h_thirdLepton_Eta")] = 200;
  m.y_maxs[make_pair("_cutWhigh", "h_thirdLepton_GlobalChi2")] = 600;
  m.y_maxs[make_pair("_cutWhigh", "h_thirdLepton_LeptonRelIso")] = 800;
  m.y_maxs[make_pair("_cutWhigh", "h_thirdLepton_Pt")] = 800;
  m.y_maxs[make_pair("_cutWhigh", "HN_mass_class3")] = 300;
  m.y_maxs[make_pair("_cutWhigh", "HN_mass_class4")] = 250;
  m.y_maxs[make_pair("_cutWhigh", "n_events")] = 1500;
  m.y_maxs[make_pair("_cutWhigh", "W_pri_highmass_mass")] = 200;
  m.y_maxs[make_pair("_cutWhigh", "W_sec_highmass_mass")] = 1000;
  m.y_maxs[make_pair("_cutWhigh", "z_candidate_mass")] = 200;
  
  //==== x_mins
  //ofstream skeleton_x_mins("./txt/skeleton_x_mins.txt", ios::trunc);
  //for(unsigned int i=0; i<m.histname_suffix.size(); i++){
  //  for(unsigned int j=0; j<m.histname.size(); j++){
  //    skeleton_x_mins
  //    << "  m.x_mins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
  //  }
  //}
  //skeleton_x_mins.close();
  //return;
  m.x_maxs[make_pair("_cut0", "h_leadingLepton_dXY")] = 0.01;
  m.x_maxs[make_pair("_cut0", "h_secondLepton_dXY")] = 0.01;
  m.x_maxs[make_pair("_cut0", "h_thirdLepton_dXY")] = 0.01;
  m.x_maxs[make_pair("_cut0", "h_leadingLepton_dZ")] = 0.05;
  m.x_maxs[make_pair("_cut0", "h_secondLepton_dZ")] = 0.05;
  m.x_maxs[make_pair("_cut0", "h_thirdLepton_dZ")] = 0.05;
  m.x_maxs[make_pair("_cut0", "h_leadingLepton_GlobalChi2")] = 10;
  m.x_maxs[make_pair("_cut0", "h_secondLepton_GlobalChi2")] = 10;
  m.x_maxs[make_pair("_cut0", "h_thirdLepton_GlobalChi2")] = 10;
  m.x_maxs[make_pair("_cut0", "h_leadingLepton_LeptonRelIso")] = 0.1;
  m.x_maxs[make_pair("_cut0", "h_secondLepton_LeptonRelIso")] = 0.1;
  m.x_maxs[make_pair("_cut0", "h_thirdLepton_LeptonRelIso")] = 0.1;
  m.x_maxs[make_pair("_cut0", "h_osllmass")] = 150;
  m.x_maxs[make_pair("_cut0", "h_PFMET")] = 150;
  m.x_maxs[make_pair("_cut0", "HN_mass_class1")] = 600;
  m.x_maxs[make_pair("_cut0", "HN_mass_class2")] = 600;
  m.x_maxs[make_pair("_cut0", "HN_mass_class3")] = 1000;
  m.x_maxs[make_pair("_cut0", "HN_mass_class4")] = 300;
  m.x_maxs[make_pair("_cut0", "W_pri_lowmass_mass")] = 1000;
  m.x_maxs[make_pair("_cut0", "W_sec_highmass_mass")] = 500;
  m.x_maxs[make_pair("_cutWlow", "h_leadingLepton_dXY")] = 0.01;
  m.x_maxs[make_pair("_cutWlow", "h_secondLepton_dXY")] = 0.01;
  m.x_maxs[make_pair("_cutWlow", "h_thirdLepton_dXY")] = 0.01;
  m.x_maxs[make_pair("_cutWlow", "h_leadingLepton_dZ")] = 0.05;
  m.x_maxs[make_pair("_cutWlow", "h_secondLepton_dZ")] = 0.05;
  m.x_maxs[make_pair("_cutWlow", "h_thirdLepton_dZ")] = 0.05;
  m.x_maxs[make_pair("_cutWlow", "h_leadingLepton_GlobalChi2")] = 10;
  m.x_maxs[make_pair("_cutWlow", "h_secondLepton_GlobalChi2")] = 10;
  m.x_maxs[make_pair("_cutWlow", "h_thirdLepton_GlobalChi2")] = 10;
  m.x_maxs[make_pair("_cutWlow", "h_leadingLepton_LeptonRelIso")] = 0.1;
  m.x_maxs[make_pair("_cutWlow", "h_secondLepton_LeptonRelIso")] = 0.1;
  m.x_maxs[make_pair("_cutWlow", "h_thirdLepton_LeptonRelIso")] = 0.1;
  m.x_maxs[make_pair("_cutWlow", "h_leadingLepton_Pt")] = 100;
  m.x_maxs[make_pair("_cutWlow", "h_secondLepton_Pt")] = 100;
  m.x_maxs[make_pair("_cutWlow", "h_thirdLepton_Pt")] = 100;
  m.x_maxs[make_pair("_cutWlow", "h_PFMET")] = 150;
  m.x_maxs[make_pair("_cutWlow", "h_osllmass")] = 150;
  m.x_maxs[make_pair("_cutWlow", "W_pri_lowmass_mass")] = 400;
  m.x_maxs[make_pair("_cutWlow", "HN_mass_class1")] = 200;
  m.x_maxs[make_pair("_cutWlow", "HN_mass_class2")] = 200;
  m.x_maxs[make_pair("_cutWhigh", "h_leadingLepton_dXY")] = 0.01;
  m.x_maxs[make_pair("_cutWhigh", "h_secondLepton_dXY")] = 0.01;
  m.x_maxs[make_pair("_cutWhigh", "h_thirdLepton_dXY")] = 0.01;
  m.x_maxs[make_pair("_cutWhigh", "h_leadingLepton_dZ")] = 0.05;
  m.x_maxs[make_pair("_cutWhigh", "h_secondLepton_dZ")] = 0.05;
  m.x_maxs[make_pair("_cutWhigh", "h_thirdLepton_dZ")] = 0.05;
  m.x_maxs[make_pair("_cutWhigh", "h_leadingLepton_GlobalChi2")] = 10;
  m.x_maxs[make_pair("_cutWhigh", "h_secondLepton_GlobalChi2")] = 10;
  m.x_maxs[make_pair("_cutWhigh", "h_thirdLepton_GlobalChi2")] = 10;
  m.x_maxs[make_pair("_cutWhigh", "h_leadingLepton_LeptonRelIso")] = 0.1;
  m.x_maxs[make_pair("_cutWhigh", "h_secondLepton_LeptonRelIso")] = 0.1;
  m.x_maxs[make_pair("_cutWhigh", "h_thirdLepton_LeptonRelIso")] = 0.1;
  m.x_maxs[make_pair("_cutWhigh", "h_thirdLepton_Pt")] = 100;
  m.x_maxs[make_pair("_cutWhigh", "h_PFMET")] = 200;
  m.x_maxs[make_pair("_cutWhigh", "HN_mass_class3")] = 1000;
  m.x_maxs[make_pair("_cutWhigh", "HN_mass_class4")] = 1000;
  m.x_maxs[make_pair("_cutWhigh", "W_sec_highmass_mass")] = 200;

  //==== k-factor
  m.k_factor = 1.;
  
  //==== mixing at generation level
  m.log_of_generation_mixing = -2.;
  
  //==== prepare plot directories
  m.make_plot_directory();
  
  //==== declare output rootfiles
  m.outputfile = new TFile(m.plotpath+"/hists.root", "RECREATE");
  
  //==== finally, draw plots
  m.draw_hist();

}






