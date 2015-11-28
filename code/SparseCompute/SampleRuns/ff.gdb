file /home/tunji/sc-dnn/code/gem5-stable/build/X86_ZMESI_Three_Level/gem5.debug
set args -d DebugOutput/zl3-ruby-m5out-feedforward /home/tunji/sc-dnn/code/gem5-stable/configs/example/se.py --ruby --cpu-type=timing --caches --num-l3caches=1 --l2cache --l1d_size=4kB --l1i_size=4kB --l2_size=8kB --l3_size=32kB --num-cpus=2 -c /home/tunji/sc-dnn/code/SparseCompute/M5SparseCompute -o "--kernel 1 --samples 1 --threads 1 --forwardsparsity 30 --backwardsparsity 83 --signalcachelinesparsity 81 --weightupdatesparsity 5 --deltacachelinesparsity 0 --pass ForwardProp"

