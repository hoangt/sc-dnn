if [ $# -lt 1 ]; then
 wc=1
else
 wc=$1
fi

 nohup ./L3_ruby_IMG22K.sh 1 1 ${wc} 1 > nohup-L3_IMG22K_T1_W${wc}_C1.txt & 
 nohup ./L3_ruby_IMG22K.sh 1 1 ${wc} 2 > nohup-L3_IMG22K_T1_W${wc}_C2.txt & 
 nohup ./ZL3_ruby_IMG22K.sh 1 1 ${wc} 1 > nohup-ZL3_IMG22K_T1_W${wc}_C1.txt & 

