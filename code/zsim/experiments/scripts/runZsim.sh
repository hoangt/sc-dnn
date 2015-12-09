#!/bin/bash

ZSIM=/home/tunji/sc-dnn/code/zsim/build/opt/zsim
config=BaseConfig.cfg

if [ $# -ne 4 ]; then 
   echo "Usage: $0 <L1$ Scale> <L2$ Scale> <L3$ Scale> <Thread>"
   exit
fi

if [ ! -f ${ZSIM} ]; then
	echo "Error: ${ZSIM} not found"
	exit
fi

if [ ! -f ${config} ]; then
	echo "Error: ${config} not found"
	exit
fi

l1dScale=$1
l2Scale=$2
l3Scale=$3
threads=$4

echo "Running ${ZSIM} with ${config}"
echo "ThreadCount: ${threads}"
echo "CacheScaling: L1D=${l1dScale} L2=${l2Scale} L3=${l3Scale}"

tgtDir="T${threads}_CacheScale_${l1dScale}_${l2Scale}_${l3Scale}"
mkdir -p ${tgtDir}

l1Rep="s/l1dCacheScale = 1/l1dCacheScale = ${l1dScale}/g"
l2Rep="s/l2CacheScale = 1/l2CacheScale = ${l2Scale}/g"
l3Rep="s/l3CacheScale = 1/l3CacheScale = ${l3Scale}/g"
threadRep="s/threads 1/threads ${threads}/g"
sampleRep="s/samples 1/samples ${threads}/g"

sed -e "${l1Rep}" -e "${l2Rep}" -e "${l3Rep}" -e "${threadRep}" -e "${sampleRep}" ${config} > ${tgtDir}/ScaleConfig.cfg 

pushd ${tgtDir}
 ${ZSIM} ScaleConfig.cfg | tee run.out 
popd


