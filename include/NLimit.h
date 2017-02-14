#ifndef NLimit_h
#define NLimit_h

class NLimit {
  public :

  enum source{
    fake = 0,
    prompt = 1,
    signal = 2,
    data = 3,
  };

  enum systsource{
    lumi = 0,
    PDF = 1,
    Q2scale = 2,
    MuonID = 3,
    MuonPt = 4,
    JES = 5,
    JER = 6,
    Uncl = 7,
    btag = 8,
    PU = 9,
    Norm = 10
  };

  double mass;

  double n_fake,   n_stat_fake,   n_syst_fake;
  std::map<systsource, double> fake_systs;

  double n_prompt, n_stat_prompt, n_syst_prompt;
  std::map<systsource, double> prompt_systs;

  double n_signal_gen, n_signal,  n_stat_signal,  n_syst_signal;
  std::map<systsource, double> signal_systs;

  double n_data;

  double err_total(source src);
  static TString SystSourceStr(systsource systsrc);
};

#endif

#ifdef NLimit_cxx
NLimit::NLimit(){

}

NLimit::~NLimit(){

}

#endif // #ifdef NLimit_cxx


