#!/bin/bash
export PLOTTER_WORKING_DIR=`pwd`
export FILE_PATH=$PLOTTER_WORKING_DIR/rootfiles/
export PLOT_PATH=$PLOTTER_WORKING_DIR/plots/
export SCRIPT_DIR=$PLOTTER_WORKING_DIR/script/
export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$PLOTTER_WORKING_DIR/include/
export CATVERSION="v8-0-7"
export CATANVERSION="v8-0-7.7"
export LIMIT_PATH=$PLOTTER_WORKING_DIR/LimitCode/CMS-StatisticalTools/RooStatsRoutines/Macros_trimuon_13TeV/

alias run="bash "$SCRIPT_DIR"/run.sh"
