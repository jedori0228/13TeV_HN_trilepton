#include "trilepton_mumumu.h"

trilepton_mumumu::trilepton_mumumu(){
  
  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);
  gStyle->SetOptStat(0);
  
}

trilepton_mumumu::~trilepton_mumumu(){
  
  
  
}

void trilepton_mumumu::draw_hist(){
  
  for(i_cut = 0; i_cut < histname_suffix.size(); i_cut++){
    
    outputfile->mkdir(histname_suffix[i_cut]);
    
    cout
    << endl
    << "################### Writing in Directory " << histname_suffix[i_cut] << " ###################" << endl
    << endl;
    
    for(i_var = 0; i_var < histname.size(); i_var++){

      if( find( CutVarSkips.begin(), CutVarSkips.end(), make_pair(histname_suffix[i_cut], histname[i_var]) ) != CutVarSkips.end() ){
        continue;
      }
      
      cout << "[Drawing " << histname[i_var] << "]" << endl;
      
      TH1D* MC_stacked_err = NULL;
      THStack* MC_stacked = new THStack("MC_stacked", "");
      TH1D* hist_data = NULL;
      vector<TH1D*> hist_signal;
      
      TLegend *lg;
      //==== draw data
      if(drawdata.at(i_cut)){
        //==== with signal (SR)
        if(signal_mass.size()==0) lg = new TLegend(0.69, 0.35, 0.96, 0.92);
        //==== without signal (CR)
        else lg = new TLegend(0.69, 0.20, 0.96, 0.92);
      }
      else lg = new TLegend(0.65, 0.40, 0.93, 0.92);
      clear_legend_info();
      
      signal_survive_mass.clear();

      for(i_file = 0; i_file < bkglist.size()+1+signal_mass.size(); i_file++){ // +1 for data
      
        TString filepath, current_sample;
        
        //==== root file path name
        //==== bkg
        if( i_file < bkglist.size() ){
          filepath = "./rootfiles/"+data_class+"/"+filename_prefix+"_SK"+bkglist[i_file]+"_trilep"+filename_suffix;
          current_sample = bkglist[i_file];
        }
        //==== data for i_file = bkglist.size()
        else if( i_file == bkglist.size() ){
          filepath = "./rootfiles/"+data_class+"/"+filename_prefix+"_data_DoubleMuon"+filename_suffix;
          current_sample = "data";
        }
        //==== signal starting from i_file = bkglist.size()+1
        else{

          int signal_index = i_file-bkglist.size()-1;

          //==== if cut is optimized cut, only draw that signal
          if(histname_suffix[i_cut].Contains("cutHN")){
            TString tmpcut = "_cutHN"+TString::Itoa(signal_mass[signal_index],10);
            if(histname_suffix[i_cut]!=tmpcut) continue;
          }
          //==== else, follow signal_draw
          else{
            if(!signal_draw[signal_index]) continue;
          }

          //cout << "signal_index = " << signal_index << " => mass = " << signal_mass[signal_index] << endl;
          TString WhichChannel = "MuMuMu";
          if(histname_suffix[i_cut].Contains("MuMuE")) WhichChannel = "SSSF_MuMuE";
          TString string_signal_mass = "HN_"+WhichChannel+"_"+TString::Itoa(signal_mass[signal_index],10);
          filepath = "./rootfiles/"+data_class+"/"+filename_prefix+"_SK"+string_signal_mass+filename_suffix;
          current_sample = string_signal_mass;
        }
         
        //cout
        //<< "filepath = " << filepath << endl
        //<< "hisname = " << histname[i_var]+histname_suffix[i_cut] << endl;
        
        //==== get root file
        TFile* file = new TFile(filepath);
        if( !file ){
          cout << "No file : " << filepath << endl;
          continue;
        }

        //==== full histogram name
        TString fullhistname = histname[i_var]+histname_suffix[i_cut];
        
        //==== get histogram
        TH1D* hist_temp = (TH1D*)file->Get(fullhistname);
        if(!hist_temp || hist_temp->GetEntries() == 0){
          cout << "No histogram : " << current_sample << endl;
          file->Close();
          delete file;
          continue;
        }

        //==== set histogram name, including sample name
        hist_temp->SetName(fullhistname+"_"+current_sample);

        //==== set error separately for fake
        if( current_sample.Contains("fake") ){
          TH1D* hist_temp_up = (TH1D*)file->Get(fullhistname+"_up");
          TH1D* hist_temp_down = (TH1D*)file->Get(fullhistname+"_down");
          int n_bins = hist_temp->GetXaxis()->GetNbins();
          for(int i=1; i<=n_bins; i++){
            double error_propagated = hist_temp_up->GetBinContent(i)-hist_temp->GetBinContent(i); // FIXME central-down should be same as up-central
            double error_sumw2 = hist_temp->GetBinError(i);
            double error_combined = sqrt( error_propagated*error_propagated + error_sumw2*error_sumw2 );

            double error_syst = CalculatedSysts["FakeLooseID"]*(hist_temp->GetBinContent(i));

            error_combined = sqrt(error_combined*error_combined + error_syst*error_syst);

            hist_temp->SetBinError(i, error_combined);
          }
        }

        //==== rebin here
        hist_temp->Rebin( n_rebin() );
        
        //==== set X-axis range
        SetXaxisRange(hist_temp);
        
        //==== make overflows bins
        TH1D *hist_final = MakeOverflowBin(hist_temp);
        
        //==== Set Attributes here
        //==== bkg
        if( i_file < bkglist.size() ){
          //==== get which MC sector
          TString current_MCsector = find_MCsector();
          int n_bins = hist_final->GetXaxis()->GetNbins();
          if(!MC_stacked_err){
            MC_stacked_err = new TH1D("MC_stacked_err", "",
                                      n_bins,
                                      hist_final->GetXaxis()->GetBinLowEdge(1),
                                      hist_final->GetXaxis()->GetBinUpEdge(n_bins));
          }
          hist_final->SetFillColor(map_sample_string_to_legendinfo[current_MCsector].second);
          hist_final->SetLineColor(map_sample_string_to_legendinfo[current_MCsector].second);

          //==== MC Norm Scaling
          if(ApplyMCNormSF.at(i_cut)){
            hist_final->Scale(MCNormSF[current_sample]);
            //==== Set MC Norm uncertainty
            for(int i=1; i<=n_bins; i++){
              double error_syst = MCNormSF_uncert[current_sample]*(hist_final->GetBinContent(i));
              double error_sumw2 = hist_final->GetBinError(i);
              double error_combined = sqrt( error_syst*error_syst + error_sumw2*error_sumw2 );

              hist_final->SetBinError(i, error_combined);
            }
          }

          MC_stacked->Add(hist_final);
          MC_stacked_err->Add(hist_final);
        }
        //==== data for i_file = bkglist.size()
        else if( i_file == bkglist.size() ){
          hist_final->SetMarkerStyle(3);
          hist_final->SetMarkerSize(1);
          TString temp_hist_name(hist_final->GetName());
          hist_final->SetName(temp_hist_name+"_data");
          hist_data = (TH1D*)hist_final->Clone();
        }
        //==== signal starting from i_file = bkglist.size()+1
        else if( i_file > bkglist.size() ){
          int signal_index = i_file-bkglist.size()-1;
          //cout << "signal index = " << signal_index << ", mass = " << signal_mass[signal_index] << endl;
          hist_final->SetLineColor(signal_color[signal_index]);
          hist_final->SetLineWidth(3);
          TString temp_hist_name(hist_final->GetName());
          hist_final->SetName(temp_hist_name+"_signal_"+TString::Itoa(signal_mass[signal_index], 10));
          //==== scaling signal
          double this_coupling_constant = coupling_constant(signal_mass[signal_index]);
          hist_final->Scale( k_factor*this_coupling_constant/(1.*TMath::Power(10,log_of_generation_mixing)) );
          hist_signal.push_back( (TH1D*)hist_final->Clone() );
          signal_survive_mass.push_back(signal_mass[signal_index]);
        }
        else{
          cout << "[Warning] attirubte setting, i_file > total sample size? This should not happen!" << endl;
        }

        fill_legend(lg, hist_final);
        
        file->Close();
        delete file;
        
        //cout << "end of this sample" << endl;
        
      } // END loop over samples
    
      //cout << "[Draw Canvas]" << endl;
    
      draw_canvas(MC_stacked, MC_stacked_err, hist_data, hist_signal, lg, drawdata.at(i_cut));

      //==== legend is already deleted in draw_canvas()
      //delete lg; 
      
    } // END loop over variables
    

    
  } // END loop over cuts
  
  
  outputfile->Close();
  
}

