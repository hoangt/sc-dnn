#!/bin/bash

SIMDIR=/home/tunji/sc-dnn/code/gem5-stable
gem5Bin="$SIMDIR/build/X86_ZMESI_Three_Level/gem5.opt"
gem5Script="$SIMDIR/configs/example/se.py"
cacheType="ZL3"

progDir=`pwd`

if [ $# -ne 2 ]; then 
   echo "Usage: $0 <SampleCount> <ThreadCount>" 
   exit
fi

model="CIFAR10"
sampleCount=$1
tc=$2
cpus=$((tc + 1))
sparsityOpts="--forwardsparsity 24 --backwardsparsity 84 --signalcachelinesparsity 83 --weightupdatesparsity 89 --deltacachelinesparsity 67"
progOpts="--samples $sampleCount --threads $tc $sparsityOpts --model ${model}"
. ./cacheSpecs.sh
cacheOpts="--l1d_size=${l1Size} --l1i_size=${l1Size} --l2_size=${l2Size} --l3_size=${l3Size}"

simOpts="--ruby --cpu-type=timing --caches --l2cache --num-l3caches=1 ${cacheOpts} --num-cpus=$cpus"

echo "Run SparseCompute in ZeroCache GEM5-Ruby" 
echo "GEM5="${gem5Bin}
echo "SCRIPT="${gem5Script}
echo "PROGDIR="${progDir}
echo "PROGOPTS="${progOpts}
echo "SIMOPTS="${simOpts}

prog="$progDir/M5SparseCompute"

#for k in 1  3 4 ; do  
for k in 1  ; do  

  outDir="${model}-${cacheType}-T${tc}/ruby-k$k"
  
  ${gem5Bin} -d $outDir ${gem5Script} $simOpts -c "$prog"  -o "--kernel $k ${progOpts}"
done


exit
