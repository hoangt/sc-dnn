#!/bin/bash

SIMDIR=/home/tunji/sc-dnn/code/gem5-stable
gem5Script="$SIMDIR/configs/example/se.py"

progDir=`pwd`

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

. ./MNIST_Sparse.sh 

progOpts="--samples $sampleCount --threads $tc $sparsityOpts --model ${model} --workers ${workerCount}"

. ./cacheSpecs.sh $cacheScale
cacheOpts="--l1d_size=${l1Size} --l1i_size=${l1Size} --l2_size=${l2Size} --l3_size=${l3Size}"

simOpts="--ruby --cpu-type=timing --caches --l2cache --num-l3caches=1 ${cacheOpts} --num-cpus=$cpus"

echo "Run SparseCompute in GEM5-Ruby" 
echo "GEM5="${gem5Bin}
echo "SCRIPT="${gem5Script}
echo "PROGDIR="${progDir}
echo "PROGOPTS="${progOpts}
echo "SIMOPTS="${simOpts}

prog="$progDir/M5SparseCompute"

dirSuffix="${sampleCount}_${tc}_${cacheScale}"
#for k in 1  3 4 ; do  
for k in 1  ; do  

  outDir="${model}_${workerCount}W-${cacheType}-${dirSuffix}/ruby-k$k"
  
  echo ${gem5Bin} -d $outDir ${gem5Script} $simOpts -c "$prog"  -o "--kernel $k ${progOpts}" | tee ${model}_${cacheType}_${workerCount}W_${threadCount}T.txt
done


exit
