
. ./setupOpts.sh

simBin="/home/tunji/sc-dnn/code/gem5-stable/build/X86_MESI_Three_Level/gem5.opt"
outDir="L3_MNIST_S${sampleCount}_C${cacheScale}/"
prog="/home/tunji/sc-dnn/code/SparseCompute/M5SparseCompute"
script="/home/tunji/sc-dnn/code/gem5-stable/configs/example/se.py"

${simBin} -d ${outDir} ${script} ${simOpts} -c ${prog} -o "${progOpts}"