void trilepton_mumumu::make_bkglist(){
  for(unsigned int i=0; i<samples_to_use.size(); i++){
    MCsector_first_index.push_back( bkglist.size() );
    //cout << "[make_bkglist] " << "MCsector_first_index.push_back(" <<  bkglist.size() << ")" << endl;
    bkglist.insert(bkglist.end(),
                   map_sample_string_to_list[samples_to_use.at(i)].begin(),
                   map_sample_string_to_list[samples_to_use.at(i)].end()
                   );
  }
  cout << "We will use :" << endl;
  for(unsigned int i=0; i<bkglist.size(); i++) cout << " " << bkglist[i] << endl;
}

void trilepton_mumumu::SetRebins(TString filepath){

  cout << "[trilepton_mumumu::SetRebins] Get rebins from " << filepath << endl;

  string elline;
  ifstream in(filepath);
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString cut;
    TString histname;
    double value;
    is >> cut;
    is >> histname;
    is >> value;
    //==== if -999, don't set
    if(value!=-999){
      rebins[make_pair(cut, histname)] = value;
    }
  }
}


void trilepton_mumumu::SetYAxis(TString filepath){

  cout << "[trilepton_mumumu::SetYAxis] Get Yaxis from " << filepath << endl;

  string elline;
  ifstream in(filepath);
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString cut;
    TString histname;
    double value;
    is >> cut;
    is >> histname;
    is >> value;
    //==== if -999, don't set
    if(value!=-999){
      y_maxs[make_pair(cut, histname)] = value;
    }
  }
}

