#include "trilepton_mumumu.cc"
#include <fstream>

void run_trilepton_mumumu_CR(int XXX, TString thisfilepath="", bool ScaleMC=false){
  
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
  
  m.data_class = dataset+"/CR_BVeto/FR_alljet/";
  if(thisfilepath!=""){
    m.data_class = dataset+thisfilepath;
  }
  
  //================================
  //==== set prefixes and suffixes
  //================================
  
  m.filename_prefix = "trilepton_mumumu_CR";
  m.filename_suffix = "_cat_"+catversion+".root";
  
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
  m.map_sample_string_to_list["fake_Dijet"] = {"fake_Dijet"};
  
  m.map_sample_string_to_legendinfo["DY"] = make_pair("DY", kYellow+2);
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
  m.map_sample_string_to_legendinfo["fake_Dijet"] = make_pair("Misd", kAzure+10);
  
  //===============================
  //==== set and make sample list
  //===============================

  //==== trilep CR
  if(XXX==0){
    m.histname_suffix = {
      "_WZ", "_WZ_3mu0el", "_WZ_2mu1el",
      "_ZJets", "_ZJets_3mu0el", "_ZJets_2mu1el",
      "_ZLep",
      "_ZGamma", "_ZGamma_3mu0el", "_ZGamma_2mu1el",
      "_ZZ", "_ZZ_4mu0el"
    };
    m.drawdata = {
      true, true, true,
      true, true, true,
      true,
      true, true, true,
      true, true
    };

    if(ScaleMC){
      m.ApplyMCNormSF = {
        true, true, true,
        true, true, true,
        true,
        true, true, true,
        true, true
      };
    }
    else{
      m.ApplyMCNormSF = {
        false, false, false,
        false, false, false,
        false,
        false, false, false,
        false, false
      };
    }

    m.samples_to_use = {"fake_sfed_HighdXY", "WZ_excl", "ZZ_excl", "Zgamma", "VVV", "ttV"};
  }
  else if(XXX==1){
    m.histname_suffix = {"_WZ", "_ZJets"};
    m.drawdata = {true, true};
    m.samples_to_use = {"fake_Dijet", "WZ_excl", "ZZ_excl", "Zgamma", "VVV", "ttV"};
    //m.samples_to_use = {"fake_sfed_HighdXY", "VV_incl", "VVV", "top"};
  }
  
  //============================
  //==== set variables to draw
  //============================
  
  m.histname = {
    "n_events",
    "n_vertices",
    "n_jets",
    "n_bjets",
    "n_jets_nearby",
    "n_bjets_nearby",
    "PFMET", "PFMET_phi",
    "osllmass",
    "m_Z_candidate",
    "mt_W_candidate",
    "m_lll",
    "Z_candidate_Pt",
    "W_candidate_Pt",
    "dRZLeptonWLepton",
    "dRMETWLepton",
    "dRNearByJetWLepton",
    "dRNearByBJetWLepton",
    "leadingLepton_Pt", "leadingLepton_Eta", "leadingLepton_RelIso", "leadingLepton_dXY", "leadingLepton_dXYSig", "leadingLepton_dZ",
    "secondLepton_Pt", "secondLepton_Eta", "secondLepton_RelIso", "secondLepton_dXY", "secondLepton_dXYSig", "secondLepton_dZ",
    "thirdLepton_Pt", "thirdLepton_Eta", "thirdLepton_RelIso", "thirdLepton_dXY", "thirdLepton_dXYSig", "thirdLepton_dZ",
    
    //==== only for ZZ
    "fourthLepton_Pt", "fourthLepton_Eta", "fourthLepton_RelIso", "fourthLepton_dXY", "fourthLepton_dXYSig", "fourthLepton_dZ",
    "m_llll",
    
    "ZLepton_leading_Pt", "ZLepton_leading_Eta", "ZLepton_leading_RelIso", "ZLepton_leading_dXY", "ZLepton_leading_dXYSig", "ZLepton_leading_dZ",
    "ZLepton_subleading_Pt", "ZLepton_subleading_Eta", "ZLepton_subleading_RelIso", "ZLepton_subleading_dXY", "ZLepton_subleading_dXYSig", "ZLepton_subleading_dZ",
    "WLepton_Pt", "WLepton_Eta", "WLepton_RelIso", "WLepton_dXY", "WLepton_dXYSig", "WLepton_dZ",
    
    //==== only for WZ
    "m_Z_candidate_before_cut", "m_lll_before_cut", "PFMET_before_cut", "n_bjets_before_cut", "n_vertices_before_cut",
    "N1_Z_mass", "N1_mlll", "N1_n_bjets", "N1_PFMET",
  };

  m.x_title = {
    "onebin",
    "# of verticies",
    "# of jets",
    "# of b-tagged jets",
    "# of lepton-nearby jets",
    "# of lepton-naerby b-tagged jets",
    "#slash{E}_{T}^{miss} [GeV]", "#phi of #slash{E}_{T}^{miss}",
    "m(OS) [GeV]",
    "m(Z) [GeV]",
    "m_{T}(W) [GeV]",
    "m(lll) [GeV]",
    "p_{T}^{Z} [GeV]",
    "p_{T}^{W} [GeV]",
    "#DeltaR(#mu^{W Lepton},#mu^{Z Leptons})",
    "#DeltaR(#slash{E}_{T}^{miss},#mu^{W Lepton})",
    "#DeltaR(#mu^{W Lepton},Jet)",
    "#DeltaR(#mu^{W Lepton},BJet)",
    "p_{T} [GeV]", "|#eta|", "LeptonRelIso", "|dXY| [cm]", "|Sig(dXY)|", "|dZ| [cm]",
    "p_{T} [GeV]", "|#eta|", "LeptonRelIso", "|dXY| [cm]", "|Sig(dXY)|", "|dZ| [cm]",
    "p_{T} [GeV]", "|#eta|", "LeptonRelIso", "|dXY| [cm]", "|Sig(dXY)|", "|dZ| [cm]",
    
    //==== only for ZZ
    "p_{T} [GeV]", "|#eta|", "LeptonRelIso", "|dXY| [cm]", "|Sig(dXY)|", "|dZ| [cm]",
    "m(llll) [GeV]",
    
    "p_{T} [GeV]", "|#eta|", "LeptonRelIso", "|dXY| [cm]", "|Sig(dXY)|", "|dZ| [cm]",
    "p_{T} [GeV]", "|#eta|", "LeptonRelIso", "|dXY| [cm]", "|Sig(dXY)|", "|dZ| [cm]",
    "p_{T} [GeV]", "|#eta|", "LeptonRelIso", "|dXY| [cm]", "|Sig(dXY)|", "|dZ| [cm]",
    
    //==== only for WZ,
    "m(Z) [GeV]", "m(lll) [GeV]", "#slash{E}_{T}^{miss} [GeV]", "# of b-tagged jets",  "# of vertices",
    "|m(ll)-m(Z)| [GeV]", "m(lll) [GeV]", "# of bjets", "#slash{E}_{T}^{miss} [GeV]",
  };

  //====================
  //==== cut-var skips
  //====================
  
  //==== N-1 and before_cut is only for WZ
  for(unsigned int i=0; i<m.histname.size(); i++){
    TString this_var = m.histname.at(i);
    
    //==== only for ZZ
    if(this_var.Contains("fourth") || this_var=="m_llll"){
      m.CutVarSkips.push_back( make_pair("_WZ_3mu0el", this_var) );
      m.CutVarSkips.push_back( make_pair("_WZ_2mu1el", this_var) );
      m.CutVarSkips.push_back( make_pair("_WZ", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZJets", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZJets_3mu0el", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZJets_2mu1el", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZLep", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZGamma", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZGamma_3mu0el", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZGamma_2mu1el", this_var) );
    }
    
    //==== only not for ZZ
    if(this_var=="m_Z_candidate" ||
       this_var=="mt_W_candidate" ||
       this_var=="m_lll" ||
       this_var=="Z_candidate_Pt" ||
       this_var=="W_candidate_Pt" ||
       this_var=="dRZLeptonWLepton" ||
       this_var=="dRMETWLepton" ||
       this_var.Contains("ZLepton") ||
       this_var.Contains("WLepton")){
      m.CutVarSkips.push_back( make_pair("_ZZ", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZZ_4mu0el", this_var) );
    }
    
    
    //==== only for WZ
    if(this_var == "m_Z_candidate_before_cut" ||
       this_var == "m_lll_before_cut" ||
       this_var == "PFMET_before_cut" ||
       this_var == "n_bjets_before_cut" ||
       this_var == "n_vertices_before_cut" ||
       this_var == "N1_Z_mass" ||
       this_var == "N1_mlll" ||
       this_var == "N1_n_bjets" ||
       this_var == "N1_PFMET"){
      m.CutVarSkips.push_back( make_pair("_WZ_3mu0el", this_var) );
      m.CutVarSkips.push_back( make_pair("_WZ_2mu1el", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZJets", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZJets_3mu0el", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZJets_2mu1el", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZLep", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZGamma", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZGamma_3mu0el", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZGamma_2mu1el", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZZ", this_var) );
      m.CutVarSkips.push_back( make_pair("_ZZ_4mu0el", this_var) );
    }
  }

 
  //============================
  //==== HOT FIX FOR DILEP CR
  //==== RE-SET HIST VARIABLES
  //============================
  
  if(XXX==2){
    m.histname_suffix = {"_OSDiMuon", "_OSDiMuon_Z_10GeV"};
    m.drawdata = {true, true};
    m.samples_to_use = {"DY", "fake_sfed_HighdXY"};
    //m.samples_to_use = {"top", "VVV", "Zgamma", "ZZ_excl", "WZ_excl", "fake_sfed_HighdXY", "DY"};
    
    m.histname = {
      "leadingLepton_Pt", "leadingLepton_Eta", "leadingLepton_RelIso", "leadingLepton_Chi2",
      "secondLepton_Pt", "secondLepton_Eta", "secondLepton_RelIso", "secondLepton_Chi2",
      "mll",
      "n_events",
      "PFMET",
      "n_bjets",
      "n_jets",
      "n_vertices"
    };
    
    m.x_title = {
      "p_{T} [GeV]", "|#eta|", "LeptonRelIso04", "GlobalChi2",
      "p_{T} [GeV]", "|#eta|", "LeptonRelIso04", "GlobalChi2",
      "m(#mu#mu) [GeV]",
      "onebin",
      "PFMET [GeV]",
      "# of b-tagged jets",
      "# of jets",
      "# of vertices"
    };
    
  }

  //====================
  //==== make bkg list
  //====================

  m.make_bkglist();

  //=====================
  //==== Fill MCNorm SF
  //=====================

  for(unsigned int i=0; i<m.bkglist.size(); i++){
    m.MCNormSF[m.bkglist.at(i)] = 1.;
    m.MCNormSF_uncert[m.bkglist.at(i)] = 0.;
  }
  m.SetMCSF(WORKING_DIR+"/data/"+dataset+"/MCSF.txt");

  /*
  //=============================
  //==== set signal mass points
  //=============================
  
  m.signal_mass = {40, 60, 150, 700};
  m.signal_color = {kRed, kBlue, kMagenta-7, kBlack};
  
  //====================================
  //==== set signal coupling constants
  //====================================
  
  m.coupling_constants[make_pair("_WZ", 40)] = 0.01;
  m.coupling_constants[make_pair("_WZ", 60)] = 0.01;
  m.coupling_constants[make_pair("_WZ", 150)] = 1;
  m.coupling_constants[make_pair("_WZ", 700)] = 0.01;
  m.coupling_constants[make_pair("_ZJets", 40)] = 0.001;
  m.coupling_constants[make_pair("_ZJets", 60)] = 0.001;
  m.coupling_constants[make_pair("_ZJets", 150)] = 0.001;
  m.coupling_constants[make_pair("_ZJets", 700)] = 0.001;
  
  //=====================================
  //==== set signal mass for each class
  //=====================================
  
  m.map_class_to_signal_mass[trilepton_mumumu::class1] = {40};
  m.map_class_to_signal_mass[trilepton_mumumu::class2] = {60};
  m.map_class_to_signal_mass[trilepton_mumumu::lowmass] = {40, 60};
  m.map_class_to_signal_mass[trilepton_mumumu::class3] = {150};
  m.map_class_to_signal_mass[trilepton_mumumu::class4] = {700};
  m.map_class_to_signal_mass[trilepton_mumumu::highmass] = {150, 700};
  */

  //=============
  //==== rebins
  //=============
  
/*
  //==== script to generate rebins
  ofstream skeleton_rebins("./txt/CR_skeleton_rebins.txt", ios::trunc);
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

  m.SetRebins(WORKING_DIR+"/data/"+dataset+"/CR_rebins.txt");

  m.rebins[make_pair("_OSDiMuon", "leadingLepton_Pt")] = 5;
  m.rebins[make_pair("_OSDiMuon", "leadingLepton_Eta")] = 5;
  m.rebins[make_pair("_OSDiMuon", "secondLepton_Pt")] = 5;
  m.rebins[make_pair("_OSDiMuon", "secondLepton_Eta")] = 5;

  m.rebins[make_pair("_OSDiMuon_Z_10GeV", "leadingLepton_Pt")] = 5;
  m.rebins[make_pair("_OSDiMuon_Z_10GeV", "leadingLepton_Eta")] = 5;
  m.rebins[make_pair("_OSDiMuon_Z_10GeV", "secondLepton_Pt")] = 5;
  m.rebins[make_pair("_OSDiMuon_Z_10GeV", "secondLepton_Eta")] = 5;

  //=============
  //==== y_maxs
  //=============
  
/*
  //==== script to generate rebins
  ofstream skeleton_y_maxs("./data/CR_skeleton_y_maxs.txt", ios::trunc);
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
  m.default_y_min = 0.;
  //==== log y?
  m.UseSetLogy = false;

  m.SetYAxis(WORKING_DIR+"/data/"+dataset+"/CR_yaxis.txt"); 

  if( std::find(m.histname_suffix.begin(), m.histname_suffix.end(), "_OSDiMuon") != m.histname_suffix.end() || 
      std::find(m.histname_suffix.begin(), m.histname_suffix.end(), "_OSDiMuon_Z_10GeV") != m.histname_suffix.end()    ){

    m.UseSetLogy = false;

    if(m.UseSetLogy){
      m.default_y_max = 100000000.;
      m.default_y_min = 10000.;
    }
    else{
      m.default_y_max = 3000000.;

      m.y_maxs[make_pair("_OSDiMuon_Z_10GeV", "n_jets")] = 15000000.;

    }

  }

  //=============
  //==== x_mins
  //=============
  
/*
  //==== script to generate rebins
  ofstream skeleton_x_mins("./data/CR_skeleton_xaxis.txt", ios::trunc);
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


  m.SetXAxis(WORKING_DIR+"/data/"+dataset+"/CR_xaxis.txt");

  m.x_mins[make_pair("_OSDiMuon_Z_10GeV", "mll")] = 75.;
  m.x_maxs[make_pair("_OSDiMuon_Z_10GeV", "mll")] = 110.;

  //===============
  //==== k-factor
  //===============
  
  m.k_factor = 1.34;
  
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






