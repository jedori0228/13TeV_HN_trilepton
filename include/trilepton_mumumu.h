#ifndef trilepton_mumumu_h
#define trilepton_mumumu_h

#include "TSystem.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"
#include <iostream>
#include <vector>
#include <map>
#include "canvas_margin.h"

using namespace std;

class trilepton_mumumu{
public:
  //==== constructors
  trilepton_mumumu();
  ~trilepton_mumumu();

  enum signal_class{
    no_class = 0,
    class1 = 1,
    class2 = 2,
    lowmass = 12,
    class3 = 3,
    class4 = 4,
    highmass = 34,
  };
  
  //==== functions for setting
  void make_bkglist();
  void SetRebins(TString filepath);
  void SetYAxis(TString filepath);
  void SetXAxis(TString filepath);

  //==== functions for drawing
  void draw_hist();
  TString find_MCsector();
  void clear_legend_info();
  double coupling_constant(int mass);
  void fill_legend(TLegend* lg, TH1D* hist);
  void draw_legend(TLegend* lg, signal_class sc, bool DrawData);
  void draw_canvas(THStack* mc_stack, TH1D* mc_error, TH1D* hist_data, vector<TH1D*> hist_signal, TLegend* legend, bool DrawData);

  int n_rebin();
  double y_max();
  void SetXaxisRange(TH1D* hist);
  void SetXaxisRange(THStack* mc_stack);
  void SetXaxisRangeBoth(THStack* mc_stack, TH1D* hist);
  TH1D* MakeOverflowBin(TH1D* hist);

  void mkdir(TString path);
  void make_plot_directory();
  TString legend_coupling_label(int mass);
  
  //==== variables
  unsigned int i_cut, i_var, i_file;
  bool UseSetLogy=false;
  bool DrawPU=true;
  TString filename_prefix, filename_suffix, data_class, plotpath;
  vector<TString> histname_suffix, bkglist, samples_to_use, histname, x_title;
  vector<bool> drawdata, ApplyMCNormSF;
  map<TString, double> MCNormSF, MCNormSF_uncert;
  vector< pair<TString, TString> > CutVarSkips;
  map< TString, vector<TString> > map_sample_string_to_list;
  map< TString, pair<TString, Color_t> > map_sample_string_to_legendinfo;
  vector<int> signal_survive_mass;
  vector<int> MCsector_first_index;
  vector<int> signal_mass;
  map< signal_class, vector<int> > map_class_to_signal_mass;
  vector<Color_t> signal_color;
  map< pair<TString, int>, double > coupling_constants;
  map<TString, bool> MCsector_survive;
  TFile *outputfile;
  TH1D *hist_for_legend_data;
  vector<TH1D*> hist_for_legend_bkg;
  vector<TH1D*> hist_for_legend_signal;
  map< pair<TString, TString>, int > rebins;
  map< pair<TString, TString>, double > y_maxs;
  map< pair<TString, TString>, double > x_mins, x_maxs;
  double default_y_max = 1000, default_y_min = 0.;
  double k_factor;
  double log_of_generation_mixing;
  


};
#endif
