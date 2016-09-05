if [ $# -lt 2 ]; then 
  echo "Usage: $0 <SampleCount> <CacheScale>"
  exit
fi

sampleCount=$1
cacheScale=$2
icacheSize=32kB
l1Size=$((8 * $cacheScale))kB
l2Size=$((64 * $cacheScale))kB
l3Size=$((5 * $cacheScale))MB

cacheOpts="--l1d_size=${l1Size} --l1i_size=${icacheSize} --l2_size=${l2Size} --l3_size=${l3Size}"

simOpts="--ruby --cpu-type=timing --caches --l2cache --num-l3caches=1 --num-cpus=2 $cacheOpts"

progOpts=" --kernel 1 --samples ${sampleCount} --threads 1 --forwardsparsity 100 --activationcachelinesparsity 14 --backwardsparsity 83 --signalcachelinesparsity 81 --weightupdatesparsity 5 --deltacachelinesparsity 0 --model MNIST --workers 1 --pass ForwardProp" 

