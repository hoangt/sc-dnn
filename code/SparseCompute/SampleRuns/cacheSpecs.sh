l1Size=32kB
l2Size=256kB
l3Size=20MB

factor=1
if [ $# -eq 1 ]; then 
  factor=$1
fi

l1Size=$(($factor * 32))kB
l2Size=$(($factor * 256))kB
l3Size=$(($factor * 20))MB

cacheSizeOpts="--l1i_size=$l1Size --l1d_size=$l1Size --l2_size=$l2Size --l3_size=$l3Size"
echo "CacheSizeOpts="$cacheSizeOpts
