#define cutop_cxx
#include "cutop.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void cutop::Loop()
{
  
  Long64_t nentries = fChain->GetEntriesFast();
  
  n_weighted = 0.;
  n_unweighted = 0;

  hist_for_error = new TH1D("hist_for_error", "", 1, 0., 1.);
  hist_for_error_up = new TH1D("hist_for_error_up", "", 1, 0., 1.);

  //if(BVeto) cout << "[cutop::Loop] Using b-jet veto" << endl;
  //else cout << "[cutop::Loop] NOT using b-jet veto" << endl;
  
  for(Long64_t jentry=0; jentry<nentries;jentry++) {
    GetEntry(jentry);
    bool pass = true;
    weight *= (MCNormSF+MCNormSF_uncert);

    if(!isSearchRegion()) continue;

    if(SearchRegion=="Preselection"){
      if(signalclass==1 || signalclass==2){
        if( ! (first_pt < cut_first_pt) ) continue;
        if( ! (second_pt < cut_second_pt) ) continue;
        if( ! (third_pt < cut_third_pt) ) continue;
        if( ! (W_pri_lowmass_mass < cut_W_pri_mass) ) continue;
        if( ! (HN_mass_by_signalclass() < cut_HN_mass ) ) continue;
        if( ! (deltaR_OS_min > cut_deltaR_OS_min) ) continue;
        if( ! (gamma_star_mass > cut_gamma_star_mass) ) continue;
      }
      else{
        if( ! (first_pt > cut_first_pt) ) continue;
        if( ! (second_pt > cut_second_pt) ) continue;
        if( ! (third_pt > cut_third_pt) ) continue;
        if( ! (W_pri_highmass_mass > cut_W_pri_mass) ) continue;
        if( ! (PFMET > cut_PFMET) ) continue;
        //if( ! (W_sec_highmass_mass < cut_W_sec_mass) ) continue;
      }
    }
    
    n_weighted += weight;
    n_unweighted++;

    hist_for_error->Fill(0. ,weight);
    hist_for_error_up->Fill(0., weight+weight_err);

    
  }
  
}

Double_t cutop::HN_mass_by_signalclass(){
  
  if(signalclass==1) return HN_1_mass;
  else if(signalclass==2) return HN_2_mass;
  else if(signalclass==3) return HN_3_mass;
  else return HN_4_mass;
  
}

bool cutop::isSearchRegion(){

  bool pass = false;

  //==== Preselection
  if(SearchRegion=="Preselection"){
    if(isPreselection==1) pass = true;
  }

  //==== WZ
  else if(SearchRegion=="WZ"){
    if(isWZ==1) pass = true;
  }
  else if(SearchRegion=="WZ_3mu0el"){
    if(isWZ==1 && ThreeLeptonConfig==0) pass = true;
  }
  else if(SearchRegion=="WZ_2mu1el"){
    if(isWZ==1 && ThreeLeptonConfig==1) pass = true;
  }

  //==== Z+lep+lowMET
  else if(SearchRegion=="ZJets"){
    if(isZJets==1) pass = true;
  }
  else if(SearchRegion=="ZJets_3mu0el"){
    if(isZJets==1 && ThreeLeptonConfig==0) pass = true;
  }

  //==== ZZ
  else if(SearchRegion=="ZZ"){
    if(isZZ==1) pass = true;
  }
  else if(SearchRegion=="ZZ_4mu0el"){
    if(isZZ==1 && FourLeptonConfig==0) pass = true;
  }
  else if(SearchRegion=="ZZ_2mu2el"){
    if(isZZ==1 && FourLeptonConfig==1) pass = true;
  }

  //==== ZGamma
  else if(SearchRegion=="ZGamma"){
    if(isZGamma==1) pass = true;
  }
  else if(SearchRegion=="ZGamma_3mu0el"){
    if(isZGamma==1 && ThreeLeptonConfig==0) pass = true;
  }
  else if(SearchRegion=="ZGamma_2mu1el"){
    if(isZGamma==1 && ThreeLeptonConfig==1) pass = true;
  }

  //==== ?
  else{
    cout << "[cutop::isSearchRegion] ?" << endl;
  }

  //==== finally, do b-jet beto
  //==== but, don't do that for ZZ
  if(BVeto){
    if(!SearchRegion.Contains("ZZ")){
      pass &= (nbjets==0);
    }
  }

  return pass;

}









