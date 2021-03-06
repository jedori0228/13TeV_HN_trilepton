#include "trilepton_mumumu.cc"
#include <fstream>

void run_trilepton_mumumu_CR_2016(int XXX){
  
  //==== decalre class
  trilepton_mumumu m;
  
  //==== set data class
  m.data_class =
  "v8-0-2.9/CR";
  
  //==== set prefixes and suffixes
  m.filename_prefix = "trilepton_mumumu_CR";
  m.filename_suffix =
  "_cat_v8-0-2.root";
  
  //==== set sample mapping
  m.map_sample_string_to_list["DY"] = {"DYJets_10to50", "DYJets"};
  m.map_sample_string_to_list["WJets"] = {"WJets"};
  m.map_sample_string_to_list["VV_excl"] = {"WZTo3LNu_powheg", "ZZTo4L_powheg"};
  m.map_sample_string_to_list["VV_incl"] = {"WZ", "WW", "ZZ"};
  m.map_sample_string_to_list["WZ_excl"] = {"WZTo3LNu_powheg"};
  m.map_sample_string_to_list["ZZ_excl"] = {"ZZTo4L_powheg"};
  m.map_sample_string_to_list["VVV"] = {"WWW", "WWZ", "WZZ", "ZZZ"};
  m.map_sample_string_to_list["ttbar"] = {"TTJets_aMC"};
  m.map_sample_string_to_list["ttV"] = {"ttW", "ttZ"};
  m.map_sample_string_to_list["ttH"] = {"ttH_nonbb"};
  m.map_sample_string_to_list["top"] = {"TTJets_aMC", "ttW", "ttZ", "ttH_nonbb"};
  m.map_sample_string_to_list["Wgamma"] = {"WGtoLNuG", "WGtoLNuMM"};
  m.map_sample_string_to_list["Zgamma"] = {"ZGto2LG"};
  m.map_sample_string_to_list["fake_HighdXY"] = {"fake_HighdXY"};
  m.map_sample_string_to_list["fake_sfed_HighdXY"] = {"fake_sfed_HighdXY"};
  m.map_sample_string_to_list["fake_pt_sfed_HighdXY"] = {"fake_pt_sfed_HighdXY"};
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
  m.map_sample_string_to_legendinfo["fake_HighdXY"] = make_pair("Misd", kAzure+10);
  m.map_sample_string_to_legendinfo["fake_sfed_HighdXY"] = make_pair("Misd", kAzure+10);
  m.map_sample_string_to_legendinfo["fake_pt_sfed_HighdXY"] = make_pair("Misd", kAzure+10);
  m.map_sample_string_to_legendinfo["fake_DiMuon_HighdXY"] = make_pair("Misd", kAzure+10);
  
  //==== set and make sample list
  if(XXX==0){
    m.histname_suffix = {"_WZ", "_ZJets"};
    m.drawdata = {true, true};
    m.samples_to_use = {"fake_HighdXY", "WZ_excl", "ZZ_excl", "VVV", "ttV", "ttH"};
  }
  if(XXX==1){
    m.histname_suffix = {"_WZ", "_ZJets"};
    m.drawdata = {true, true};
    m.samples_to_use = {"fake_sfed_HighdXY", "WZ_excl", "ZZ_excl", "Wgamma", "VVV", "top"};
  }
  if(XXX==2){
    m.histname_suffix = {"_WZ", "_ZJets"};
    m.drawdata = {true, true};
    m.samples_to_use = {"fake_pt_sfed_HighdXY", "WZ_excl", "ZZ_excl", "Wgamma", "VVV", "ttV", "ttH"};
  }
  
  m.make_bkglist();

  
  //==== set variables to draw
  m.histname = {
    "leadingLepton_Pt",
    "leadingLepton_Eta",
    "leadingLepton_RelIso",
    "leadingLepton_Chi2",
    "secondLepton_Pt",
    "secondLepton_Eta",
    "secondLepton_RelIso",
    "secondLepton_Chi2",
    "thirdLepton_Pt",
    "thirdLepton_Eta",
    "thirdLepton_RelIso",
    "thirdLepton_Chi2",
    "mll",
    "n_events",
    "PFMET",
    "n_bjets",
    "m_Z_candidate_before_cut",
    "m_lll_before_cut",
    "PFMET_before_cut",
    "n_electrons_before_cut",
    "n_bjets_before_cut",
    "m_Z_candidate",
    "mt_W_candidate",
    "osllmass",
    "m_lll",
    "Z_candidate_Pt",
    "W_candidate_Pt",
    "dRZMuonWMuon",
    "N1_Z_mass",
    "N1_mlll",
    "N1_n_bjets",
    "N1_PFMET",
    "n_vertices_before_cut",
    "n_vertices",
    "n_jets",
    "ZMuon_leading_Pt",
    "ZMuon_leading_Eta",
    "ZMuon_leading_RelIso",
    "ZMuon_leading_Chi2",
    "ZMuon_leading_dXY",
    "ZMuon_subleading_Pt",
    "ZMuon_subleading_Eta",
    "ZMuon_subleading_RelIso",
    "ZMuon_subleading_Chi2",
    "ZMuon_subleading_dXY",
    "WMuon_Pt",
    "WMuon_Eta",
    "WMuon_RelIso",
    "WMuon_Chi2",
    "WMuon_dXY",
    "dRMETWMuon",
  };

  m.x_title = {
    "p_{T} [GeV]",
    "|#eta|",
    "LeptonRelIso04",
    "GlobalChi2",
    "p_{T} [GeV]",
    "|#eta|",
    "LeptonRelIso04",
    "GlobalChi2",
    "p_{T} [GeV]",
    "|#eta|",
    "LeptonRelIso04",
    "GlobalChi2",
    "m(#mu#mu) [GeV]",
    "onebin",
    "PFMET [GeV]",
    "# of b-tagged jets",
    "m(Z) [GeV]",
    "m(lll) [GeV]",
    "PFMET [GeV]",
    "# of electrons",
    "# of bjets",
    "m(Z) [GeV]",
    "m_{T}(W) [GeV]",
    "m(OS) [GeV]",
    "m(lll) [GeV]",
    "p_{T}^{Z} [GeV]",
    "p_{T}^{W} [GeV]",
    "#DeltaR(#mu^{W Lepton}#mu^{Z Leptons})",
    "|m(ll)-m(Z)| [GeV]",
    "m(lll) [GeV]",
    "# of bjets",
    "PFMET [GeV]",
    "# of vertices",
    "# of vertices",
    "n_jets",
    "p_{T} [GeV]",
    "|#eta|",
    "LeptonRelIso04",
    "GlobalChi2",
    "|dXY| [cm]",
    "p_{T} [GeV]",
    "|#eta|",
    "LeptonRelIso04",
    "GlobalChi2",
    "|dXY| [cm]",
    "p_{T} [GeV]",
    "|#eta|",
    "LeptonRelIso04",
    "GlobalChi2",
    "|dXY| [cm]",
    "#DeltaR(MET,#mu^{W Lepton})"
  };

  //==== cut-var skips
  for(unsigned int i=0; i<m.histname.size(); i++){
    TString this_var = m.histname.at(i);
    if(this_var.Contains("third") ||
       this_var == "m_Z_candidate_before_cut" ||
       this_var == "m_lll_before_cut" ||
       this_var == "PFMET_before_cut" ||
       this_var == "n_electrons_before_cut" ||
       this_var == "n_bjets_before_cut" ||
       this_var == "m_Z_candidate" ||
       this_var == "mt_W_candidate" ||
       this_var == "osllmass" ||
       this_var == "m_lll" ||
       this_var == "WMuon_Pt" ||
       this_var == "Z_candidate_Pt" ||
       this_var == "W_candidate_Pt" ||
       this_var == "dRZMuonWMuon" ||
       this_var == "N1_Z_mass" ||
       this_var == "N1_mlll" ||
       this_var == "N1_n_bjets" ||
       this_var == "N1_PFMET" ||
       this_var == "n_vertices_before_cut" ||
       this_var == "n_vertices" ||
       this_var.Contains("ZMuon") ||
       this_var.Contains("WMuon") ||
       this_var == "dRMETWMuon"
      ){
      if(this_var.Contains("before_cut") || this_var.Contains("N1")){
        m.CutVarSkips.push_back( make_pair("_ZJets", this_var) );
      }
    }
  }
  m.CutVarSkips.push_back( make_pair("_WZ", "mll") );
  m.CutVarSkips.push_back( make_pair("_ZJets", "mll") );
 
  //==== set signal mass points
  //m.signal_mass = {40, 60, 150, 700};
  m.signal_color = {kRed, kBlue, kMagenta-7, kBlack};
  
  //==== set signal coupling constants
  m.coupling_constants[make_pair("_WZ", 40)] = 0.01;
  m.coupling_constants[make_pair("_WZ", 60)] = 0.01;
  m.coupling_constants[make_pair("_WZ", 150)] = 1;
  m.coupling_constants[make_pair("_WZ", 700)] = 0.01;
  m.coupling_constants[make_pair("_ZJets", 40)] = 0.001;
  m.coupling_constants[make_pair("_ZJets", 60)] = 0.001;
  m.coupling_constants[make_pair("_ZJets", 150)] = 0.001;
  m.coupling_constants[make_pair("_ZJets", 700)] = 0.001;
  
  //==== set signal mass for each class
  m.map_class_to_signal_mass[trilepton_mumumu::class1] = {40};
  m.map_class_to_signal_mass[trilepton_mumumu::class2] = {60};
  m.map_class_to_signal_mass[trilepton_mumumu::lowmass] = {40, 60};
  m.map_class_to_signal_mass[trilepton_mumumu::class3] = {150};
  m.map_class_to_signal_mass[trilepton_mumumu::class4] = {700};
  m.map_class_to_signal_mass[trilepton_mumumu::highmass] = {150, 700};
  
  //==== rebins
  //ofstream skeleton_rebins("./txt/CR_skeleton_rebins.txt", ios::trunc);
  //for(unsigned int i=0; i<m.histname_suffix.size(); i++){
  //  for(unsigned int j=0; j<m.histname.size(); j++){
  //    skeleton_rebins
  //    << "m.rebins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
  //  }
  //}
  //skeleton_rebins.close();
  //return;
  m.rebins[make_pair("_WZ", "leadingLepton_Pt")] = 5;
  m.rebins[make_pair("_WZ", "leadingLepton_Eta")] = 5;
  m.rebins[make_pair("_WZ", "secondLepton_Pt")] = 5;
  m.rebins[make_pair("_WZ", "secondLepton_Eta")] = 5;
  m.rebins[make_pair("_WZ", "thirdLepton_Pt")] = 5;
  m.rebins[make_pair("_WZ", "thirdLepton_Eta")] = 5;
  m.rebins[make_pair("_WZ", "PFMET")] = 10;
  m.rebins[make_pair("_WZ", "m_Z_candidate_before_cut")] = 5;
  m.rebins[make_pair("_WZ", "m_lll_before_cut")] = 20;
  m.rebins[make_pair("_WZ", "PFMET_before_cut")] = 10;
  m.rebins[make_pair("_WZ", "m_Z_candidate")] = 5;
  m.rebins[make_pair("_WZ", "mt_W_candidate")] = 10;
  m.rebins[make_pair("_WZ", "osllmass")] = 5;
  m.rebins[make_pair("_WZ", "m_lll")] = 20;
  m.rebins[make_pair("_WZ", "Z_candidate_Pt")] = 20;
  m.rebins[make_pair("_WZ", "W_candidate_Pt")] = 20;
  m.rebins[make_pair("_WZ", "ZMuon_leading_Pt")] = 5;
  m.rebins[make_pair("_WZ", "ZMuon_leading_Eta")] = 5;
  m.rebins[make_pair("_WZ", "ZMuon_subleading_Pt")] = 5;
  m.rebins[make_pair("_WZ", "ZMuon_subleading_Eta")] = 5;
  m.rebins[make_pair("_WZ", "WMuon_Pt")] = 5;
  m.rebins[make_pair("_WZ", "WMuon_Eta")] = 5;
  m.rebins[make_pair("_WZ", "dRMETWMuon")] = 5;
  m.rebins[make_pair("_WZ", "dRZMuonWMuon")] = 5;
  m.rebins[make_pair("_ZJets", "leadingLepton_Pt")] = 5;
  m.rebins[make_pair("_ZJets", "leadingLepton_Eta")] = 5;
  m.rebins[make_pair("_ZJets", "secondLepton_Pt")] = 5;
  m.rebins[make_pair("_ZJets", "secondLepton_Eta")] = 5;
  m.rebins[make_pair("_ZJets", "thirdLepton_Pt")] = 5;
  m.rebins[make_pair("_ZJets", "thirdLepton_Eta")] = 5;
  m.rebins[make_pair("_ZJets", "PFMET")] = 10;
  m.rebins[make_pair("_ZJets", "m_Z_candidate_before_cut")] = 5;
  m.rebins[make_pair("_ZJets", "m_Z_candidate")] = 5;
  m.rebins[make_pair("_ZJets", "mt_W_candidate")] = 10;
  m.rebins[make_pair("_ZJets", "osllmass")] = 5;
  m.rebins[make_pair("_ZJets", "m_lll")] = 20;
  m.rebins[make_pair("_ZJets", "Z_candidate_Pt")] = 20;
  m.rebins[make_pair("_ZJets", "W_candidate_Pt")] = 20;
  m.rebins[make_pair("_ZJets", "Z_candidate_Pt")] = 20;
  m.rebins[make_pair("_ZJets", "W_candidate_Pt")] = 20;
  m.rebins[make_pair("_ZJets", "ZMuon_leading_Pt")] = 5;
  m.rebins[make_pair("_ZJets", "ZMuon_leading_Eta")] = 5;
  m.rebins[make_pair("_ZJets", "ZMuon_subleading_Pt")] = 5;
  m.rebins[make_pair("_ZJets", "ZMuon_subleading_Eta")] = 5;
  m.rebins[make_pair("_ZJets", "WMuon_Pt")] = 5;
  m.rebins[make_pair("_ZJets", "WMuon_Eta")] = 5;
  m.rebins[make_pair("_ZJets", "dRMETWMuon")] = 5;
  m.rebins[make_pair("_ZJets", "dRZMuonWMuon")] = 5;


  //==== y_maxs
  //ofstream skeleton_y_maxs("./txt/CR_skeleton_y_maxs.txt", ios::trunc);
  //for(unsigned int i=0; i<m.histname_suffix.size(); i++){
  //  for(unsigned int j=0; j<m.histname.size(); j++){
  //    skeleton_y_maxs
  //    << "  m.y_maxs[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
  //  }
  //}
  //skeleton_y_maxs.close();
  //return;
  m.default_y_max = 50;
  m.y_maxs[make_pair("_WZ", "dRMETWMuon")] = 300;
  m.y_maxs[make_pair("_WZ", "dRZMuonWMuon")] = 600;
  m.y_maxs[make_pair("_WZ", "leadingLepton_Chi2")] = 200;
  m.y_maxs[make_pair("_WZ", "leadingLepton_Eta")] = 300;
  m.y_maxs[make_pair("_WZ", "leadingLepton_Pt")] = 150;
  m.y_maxs[make_pair("_WZ", "leadingLepton_RelIso")] = 500;
  m.y_maxs[make_pair("_WZ", "m_lll_before_cut")] = 700;
  m.y_maxs[make_pair("_WZ", "m_lll")] = 250;
  m.y_maxs[make_pair("_WZ", "m_Z_candidate_before_cut")] = 1500;
  m.y_maxs[make_pair("_WZ", "m_Z_candidate")] = 800;
  m.y_maxs[make_pair("_WZ", "mt_W_candidate")] = 250;
  m.y_maxs[make_pair("_WZ", "n_bjets_before_cut")] = 5000;
  m.y_maxs[make_pair("_WZ", "n_bjets")] = 2000;
  m.y_maxs[make_pair("_WZ", "n_electrons_before_cut")] = 2000;
  m.y_maxs[make_pair("_WZ", "n_events")] = 2000;
  m.y_maxs[make_pair("_WZ", "n_jets")] = 500;
  m.y_maxs[make_pair("_WZ", "n_vertices_before_cut")] = 350;
  m.y_maxs[make_pair("_WZ", "n_vertices")] = 100;
  m.y_maxs[make_pair("_WZ", "N1_mlll")] = 250;
  m.y_maxs[make_pair("_WZ", "N1_n_bjets")] = 1500;
  m.y_maxs[make_pair("_WZ", "N1_PFMET")] = 300;
  m.y_maxs[make_pair("_WZ", "N1_Z_mass")] = 350;
  m.y_maxs[make_pair("_WZ", "osllmass")] = 800;
  m.y_maxs[make_pair("_WZ", "PFMET_before_cut")] = 1000;
  m.y_maxs[make_pair("_WZ", "PFMET")] = 350;
  m.y_maxs[make_pair("_WZ", "secondLepton_Chi2")] = 200;
  m.y_maxs[make_pair("_WZ", "secondLepton_Eta")] = 300;
  m.y_maxs[make_pair("_WZ", "secondLepton_Pt")] = 200;
  m.y_maxs[make_pair("_WZ", "secondLepton_RelIso")] = 500;
  m.y_maxs[make_pair("_WZ", "thirdLepton_Chi2")] = 200;
  m.y_maxs[make_pair("_WZ", "thirdLepton_Eta")] = 300;
  m.y_maxs[make_pair("_WZ", "thirdLepton_Pt")] = 300;
  m.y_maxs[make_pair("_WZ", "thirdLepton_RelIso")] = 500;
  m.y_maxs[make_pair("_WZ", "W_candidate_Pt")] = 250;
  m.y_maxs[make_pair("_WZ", "WMuon_Chi2")] = 200;
  m.y_maxs[make_pair("_WZ", "WMuon_dXY")] = 150;
  m.y_maxs[make_pair("_WZ", "WMuon_Eta")] = 300;
  m.y_maxs[make_pair("_WZ", "WMuon_Pt")] = 150;
  m.y_maxs[make_pair("_WZ", "WMuon_RelIso")] = 500;
  m.y_maxs[make_pair("_WZ", "Z_candidate_Pt")] = 250;
  m.y_maxs[make_pair("_WZ", "ZMuon_leading_Chi2")] = 200;
  m.y_maxs[make_pair("_WZ", "ZMuon_leading_dXY")] = 200;
  m.y_maxs[make_pair("_WZ", "ZMuon_leading_Eta")] = 200;
  m.y_maxs[make_pair("_WZ", "ZMuon_leading_Pt")] = 150;
  m.y_maxs[make_pair("_WZ", "ZMuon_leading_RelIso")] = 500;
  m.y_maxs[make_pair("_WZ", "ZMuon_subleading_Chi2")] = 200;
  m.y_maxs[make_pair("_WZ", "ZMuon_subleading_dXY")] = 200;
  m.y_maxs[make_pair("_WZ", "ZMuon_subleading_Eta")] = 200;
  m.y_maxs[make_pair("_WZ", "ZMuon_subleading_Pt")] = 200;
  m.y_maxs[make_pair("_WZ", "ZMuon_subleading_RelIso")] = 500;
  m.y_maxs[make_pair("_ZJets", "dRMETWMuon")] = 150;
  m.y_maxs[make_pair("_ZJets", "dRZMuonWMuon")] = 300;
  m.y_maxs[make_pair("_ZJets", "leadingLepton_Chi2")] = 100;
  m.y_maxs[make_pair("_ZJets", "leadingLepton_Eta")] = 150;
  m.y_maxs[make_pair("_ZJets", "leadingLepton_Pt")] = 100;
  m.y_maxs[make_pair("_ZJets", "leadingLepton_RelIso")] = 500;
  m.y_maxs[make_pair("_ZJets", "m_lll")] = 150;
  m.y_maxs[make_pair("_ZJets", "m_Z_candidate")] = 250;
  m.y_maxs[make_pair("_ZJets", "mt_W_candidate")] = 150;
  m.y_maxs[make_pair("_ZJets", "n_bjets")] = 500;
  m.y_maxs[make_pair("_ZJets", "n_events")] = 500;
  m.y_maxs[make_pair("_ZJets", "n_jets")] = 300;
  m.y_maxs[make_pair("_ZJets", "n_vertices")] = 50;
  m.y_maxs[make_pair("_ZJets", "osllmass")] = 250;
  m.y_maxs[make_pair("_ZJets", "PFMET")] = 400;
  m.y_maxs[make_pair("_ZJets", "secondLepton_Chi2")] = 100;
  m.y_maxs[make_pair("_ZJets", "secondLepton_Eta")] = 150;
  m.y_maxs[make_pair("_ZJets", "secondLepton_Pt")] = 150;
  m.y_maxs[make_pair("_ZJets", "secondLepton_RelIso")] = 500;
  m.y_maxs[make_pair("_ZJets", "thirdLepton_Chi2")] = 100;
  m.y_maxs[make_pair("_ZJets", "thirdLepton_Eta")] = 150;
  m.y_maxs[make_pair("_ZJets", "thirdLepton_Pt")] = 200;
  m.y_maxs[make_pair("_ZJets", "thirdLepton_RelIso")] = 500;
  m.y_maxs[make_pair("_ZJets", "W_candidate_Pt")] = 200;
  m.y_maxs[make_pair("_ZJets", "WMuon_Chi2")] = 100;
  m.y_maxs[make_pair("_ZJets", "WMuon_dXY")] = 60;
  m.y_maxs[make_pair("_ZJets", "WMuon_Eta")] = 150;
  m.y_maxs[make_pair("_ZJets", "WMuon_Pt")] = 200;
  m.y_maxs[make_pair("_ZJets", "WMuon_RelIso")] = 500;
  m.y_maxs[make_pair("_ZJets", "Z_candidate_Pt")] = 200;
  m.y_maxs[make_pair("_ZJets", "ZMuon_leading_Chi2")] = 100;
  m.y_maxs[make_pair("_ZJets", "ZMuon_leading_dXY")] = 60;
  m.y_maxs[make_pair("_ZJets", "ZMuon_leading_Eta")] = 150;
  m.y_maxs[make_pair("_ZJets", "ZMuon_leading_Pt")] = 100;
  m.y_maxs[make_pair("_ZJets", "ZMuon_leading_RelIso")] = 500;
  m.y_maxs[make_pair("_ZJets", "ZMuon_subleading_Chi2")] = 100;
  m.y_maxs[make_pair("_ZJets", "ZMuon_subleading_dXY")] = 60;
  m.y_maxs[make_pair("_ZJets", "ZMuon_subleading_Eta")] = 150;
  m.y_maxs[make_pair("_ZJets", "ZMuon_subleading_Pt")] = 100;
  m.y_maxs[make_pair("_ZJets", "ZMuon_subleading_RelIso")] = 500;

  //==== x_mins
  //ofstream skeleton_x_mins("./txt/CR_skeleton_x_mins.txt", ios::trunc);
  //for(unsigned int i=0; i<m.histname_suffix.size(); i++){
  //  for(unsigned int j=0; j<m.histname.size(); j++){
  //    skeleton_x_mins
  //    << "  m.x_mins[make_pair(\""+m.histname_suffix.at(i)+"\", \""+m.histname.at(j)+"\")] = 1;" << endl;
  //  }
  //}
  //skeleton_x_mins.close();
  //return;
  m.x_maxs[make_pair("_WZ", "mll")] = 200.;
  m.x_maxs[make_pair("_WZ", "PFMET_before_cut")] = 200.;
  m.x_maxs[make_pair("_WZ", "PFMET")] = 200.;
  m.x_maxs[make_pair("_WZ", "osllmass")] = 150.;
  m.x_maxs[make_pair("_ZJets", "mll")] = 200.;
  m.x_maxs[make_pair("_ZJets", "PFMET_before_cut")] = 200.;
  m.x_maxs[make_pair("_ZJets", "PFMET")] = 200.;
  m.x_maxs[make_pair("_ZJets", "osllmass")] = 150.;
  
  //==== k-factor
  m.k_factor = 1.34;
  
  //==== mixing at generation level
  m.log_of_generation_mixing = -2.;
  
  //==== prepare plot directories
  m.make_plot_directory();
  
  //==== declare output rootfiles
  m.outputfile = new TFile(m.plotpath+"/hists.root", "RECREATE");
  
  //==== draw in logy
  m.UseSetLogy = false;
  
  //==== finally, draw plots
  m.draw_hist();
  
}






