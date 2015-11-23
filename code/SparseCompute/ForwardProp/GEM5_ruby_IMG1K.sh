#!/bin/bash

SIMDIR=/home/tunji/sc-dnn/code/gem5-stable
gem5Script="$SIMDIR/configs/example/se.py"

progDir=/home/tunji/sc-dnn/code/SparseCompute

if [ $# -lt 3 ]; then 
   echo "Usage: $0 <SampleCount> <ThreadCount> <WorkerCount> [CacheScale]" 
   exit
fi

sampleCount=$1
tc=$2
workerCount=$3
cpus=$((tc + 1))
cacheScale=1

if [ $# -gt 3 ]; then
  cacheScale=$4
fi

. ./IMG1K_Sparse.sh 
progOpts="--samples $sampleCount --threads $tc $sparsityOpts --model ${model} --workers ${workerCount} --pass ForwardProp --outputscale 10"

. ./cacheSpecs.sh $cacheScale
cacheOpts="--l1d_size=${l1Size} --l1i_size=${l1Size} --l2_size=${l2Size} --l3_size=${l3Size}"

simOpts="--ruby --cpu-type=timing --caches --l2cache --num-l3caches=1 ${cacheOpts} --num-cpus=$cpus  --mem-size=4GB --mem-type=ruby_memory"

echo "Run SparseCompute in GEM5-Ruby" 
echo "GEM5="${gem5Bin}
echo "SCRIPT="${gem5Script}
echo "PROGDIR="${progDir}
echo "PROGOPTS="${progOpts}
echo "SIMOPTS="${simOpts}

prog="$progDir/M5SparseCompute"

dirSuffix="S${sampleCount}_T${tc}_C${cacheScale}"
#for k in 1  3 4 ; do  
for k in 1  ; do  

  outDir="Output/${model}_${workerCount}W-${cacheType}-${dirSuffix}/"
  
  ${gem5Bin} -d $outDir ${gem5Script} $simOpts -c "$prog"  -o "--kernel $k ${progOpts}"
done


exit
