#!/bin/bash

SIMDIR=/home/tunji/sc-dnn/code/gem5-stable
gem5Bin="$SIMDIR/build/X86_MESI_Three_Level/gem5.opt"
dirString="L3Output"

echo $cacheSizeOpts

. ./GEM5_ruby_ff.sh 

