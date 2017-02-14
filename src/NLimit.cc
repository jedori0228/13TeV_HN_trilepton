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

