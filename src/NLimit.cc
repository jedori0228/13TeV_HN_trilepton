#include "NLimit.h"

double NLimit::err_total(source src){

  if(src==fake){
    return sqrt(n_stat_fake*n_stat_fake+n_syst_fake*n_syst_fake);
  }
  else if(src==prompt){
    return sqrt(n_stat_prompt*n_stat_prompt+n_syst_prompt*n_syst_prompt);
  }
  else if(src==signal){
    return sqrt(n_stat_signal*n_stat_signal+n_syst_signal*n_syst_signal);
  }
  else if(src==data){
    return sqrt(n_data);
  }
  else{
    cout << "[ERROR][NLimit::err_total(TString src)] Wrong source" << endl;
    return 0.;
  }

}

TString NLimit::SystSourceStr(systsource systsrc){

  if(systsrc==lumi) return "Luminosity";
  else if(systsrc==PDF) return "PDF";
  else if(systsrc==Q2scale) return "Q2Scale";
  else if(systsrc==MuonID) return "MuonID";
  else if(systsrc==MuonPt) return "MuonPt";
  else if(systsrc==JES) return "JES";
  else if(systsrc==JER) return "JER";
  else if(systsrc==Uncl) return "UnclusteredE";
  else if(systsrc==btag) return "Btagging";
  else if(systsrc==PU) return "PUModel";
  else if(systsrc==Norm) return "MCNormalization";
  else return "Wrong";

}

void NLimit::CheckZeroEntry(){

  if(n_fake==0){
    n_stat_fake = 0;
    n_syst_fake = 0;
    for(std::map<systsource, double>::iterator it=fake_systs.begin(); it!=fake_systs.end(); it++){
      it->second = 0;
    }
  }
  if(n_prompt==0){
    n_stat_prompt = 0;
    n_syst_prompt = 0;
    for(std::map<systsource, double>::iterator it=prompt_systs.begin(); it!=prompt_systs.end(); it++){
      it->second = 0;
    }
  }
  if(n_signal==0){
    n_stat_signal = 0;
    n_syst_signal = 0;
    for(std::map<systsource, double>::iterator it=signal_systs.begin(); it!=signal_systs.end(); it++){
      it->second = 0;
    }
  }

}

