if [ $# -lt 2 ]; then 
  echo "Usage: $0 <SampleCount> <CacheScale> [Sparsity]"
  exit
fi

sparsity=24
if [ $# -gt 2 ]; then 
 sparsity=$3
fi

model=CIFAR10
sampleCount=$1
cacheScale=$2
icacheSize=32kB
l1Size=$((8 * $cacheScale))kB
l2Size=$((64 * $cacheScale))kB
l3Size=$((5 * $cacheScale))MB

cacheOpts="--l1d_size=${l1Size} --l1i_size=${icacheSize} --l2_size=${l2Size} --l3_size=${l3Size}"

simOpts="--ruby --cpu-type=timing --caches --l2cache --num-l3caches=1 --num-cpus=2 $cacheOpts"

progOpts=" --kernel 1 --samples ${sampleCount} --threads 1 --forwardsparsity ${sparsity} --activationcachelinesparsity 14 --backwardsparsity 83 --signalcachelinesparsity 81 --weightupdatesparsity 5 --deltacachelinesparsity 0 --model CIFAR10 --workers 1 --pass ForwardProp --outputscale 8" 

