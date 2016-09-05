
. ./CIFAR10_Opts.sh

simBin="/home/tunji/sc-dnn/code/gem5-stable/build/X86_ZMESI_Three_Level/gem5.opt"
outDir="ZL3_CIFAR10_S${sampleCount}_C${cacheScale}_Z${sparsity}/"
prog="/home/tunji/sc-dnn/code/SparseCompute/M5SparseCompute"
script="/home/tunji/sc-dnn/code/gem5-stable/configs/example/se.py"

${simBin} -d ${outDir} ${script} ${simOpts} -c ${prog} -o "${progOpts}"