void trilepton_mumumu::SetXAxis(TString filepath){

  cout << "[trilepton_mumumu::SetXAxis] Get Xaxis from " << filepath << endl;

  string elline;
  ifstream in(filepath);
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString cut;
    TString histname;
    double value_min, value_max;
    is >> cut;
    is >> histname;
    is >> value_min;
    is >> value_max;
    //==== if -999, don't set
    if(value_min!=-999){
      x_mins[make_pair(cut, histname)] = value_min;
    }
    if(value_max!=-999){
      x_maxs[make_pair(cut, histname)] = value_max;
    }
  }
}

void trilepton_mumumu::SetMCSF(TString filepath){

  cout << "[trilepton_mumumu::SetMCSF] Get MC SF from " << filepath << endl;

  string elline;
  ifstream in(filepath);
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString sample;
    double MCSF, MCSF_err;
    is >> sample;
    is >> MCSF;
    is >> MCSF_err;

    cout << "[trilepton_mumumu::SetMCSF] sample : " << sample << endl;
    cout << "[trilepton_mumumu::SetMCSF] => MCSF = " << MCSF << ", MCSF_err = " << MCSF_err << endl;

    MCNormSF[sample] = MCSF;
    MCNormSF_uncert[sample] = MCSF_err;

  }
}

void trilepton_mumumu::SetCalculatedSysts(TString filepath){

  string elline_syst;
  ifstream in_syst(filepath);

  cout << "[trilepton_mumumu::SetCalculatedSysts] Get Systematics from " << filepath << endl;

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

}

TString trilepton_mumumu::find_MCsector(){
  for(unsigned int i=0; i<MCsector_first_index.size()-1; i++){
    if(MCsector_first_index.at(i) <= i_file && i_file < MCsector_first_index.at(i+1)){
      //cout << "[find_MCsector] " << "("<<MCsector_first_index.at(i)<<","<<MCsector_first_index.at(i+1)<<") => index " << index << ", returned MCsector is " << samples_to_use.at(i) << endl;
      return samples_to_use.at(i);
    }
  }
  //cout << "[find_MCsector] " << "index " << index << ", returned MCsector is " << *samples_to_use.end() << endl;
  return samples_to_use.back();
}

void trilepton_mumumu::clear_legend_info(){
  hist_for_legend_bkg.clear();
  hist_for_legend_signal.clear();
  MCsector_survive.clear();
  for(int i=0; i<samples_to_use.size(); i++){
    MCsector_survive[samples_to_use.at(i)] = false;
  }

}

double trilepton_mumumu::coupling_constant(int mass){

  TString cut = histname_suffix[i_cut];
  
  if( coupling_constants.find( make_pair(cut, mass) ) != coupling_constants.end() ){
    //cout << "cut = " << cut << ", mass = " << mass << " => coupling constant = " << coupling_consts[make_pair(cut, mass)] << endl;
    return coupling_constants[make_pair(cut, mass)];
  }
  else return 1.;

}

void trilepton_mumumu::fill_legend(TLegend* lg, TH1D* hist){
  //==== here, hist_for_legned = {"A", "B", "D"}
  //==== now, push_back data and signal to make
  //==== hist_for_legned = {"A", "B", "D", "data", "HN40", "HN50", "HN60"}
  
  //==== bkg
  if( i_file < bkglist.size() ){
    TString current_MCsector = find_MCsector();
    //cout << "[fill_legend] " << "index " << index << ", current_MCsector is " << current_MCsector << endl;
    if( !MCsector_survive[current_MCsector] ){
      //cout << "[fill_legend] " << bkglist[index] << " is saved" << endl;
      hist_for_legend_bkg.push_back((TH1D*)hist->Clone());
      MCsector_survive[current_MCsector] = true;
    }
  }
  //==== data
  else if( i_file == (int)bkglist.size() ){
    hist_for_legend_data = (TH1D*)hist->Clone();
    //cout << "Data added in hist_for_legend" << endl;
  }
  //==== signals
  else if( i_file > (int)bkglist.size() ){
    hist_for_legend_signal.push_back((TH1D*)hist->Clone());
    //cout << "Signal added in hist_for_legend" << endl;
  }
  else{
    cout << "[Warning] fill_legend, i_file > total sample size? This should not happen!" << endl;
  }


}

