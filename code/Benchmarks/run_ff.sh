#!/bin/bash

if [ $# -ne 4 ]; then 
   echo "Usage: $0 <InputCoun> <OutputCount> <Sparsity> <Iteration>"
   exit
fi
 
ic=$1
oc=$2
sr=$3
iter=$4

echo "Running Feedforward variants InputCount=$ic OutputCount=$oc Sparsity=$sr Iteration=$iter"


./feedforward $ic $oc $sr $iter

./feedforward-zero_opt $ic $oc $sr $iter

./feedforward-simd_opt $ic $oc $sr $iter


./feedforward-simd_zero_opt $ic $oc $sr $iter


exit
