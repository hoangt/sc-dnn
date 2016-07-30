#!/bin/bash

CheckFileExists ()
{
	file=$1
	if [ ! -f ${file} ]; then
		echo "${file} not found"
		exit
	fi
}

experimentDir=/home/tunji/sc-dnn/code/zsim/experiments
scriptDir=${experimentDir}/scripts
zsimScript=${scriptDir}/runZsim.sh 

if [ $# -ne 3 ]; then 
	echo "Usage: $0 <machine> <benchmark> <L3CacheScale>"
	exit
fi

machineName=$1
benchmarkName=$2
l3CacheScale=$3
machineConfig=${experimentDir}/machines/${machineName}.cfg
benchmarkConfig=${experimentDir}/benchmarks/${benchmarkName}.cfg

CheckFileExists ${machineConfig}
CheckFileExists ${benchmarkConfig}

resultsDir=${experimentDir}/results/${machineName}_${benchmarkName}/L1Cache_${l3CacheScale}
baseConfig=${resultsDir}/BaseConfig.cfg

mkdir -p ${resultsDir}

cat ${machineConfig} ${benchmarkConfig} > ${baseConfig}

echo "Thread scalability experiments"
echo "Directory:" ${resultsDir}
echo "Script:" ${zsimScript} 
echo "Config:" ${baseConfig}
echo "L3CacheScale:" ${l3CacheScale}

pushd ${resultsDir}
for threads in 1 2 4 6 8; do 
 ${zsimScript} 1 1 ${l3CacheScale} $threads
done


popd

