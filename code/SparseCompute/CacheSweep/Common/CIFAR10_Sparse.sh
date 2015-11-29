model="CIFAR10"

sparsityOpts="--forwardsparsity 100 --activationcachelinesparsity 8 --backwardsparsity 84 --signalcachelinesparsity 83 --weightupdatesparsity 89 --deltacachelinesparsity 67"

modelOpts="$sparsityOpts --model ${model}"

echo "MODELOPTS="${modelOpts}