void trilepton_mumumu::draw_legend(TLegend* lg, signal_class sc, bool DrawData){
  // Example :
  //                      0    1    2    3
  // samples_to_use   = {"A", "B", "C", "D"}
  // MCsector_survive = { T ,  T ,  F ,  T }
  //
  //                       0    1    2      3        4          5
  // hist_for_legend are {"A", "B", "D", "data", "signal1", "signal2"}
  // i_data = 6 - 2 - 1 = 3
  
  if(DrawData && hist_for_legend_data){
    lg->AddEntry(hist_for_legend_data, "data", "p");
  }
  //cout << "[draw_legend] printing MCsector_survive" << endl;
  for(auto it = MCsector_survive.begin(); it != MCsector_survive.end(); ++it){
    //cout << "[draw_legend] " << it->first << " is " << it->second << endl;
  }
  for(int i=samples_to_use.size()-1, j=hist_for_legend_bkg.size()-1; i>=0; i--){
    if(MCsector_survive[samples_to_use.at(i)]){
      //cout << "[draw_legend] " << samples_to_use.at(i) << " is added in legend" << endl;
      lg->AddEntry(hist_for_legend_bkg.at(j), map_sample_string_to_legendinfo[samples_to_use.at(i)].first, "f");
      j--;
    }
  }
  if(sc == class1){
    for(unsigned int i=0; i<signal_survive_mass.size(); i++){
      int this_mass = signal_survive_mass.at(i);
      if( find(map_class_to_signal_mass[class1].begin(), map_class_to_signal_mass[class1].end(), this_mass) != map_class_to_signal_mass[class1].end()){
        lg->AddEntry(hist_for_legend_signal.at(i), legend_coupling_label(this_mass), "l");
      }
    }
  }
  else if(sc == class2){
    for(unsigned int i=0; i<signal_survive_mass.size(); i++){
      int this_mass = signal_survive_mass.at(i);
      if( find(map_class_to_signal_mass[class2].begin(), map_class_to_signal_mass[class2].end(), this_mass) != map_class_to_signal_mass[class2].end()){
        lg->AddEntry(hist_for_legend_signal.at(i), legend_coupling_label(this_mass), "l");
      }
    }
  }
  else if(sc == lowmass){
    for(unsigned int i=0; i<signal_survive_mass.size(); i++){
      int this_mass = signal_survive_mass.at(i);
      if( find(map_class_to_signal_mass[lowmass].begin(), map_class_to_signal_mass[lowmass].end(), this_mass) != map_class_to_signal_mass[lowmass].end()){
        lg->AddEntry(hist_for_legend_signal.at(i), legend_coupling_label(this_mass), "l");
      }
    }
  }
  else if(sc == class3){
    for(unsigned int i=0; i<signal_survive_mass.size(); i++){
      int this_mass = signal_survive_mass.at(i);
      if( find(map_class_to_signal_mass[class3].begin(), map_class_to_signal_mass[class3].end(), this_mass) != map_class_to_signal_mass[class3].end()){
        lg->AddEntry(hist_for_legend_signal.at(i), legend_coupling_label(this_mass), "l");
      }
    }
  }
  else if(sc == class4){
    for(unsigned int i=0; i<signal_survive_mass.size(); i++){
      int this_mass = signal_survive_mass.at(i);
      if( find(map_class_to_signal_mass[class4].begin(), map_class_to_signal_mass[class4].end(), this_mass) != map_class_to_signal_mass[class4].end()){
        lg->AddEntry(hist_for_legend_signal.at(i), legend_coupling_label(this_mass), "l");
      }
    }
  }
  else if(sc == highmass){
    for(unsigned int i=0; i<signal_survive_mass.size(); i++){
      int this_mass = signal_survive_mass.at(i);
      if( find(map_class_to_signal_mass[highmass].begin(), map_class_to_signal_mass[highmass].end(), this_mass) != map_class_to_signal_mass[highmass].end()){
        lg->AddEntry(hist_for_legend_signal.at(i), legend_coupling_label(this_mass), "l");
      }
    }
  }
  else if(sc == no_class){
    for(unsigned int i=0; i<signal_survive_mass.size(); i++){
      int this_mass = signal_survive_mass.at(i);
      if( find(signal_mass.begin(), signal_mass.end(), this_mass) != signal_mass.end()){
        lg->AddEntry(hist_for_legend_signal.at(i), legend_coupling_label(this_mass), "l");
      }
    }
  }
  else{
    cout << "[Warning] This should not happen!" << endl;
  }
  lg->SetFillStyle(0);
  lg->SetBorderSize(0);
  lg->Draw();
}

