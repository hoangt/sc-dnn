model="IMG22K"

sparsityOpts="--forwardsparsity 20 --activationcachelinesparsity 15 --backwardsparsity 92 --signalcachelinesparsity 63 --weightupdatesparsity 7 --deltacachelinesparsity 0"

modelOpts="$sparsityOpts --model ${model}"

echo "MODELOPTS="${modelOpts}

