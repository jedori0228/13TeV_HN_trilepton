#include "canvas_margin.h"

double GetYMaxs(TString hist);
double GetPerRatio(TString per);

void Validation(){
  
  gStyle->SetOptStat(0);
  TH1::AddDirectory(kFALSE);
  
  TString cmssw_version = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");
  
  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Validation/";
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/Validation/";
  
  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
  
  //==== y=1 line
  double x_1[2], y_1[2];
  x_1[0] = 10000;  y_1[0] = 1;
  x_1[1] = -10000;  y_1[1] = 1;
  TGraph *g1 = new TGraph(2, x_1, y_1);
  
  vector<TString> ids = {"MUON_HN_TRI_TIGHT", "MUON_POG_TIGHT"};
  vector<TString> pus = {"PUCATTools", "PUJohn"};
  
  vector<TString> samples = {"DYJets_10to50", "DYJets"};
  
  vector<TString> cuts = {"OSDiMuon", "OSDiMuon_Z_10GeV"};
  vector<TString> vars = {"mll", "n_vertices", "leadingLepton_Pt", "secondLepton_Pt", "PFMET"};
  
  std::map<TString, TFile*> map_str_to_file;
  map_str_to_file["data"] = new TFile(filepath+"Validation_trilepton_data_DoubleMuon_cat_"+cmssw_version+".root");
  map_str_to_file["data_BCDEF"] = new TFile(filepath+"Validation_trilepton_data_BCDEF_DoubleMuon_cat_"+cmssw_version+".root");
  map_str_to_file["data_GH"] = new TFile(filepath+"Validation_trilepton_data_GH_DoubleMuon_cat_"+cmssw_version+".root");
  
  vector<TString> pers = {"all", "BCDEF", "GH"};
  
  for(unsigned int i_sample=0; i_sample<samples.size(); i_sample++){
    map_str_to_file[samples.at(i_sample)] = new TFile(filepath+"Validation_trilepton_SK"+samples.at(i_sample)+"_dilep_cat_"+cmssw_version+".root");
  }
  
  for(unsigned int i_id=0; i_id<ids.size(); i_id++){
    
    TString thid_id = ids.at(i_id);
    
    for(unsigned int i_cut=0; i_cut<cuts.size(); i_cut++){
      
      TString this_cut = cuts.at(i_cut);
      
      for(unsigned int i_pu=0; i_pu<pus.size(); i_pu++){
        
        TString this_pu = pus.at(i_pu);
        
        for(unsigned int i_var=0; i_var<vars.size(); i_var++){
          
          TString this_var = vars.at(i_var);
          
          for(unsigned int i_per=0; i_per<pers.size(); i_per++){
            
            TString this_per = pers.at(i_per);
            double this_ratio = GetPerRatio(this_per);
            
            TCanvas* c1 = new TCanvas("c_"+thid_id+"_"+this_cut+"_"+this_pu+"_"+this_var, "", 800, 800);
            c1->Draw();
            TPad *c1_up;
            TPad *c1_down;
            
            c1_up = new TPad("c1", "", 0, 0.25, 1, 1);
            c1_down = new TPad("c1_down", "", 0, 0, 1, 0.25);
            
            canvas_margin(c1, c1_up, c1_down);
            
            c1_down->SetGridx();
            c1_down->SetGridy();
            
            c1_up->Draw();
            c1_down->Draw();
            c1_up->cd();
            
            THStack* MC_stacked = new THStack("MC_stacked", "");
            TH1D *hist_for_errorbar;
            for(unsigned int i_sample=0; i_sample<samples.size(); i_sample++){
              TString this_sample = samples.at(i_sample);
              TH1D *hist_tmp = (TH1D*)map_str_to_file[this_sample]->Get(this_var+"_"+thid_id+"_"+this_cut+"_"+this_pu);
              if(this_sample=="DYJets") hist_tmp->Scale(5765.40000/6025.20000);
              hist_tmp->SetFillColor(kYellow+2);
              hist_tmp->SetLineColor(kYellow+2);
              hist_tmp->Scale(this_ratio);
              MC_stacked->Add(hist_tmp);
              if(i_sample==0) hist_for_errorbar = (TH1D*)hist_tmp->Clone();
              else hist_for_errorbar->Add(hist_tmp);
            }
            TString whichData = "data";
            if(this_per=="BCDEF") whichData = "data_BCDEF";
            if(this_per=="GH") whichData = "data_GH";

            TH1D *hist_data = (TH1D*)map_str_to_file[whichData]->Get(this_var+"_"+thid_id+"_"+this_cut+"_"+this_pu);
            hist_data->SetMarkerStyle(3);
            hist_data->SetMarkerSize(1);
            
            hist_for_errorbar->SetMarkerColorAlpha(kAzure-9, 0);
            hist_for_errorbar->SetFillStyle(3004);
            hist_for_errorbar->SetFillColor(kBlue);
            
            //==== empty histogram for axis
            TH1D *hist_empty = (TH1D*)hist_data->Clone();
            hist_empty->Draw("histsame");
            hist_empty->GetYaxis()->SetRangeUser( 0, GetYMaxs(this_var)  );
            hist_empty->GetYaxis()->SetTitle("Events"); //FIXME
            if(this_var=="mll") hist_empty->GetXaxis()->SetRangeUser(75,105);
            if(this_var=="PFMET") hist_empty->GetXaxis()->SetRangeUser(0,150);
            hist_empty->SetLineWidth(0);
            
            MC_stacked->Draw("histsame");
            hist_for_errorbar->Draw("sameE2");
            hist_data->Draw("pe1same");
            
            c1_down->cd();
            TH1D *hist_ratio = (TH1D*)hist_data->Clone();
            TH1D *hist_MC_stacked = (TH1D*)MC_stacked->GetStack()->Last()->Clone();
            hist_ratio->Divide(hist_MC_stacked);
            hist_ratio->GetYaxis()->SetRangeUser(0.8, 1.2);
            //hist_ratio->GetYaxis()->SetRangeUser(0., 2.);
            hist_ratio->GetYaxis()->SetTitle("DATA/MC");
            if(this_var=="mll") hist_ratio->GetXaxis()->SetRangeUser(75,105);
            if(this_var=="PFMET") hist_ratio->GetXaxis()->SetRangeUser(0,150);
            hist_ratio->Draw("pe1same");
            g1->Draw("lsame");
            
            hist_axis(hist_empty, hist_ratio);
            
            gSystem->mkdir(plotpath+this_per, kTRUE);
            c1->SaveAs(plotpath+this_per+"/"+thid_id+"_"+this_cut+"_"+this_pu+"_"+this_var+".pdf");
            c1->Close();
            delete c1;
            
          }
          
        }
      }
      
    }
  }
  
  
}


double GetYMaxs(TString hist){
  
  if(hist=="mll") return 4000000.;
  if(hist=="PFMET") return 1000000;
  else return 2000000;
  //if(hist=="n_vertices") return 2000000;
  
}


double GetPerRatio(TString per){
  
  double lumi_periodB = 5.929001722;
  double lumi_periodC = 2.645968083;
  double lumi_periodD = 4.35344881;
  double lumi_periodE = 4.049732039;
  double lumi_periodF = 3.157020934;
  double lumi_periodG = 7.549615806;
  double lumi_periodH = 8.545039549 + 0.216782873;
  double total_lumi = (lumi_periodB+lumi_periodC + lumi_periodD + lumi_periodE + lumi_periodF + lumi_periodG + lumi_periodH) ;
  
  if(per=="all") return 1.;
  if(per=="BCDEF") return (lumi_periodB+lumi_periodC + lumi_periodD + lumi_periodE + lumi_periodF)/total_lumi;
  if(per=="GH") return (lumi_periodG+lumi_periodH)/total_lumi;
  
  return 1.;
}