void trilepton_mumumu::draw_canvas(THStack* mc_stack, TH1D* mc_error, TH1D* hist_data, vector<TH1D*> hist_signal, TLegend* legend, bool DrawData){

  if(!hist_data) return;

  //==== signal_class
  signal_class this_sc = no_class;
  //==== cutdR_cutW is only applied for low mass yet
  if( histname_suffix[i_cut] == "_cutWlow" ) this_sc = lowmass;
  if( histname_suffix[i_cut] == "_cutWhigh" ) this_sc = highmass;
  if( histname[i_var].Contains("class1") ) this_sc = class1;
  else if( histname[i_var].Contains("class2") ) this_sc = class2;
  else if( histname[i_var].Contains("class3") ) this_sc = class3;
  else if( histname[i_var].Contains("class4") ) this_sc = class4;
  else if( histname[i_var].Contains("lowmass") ) this_sc = lowmass;
  else if( histname[i_var].Contains("highmass") ) this_sc = highmass;
  
  //==== y=0 line
  double x_0[2], y_0[2];
  x_0[0] = -1000;  y_0[0] = 0;
  x_0[1] = 1000;  y_0[1] = 0;
  TGraph *g0 = new TGraph(2, x_0, y_0);
  //==== y=1 line
  double x_1[2], y_1[2];
  x_1[0] = 1000;  y_1[0] = 1;
  x_1[1] = -1000;  y_1[1] = 1;
  TGraph *g1 = new TGraph(2, x_1, y_1);
  
  //==== If we draw data, prepare top/bottom pads
  TCanvas* c1 = new TCanvas(histname[i_var], "", 800, 800);
  c1->Draw();
  TPad *c1_up;
  TPad *c1_down;
  if(DrawData){
    c1_up = new TPad("c1", "", 0, 0.25, 1, 1);
    c1_down = new TPad("c1_down", "", 0, 0, 1, 0.25);

    canvas_margin(c1, c1_up, c1_down);
    
    c1_up->SetGridx();
    c1_up->SetGridy();
    c1_down->SetGridx();
    c1_down->SetGridy();
    
    c1_up->Draw();
    c1_down->Draw();
    c1_up->cd();
  }
  else{
    canvas_margin(c1);
    c1->SetGridx();
    c1->SetGridy();
    c1->cd();
  }

  if(UseSetLogy) gPad->SetLogy();

  //==== empty histogram for axis
  TH1D *hist_empty = (TH1D*)mc_stack->GetHists()->At(0)->Clone();
  hist_empty->GetYaxis()->SetRangeUser( default_y_min, y_max() );
  //=== get dX
  double dx = (hist_empty->GetXaxis()->GetXmax() - hist_empty->GetXaxis()->GetXmin())/hist_empty->GetXaxis()->GetNbins();
  TString YTitle = DoubleToString(dx);

  hist_empty->GetYaxis()->SetTitle(YTitle); //FIXME
  hist_empty->SetLineWidth(0);
  hist_empty->SetLineColor(0);
  hist_empty->SetMarkerSize(0);
  hist_empty->SetMarkerColor(0);
  hist_empty->Draw("histsame");
  
  //==== bkg
  if(!mc_stack->GetHists()){
    cout << "[No Background]" << endl;
    return;
  }
  mc_stack->Draw("histsame");

  if(DrawData){
    //==== hide X Label for top plot
    //==== axis setting will be done after we get bottom plot
    hist_empty->GetXaxis()->SetLabelSize(0);
    //==== draw data
    hist_data->Draw("PE1same");
  }
  else{
    //==== set X axis title
    hist_empty->GetXaxis()->SetTitle(x_title[i_var]);
    hist_axis(hist_empty);
  }
  
  //==== signal
  if(this_sc == class1){
    for(unsigned int i=0; i<signal_survive_mass.size(); i++){
      int this_mass = signal_survive_mass.at(i);
      if( find(map_class_to_signal_mass[class1].begin(), map_class_to_signal_mass[class1].end(), this_mass) != map_class_to_signal_mass[class1].end()){
        hist_signal[i]->Draw("histsame");
      }
    }
  }
  else if(this_sc == class2){
    for(unsigned int i=0; i<signal_survive_mass.size(); i++){
      int this_mass = signal_survive_mass.at(i);
      if( find(map_class_to_signal_mass[class2].begin(), map_class_to_signal_mass[class2].end(), this_mass) != map_class_to_signal_mass[class2].end()){
        hist_signal[i]->Draw("histsame");
      }
    }
  }
  else if(this_sc == lowmass){
    for(unsigned int i=0; i<signal_survive_mass.size(); i++){
      int this_mass = signal_survive_mass.at(i);
      if( find(map_class_to_signal_mass[lowmass].begin(), map_class_to_signal_mass[lowmass].end(), this_mass) != map_class_to_signal_mass[lowmass].end()){
        hist_signal[i]->Draw("histsame");
      }
    }
  }
  else if(this_sc == class3){
    for(unsigned int i=0; i<signal_survive_mass.size(); i++){
      int this_mass = signal_survive_mass.at(i);
      if( find(map_class_to_signal_mass[class3].begin(), map_class_to_signal_mass[class3].end(), this_mass) != map_class_to_signal_mass[class3].end()){
        hist_signal[i]->Draw("histsame");
      }
    }
  }
  else if(this_sc == class4){
    for(unsigned int i=0; i<signal_survive_mass.size(); i++){
      int this_mass = signal_survive_mass.at(i);
      if( find(map_class_to_signal_mass[class4].begin(), map_class_to_signal_mass[class4].end(), this_mass) != map_class_to_signal_mass[class4].end()){
        hist_signal[i]->Draw("histsame");
      }
    }
  }
  else if(this_sc == highmass){
    for(unsigned int i=0; i<signal_survive_mass.size(); i++){
      int this_mass = signal_survive_mass.at(i);
      if( find(map_class_to_signal_mass[highmass].begin(), map_class_to_signal_mass[highmass].end(), this_mass) != map_class_to_signal_mass[highmass].end()){
        hist_signal[i]->Draw("histsame");
      }
    }
  }
  else if(this_sc == no_class){
    for(unsigned int i=0; i<signal_survive_mass.size(); i++){
      int this_mass = signal_survive_mass.at(i);
      if( find(signal_mass.begin(), signal_mass.end(), this_mass) != signal_mass.end()){
        hist_signal[i]->Draw("histsame");
      }
    }
  }
  else{
    cout << "[Warning] This should not happen!" << endl;
  }
  //==== err
  mc_error->SetMarkerColorAlpha(kAzure-9, 0);
  mc_error->SetFillStyle(3013);
  mc_error->SetFillColor(kBlack);
  mc_error->SetLineColor(0);
  mc_error->Draw("sameE2");
  //==== legend
  legend->AddEntry(mc_error, "Stat.+Syst. Uncert.", "f");
  draw_legend(legend, this_sc, DrawData);
  
  //==== MC-DATA
  if(DrawData){
    c1_down->cd();
    TH1D* hist_compare = (TH1D*)hist_data->Clone();
    hist_compare->Divide(mc_error);
    hist_compare->SetMaximum(1.5);
    hist_compare->SetMinimum(0.5);
    hist_compare->GetXaxis()->SetTitle(x_title[i_var]);
    hist_compare->SetYTitle("#frac{DATA}{MC}");
    hist_compare->Draw("PE1same");
    hist_axis(hist_empty, hist_compare);
    //==== y=1 line
    g1->Draw("same");
  }
  
  //==== write lumi on the top
  c1->cd();
  TLatex latex_CMSPriliminary, latex_Lumi;
  latex_CMSPriliminary.SetNDC();
  latex_Lumi.SetNDC();
  if(DrawData){
    latex_CMSPriliminary.SetTextSize(0.035);
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");
    latex_Lumi.SetTextSize(0.035);
    latex_Lumi.DrawLatex(0.7, 0.96, "35.9 fb^{-1} (13 TeV)");
  }
  else{
    latex_CMSPriliminary.SetTextSize(0.035);
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} Simulation");
    latex_Lumi.SetTextSize(0.035);
    latex_Lumi.DrawLatex(0.7, 0.96, "35.9 fb^{-1} (13 TeV)");
  }

  TString this_plotpath = plotpath+"/"+histname_suffix[i_cut];
  if(ApplyMCNormSF.at(i_cut)) this_plotpath = plotpath+"/MCNormSFed"+histname_suffix[i_cut];
  mkdir(this_plotpath);
  c1->SaveAs(this_plotpath+"/"+histname[i_var]+histname_suffix[i_cut]+".pdf");
  outputfile->cd(histname_suffix[i_cut]);
  c1->Write();
  
  delete legend;
  delete c1;
}

