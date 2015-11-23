reset
unset multiplot
show variables outFile
show variables cifar10File
show variables mnistFile
show variables img1KFile
show variables img22KFile
set terminal png enhanced size 1280,300
#set yrange[0:45]
#set xrange[0:6]
set ylabel "Normalized Performance"
set xlabel "Thread Count"
set xtics ("1" 1 ,"2" 2,"4" 3, "8" 4, "16" 5) 
set ytics 0, 10
set style data linespoints
set key outside
set out outFile

baselineKey='LoopUnrolling'
softwareKey='SW-SparseOpt'
hardwareKey='HW-SparseOpt'

set multiplot layout 1,5 rowsfirst
set label 11 center at graph 0.5,char 1 "(a)" 
set bmargin 5

set style line 1 lt 1 lw 2 
set style line 2 lt 2 lw 2
set style line 3 lt 3 lw 2

set title 'MNIST'
unset key
plot mnistFile using 1:2 title baselineKey ls 1, '' using 1:3 ti softwareKey ls 2, '' using 1:4 ti hardwareKey ls 3 

set label 11 "(b)"
set title 'CIFAR10'
unset key
plot cifar10File using 1:2 title baselineKey ls 1, '' using 1:3 ti softwareKey ls 2, '' using 1:4 ti hardwareKey ls 3

set label 11 "(c)"
set title 'ImageNet-1K'
plot img1KFile using 1:2 title baselineKey ls 1, '' using 1:3 ti softwareKey ls 2, '' using 1:4 ti hardwareKey ls 3

set label 11 "(d)"
set title 'ImageNet-22K'
plot img22KFile using 1:2 title baselineKey ls 1, '' using 1:3 ti softwareKey ls 2, '' using 1:4 ti hardwareKey ls 3


set border 0
unset label
unset title
unset tics
unset xlabel
unset ylabel
set yrange[0:1]
set key lmargin
plot 2 title baselineKey ls 1, '' ti softwareKey ls 2, '' ti hardwareKey ls 3 
unset multiplot
set out
