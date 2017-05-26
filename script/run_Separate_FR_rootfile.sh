#!/bin/bash

WhatToRun=2

if [ $WhatToRun = 0 ]; then
  outputdir=DiMuonTrkVVL
  mkdir -p $PLOTTER_WORKING_DIR/rootfiles/$CATANVERSION/FakeRateCalculator/$outputdir
  root -l -b -q "script/Separate_FR_rootfile_data.C(\"\", \"$outputdir\", 0)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKDY_cat_v8-0-7.root\", \"\", \"$outputdir\", 0)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKQCD_mu_cat_v8-0-7.root\", \"\", \"$outputdir\", 0)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKsingletop_cat_v8-0-7.root\", \"\", \"$outputdir\", 0)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKTTJets_aMC_cat_v8-0-7.root\", \"\", \"$outputdir\", 0)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKVGamma_cat_v8-0-7.root\", \"\", \"$outputdir\", 0)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKVV_cat_v8-0-7.root\", \"\", \"$outputdir\", 0)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKWJets_cat_v8-0-7.root\", \"\", \"$outputdir\", 0)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKHN_MuMuMu_40_cat_v8-0-7.root\", \"\", \"$outputdir\", 0)"
fi

if [ $WhatToRun = 1 ]; then
  outputdir=NoBias
  mkdir -p $PLOTTER_WORKING_DIR/rootfiles/$CATANVERSION/FakeRateCalculator/$outputdir
  root -l -b -q "script/Separate_FR_rootfile_data.C(\"\", \"$outputdir\", 1)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKDY_cat_v8-0-7.root\", \"\", \"$outputdir\", 1)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKQCD_mu_cat_v8-0-7.root\", \"\", \"$outputdir\", 1)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKsingletop_cat_v8-0-7.root\", \"\", \"$outputdir\", 1)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKTTJets_aMC_cat_v8-0-7.root\", \"\", \"$outputdir\", 1)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKVGamma_cat_v8-0-7.root\", \"\", \"$outputdir\", 1)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKVV_cat_v8-0-7.root\", \"\", \"$outputdir\", 1)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKWJets_cat_v8-0-7.root\", \"\", \"$outputdir\", 1)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKHN_MuMuMu_40_cat_v8-0-7.root\", \"\", \"$outputdir\", 1)"
fi

if [ $WhatToRun = 2 ]; then
  inputdir=HalfSample
  outputdir=HalfSample/DiMuonTrkVVL
  mkdir -p $PLOTTER_WORKING_DIR/rootfiles/$CATANVERSION/FakeRateCalculator/$outputdir
  root -l -b -q "script/Separate_FR_rootfile_data.C(\"$inputdir\", \"$outputdir\", 0)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKDY_cat_v8-0-7.root\", \"$inputdir\", \"$outputdir\", 0)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKsingletop_cat_v8-0-7.root\", \"$inputdir\", \"$outputdir\", 0)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKTTJets_aMC_cat_v8-0-7.root\", \"$inputdir\", \"$outputdir\", 0)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKVGamma_cat_v8-0-7.root\", \"$inputdir\", \"$outputdir\", 0)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKVV_cat_v8-0-7.root\", \"$inputdir\", \"$outputdir\", 0)"
  root -l -b -q "script/Separate_FR_rootfile.C(\"FakeRateCalculator_Mu_dxysig_SKWJets_cat_v8-0-7.root\", \"$inputdir\", \"$outputdir\", 0)"
fi