int trilepton_mumumu::n_rebin(){
  
  TString cut = histname_suffix[i_cut];
  TString var = histname[i_var];
    
  if( rebins.find( make_pair(cut, var) ) != rebins.end() ){
    //cout << "cut = " << cut << ", var = " << var << " => rebins = " << rebins[make_pair(cut, var)] << endl;
    return rebins[make_pair(cut, var)];
  }
  else return 1;
  
}

double trilepton_mumumu::y_max(){
  
  TString cut = histname_suffix[i_cut];
  TString var = histname[i_var];
  
  if( y_maxs.find( make_pair(cut, var) ) != y_maxs.end() ){
    //cout << "cut = " << cut << ", var = " << var << " => rebins = " << rebins[make_pair(cut, var)] << endl;
    return y_maxs[make_pair(cut, var)];
  }
  else return default_y_max;

}

void trilepton_mumumu::SetXaxisRange(TH1D* hist){
  
  TString cut = histname_suffix[i_cut];
  TString var = histname[i_var];
  
  double this_x_min = hist->GetXaxis()->GetBinLowEdge(1);
  double this_x_max = hist->GetXaxis()->GetBinUpEdge( hist->GetXaxis()->GetNbins() );
  
  if( x_mins.find( make_pair(cut, var) ) != x_mins.end() ){
    //cout << "cut = " << cut << ", var = " << var << " => x_min = " << x_mins[make_pair(cut, var)] << endl;
    this_x_min = x_mins[make_pair(cut, var)];
  }
  if( x_maxs.find( make_pair(cut, var) ) != x_maxs.end() ){
    //cout << "cut = " << cut << ", var = " << var << " => x_max = " << x_maxs[make_pair(cut, var)] << endl;
    this_x_max = x_maxs[make_pair(cut, var)];
  }
  
  hist->GetXaxis()->SetRangeUser(this_x_min, this_x_max);
}

