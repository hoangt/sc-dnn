if [ $# -lt 1 ]; then
 t1=1
else
 t1=$1
fi

 nohup ./L3_ruby_CIFAR10.sh $t1 $t1 1 1 > nohup-L3_CIFAR10_${t1}_${t1}_C1.txt & 
 nohup ./L3_ruby_CIFAR10.sh ${t1} ${t1} 1 2 > nohup-L3_CIFAR10_${t1}_${t1}_C2.txt & 
 nohup ./ZL3_ruby_CIFAR10.sh ${t1} ${t1} 1 1 > nohup-ZL3_CIFAR10_${t1}_${t1}_C1.txt & 

t1=$((t1 * 2))

 nohup ./L3_ruby_CIFAR10.sh $t1 $t1 1 1 > nohup-L3_CIFAR10_${t1}_${t1}_C1.txt & 
 nohup ./L3_ruby_CIFAR10.sh ${t1} ${t1} 1 2 > nohup-L3_CIFAR10_${t1}_${t1}_C2.txt & 
 nohup ./ZL3_ruby_CIFAR10.sh ${t1} ${t1} 1 1 > nohup-ZL3_CIFAR10_${t1}_${t1}_C1.txt & 
 


