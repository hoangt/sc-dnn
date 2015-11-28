model="IMG1K"

sparsityOpts="--forwardsparsity 37 --activationcachelinesparsity 17 --backwardsparsity 93 --signalcachelinesparsity 54 --weightupdatesparsity 31 --deltacachelinesparsity 5"

modelOpts="$sparsityOpts --model ${model}"

echo "MODELOPTS="${modelOpts}