void trilepton_mumumu::SetXaxisRange(THStack* mc_stack){
  
  TString cut = histname_suffix[i_cut];
  TString var = histname[i_var];
  
  double this_x_min = mc_stack->GetXaxis()->GetBinLowEdge(1);
  double this_x_max = mc_stack->GetXaxis()->GetBinUpEdge( mc_stack->GetXaxis()->GetNbins() );
  
  if( x_mins.find( make_pair(cut, var) ) != x_mins.end() ){
    //cout << "cut = " << cut << ", var = " << var << " => rebins = " << rebins[make_pair(cut, var)] << endl;
    this_x_min = x_mins[make_pair(cut, var)];
  }
  if( x_maxs.find( make_pair(cut, var) ) != x_maxs.end() ){
    //cout << "cut = " << cut << ", var = " << var << " => rebins = " << rebins[make_pair(cut, var)] << endl;
    this_x_max = x_maxs[make_pair(cut, var)];
  }
  
  mc_stack->GetXaxis()->SetRangeUser(this_x_min, this_x_max);
}

void trilepton_mumumu::SetXaxisRangeBoth(THStack* mc_stack, TH1D* hist){

  TString cut = histname_suffix[i_cut];
  TString var = histname[i_var];
  
  double this_x_min = mc_stack->GetXaxis()->GetBinLowEdge(1);
  double this_x_max = mc_stack->GetXaxis()->GetBinUpEdge( mc_stack->GetXaxis()->GetNbins() );
  
  if( x_mins.find( make_pair(cut, var) ) != x_mins.end() ){
    //cout << "cut = " << cut << ", var = " << var << " => rebins = " << rebins[make_pair(cut, var)] << endl;
    this_x_min = x_mins[make_pair(cut, var)];
  }
  if( x_maxs.find( make_pair(cut, var) ) != x_maxs.end() ){
    //cout << "cut = " << cut << ", var = " << var << " => rebins = " << rebins[make_pair(cut, var)] << endl;
    this_x_max = x_maxs[make_pair(cut, var)];
  }

  mc_stack->GetXaxis()->SetRangeUser(this_x_min, this_x_max);
  hist->GetXaxis()->SetRangeUser(this_x_min, this_x_max);
}

TH1D* trilepton_mumumu::MakeOverflowBin(TH1D* hist){
  
  int bin_first = hist->GetXaxis()->GetFirst();
  int bin_last = hist->GetXaxis()->GetLast();
  int n_bin_inrange = bin_last-bin_first+1;
  int n_bin_origin = hist->GetXaxis()->GetNbins();
  
  double x_first_lowedge = hist->GetXaxis()->GetBinLowEdge(bin_first);
  double x_last_upedge = hist->GetXaxis()->GetBinUpEdge(bin_last);
  
  double overflows = hist->Integral(bin_last+1, n_bin_origin+1);
  
  TH1D *hist_out = new TH1D(hist->GetName(), hist->GetTitle(), n_bin_inrange, x_first_lowedge, x_last_upedge);
  for(unsigned int i=1; i<=n_bin_inrange; i++){
    double this_content = hist->GetBinContent(bin_first-1+i);
    double this_error = hist->GetBinError(bin_first-1+i);
    //cout << "["<<hist_out->GetXaxis()->GetBinLowEdge(i)<<", "<<hist_out->GetXaxis()->GetBinUpEdge(i)<<"] : "<<this_content<<endl;
    if(i==n_bin_inrange){
      this_content += overflows;
      double overflowerror = hist->GetBinError(n_bin_origin+1);
      this_error = TMath::Sqrt( this_error*this_error + overflowerror*overflowerror );
    }
    
    hist_out->SetBinContent(i, this_content);
    hist_out->SetBinError(i, this_error);
    
  }
  
  return hist_out;
  
}

