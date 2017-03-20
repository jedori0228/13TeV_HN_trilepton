#!/bin/bash
whichRun=$1

if [ $whichRun = "SR" ]; then
  root -l -b -q "src/run_trilepton_mumumu_SR.C(2)"
fi

if [ $whichRun = "CR" ]; then
  if [ -z "$2" ]; then
    root -l -b -q "src/run_trilepton_mumumu_CR.C(0)"
  else
    applySF=$2
    root -l -b -q "src/run_trilepton_mumumu_CR.C(0, $applySF)"
  fi
fi

if [ $whichRun = "FR" ]; then
  dXYSig=$2
  RelIso=$3

  if [ $dXYSig = "all" ]; then
		for i in 3.0 4.0 5.0
		do
			for j in 0.2 0.3 0.4 0.6 0.8 1.0
			do
				root -l -b -q "src/fake_calculator.C($i, $j)"
			done
		done
  else
    root -l -b -q "src/fake_calculator.C($dXYSig, $RelIso)" 
  fi
fi

#NLimit syst_UpDowns(int sig_mass, bool printnumber=true, bool forlatex=false, bool inclusive=false, bool fillNlimit=false){
if [ $whichRun = "syst" ]; then
  for mass in 5 10 20 30 40 50 60 70
  do
    root -l -b -q "src/syst_UpDowns.C($mass,true, true, false, false)" &> $PLOTTER_WORKING_DIR/tmp.txt
    python script/syst_latex_output_cleanup.py $PLOTTER_WORKING_DIR/tmp.txt
  done

  for mass in 90 100 150 200 300 400 500 700 1000
  do
    root -l -b -q "src/syst_UpDowns.C($mass,true, true, false, false)" &> $PLOTTER_WORKING_DIR/tmp.txt
    python script/syst_latex_output_cleanup.py $PLOTTER_WORKING_DIR/tmp.txt
  done
fi

if [ $whichRun = "MCSF" ]; then
  root -l -b -q "src/MCNormalizationSF.C(true)"
  root -l -b -q "src/MCNormalizationSF.C(false)"
fi

if [ $whichRun = "cut" ]; then
  root -l -b -q script/draw_mlll.C
  root -l -b -q script/draw_mll_os.C
  root -l -b -q script/draw_mz.C
  root -l -b -q script/draw_bjet.C
fi
