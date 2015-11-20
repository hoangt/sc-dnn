#!/bin/bash

progDir=`pwd`

if [ $# -ne 2 ]; then 
   echo "Usage: $0 <SampleCount> <ThreadCount>" 
   exit
fi
 
sc=$1
tc=$2
cpus=$((tc + 1))
sparsityOpts="--forwardsparsity 30 --activationcachelinesparsity 14 --backwardsparsity 83 --signalcachelinesparsity 81 --weightupdatesparsity 5 --deltacachelinesparsity 0"
#sparsityOpts="--forwardsparsity 30 --backwardsparsity 100 --signalcachelinesparsity 100 --weightupdatesparsity 5 --deltacachelinesparsity 0"
progOpts="--samples $sc --threads $tc $sparsityOpts"

echo "Run SparseCompute in Native" 
echo "PROGDIR="${progDir}
echo "PROGOPTS="${progOpts}

prog="$progDir/M5SparseCompute"

for k in 1 3 4; do  
#for k in 1 ; do  

  $prog  --kernel $k ${progOpts}
done


exit