TString trilepton_mumumu::DoubleToString(double dx){

  //cout << "[trilepton_mumumu::DoubleToString] var = " << histname[i_var] << endl;
  //cout << "[trilepton_mumumu::DoubleToString] unit = " << units[i_var] << endl;
  //cout << "[trilepton_mumumu::DoubleToString] dx = " << dx << endl;

  //==== onebin
  if(units[i_var]=="int"){
    return "Events";
  }
  else{

    int dx_int = int(dx);
    int dx_first_two_digits = int((dx-dx_int)*100);
    int dx_first_three_digits = int((dx-dx_int)*1000);

    //==== has integer
    if(dx_int!=0){
      //==== but no digits
      if(dx_first_two_digits==0){
        return "Events / "+TString::Itoa(dx_int,10)+" "+units[i_var];
      }
      //=== also has digits
      else{
        return "Events / "+TString::Itoa(dx_int,10)+"."+TString::Itoa(dx_first_two_digits,10)+" "+units[i_var];
      }
    }

  }

  return "Events";

}


TString trilepton_mumumu::legend_coupling_label(int mass){
 
  //cout << "mass = " << mass << endl;
  //cout << " coupling = " << coupling_const.at(signal_survive_index[mass]) << endl;
  double log_coupling = TMath::Log10(coupling_constant(mass)/(1.*TMath::Power(10,log_of_generation_mixing)));
  //cout << " log coupling = " << log_coupling << endl;
  
  //if(log_coupling == 0) return "HN"+TString::Itoa(mass, 10)+", |V_{N#mu}|^{2}=1";
  //else return "HN"+TString::Itoa(mass, 10)+", |V_{N#mu}|^{2}=10^{"+TString::Itoa(log_coupling, 10)+"}";

  //if(log_coupling == 0) return "m(HN) = "+TString::Itoa(mass, 10)+" GeV/c^{2}, |V_{N#mu}|^{2}=0.01";
  //return "10^{"+TString::Itoa(log_coupling, 10)+"} #times m(HN) = "+TString::Itoa(mass, 10)+" GeV/c^{2}, |V_{N#mu}|^{2}=0.01";

  if(log_coupling == 0) return "m(HN) = "+TString::Itoa(mass, 10)+" GeV/c^{2}";
  return "10^{"+TString::Itoa(log_coupling, 10)+"} #times m(HN) = "+TString::Itoa(mass, 10)+" GeV/c^{2}";

  //if(log_coupling == 0) return "#splitline{m(N) = "+TString::Itoa(mass, 10)+" GeV/c^{2}}{|V_{N#mu}|^{2}=0.01}";
  //return "#splitline{10^{"+TString::Itoa(log_coupling, 10)+"} #times m(N) = "+TString::Itoa(mass, 10)+" GeV}{|V_{N#mu}|^{2}=0.01}";
  
}

void trilepton_mumumu::mkdir(TString path){
  
  if( !gSystem->mkdir(path, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << path << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
  
}

void trilepton_mumumu::make_plot_directory(){

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  plotpath = WORKING_DIR+"/plots/"+data_class;

  if( find(samples_to_use.begin(), samples_to_use.end(), "fake_Dijet") != samples_to_use.end() ){
    plotpath = plotpath+"/use_FR_method/Dijet";
  }

  if( find(samples_to_use.begin(), samples_to_use.end(), "fake_HighdXY") != samples_to_use.end() ){
    plotpath = plotpath+"/use_FR_method/HighdXY";
  }
  if( find(samples_to_use.begin(), samples_to_use.end(), "fake_DiMuon_HighdXY") != samples_to_use.end() ){
    plotpath = plotpath+"/use_FR_method/DiMuon_HighdXY";
  }

  if( find(samples_to_use.begin(), samples_to_use.end(), "fake_sfed_HighdXY") != samples_to_use.end() ){
    plotpath = plotpath+"/use_FR_method/SFed_HighdXY";
  }
  if( find(samples_to_use.begin(), samples_to_use.end(), "fake_sfed_DiMuon_HighdXY") != samples_to_use.end() ){
    plotpath = plotpath+"/use_FR_method/SFed_DiMuon_HighdXY";
  }

  cout
  << endl
  << "###################################################" << endl
  << "OUTPUT ===> " << plotpath << endl
  << "###################################################" << endl
  << endl;
  
  mkdir(plotpath);
  
}




