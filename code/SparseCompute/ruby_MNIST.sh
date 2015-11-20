#!/bin/bash

if [ $# -ne 2 ]; then 
   echo "Usage: $0 <SampleCount> <ThreadCount>"
   exit
fi



./ZL3_ruby_run.sh  1 

./L3_ruby_run.sh 10 1
