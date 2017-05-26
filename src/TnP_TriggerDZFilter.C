#include "canvas_margin.h"

void TnP_TriggerDZFilter(int period){
  
  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);
  
  bool DrawFitResult = true;
  
  TString DataPeriod = "";
  if(period==1) DataPeriod="BCDEF";
  else if(period==2) DataPeriod="GH";
  else{
    cout << "period wrong" << endl;
    return;
  }

  TString CutsOnDen = "pair_dPhiPrimeDeg_bin0__pair_deltaR_bin0";
  if(DataPeriod=="GH") CutsOnDen = "pair_deltaR_bin0";

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString filepath = WORKING_DIR+"/rootfiles/TnP_Results/Run"+DataPeriod+"/";
  TString plotpath = WORKING_DIR+"/plots/TnP_Results/Run"+DataPeriod+"/TriggerDZFilter/";
  
  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
  
  vector<double> abseta = {0., 0.9, 1.2, 2.1, 2.4};
  
  vector<TString> fitftns = {"vpvPlusExpo", "vpvPlusCheb", "vpvPlusCheb_4th", "gaussPlusExpo", "vpvPlusCMSbeta0p2", "vpvPlusExpo3", "vpvPlusCheb_3rd"};
  
  //vector<TString> triggers = {"DoubleIsoMu17Mu8_IsoMu17leg", "DoubleIsoMu17Mu8_IsoMu8leg", "DoubleIsoMu17TkMu8_IsoMu17leg", "DoubleIsoMu17TkMu8_IsoMu8leg", "Mu8_OR_TkMu8", "test1"};
  vector<TString> triggers = {"DoubleIsoMu17Mu8_IsoMu17leg", "Mu8_OR_TkMu8"};
  
	gSystem->mkdir(plotpath, kTRUE);
	gSystem->mkdir(plotpath+"fitresult/", kTRUE);
	gSystem->mkdir(plotpath+"fitresult/Data", kTRUE);
	gSystem->mkdir(plotpath+"fitresult/MC", kTRUE);

	TFile *file_Trigger_Data = new TFile(filepath+"DZFilter_Data_"+DataPeriod+".root");
	TFile *file_Trigger_MC = new TFile(filepath+"DZFilter_MC_"+DataPeriod+".root");
	
	TString dirname = "tpTree/HN_TRI_TIGHT_TriggerDZFilter_tag_eta_eta/";
	
	//==== FitResult
	if(DrawFitResult){
		for(unsigned int i_eta = 0; i_eta<abseta.size()-1; i_eta++){
      for(unsigned int i_tag_eta = 0; i_tag_eta<abseta.size()-1; i_tag_eta++){
				
				TString dirname_fit_result = "abseta_bin"+TString::Itoa(i_eta,10)+"__"+CutsOnDen+"__tag_abseta_bin"+TString::Itoa(i_tag_eta,10)+"__";
				
				for(unsigned int i_fn = 0; i_fn<fitftns.size(); i_fn++){
					TCanvas *c_data = (TCanvas*)file_Trigger_Data->Get(dirname+dirname_fit_result+fitftns.at(i_fn)+"/fit_canvas");
					if(c_data) c_data->SaveAs(plotpath+"/fitresult/Data/"+dirname_fit_result+fitftns.at(i_fn)+".pdf");
					
					TCanvas *c_MC = (TCanvas*)file_Trigger_MC->Get(dirname+dirname_fit_result+fitftns.at(i_fn)+"/fit_canvas");
					if(c_MC) c_MC->SaveAs(plotpath+"/fitresult/MC/"+dirname_fit_result+fitftns.at(i_fn)+".pdf");
					
				}
				
			}
		}
	}

	TString outputTriggerEffname = "MuonTriggerDZFilterEfficiency_TRILEP_Run"+DataPeriod+".root";
	TFile *file_TriggerEff = new TFile(plotpath+outputTriggerEffname, "RECREATE");

	TString CutsOnDen2 = "pair_dPhiPrimeDeg_bin0_&_pair_deltaR_bin0";
	if(DataPeriod=="GH") CutsOnDen2 = "pair_deltaR_bin0";

	TCanvas *c_TriggerEff_Data = (TCanvas*)file_Trigger_Data->Get(dirname+"fit_eff_plots/abseta_tag_abseta_PLOT_"+CutsOnDen2);
	TCanvas *c_TriggerEff_MC = (TCanvas*)file_Trigger_MC->Get(dirname+"fit_eff_plots/abseta_tag_abseta_PLOT_"+CutsOnDen2);

	TH2F *hist_TriggerEff_Data = (TH2F*)c_TriggerEff_Data->GetPrimitive("abseta_tag_abseta_PLOT_"+CutsOnDen2)->Clone();
	TH2F *hist_TriggerEff_MC = (TH2F*)c_TriggerEff_MC->GetPrimitive("abseta_tag_abseta_PLOT_"+CutsOnDen2)->Clone();

	c_TriggerEff_Data->Close();
	c_TriggerEff_MC->Close();

	hist_TriggerEff_Data->SetName("TriggerDZFilterEff_Trilepton_"+DataPeriod+"_Data");
	hist_TriggerEff_MC->SetName("TriggerDZFilterEff_Trilepton_"+DataPeriod+"_MC");

	file_TriggerEff->cd();
	hist_TriggerEff_Data->Write();
	hist_TriggerEff_MC->Write();
	file_TriggerEff->Close();

	//=== draw MC efficiency 
	TCanvas *c_eff_MC = new TCanvas("c_eff_MC", "", 800, 800);
	canvas_margin(c_eff_MC);
	c_eff_MC->cd();
	gStyle->SetPaintTextFormat("0.4f");
	hist_TriggerEff_MC->Draw("texte1");
	hist_axis(hist_TriggerEff_MC);
	hist_TriggerEff_MC->GetXaxis()->SetTitle("probe muon |#eta|");
	hist_TriggerEff_MC->GetYaxis()->SetTitle("tag muon |#eta|");
	c_eff_MC->SaveAs(plotpath+"/abseta_tag_abseta_MC.pdf");
	c_eff_MC->Close();

	//=== draw Data efficiency 
	TCanvas *c_eff_Data = new TCanvas("c_eff_Data", "", 800, 800);
	canvas_margin(c_eff_Data);
	c_eff_Data->cd();
	gStyle->SetPaintTextFormat("0.4f");
	hist_TriggerEff_Data->Draw("texte1");
	hist_axis(hist_TriggerEff_Data);
	hist_TriggerEff_Data->GetXaxis()->SetTitle("probe muon |#eta|");
	hist_TriggerEff_Data->GetYaxis()->SetTitle("tag muon |#eta|");
	c_eff_Data->SaveAs(plotpath+"/abseta_tag_abseta_Data.pdf");
	//==== ratio
	hist_TriggerEff_Data->Divide(hist_TriggerEff_MC);
	hist_TriggerEff_Data->Draw("texte1");
	c_eff_Data->SaveAs(plotpath+"/abseta_tag_abseta_SF.pdf");
	c_eff_Data->Close();



    
}











