#!/bin/bash

SIMDIR=/home/tunji/sc-dnn/code/gem5-stable
gem5Bin="$SIMDIR/build/X86_ZMESI_Three_Level/gem5.opt"
gem5Script="$SIMDIR/configs/example/se.py"

progDir=`pwd`

if [ $# -ne 4 ]; then 
   echo "Usage: $0 <InputCoun> <OutputCount> <Sparsity> <Iteration>"
   exit
fi
 
ic=$1
oc=$2
sr=$3
iter=$4
cpus=2
progOpts="$ic $oc $sr $iter"

dCacheSize=4kB
L2CacheSize=8kB
L3CacheSize=32kB
cacheSizeOpts="--l1i_size=$dCacheSize --l1d_size=$dCacheSize --l2_size=$L2CacheSize --l3_size=$L3CacheSize"
#cpuOpts="--ruby --cpu-type=timing --caches --l2cache --num-l3caches=1"
cpuOpts="--ruby -cpu-type=timing --caches --l2cache --num-l3caches=1 --num-cpus=2 $cacheSizeOpts"

echo "Run Feedforward variants in GEM5-RUby" 
echo "GEM5="${gem5Bin}
echo "SCRIPT="${gem5Script}
echo "PROGDIR="${progDir}
echo "InputCount=$ic OutputCount=$oc Sparsity=$sr Iteration=$iter"
echo "PROGOPTS="${progOpts}
echo "CPUOPTS="${cpuOpts}

for p in feedforward feedforward-zero_opt feedforward-simd_opt; do 
#for p in feedforward feedforward-zero_opt; do  

  prog="$progDir/$p"
  outDir="ZL3Output/zl3-ruby-m5out-$p"
  
  ${gem5Bin} -d $outDir ${gem5Script} $cpuOpts -c "$prog" -o "$progOpts"
done


exit
