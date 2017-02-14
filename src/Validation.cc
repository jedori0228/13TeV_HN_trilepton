#include "canvas_margin.h"

void Validation(){
  
  gStyle->SetOptStat(0);
  
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
  
  vector<TString> ids = {"MUON_HN_TRI_TIGHT", "MUON_POG_TIGHT"};
  vector<TString> pus = {"PUCATTools", "PUJohn"};
  
  vector<TString> samples = {"DYJets_10to50", "DYJets"};
  
  vector<TString> cuts = {"OSDiMuon", "OSDiMuon_Z_10GeV"};
  vector<TString> vars = {"mll", "n_vertices"};
  
  std::map<TString, TFile*> map_str_to_file;
  map_str_to_file["data"] = new TFile(filepath+"Validation_trilepton_data_DoubleMuon_cat_"+cmssw_version+".root");
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
          for(unsigned int i_sample=0; i_sample<samples.size(); i_sample++){
            TString this_sample = samples.at(i_sample);
            TH1D *hist_tmp = (TH1D*)map_str_to_file[this_sample]->Get(this_var+"_"+thid_id+"_"+this_cut+"_"+this_pu);
            hist_tmp->SetFillColor(kYellow+2);
            hist_tmp->SetLineColor(kYellow+2);
            MC_stacked->Add(hist_tmp);
          }
          TH1D *hist_data = (TH1D*)map_str_to_file["data"]->Get(this_var+"_"+thid_id+"_"+this_cut+"_"+this_pu);
          hist_data->SetMarkerStyle(3);
          hist_data->SetMarkerSize(1);
          
          //==== empty histogram for axis
          TH1D *hist_empty = (TH1D*)hist_data->Clone();
          hist_empty->Draw("histsame");
          hist_empty->GetYaxis()->SetRangeUser( 0, 4000000  );
          hist_empty->GetYaxis()->SetTitle("Events"); //FIXME
          hist_empty->SetLineWidth(0);
          
          MC_stacked->Draw("histsame");
          hist_data->Draw("pe1same");
          
          c1_down->cd();
          TH1D *hist_ratio = (TH1D*)hist_data->Clone();
          TH1D *hist_MC_stacked = (TH1D*)MC_stacked->GetStack()->Last()->Clone();
          hist_ratio->Divide(hist_MC_stacked);
          hist_ratio->GetYaxis()->SetRangeUser(0.5, 1.5);
          hist_ratio->Draw("pe1same");
          
          hist_axis(hist_empty, hist_ratio);
          
          c1->SaveAs(plotpath+thid_id+"_"+this_cut+"_"+this_pu+"_"+this_var+".png");
          
        }
      }
      
    }
  }
  
  
}
