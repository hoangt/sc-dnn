model="MNIST"

sparsityOpts="--forwardsparsity 33 --activationcachelinesparsity 14 --backwardsparsity 83 --signalcachelinesparsity 81 --weightupdatesparsity 5 --deltacachelinesparsity 0"
p
modelOpts="$sparsityOpts --model ${model}"

echo "MODELOPTS="${modelOpts}

