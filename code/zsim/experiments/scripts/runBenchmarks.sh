#!/bin/bash


experimentDir=/home/tunji/sc-dnn/code/zsim/experiments
scriptDir=${experimentDir}/scripts
cacheScaleScript=${scriptDir}/scaleCache.sh 
progName="SparseCompute"

if [ $# -lt 2 ]; then
	echo "Usage: $<0> <machine> <benchmark0> [benchmark1 ... benchmark2]"
	exit
fi

machineName=$1 

for var in "${@:2}"; do 
	${cacheScaleScript} ${machineName} ${var}
# Wait for processes to start
	echo "Waiting for ${var} simulations to start"
	sleep 5
# Wait for processes to end
	echo "Waiting for ${var} simulations to end"	
	while pgrep ${progName} > /dev/null; do 
		sleep 5
	done
	echo "All ${var} simulations have ended"
done
