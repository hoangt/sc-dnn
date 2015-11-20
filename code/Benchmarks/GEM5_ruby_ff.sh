#!/bin/bash

SIMDIR=/home/tunji/sc-dnn/code/gem5-stable
gem5Script="$SIMDIR/configs/example/se.py"

progDir=`pwd`

if [ $# -lt 4 ]; then 
   echo "Usage: $0 <InputCount> <OutputCount> <Sparsity> <Iteration> [CacheScale]"
   exit
fi

cacheScale=1
if [ $# -eq 5 ]; then
  cacheScale=$5
fi

cpus=1
if [ $# -eq 6 ]; then
  cpus=$6
fi
 
ic=$1
oc=$2
sr=$3
iter=$4

progOpts="$ic $oc $sr $iter"
optString="${ic}_${oc}_${sr}_${iter}"

. ./CacheSizes.sh $cacheScale

cpuOpts="--ruby -cpu-type=timing --caches --l2cache --num-l2caches=${cpus} --num-l3caches=1 --num-cpus=${cpus} $cacheSizeOpts"
echo "Run Feedforward variants in GEM5-RUby" 
echo "GEM5="${gem5Bin}
echo "SCRIPT="${gem5Script}
echo "PROGDIR="${progDir}
echo "InputCount=$ic OutputCount=$oc Sparsity=$sr Iteration=$iter"
echo "PROGOPTS="${progOpts}
echo "CPUOPTS="${cpuOpts}
echo "OPTSTRING"=${optString}

#for p in feedforward feedforward-zero_opt feedforward-simd_opt; do 
for p in feedforward-simd_opt; do  

  prog="$progDir/$p"
  outDir="${dirString}/$p-${optString}-${cacheScale}xC"
  
  ${gem5Bin} -d $outDir ${gem5Script} $cpuOpts -c "$prog" -o "$progOpts"
done


exit
