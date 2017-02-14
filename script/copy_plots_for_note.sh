#!/bin/bash

notepath=/Users/jskim/Documents/CERNSVN/notes/AN-16-486/trunk/
fakepath=$PLOT_PATH/$CATANVERSION/FakeRateCalculator/dXYSigMin_4p0_LooseRelIsoMax_0p4/
CRpath=$PLOT_PATH/$CATANVERSION/CR/use_FR_method/SFed_HighdXY/
SRpath=$PLOT_PATH/$CATANVERSION/SR/use_FR_method/SFed_HighdXY/

## datasets ##
cp  $PLOT_PATH/$CATANVERSION/signal_study/BR.pdf $notepath/datasets_stuffs/

## bkgd ##

cp $fakepath/dXYSig_Study_LooseIsoMuon.pdf $notepath/bkgd_stuffs/
cp $fakepath/LooseMuon_Study_SingleMuonTrigger_HighdXY_RelIso.pdf $notepath/bkgd_stuffs/
cp $fakepath/num_SingleMuonTrigger_HighdXY_pt.pdf $notepath/bkgd_stuffs/
cp $fakepath/den_SingleMuonTrigger_HighdXY_pt.pdf $notepath/bkgd_stuffs/
cp $fakepath/num_SingleMuonTrigger_HighdXY_eta.pdf $notepath/bkgd_stuffs/
cp $fakepath/den_SingleMuonTrigger_HighdXY_eta.pdf $notepath/bkgd_stuffs/
cp $fakepath/1D_pt_each_eta_FR_SingleMuonTrigger_HighdXY.pdf $notepath/bkgd_stuffs/
cp $fakepath/2D_fakerate_SingleMuonTrigger_HighdXY_after_Prompt_subtraction.pdf $notepath/bkgd_stuffs/
cp $fakepath/2D_FRSF_MCTruth_SingleMuonTrigger_QCD_mu.pdf $notepath/bkgd_stuffs/
cp $CRpath/_ZJets/m_Z_candidate_ZJets.pdf $notepath/bkgd_stuffs/
cp $CRpath/_ZJets/ZMuon_leading_Pt_ZJets.pdf $notepath/bkgd_stuffs/
cp $CRpath/_ZJets/ZMuon_subleading_Pt_ZJets.pdf $notepath/bkgd_stuffs/
cp $CRpath/_WZ/mt_W_candidate_WZ.pdf $notepath/bkgd_stuffs/
cp $PLOT_PATH/$CATANVERSION/FR_syst/HighdXY_FR.pdf $notepath/bkgd_stuffs/
cp $PLOT_PATH/$CATANVERSION/FR_syst/n_Loose_curves.pdf $notepath/bkgd_stuffs/
cp $PLOT_PATH/$CATANVERSION/FR_syst/yield.pdf $notepath/bkgd_stuffs/

## selection ##

cp $SRpath/_cut0/W_pri_highmass_mass_cut0.pdf $notepath/selection_stuffs 
cp $SRpath/_cut0/W_pri_lowmass_mass_cut0.pdf $notepath/selection_stuffs
cp $PLOT_PATH/$CATANVERSION/SR/mll_os/mll_os.pdf $notepath/selection_stuffs

## result ##

cp $SRpath/_cutWlow/HN_mass_class1_cutWlow.pdf $notepath/results_stuffs/
cp $SRpath/_cutWlow/HN_mass_class2_cutWlow.pdf $notepath/results_stuffs/
cp $SRpath/_cutWlow/W_pri_lowmass_mass_cutWlow.pdf $notepath/results_stuffs/
cp $SRpath/_cutWlow/h_PFMET_cutWlow.pdf $notepath/results_stuffs/
cp $SRpath/_cutWlow/h_leadingLepton_Pt_cutWlow.pdf $notepath/results_stuffs/
cp $SRpath/_cutWlow/h_secondLepton_Pt_cutWlow.pdf $notepath/results_stuffs/
cp $SRpath/_cutWlow/h_thirdLepton_Pt_cutWlow.pdf $notepath/results_stuffs/

cp $SRpath/_cutWhigh/HN_mass_class3_cutWhigh.pdf $notepath/results_stuffs/
cp $SRpath/_cutWhigh/W_pri_highmass_mass_cutWhigh.pdf $notepath/results_stuffs/
cp $SRpath/_cutWhigh/h_PFMET_cutWhigh.pdf $notepath/results_stuffs/
cp $SRpath/_cutWhigh/h_leadingLepton_Pt_cutWhigh.pdf $notepath/results_stuffs/
cp $SRpath/_cutWhigh/h_secondLepton_Pt_cutWhigh.pdf $notepath/results_stuffs/
cp $SRpath/_cutWhigh/h_thirdLepton_Pt_cutWhigh.pdf $notepath/results_stuffs/
