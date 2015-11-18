#!/bin/bash

SIMDIR=/home/tunji/sc-dnn/code/gem5-stable
gem5Bin="$SIMDIR/build/X86_ZMESI_Three_Level/gem5.opt"
gem5Script="$SIMDIR/configs/example/se.py"

progDir=`pwd`

if [ $# -ne 2 ]; then 
   echo "Usage: $0 <SampleCount> <ThreadCount>" 
   exit
fi
 
sampleCount=$1
tc=$2
cpus=$((tc + 1))
sparsityOpts="--forwardsparsity 30 --backwardsparsity 83 --signalcachelinesparsity 81 --weightupdatesparsity 5 --deltacachelinesparsity 0"
progOpts="--samples $sampleCount --threads $tc $sparsityOpts"
. ./cacheSpecs.sh
cacheOpts="--l1d_size=${l1Size} --l1i_size=${l1Size} --l2_size=${l2Size} --l3_size=${l3Size}"

simOpts="--ruby --cpu-type=timing --caches --l2cache --num-l3caches=1 ${cacheOpts} --num-cpus=$cpus"

echo "Run SparseCompute in GEM5-RUby" 
echo "GEM5="${gem5Bin}
echo "SCRIPT="${gem5Script}
echo "PROGDIR="${progDir}
echo "InputCount=$ic OutputCount=$oc Sparsity=$sr Iteration=$iter"
echo "PROGOPTS="${progOpts}
echo "SIMOPTS="${simOpts}

prog="$progDir/M5SparseCompute"

#for k in 1  3 4 ; do  
for k in 1  ; do  

  outDir="ZL3Output/zl3-ruby-m5out-k$k"
  
  ${gem5Bin} -d $outDir ${gem5Script} $simOpts -c "$prog"  -o "--kernel $k ${progOpts}"
done


exit
