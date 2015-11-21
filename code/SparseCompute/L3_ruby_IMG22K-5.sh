#!/bin/bash

progDir=`pwd`

if [ $# -lt 3 ]; then 
   echo "Usage: $0 <SampleCount> <ThreadCount> <WorkerCount>" 
   exit
fi

SIMDIR=/home/tunji/sc-dnn/code/gem5-stable
gem5Bin="$SIMDIR/build/X86_MESI_Three_Level/gem5.opt"
cacheType="L3"
scaleOpt=5
. ./GEM5_ruby_IMG22K.sh 

