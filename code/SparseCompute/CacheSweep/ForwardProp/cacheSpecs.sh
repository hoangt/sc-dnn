
l1Scale=1
l2Scale=1
l3Scale=1
if [ $# -eq 1 ]; then 
  l3Scale=$1
fi
 
l1Size=$(($l1Scale * 32))kB
l2Size=$(($l2Scale * 256))kB
l3Size=$(($l3Scale * 2))MB

cacheSizeOpts="--l1i_size=$l1Size --l1d_size=$l1Size --l2_size=$l2Size --l3_size=$l3Size"
echo "CacheSizeOpts="$cacheSizeOpts
