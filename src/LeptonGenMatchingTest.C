#include "mylib.h"
#include "canvas_margin.h"

void LeptonGenMatchingTest(){

  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");

  vector<TString> samples = {
  "DYJets", "TTJets_aMC", "ZGto2LG", //"DYJets_MG",
  "TTLL_powheg",
  "WJets", "WGtoLNuG",
  "WZTo3LNu_powheg", "ZZTo4L_powheg"
  };
  vector<int> FakeTypes, PromptTypes;

  //TString histname = "TEST_Electron_GetType_F";
  TString histname = "TEST_Muon_GetType_F";

  TString filepath = WORKING_DIR+"/rootfiles/"+dataset+"/LeptonGenMatchingTest/";
  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/LeptonGenMatchingTest/";

  plotpath += histname+"/";

  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  double FakeMax = 0.30, PromptMin = 0.35;
  double FakeRelErrorMax = 0.5;
  int MinEntryDen = 1000;

  for(unsigned int i=0; i<samples.size(); i++){

    TString sample = samples.at(i);
    cout << "["<<sample<<"]" << endl;

    TString filename = "trilepton_mumumu_SK"+sample+"_cat_v8-0-7.root";
    TFile *file = new TFile(filepath+filename);

    TH1D* hist_F0 = (TH1D*)file->Get(histname+"0");
    vector<bool> EnoughStat;
    TH1D* hist_F = (TH1D*)file->Get(histname);
    for(unsigned j=1; j<=hist_F->GetXaxis()->GetNbins(); j++){
      if(hist_F->GetBinContent(j) > MinEntryDen) EnoughStat.push_back(true);
      else EnoughStat.push_back(false);
    }

    hist_F->Divide(hist_F0);
    hist_F->GetXaxis()->SetRangeUser(0., 50.);
    hist_F->GetYaxis()->SetRangeUser(0., 1.);

    //==== Find Fake
    cout << "---- FR < "<<FakeMax<<" ----" << endl;
    for(unsigned j=1; j<=hist_F->GetXaxis()->GetNbins(); j++){
      if(!EnoughStat.at(j-1)) continue;
      if( hist_F->GetBinContent(j) != 0 && hist_F->GetBinContent(j) < FakeMax && hist_F->GetBinError(j)/hist_F->GetBinContent(j) < FakeRelErrorMax){
        cout << j-1 << "\t" << std::fixed<<std::setprecision(3)<< hist_F->GetBinContent(j) << "\t" << std::fixed<<std::setprecision(3)<< hist_F->GetBinError(j) << endl;
        if(std::find(FakeTypes.begin(), FakeTypes.end(), j-1)==FakeTypes.end()) FakeTypes.push_back(j-1);
      }
    }

    //==== Find Prompt
    cout << "---- FR > "<<PromptMin<<" ----" << endl;
    for(unsigned j=1; j<=hist_F->GetXaxis()->GetNbins(); j++){
      if(!EnoughStat.at(j-1)) continue;
      if( hist_F->GetBinContent(j) > PromptMin && hist_F->GetBinError(j)/hist_F->GetBinContent(j) < FakeRelErrorMax){
        cout << j-1 << "\t" << std::fixed<<std::setprecision(3)<< hist_F->GetBinContent(j) << "\t" << std::fixed<<std::setprecision(3)<< hist_F->GetBinError(j) << endl;
        if(std::find(PromptTypes.begin(), PromptTypes.end(), j-1)==PromptTypes.end()) PromptTypes.push_back(j-1);
      }
    }

    TCanvas *c1 = new TCanvas("c1", "", 800, 800);
    c1->cd();
    hist_F->Draw("text");
    c1->SaveAs(plotpath+"/FR_"+sample+".pdf");

    c1->Close();

  }

  cout << "#### Fake Found ####" << endl;
  std::sort(FakeTypes.begin(), FakeTypes.end(), std::less<int>());
  for(unsigned int i=0; i<FakeTypes.size(); i++){
    if(i!=FakeTypes.size()-1) cout << FakeTypes.at(i)<<", ";
    else cout << FakeTypes.at(i)<<endl;
  }

  cout << "#### Prompt Found ####" << endl;
  std::sort(PromptTypes.begin(), PromptTypes.end(), std::less<int>());
  for(unsigned int i=0; i<PromptTypes.size(); i++){
    if(i!=PromptTypes.size()-1) cout << PromptTypes.at(i)<<", ";
    else cout << PromptTypes.at(i)<<endl;
  }


  for(unsigned int i=0; i<samples.size(); i++){

    TString sample = samples.at(i);
    cout << "["<<sample<<"]" << endl;

    TString filename = "trilepton_mumumu_SK"+sample+"_cat_v8-0-7.root";
    TFile *file = new TFile(filepath+filename);

    TH1D* hist_F0 = (TH1D*)file->Get(histname+"0");
    TH1D* hist_F = (TH1D*)file->Get(histname);

    hist_F->Divide(hist_F0);

    cout << "---- FR < "<<FakeMax<<" ----" << endl;
    for(unsigned int k=0; k<FakeTypes.size(); k++){
      int j = FakeTypes.at(k)+1;
      cout << j-1 << "\t" << std::fixed<<std::setprecision(3)<< hist_F->GetBinContent(j) << "\t" << std::fixed<<std::setprecision(3)<< hist_F->GetBinError(j) << endl;
    }

    //==== Find Prompt
    cout << "---- FR > "<<PromptMin<<" ----" << endl;
    for(unsigned int k=0; k<PromptTypes.size(); k++){
      int j = PromptTypes.at(k)+1;
      cout << j-1 << "\t" << std::fixed<<std::setprecision(3)<< hist_F->GetBinContent(j) << "\t" << std::fixed<<std::setprecision(3)<< hist_F->GetBinError(j) << endl;
    }


  }
}
