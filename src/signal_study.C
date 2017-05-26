#define n_all 17
#define n_high 9

#include "canvas_margin.h"
#include "TSystem.h"
#include "mylib.h"

void signal_study(){

  gStyle->SetOptStat(0);
  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString catversion = getenv("CATVERSION");
  TString dataset = getenv("CATANVERSION");

  TString plotpath = WORKING_DIR+"/plots/"+dataset+"/signal_study";
  
  if( !gSystem->mkdir(plotpath, kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }
  
  double x_all[n_all] = {
    5, 10, 20, 30, 40, 50, 60, 70,
    90, 100, 150, 200, 300, 400, 500, 700, 1000
  };
  double xmax_pt[n_all] = {
    100, 100, 100, 100, 100, 100, 100, 100,
    200, 200, 300, 400, 500, 600, 700, 800, 900
  };
  double x_high[n_high] = {90, 100, 150, 200, 300, 400, 500, 700, 1000};
  
  double y_max_dR[n_all-n_high] = {20, 100, 120, 160, 300, 400, 500, 300};
  
  //==== Primary Lepton pT relations w.r.t. SS lepton
  double y_reco_leading_SS_match_gen_l_1[n_all], y_reco_subleading_SS_match_gen_l_1[n_all], y_reco_SS_match_gen_l_1[n_all];
  double y_reco_leading_match_gen_l_1[n_all];
  double y_reco_leading_match_gen_l_2[n_all];
  double y_gen_l_1_leadingSS[n_all], y_gen_l_1_subleadingSS[n_all];
  double y_gen_l_1_leading[n_all], y_gen_l_2_leading[n_all];
  //==== For high mass, check two methods
  double y_gen_l_1_first[n_high], y_gen_l_1_first_mlmet_next[n_high], y_gen_l_1_first_mlmet_next_total[n_high];
  double y_gen_l_3_first[n_high];
  double y_gen_l_3_first_gen_l_1_leading_next[n_high], y_gen_l_3_first_gen_l_1_leading_next_total[n_high];
  double y_gen_l_3_first_gen_l_2_leading_next[n_high], y_gen_l_3_first_gen_l_2_leading_next_total[n_high];
  double y_gen_l_3_first_pt_order_next[n_high], y_gen_l_3_first_pt_order_next_total[n_high];
  
  for(unsigned int i = 0, j = 0; i < n_all; i++){
    
    //cout << x_all[i] << endl;
    
    TFile* file = new TFile(WORKING_DIR+"/rootfiles/"+dataset+"/SR/trilepton_mumumu_SKHN_MuMuMu_"+TString::Itoa(x_all[i], 10)+"_cat_"+catversion+".root");
    
    //==== Gen All Found?
    TH1D *hist_GEN_all_found = (TH1D*)file->Get("GenFound");
    double n_found = hist_GEN_all_found->GetMean();
    if(n_found!=1.) cout << "HN "<<x_all[i]<<" GEN NOT ALL FOUND! : "<< n_found << endl;
    
    //==== Get Probabilities
    TH1D *hist_reco_leading_SS_match_gen_l_1 = (TH1D*)file->Get("GEN_reco_leading_SS_match_gen_l_1");
    TH1D *hist_reco_subleading_SS_match_gen_l_1 = (TH1D*)file->Get("GEN_reco_subleading_SS_match_gen_l_1");
    TH1D *hist_reco_leading_match_gen_l_1 = (TH1D*)file->Get("GEN_reco_leading_match_gen_l_1");
    TH1D *hist_reco_leading_match_gen_l_2 = (TH1D*)file->Get("GEN_reco_leading_match_gen_l_2");
    TH1D *hist_gen_l_1_leadingSS = (TH1D*)file->Get("GEN_gen_l_1_leadingSS");
    TH1D *hist_gen_l_1_leading = (TH1D*)file->Get("GEN_gen_l_1_leading");
    TH1D *hist_gen_l_2_leading = (TH1D*)file->Get("GEN_gen_l_2_leading");
    
    y_reco_leading_SS_match_gen_l_1[i] = hist_reco_leading_SS_match_gen_l_1->GetMean();
    y_reco_subleading_SS_match_gen_l_1[i] = hist_reco_subleading_SS_match_gen_l_1->GetMean();
    y_reco_SS_match_gen_l_1[i] = y_reco_leading_SS_match_gen_l_1[i]+y_reco_subleading_SS_match_gen_l_1[i];
    
    y_reco_leading_match_gen_l_1[i] = hist_reco_leading_match_gen_l_1->GetMean();
    y_reco_leading_match_gen_l_2[i] = hist_reco_leading_match_gen_l_2->GetMean();
    
    y_gen_l_1_leadingSS[i] = hist_gen_l_1_leadingSS->GetMean();
    y_gen_l_1_subleadingSS[i] = 1-y_gen_l_1_leadingSS[i];
    y_gen_l_1_leading[i] = hist_gen_l_1_leading->GetMean();
    y_gen_l_2_leading[i] = hist_gen_l_2_leading->GetMean();

    //==== gen kinematics
    TCanvas *c_pt = new TCanvas("c_pt", "", 800, 800);
    canvas_margin(c_pt);
    Color_t ptcolor[] = {kBlack, kBlue, kRed};
    c_pt->cd();
    TLegend *lg_pt = new TLegend(0.7, 0.75, 0.90, 0.90);
    lg_pt->SetBorderSize(0);
    lg_pt->SetFillStyle(0);
    for(int aaa=0; aaa<3; aaa++){
      TString index = TString::Itoa(aaa+1,10);
      TH1D *hist_pt = (TH1D*)file->Get("GEN_gen_l_"+index+"_Pt");

      if(x_all[i]>80) hist_pt->Rebin(10);
      hist_pt->SetTitle("");
      hist_pt->Scale( 1./ hist_pt->GetMaximum());
      hist_pt->SetLineColor(ptcolor[aaa]);
      hist_pt->SetLineWidth(3);
      hist_pt->GetXaxis()->SetRangeUser(0, xmax_pt[i]);
      lg_pt->AddEntry(hist_pt, "GEN l_{"+index+"}", "l");
      hist_pt->Draw("histsame");
    }
    TH1D *hist_SS = (TH1D*)file->Get("GEN_gen_SS_Pt");
    hist_SS->SetLineColor(kGreen);
    hist_SS->SetLineWidth(3);
    if(x_all[i]>80) hist_SS->Rebin(10);
    hist_SS->Scale( 1./ hist_SS->GetMaximum());
    hist_SS->Draw("histsame");
    lg_pt->AddEntry(hist_SS, "GEN SS", "l");
    lg_pt->Draw();
    c_pt->SaveAs(plotpath+"/Pts_HN"+TString::Itoa(int(x_all[i]), 10)+".pdf");
    c_pt->Close();
    delete c_pt;

    
    //==== High mass
    if(x_all[i] > 80){
      
      //==== 1) gen_l_1 first by pt
      TH1D *hist_gen_l_1_first = (TH1D*)file->Get("GEN_highmass_gen_l_1_first");
      TH1D *hist_gen_l_1_first_mlmet_next = (TH1D*)file->Get("GEN_highmass_gen_l_1_first_mlmet_next");
      y_gen_l_1_first[j] = hist_gen_l_1_first->GetMean();
      y_gen_l_1_first_mlmet_next[j] = hist_gen_l_1_first_mlmet_next->GetMean();
      y_gen_l_1_first_mlmet_next_total[j] = y_gen_l_1_first[j] * y_gen_l_1_first_mlmet_next[j];
      
      //==== 2-1)
      //==== gen_l_3 first by mlmet
      //==== gen_l_1 leading (ignore charge)
      TH1D *hist_gen_l_3_first = (TH1D*)file->Get("GEN_highmass_gen_l_3_first");
      TH1D *hist_gen_l_3_first_gen_l_1_leading_next = (TH1D*)file->Get("GEN_highmass_gen_l_3_first_gen_l_1_leading_next");
      y_gen_l_3_first[j] = hist_gen_l_3_first->GetMean();
      y_gen_l_3_first_gen_l_1_leading_next[j] = hist_gen_l_3_first_gen_l_1_leading_next->GetMean();
      y_gen_l_3_first_gen_l_1_leading_next_total[j] = y_gen_l_3_first[j] * y_gen_l_3_first_gen_l_1_leading_next[j];
      
      //==== 2-2)
      //==== gen_l_3 first by mlmet
      //==== gen_l_2 leading (ifnore charge)
      TH1D *hist_gen_l_3_first_gen_l_2_leading_next = (TH1D*)file->Get("GEN_highmass_gen_l_3_first_gen_l_2_leading_next");
      y_gen_l_3_first[j] = hist_gen_l_3_first->GetMean();
      y_gen_l_3_first_gen_l_2_leading_next[j] = hist_gen_l_3_first_gen_l_2_leading_next->GetMean();
      y_gen_l_3_first_gen_l_2_leading_next_total[j] = y_gen_l_3_first[j] * y_gen_l_3_first_gen_l_2_leading_next[j];
      
      //==== 2-3)
      //==== gen_l_3 first by mlmet
      //==== combination of pt ordering, based on 2-1) and 2-2) results
      //TH1D *hist_gen_l_3_first = (TH1D*)file->Get("GEN_highmass_gen_l_3_first");
      TH1D *hist_gen_l_3_first_pt_order_next = (TH1D*)file->Get("GEN_highmass_gen_l_3_first_pt_order_next");
      y_gen_l_3_first[j] = hist_gen_l_3_first->GetMean();
      y_gen_l_3_first_pt_order_next[j] = hist_gen_l_3_first_pt_order_next->GetMean();
      y_gen_l_3_first_pt_order_next_total[j] = y_gen_l_3_first[j] * y_gen_l_3_first_pt_order_next[j];
      
      j++;
    }
    
    else{

      //==== dR(sol, true)
      TCanvas *c_dR = new TCanvas("c_dR", "", 800, 800);
      canvas_margin(c_dR);
      c_dR->cd();
      
      TH1D *hist_GEN_solsel_dR_best = (TH1D*)file->Get("GEN_solsel_dR_best");
      TH1D *hist_GEN_solsel_dR_plus = (TH1D*)file->Get("GEN_solsel_dR_plus");
      TH1D *hist_GEN_solsel_dR_minus = (TH1D*)file->Get("GEN_solsel_dR_minus");
      TH1D *hist_GEN_solsel_dR_smaller = (TH1D*)file->Get("GEN_solsel_dR_smaller");
      TH1D *hist_GEN_solsel_dR_larger = (TH1D*)file->Get("GEN_solsel_dR_larger");
      
      hist_GEN_solsel_dR_best->Rebin(5);
      hist_GEN_solsel_dR_plus->Rebin(5);
      hist_GEN_solsel_dR_minus->Rebin(5);
      hist_GEN_solsel_dR_smaller->Rebin(5);
      hist_GEN_solsel_dR_larger->Rebin(5);
      
      hist_GEN_solsel_dR_best->SetLineColor(kBlack);
      hist_GEN_solsel_dR_plus->SetLineColor(kBlue);
      hist_GEN_solsel_dR_minus->SetLineColor(kBlue);
      hist_GEN_solsel_dR_minus->SetLineStyle(3);
      hist_GEN_solsel_dR_smaller->SetLineColor(kRed);
      hist_GEN_solsel_dR_larger->SetLineColor(kRed);
      hist_GEN_solsel_dR_larger->SetLineStyle(3);
      
      hist_GEN_solsel_dR_best->SetLineWidth(2);
      hist_GEN_solsel_dR_plus->SetLineWidth(3);
      hist_GEN_solsel_dR_minus->SetLineWidth(2);
      hist_GEN_solsel_dR_smaller->SetLineWidth(3);
      hist_GEN_solsel_dR_larger->SetLineWidth(2);
      
      hist_GEN_solsel_dR_best->Draw("histsame");
      hist_GEN_solsel_dR_plus->Draw("histsame");
      hist_GEN_solsel_dR_minus->Draw("histsame");
      hist_GEN_solsel_dR_smaller->Draw("histsame");
      hist_GEN_solsel_dR_larger->Draw("histsame");
      
      TLegend *lg = new TLegend(0.7, 0.65, 0.98, 0.95);
      lg->SetBorderSize(0);
      lg->SetFillStyle(0);
      lg->AddEntry(hist_GEN_solsel_dR_best, "Best", "l");
      lg->AddEntry(hist_GEN_solsel_dR_plus, "Plus", "l");
      lg->AddEntry(hist_GEN_solsel_dR_minus, "Minus", "l");
      lg->AddEntry(hist_GEN_solsel_dR_smaller, "Smaller", "l");
      lg->AddEntry(hist_GEN_solsel_dR_larger, "Larger", "l");
      lg->Draw();
      
      hist_GEN_solsel_dR_best->GetYaxis()->SetRangeUser(0, y_max_dR[i]);
      hist_GEN_solsel_dR_best->GetYaxis()->SetTitle("Events");
      hist_GEN_solsel_dR_best->GetXaxis()->SetTitle("#DeltaR(#nu_{True}, #nu_{Sol})");
      hist_axis(hist_GEN_solsel_dR_best);
      
      c_dR->SaveAs(plotpath+"/dR_nus_HN"+TString::Itoa(int(x_all[i]), 10)+".pdf");
      c_dR->Close();
      delete c_dR;

    }
    
    
  }
  
  double xx[2] = {0, 2000}, yy[2] = {0.5, 0.5};
  TGraph* gr_05 = new TGraph(2, xx, yy);
  
  //==================================================================
  //==== Reco Leading SS matched to gen l_1 (both low and high mass)
  //==================================================================
  TCanvas* c1 = new TCanvas("c1", "", 1000, 1000);
  gPad->SetLogx();
  c1->SetGrid();
  canvas_margin(c1);
  c1->cd();
  //==== leading is matched to gen_l_1
  TGraph* gr_reco_leading_SS_match_gen_l_1 = new TGraph(n_all, x_all, y_reco_leading_SS_match_gen_l_1);
  gr_reco_leading_SS_match_gen_l_1->Draw("alp");
  gr_reco_leading_SS_match_gen_l_1->SetMarkerStyle(21);
  gr_reco_leading_SS_match_gen_l_1->SetMarkerColor(kBlue);
  gr_reco_leading_SS_match_gen_l_1->SetLineWidth(3);
  gr_reco_leading_SS_match_gen_l_1->SetLineColor(kBlue);
  gr_reco_leading_SS_match_gen_l_1->GetYaxis()->SetRangeUser(0, 1);
  gr_reco_leading_SS_match_gen_l_1->GetYaxis()->SetTitle("Probability");
  gr_reco_leading_SS_match_gen_l_1->GetXaxis()->SetRangeUser(1, 2000);
  gr_reco_leading_SS_match_gen_l_1->GetXaxis()->SetTitle("m(HN) [GeV]");
  hist_axis(gr_reco_leading_SS_match_gen_l_1);
  //==== GEN gen_l_1 leadingSS
  TGraph* gr_gen_l_1_leadingSS = new TGraph(n_all, x_all, y_gen_l_1_leadingSS);
  gr_gen_l_1_leadingSS->SetMarkerStyle(21);
  gr_gen_l_1_leadingSS->SetMarkerColor(kBlue);
  gr_gen_l_1_leadingSS->SetLineWidth(3);
  gr_gen_l_1_leadingSS->SetLineColor(kBlue);
  gr_gen_l_1_leadingSS->SetLineStyle(3);
  gr_gen_l_1_leadingSS->Draw("lpsame");
  //==== subleading is matched to gen_l_1
  TGraph* gr_reco_subleading_SS_match_gen_l_1 = new TGraph(n_all, x_all, y_reco_subleading_SS_match_gen_l_1);
  gr_reco_subleading_SS_match_gen_l_1->SetMarkerStyle(21);
  gr_reco_subleading_SS_match_gen_l_1->SetMarkerColor(kRed);
  gr_reco_subleading_SS_match_gen_l_1->SetLineWidth(3);
  gr_reco_subleading_SS_match_gen_l_1->SetLineColor(kRed);
  gr_reco_subleading_SS_match_gen_l_1->Draw("lpsame");
  // GEN gen_l_1 subleadingSS
  TGraph* gr_gen_l_1_subleadingSS = new TGraph(n_all, x_all, y_gen_l_1_subleadingSS);
  gr_gen_l_1_subleadingSS->SetMarkerStyle(21);
  gr_gen_l_1_subleadingSS->SetMarkerColor(kRed);
  gr_gen_l_1_subleadingSS->SetLineColor(kRed);
  gr_gen_l_1_subleadingSS->SetLineWidth(3);
  gr_gen_l_1_subleadingSS->SetLineStyle(3);
  gr_gen_l_1_subleadingSS->Draw("lpsame");
  //==== gen_l_1 leading
  TGraph* gr_gen_l_1_leading = new TGraph(n_all, x_all, y_gen_l_1_leading);
  gr_gen_l_1_leading->SetMarkerStyle(21);
  gr_gen_l_1_leading->SetMarkerColor(kBlack);
  gr_gen_l_1_leading->SetLineWidth(3);
  gr_gen_l_1_leading->SetLineColor(kBlack);
  gr_gen_l_1_leading->SetLineStyle(3);
  gr_gen_l_1_leading->Draw("lpsame");
  //==== legend
  TLegend *lg1 = new TLegend(0.17, 0.45, 0.50, 0.65);
  lg1->AddEntry(gr_reco_leading_SS_match_gen_l_1, "GEN l_{1} - RECO Leading SS", "lp");
  lg1->AddEntry(gr_gen_l_1_leadingSS, "GEN l_{1} : Leading SS", "lp");
  lg1->AddEntry(gr_reco_subleading_SS_match_gen_l_1, "GEN l_{1} - RECO SubLeading SS", "lp");
  lg1->AddEntry(gr_gen_l_1_subleadingSS, "GEN l_{1} : SubLeading SS", "lp");
  lg1->AddEntry(gr_gen_l_1_leading, "GEN l_{1} : Leading", "lp");
  lg1->Draw();
  //==== 0.5 line
  //gr_05->Draw("same");
  //==== print values
  cout << "====== pt order ======" << endl;
  for(int i=0; i<n_all; i++){
    //cout << x_all[i] << '\t' << y_reco_leading_SS_match_gen_l_1[i]*100. << '\t' << y_reco_subleading_SS_match_gen_l_1[i]*100. << endl;
    //cout << x_all[i] << '\t' << y_gen_l_1_leadingSS[i]*100. << '\t' << 100.-y_gen_l_1_leadingSS[i]*100. << endl;
    cout << int(x_all[i]) << " & " << std::fixed<<std::setprecision(1) << y_gen_l_1_leadingSS[i]*100. << " & " << std::fixed<<std::setprecision(1) << 100.-y_gen_l_1_leadingSS[i]*100. << " \\\\" << endl;
    if(x_all[i]==70) cout << "\\hline" << endl;
  }
  //cout << "1) leading it matched to gen_l_1" << endl;
  //gr_reco_leading_SS_match_gen_l_1->Print();
  //cout << "2) subleading is matched to gen_l_1" << endl;
  //gr_reco_subleading_SS_match_gen_l_1->Print();
  //==== save canvas
  c1->SaveAs(plotpath+"/gen_l_1_assigning.pdf");
  c1->Close();
  
  //==================================================================
  //==== Reco Leading matched to gen (both low and high mass)
  //==================================================================
  TCanvas* c2 = new TCanvas("c2", "", 1000, 1000);
  gPad->SetLogx();
  c2->SetGrid();
  canvas_margin(c2);
  c2->cd();
  //==== gen_l_1 leading
  gr_gen_l_1_leading->SetMarkerStyle(21);
  gr_gen_l_1_leading->SetMarkerColor(kBlack);
  gr_gen_l_1_leading->SetLineColor(kBlack);
  gr_gen_l_1_leading->SetLineWidth(3);
  gr_gen_l_1_leading->SetLineStyle(3);
  gr_gen_l_1_leading->Draw("alp");
  gr_gen_l_1_leading->GetYaxis()->SetRangeUser(0, 1);
  gr_gen_l_1_leading->GetYaxis()->SetTitle("Probability");
  gr_gen_l_1_leading->GetXaxis()->SetRangeUser(1, 2000);
  gr_gen_l_1_leading->GetXaxis()->SetTitle("m(HN) [GeV]");
  hist_axis(gr_gen_l_1_leading);
  //==== RECO Leading - GEN_l_1
  TGraph *gr_reco_leading_match_gen_l_1 = new TGraph(n_all, x_all, y_reco_leading_match_gen_l_1);
  gr_reco_leading_match_gen_l_1->SetMarkerStyle(21);
  gr_reco_leading_match_gen_l_1->SetMarkerColor(kBlack);
  gr_reco_leading_match_gen_l_1->SetLineColor(kBlack);
  gr_reco_leading_match_gen_l_1->SetLineWidth(3);
  gr_reco_leading_match_gen_l_1->Draw("lpsame");
  //==== gen_l_2 leading
  TGraph *gr_gen_l_2_leading = new TGraph(n_all, x_all, y_gen_l_2_leading);
  gr_gen_l_2_leading->SetMarkerStyle(21);
  gr_gen_l_2_leading->SetMarkerColor(kBlue);
  gr_gen_l_2_leading->SetLineColor(kBlue);
  gr_gen_l_2_leading->SetLineWidth(3);
  gr_gen_l_2_leading->SetLineStyle(3);
  gr_gen_l_2_leading->Draw("lpsame");
  //==== RECO Leading - GEN_l_2
  TGraph *gr_reco_leading_match_gen_l_2 = new TGraph(n_all, x_all, y_reco_leading_match_gen_l_2);
  gr_reco_leading_match_gen_l_2->SetMarkerStyle(21);
  gr_reco_leading_match_gen_l_2->SetMarkerColor(kBlue);
  gr_reco_leading_match_gen_l_2->SetLineColor(kBlue);
  gr_reco_leading_match_gen_l_2->SetLineWidth(3);
  gr_reco_leading_match_gen_l_2->Draw("lpsame");
  //==== Legend
  TLegend *lg2 = new TLegend(0.17, 0.45, 0.50, 0.65);
  lg2->AddEntry(gr_reco_leading_match_gen_l_1, "GEN l_{1} - RECO Leading", "lp");
  lg2->AddEntry(gr_gen_l_1_leading, "GEN l_{1} : Leading", "lp");
  lg2->AddEntry(gr_reco_leading_match_gen_l_2, "GEN l_{2} - RECO Leading", "lp");
  lg2->AddEntry(gr_gen_l_2_leading, "GEN l_{2} : Leading", "lp");
  lg2->Draw();
  
  c2->SaveAs(plotpath+"/leading_lepton.pdf");
  c2->Close();

  
  //==========================================================================
  //==== Lepton Assigning Method 1 (high mass only)
  //==== 1) Compare pt of the SS leptons
  //====    - For m(HN) <  1000 GeV, larger  pt -> l_1
  //====    - For m(HN) >= 1000 GeV, smaller pt -> l_1
  //==== 2) Then, find the one whose m(MET+lep) is closer to 80.4 GeV -> l_3
  //==== 3) Remaining -> l_2
  //==========================================================================
  TCanvas* c3 = new TCanvas("c3", "", 1000, 1000);
  gPad->SetLogx();
  c3->SetGrid();
  canvas_margin(c3);
  c3->cd();
  //==== 1) gen_l_1 by pt-ordering
  TGraph *gr_gen_l_1_first = new TGraph(n_high, x_high, y_gen_l_1_first);
  gr_gen_l_1_first->SetMarkerStyle(21);
  gr_gen_l_1_first->SetMarkerColor(kBlack);
  gr_gen_l_1_first->SetLineColor(kBlack);
  gr_gen_l_1_first->SetLineWidth(3);
  gr_gen_l_1_first->Draw("alp");
  gr_gen_l_1_first->GetYaxis()->SetRangeUser(0, 1);
  gr_gen_l_1_first->GetYaxis()->SetTitle("Probability");
  gr_gen_l_1_first->GetXaxis()->SetTitle("m(HN) [GeV]");
  hist_axis(gr_gen_l_1_first);
  //==== 2) gen_l_3 by mlmet, over 1)
  TGraph* gr_gen_l_1_first_mlmet_next = new TGraph(n_high, x_high, y_gen_l_1_first_mlmet_next);
  gr_gen_l_1_first_mlmet_next->SetMarkerStyle(21);
  gr_gen_l_1_first_mlmet_next->SetMarkerColor(kBlue);
  gr_gen_l_1_first_mlmet_next->SetLineColor(kBlue);
  gr_gen_l_1_first_mlmet_next->SetLineWidth(3);
  gr_gen_l_1_first_mlmet_next->Draw("lpsame");
  //==== 3) 1) times 2)
  TGraph* gr_gen_l_1_first_mlmet_next_total = new TGraph(n_high, x_high, y_gen_l_1_first_mlmet_next_total);
  gr_gen_l_1_first_mlmet_next_total->SetMarkerStyle(21);
  gr_gen_l_1_first_mlmet_next_total->SetMarkerColor(kRed);
  gr_gen_l_1_first_mlmet_next_total->SetLineColor(kRed);
  gr_gen_l_1_first_mlmet_next_total->SetLineWidth(3);
  gr_gen_l_1_first_mlmet_next_total->Draw("lpsame");
  //==== legend
  TLegend *lg3 = new TLegend(0.40, 0.15, 0.95, 0.30);
  //lg3->SetBorderSize(0);
  //lg3->SetFillStyle(0);
  lg3->AddEntry(gr_gen_l_1_first, "1) GEN l_{1} : p_{T} ordering", "lp");
  lg3->AddEntry(gr_gen_l_1_first_mlmet_next, "2) GEN l_{3} : m_{T}(MET+l), over 1)", "lp");
  lg3->AddEntry(gr_gen_l_1_first_mlmet_next_total, "3) 1) #times 2)", "lp");
  lg3->Draw();
  //==== print values
  cout << "===== pt first (after pt order assigning) =====" << endl;
  cout << "1) mlmet only" << endl;
  gr_gen_l_1_first_mlmet_next->Print();
  cout << "2) total" << endl;
  gr_gen_l_1_first_mlmet_next_total->Print();
  //==== save canvas
  c3->SaveAs(plotpath+"/highmass_gen_l_1_first.pdf");
  c3->Close();
  
  //=====================================================================================
  //==== Lepton Assigning Method 2 (high mass only)
  //==== 1) Find the one whose m(MET+lep) is closest to 80.4 GeV -> l_3
  //==== 2-1) If the remaining two leptons are SS, compare their pt
  //====      - For m(HN) <  1000 GeV, larger  pt -> l_1
  //====      - For m(HN) >= 1000 GeV, smaller pt -> l_1
  //==== 2-2) If the remaing to two leptons are OS, then the one opposite to l_3 -> l_2
  //=====================================================================================
  TCanvas* c4 = new TCanvas("c4", "", 1000, 1000);
  gPad->SetLogx();
  c4->SetGrid();
  canvas_margin(c4);
  c4->cd();
  //==== 1) gen_l_3 by mlmet
  TGraph *gr_gen_l_3_first = new TGraph(n_high, x_high, y_gen_l_3_first);
  gr_gen_l_3_first->SetMarkerStyle(21);
  gr_gen_l_3_first->SetMarkerColor(kBlack);
  gr_gen_l_3_first->SetLineColor(kBlack);
  gr_gen_l_3_first->SetLineWidth(3);
  gr_gen_l_3_first->Draw("alp");
  gr_gen_l_3_first->GetYaxis()->SetRangeUser(0, 1);
  gr_gen_l_3_first->GetYaxis()->SetTitle("Probability");
  gr_gen_l_3_first->GetXaxis()->SetTitle("m(HN) [GeV]");
  hist_axis(gr_gen_l_3_first);
  //==== 2-1) gen_l_1 leading
  TGraph* gr_gen_l_3_first_gen_l_1_leading_next = new TGraph(n_high, x_high, y_gen_l_3_first_gen_l_1_leading_next);
  gr_gen_l_3_first_gen_l_1_leading_next->SetMarkerStyle(21);
  gr_gen_l_3_first_gen_l_1_leading_next->SetMarkerColor(kGreen);
  gr_gen_l_3_first_gen_l_1_leading_next->SetLineColor(kGreen);
  gr_gen_l_3_first_gen_l_1_leading_next->SetLineWidth(3);
  gr_gen_l_3_first_gen_l_1_leading_next->SetLineStyle(2);
  gr_gen_l_3_first_gen_l_1_leading_next->Draw("lpsame");
  //==== 2-2) gen_l_2 leading
  TGraph* gr_gen_l_3_first_gen_l_2_leading_next = new TGraph(n_high, x_high, y_gen_l_3_first_gen_l_2_leading_next);
  gr_gen_l_3_first_gen_l_2_leading_next->SetMarkerStyle(21);
  gr_gen_l_3_first_gen_l_2_leading_next->SetMarkerColor(kViolet);
  gr_gen_l_3_first_gen_l_2_leading_next->SetLineColor(kViolet);
  gr_gen_l_3_first_gen_l_2_leading_next->SetLineWidth(3);
  gr_gen_l_3_first_gen_l_2_leading_next->SetLineStyle(10);
  gr_gen_l_3_first_gen_l_2_leading_next->Draw("lpsame");
  //==== 2-3) combination
  TGraph* gr_gen_l_3_first_pt_order_next = new TGraph(n_high, x_high, y_gen_l_3_first_pt_order_next);
  gr_gen_l_3_first_pt_order_next->SetMarkerStyle(21);
  gr_gen_l_3_first_pt_order_next->SetMarkerColor(kBlue);
  gr_gen_l_3_first_pt_order_next->SetLineColor(kBlue);
  gr_gen_l_3_first_pt_order_next->SetLineWidth(3);
  gr_gen_l_3_first_pt_order_next->SetLineStyle(8);
  gr_gen_l_3_first_pt_order_next->Draw("lpsame");
  //==== 3) 1) times 2)
  TGraph* gr_gen_l_3_first_pt_order_next_total = new TGraph(n_high, x_high, y_gen_l_3_first_pt_order_next_total);
  gr_gen_l_3_first_pt_order_next_total->SetMarkerStyle(21);
  gr_gen_l_3_first_pt_order_next_total->SetMarkerColor(kRed);
  gr_gen_l_3_first_pt_order_next_total->SetLineColor(kRed);
  gr_gen_l_3_first_pt_order_next_total->SetLineWidth(3);
  gr_gen_l_3_first_pt_order_next_total->Draw("lpsame");
  //==== legend
  TLegend *lg4 = new TLegend(0.40, 0.15, 0.95, 0.40);
  //lg4->SetBorderSize(0);
  //lg4->SetFillStyle(0);
  lg4->AddEntry(gr_gen_l_3_first, "1) GEN l_{3} : m_{T}(MET+l)", "lp");
  lg4->AddEntry(gr_gen_l_3_first_gen_l_1_leading_next, "2-3) GEN l_{1} : leading, over 1)", "lp");
  lg4->AddEntry(gr_gen_l_3_first_gen_l_2_leading_next, "2-3) GEN l_{1} : sub-leading, over 1)", "lp");
  lg4->AddEntry(gr_gen_l_3_first_pt_order_next, "2-3) GEN l_{1} : p_{T} ordering, over 1)", "lp");
  lg4->AddEntry(gr_gen_l_3_first_pt_order_next_total, "3) 1) #times 2-3)", "lp");
  lg4->Draw();
  //==== print values
  cout << "===== mlmet first (after mlmet assigning) =====" << endl;
  cout << "1) pt order only" << endl;
  gr_gen_l_3_first_pt_order_next->Print();
  cout << "2) total" << endl;
  gr_gen_l_3_first_pt_order_next_total->Print();
  //==== save canvas
  c4->SaveAs(plotpath+"/highmass_gen_l_3_first.pdf");
  c4->Close();

  
  
  
  
}


