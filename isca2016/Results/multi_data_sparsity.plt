reset
unset multiplot
show variables wordFile
show variables cacheLineFile
show variables outFile
set terminal png enhanced size 1280,300
set yrange[0:100]
set xrange[0:10]
set ylabel "Percentage Sparsity"
set xlabel "Epoch Count"
set xtics 0,1,10
set style data linespoints
set key outside
set out outFile
set multiplot layout 1,3 columnsfirst
set label 11 center at graph 0.5,char 1 "(a)" 
set bmargin 5 

actKey='Activations'
errKey='Error Gradients'
deltaKey='Weight Deltas'
weightKey='Weights'

set title "Word Granularity"
unset key
plot wordFile using 1:2 ti actKey lw 2, '' using 1:3 ti errKey lw 2, '' using 1:4 ti deltaKey lw 2 , '' using 1:5 ti weightKey lw 2 

set label 11 "(b)"
set title "CacheLine Granularity"
plot cacheLineFile using 1:2 ti actKey lw 2, '' using 1:3 ti errKey lw 2, '' using 1:4 ti deltaKey lw 2 , '' using 1:5 ti weightKey lw 2 

set border 0
unset label
unset title
unset tics
unset xlabel
unset ylabel
set yrange[0:1]
set xrange[0:1]
set key lmargin
plot 2 ti actKey lw 2, '' ti errKey lw 2, '' ti deltaKey lw 2, '' ti weightKey lw 2
unset multiplot
set out
