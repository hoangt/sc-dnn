#!/bin/bash

experimentDir=/home/tunji/sc-dnn/code/zsim/experiments
scriptDir=${experimentDir}/scripts
threadScript=${scriptDir}/scaleThreads.sh 

if [ $# -ne 2 ]; then 
	echo "Usage: $0 <machine> <benchmark>"
	exit
fi

machineName=$1
benchmarkName=$2
nohupDir=nohupOut
mkdir -p ${nohupDir}

for i in 1 2 4 8 16 32 64; do 
  outputFile=${nohupDir}/${machineName}_${benchmarkName}_C${i}.txt
  nohup ${threadScript} ${machineName} ${benchmarkName} $i > ${outputFile} & 
done


