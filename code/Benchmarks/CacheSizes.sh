factor=1
if [ $# -eq 1 ]; then 
  factor=$1
fi

dCacheSize=$(($factor * 1))kB
L2CacheSize=$(($factor * 2))kB
L3CacheSize=$(($factor * 4))kB

cacheSizeOpts="--l1i_size=$dCacheSize --l1d_size=$dCacheSize --l2_size=$L2CacheSize --l3_size=$L3CacheSize"
echo "CacheSizeOpts="$cacheSizeOpts
