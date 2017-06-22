#include "canvas_margin.h"
#define n_xbins 9
#define n_ybins 4

void syst_FR_make_matrix(){

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);
  gStyle->SetOptStat(0);
  TH1::AddDirectory(kFALSE);
  //gStyle->SetPaintTextFormat("0.4f");

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");  
  TString dataset = getenv("CATANVERSION");

  TString WhichRootFile = "DiMuonTrkVVL";
  TString filepath = WORKING_DIR+"/plots/"+dataset+"/FakeRateCalculator/"+WhichRootFile+"/";
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/FR_syst/"+WhichRootFile+"/";
  
  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
  gSystem->mkdir(plotpath+"/AllFRs/", kTRUE);

  vector<double> dXYMins = {3.0, 4.0, 5.0};
  double bins_dXYMins[] =  {3.0, 4.0, 5.0, 6.0};
  vector<double> RelIsoMaxs = {0.2, 0.3, 0.4, 0.6, 0.8, 1.0};
  double bins_RelIsoMaxs[] =  {0.2, 0.3, 0.4, 0.6, 0.8, 1.0, 1.2};

  vector<TString> jet_conf = {"alljet", "0jet", "withjet", "0bjet", "withbjet"};
  
  TH1D *hist_dXYMins = new TH1D("hist_dXYMins", "", dXYMins.size(), 0., dXYMins.size());
  TH1D *hist_RelIsoMaxs = new TH1D("hist_RelIsoMaxs", "", RelIsoMaxs.size(), 0., RelIsoMaxs.size());
  for(int aaa=0; aaa<dXYMins.size(); aaa++) hist_dXYMins->SetBinContent(aaa+1, dXYMins.at(aaa));
  for(int bbb=0; bbb<RelIsoMaxs.size(); bbb++) hist_RelIsoMaxs->SetBinContent(bbb+1, RelIsoMaxs.at(bbb));

  TFile *file_FRs = new TFile(plotpath+"Trilep_Muon_FakeRate.root", "RECREATE");
  file_FRs->cd();
  hist_dXYMins->Write();
  hist_RelIsoMaxs->Write();
  
  TH2D *hist_n_muons = new TH2D("hist_n_muons", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_HighdXY_FR = new TH2D("hist_HighdXY_FR", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_QCD_FR_Large = new TH2D("hist_QCD_FR_Large", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_QCD_FR_Small = new TH2D("hist_QCD_FR_Small", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_QCD_FRSF = new TH2D("hist_QCD_FRSF", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_TagZ_FR_Large = new TH2D("hist_TagZ_FR_Large", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_TagZ_FR_Small = new TH2D("hist_TagZ_FR_Small", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);
  TH2D *hist_TagZ_FRSF = new TH2D("hist_TagZ_FRSF", "", dXYMins.size(), bins_dXYMins, RelIsoMaxs.size(), bins_RelIsoMaxs);

  vector<TString> xyvars = {"pt_vs_eta", "pt_cone_vs_eta"};

  for(int aaa=0; aaa<dXYMins.size(); aaa++){
    for(int bbb=0; bbb<RelIsoMaxs.size(); bbb++){

      int Digit1 = int(dXYMins.at(aaa));
      int Digit0p1 = 10*dXYMins.at(aaa)-10*Digit1;
      TString str_dXYCut = "dXYSigMin_"+TString::Itoa(Digit1,10)+"p"+TString::Itoa(Digit0p1,10);
      //cout << str_dXYCut << endl;

      int iso_Digit1 = int(RelIsoMaxs[bbb]);
      int iso_Digit0p1 = 10*RelIsoMaxs[bbb]-10*iso_Digit1;
      TString str_iso = "LooseRelIsoMax_"+TString::Itoa(iso_Digit1,10)+"p"+TString::Itoa(iso_Digit0p1,10);

      str_dXYCut = str_dXYCut+"_"+str_iso;
      
      //cout << "["<<str_dXYCut<<"]"<< endl;

      for(unsigned it_xyvars=0;it_xyvars<xyvars.size();it_xyvars++){
        TString xyvar = xyvars.at(it_xyvars);

        for(unsigned int ccc=0; ccc<jet_conf.size(); ccc++){

          TString thisjetconf = jet_conf.at(ccc);

          TFile *file_FR = new TFile(filepath+str_dXYCut+"/13TeV_trimuon_FR_HighdXY_"+thisjetconf+"_"+xyvar+".root");
          TH2D *hist_FR = (TH2D*)file_FR->Get(str_dXYCut+"_HighdXY_"+thisjetconf+"_events_"+xyvar+"_F")->Clone();
          hist_FR->SetName(str_dXYCut+"_FR_"+thisjetconf+"_"+xyvar);

          TFile *file_FR_QCD = new TFile(filepath+str_dXYCut+"/13TeV_trimuon_FR_"+thisjetconf+"_QCD_mu"+"_"+xyvar+".root");
          TH2D *hist_FR_QCD = (TH2D*)file_FR_QCD->Get("FR_Small_dXYSig_"+xyvar)->Clone();
          hist_FR_QCD->SetTitle("");
          hist_FR_QCD->SetName(str_dXYCut+"_FR_QCD_"+thisjetconf+"_"+xyvar);

          TH2D *hist_FRSF_QCD = (TH2D*)file_FR_QCD->Get("FRSF_"+xyvar)->Clone();
          hist_FRSF_QCD->SetName(str_dXYCut+"_FRSF_QCD_"+thisjetconf+"_"+xyvar);

          //==== write files

          file_FRs->cd();

          hist_FR->Write(); 
          hist_FR_QCD->Write();
          hist_FRSF_QCD->Write();

          //==== multiply SF
          TH2D *hist_FR_sfed = (TH2D*)hist_FR->Clone();
          hist_FR_sfed->SetName(str_dXYCut+"_FR_"+thisjetconf+"_sfed"+"_"+xyvar);
          hist_FR_sfed->Multiply(hist_FRSF_QCD);
          hist_FR_sfed->Write();

          if(xyvar=="pt_vs_eta" && thisjetconf=="alljet"){

            //==== 1-binned FR for each wp
            TH1D *hist_n_data_prompt_subtraction_den = (TH1D*)file_FR->Get("hist_n_data_prompt_subtraction_den");
            TH1D *hist_n_data_prompt_subtraction_num = (TH1D*)file_FR->Get("hist_n_data_prompt_subtraction_num");
            hist_n_muons->SetBinContent(aaa+1, bbb+1, hist_n_data_prompt_subtraction_den->GetBinContent(1));
            hist_HighdXY_FR->SetBinContent(aaa+1, bbb+1, hist_n_data_prompt_subtraction_num->GetBinContent(1)/hist_n_data_prompt_subtraction_den->GetBinContent(1));

            //=== 1D QCD Large dXYSig / Small dXYSig / Ratio (Scale Factor)
            TH1D *hist_QCD_tmp = (TH1D*)file_FR_QCD->Get("hist_FR_QCD");
            hist_QCD_FR_Large->SetBinContent(aaa+1, bbb+1, hist_QCD_tmp->GetBinContent(1));
            hist_QCD_FR_Small->SetBinContent(aaa+1, bbb+1, hist_QCD_tmp->GetBinContent(2));
            hist_QCD_FRSF->SetBinContent(aaa+1, bbb+1, hist_QCD_tmp->GetBinContent(3));

          } // fill 1-binned loop

        } // END jet_conf loop

      } // END xyvar loop

      //==== Get TagZ FR/FRSF
      TFile *file_TagZ_tmp = new TFile(filepath+str_dXYCut+"/13TeV_trimuon_FR_DiMuonTrigger_TagZ.root");
      TH1D *hist_TagZ_tmp = (TH1D*)file_TagZ_tmp->Get("hist_FR_TagZ");
      hist_TagZ_FR_Large->SetBinContent(aaa+1, bbb+1, hist_TagZ_tmp->GetBinContent(1));
      hist_TagZ_FR_Small->SetBinContent(aaa+1, bbb+1, hist_TagZ_tmp->GetBinContent(2));
      hist_TagZ_FRSF->SetBinContent(aaa+1, bbb+1, hist_TagZ_tmp->GetBinContent(3));

    }
  }

  //==== HOT FIX FOR DIJET
  for(unsigned it_xyvars=0;it_xyvars<xyvars.size();it_xyvars++){
    TString xyvar = xyvars.at(it_xyvars);

    TFile *file_dijet = new TFile(filepath+"dXYSigMin_4p0_LooseRelIsoMax_0p4/13TeV_trimuon_FR_SingleMuonTrigger_Dijet_"+xyvar+".root");
    TH2D *hist_FR_dijet = (TH2D*)file_dijet->Get("SingleMuonTrigger_Dijet_events_"+xyvar+"_F");
    hist_FR_dijet->SetName("FR_Dijet_"+xyvar);
    file_FRs->cd();
    hist_FR_dijet->Write();
    file_dijet->Close();

  }

  //==== n_muons
  TCanvas *c_n_muons = new TCanvas("c_n_muons", "", 800, 800);
  canvas_margin(c_n_muons);
  c_n_muons->cd();
  hist_n_muons->SetMarkerSize(2);
  hist_n_muons->Draw("coltext");
  hist_n_muons->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_n_muons->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_n_muons);
  //TGaxis::SetMaxDigits(2);
  c_n_muons->SaveAs(plotpath+"n_muons.pdf");

  //==== HighdXY one-binned FR
  TCanvas *c_HighdXY_FR = new TCanvas("c_HighdXY_FR", "", 800, 800);
  canvas_margin(c_HighdXY_FR);
  c_HighdXY_FR->cd();
  gStyle->SetPaintTextFormat("0.2f");
  hist_HighdXY_FR->SetMarkerSize(2);
  hist_HighdXY_FR->Draw("coltext");
  hist_HighdXY_FR->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_HighdXY_FR->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_HighdXY_FR);
  c_HighdXY_FR->SaveAs(plotpath+"/HighdXY_FR.pdf");

  //==== QCD Large
  TCanvas *c_QCD_FR_Large = new TCanvas("c_QCD_FR_Large", "", 800, 800);
  canvas_margin(c_QCD_FR_Large);
  c_QCD_FR_Large->cd();
  hist_QCD_FR_Large->SetMarkerSize(2);
  hist_QCD_FR_Large->Draw("coltext");
  hist_QCD_FR_Large->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_QCD_FR_Large->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_QCD_FR_Large);
  c_QCD_FR_Large->SaveAs(plotpath+"/QCD_FR_Large.pdf");
  //==== QCD Small
  TCanvas *c_QCD_FR_Small = new TCanvas("c_QCD_FR_Small", "", 800, 800);
  canvas_margin(c_QCD_FR_Small);
  c_QCD_FR_Small->cd();
  hist_QCD_FR_Small->SetMarkerSize(2);
  hist_QCD_FR_Small->Draw("coltext");
  hist_QCD_FR_Small->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_QCD_FR_Small->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_QCD_FR_Small);
  c_QCD_FR_Small->SaveAs(plotpath+"/QCD_FR_Small.pdf");
  //==== QCD SF
  TCanvas *c_QCD_FRSF = new TCanvas("c_QCD_FRSF", "", 800, 800);
  canvas_margin(c_QCD_FRSF);
  c_QCD_FRSF->cd();
  hist_QCD_FRSF->SetMarkerSize(2);
  hist_QCD_FRSF->Draw("coltexte1");
  hist_QCD_FRSF->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_QCD_FRSF->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_QCD_FRSF);
  c_QCD_FRSF->SaveAs(plotpath+"/QCD_FRSF.pdf");

  //==== TagZ Large
  TCanvas *c_TagZ_FR_Large = new TCanvas("c_TagZ_FR_Large", "", 800, 800);
  canvas_margin(c_TagZ_FR_Large);
  c_TagZ_FR_Large->cd();
  hist_TagZ_FR_Large->SetMarkerSize(2);
  hist_TagZ_FR_Large->Draw("coltext");
  hist_TagZ_FR_Large->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_TagZ_FR_Large->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_TagZ_FR_Large);
  c_TagZ_FR_Large->SaveAs(plotpath+"/TagZ_FR_Large.pdf");
  //==== TagZ Small
  TCanvas *c_TagZ_FR_Small = new TCanvas("c_TagZ_FR_Small", "", 800, 800);
  canvas_margin(c_TagZ_FR_Small);
  c_TagZ_FR_Small->cd();
  hist_TagZ_FR_Small->SetMarkerSize(2);
  hist_TagZ_FR_Small->Draw("coltexte1");
  hist_TagZ_FR_Small->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_TagZ_FR_Small->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_TagZ_FR_Small);
  c_TagZ_FR_Small->SaveAs(plotpath+"/TagZ_FR_Small.pdf");
  //==== TagZ SF
  TCanvas *c_TagZ_FRSF = new TCanvas("c_TagZ_FRSF", "", 800, 800);
  canvas_margin(c_TagZ_FRSF);
  c_TagZ_FRSF->cd();
  hist_TagZ_FRSF->SetMarkerSize(2);
  hist_TagZ_FRSF->Draw("coltext");
  hist_TagZ_FRSF->GetYaxis()->SetTitle("Maximum Loose Isolation");
  hist_TagZ_FRSF->GetXaxis()->SetTitle("Minimum |dXYSig|");
  hist_axis(hist_TagZ_FRSF);
  c_TagZ_FRSF->SaveAs(plotpath+"/TagZ_FRSF.pdf");




  TList *thislist = file_FRs->GetListOfKeys();
  cout << thislist->Capacity() << endl;
  for(int i=0; i<thislist->Capacity(); i++){
    TString thisname = thislist->At(i)->GetName();
    if(!thisname.Contains("dXY")) continue;
    TH2D *hist = (TH2D*)file_FRs->Get(thisname)->Clone();

    TCanvas *c_thisfr = new TCanvas("c_thisfr", "", 1600, 1100);
    c_thisfr->SetLeftMargin(0.07);
    c_thisfr->SetRightMargin( 0.1 );
    gStyle->SetPaintTextFormat("0.4f");
    hist->Draw("colztexte1");
    hist->GetXaxis()->SetRangeUser(5, 60);
    hist->SetXTitle("p_{T} [GeV/c]");
    hist->SetYTitle("|#eta|");
    hist->SetTitle("");
    hist->SetMarkerSize(1.3); 
    c_thisfr->SaveAs(plotpath+"/AllFRs/"+thisname+".pdf");
    c_thisfr->Close();
    delete c_thisfr;

  }

  file_FRs->Close();


}


