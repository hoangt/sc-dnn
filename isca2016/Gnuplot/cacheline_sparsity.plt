reset
unset multiplot
set terminal png enhanced size 1280,300
set yrange[0:100]
set xrange[0:10]
set ylabel "Percentage Sparsity"
set xlabel "Epoch Count"
set xtics 0,1,10
set style data linespoints
set key outside
set out 'cifar-10_cacheline_sparsity.png'
set multiplot layout 1,2 columnsfirst
set label 11 center at graph 0.5,char 1 "(a)" 
set bmargin 5 
plot 'cifar-10_cacheline_data_sparsity.txt' using 1:2 title "Activations" lw 2, '' using 1:3 ti "Errors" lw 2, '' using 1:4 ti "Weight Deltas" lw 2 , '' using 1:5 ti "Weights" lw 2 
set label 11 "(b)"
#unset ylabel
plot 'cifar-10_cacheline_compute_sparsity.txt' using 1:2 title "Feed-forward Eval." lw 2 pt 5, '' using 1:3 ti "Back-propagation" lw 2 pt 6, '' using 1:4 ti "Delta Computation" lw 2 pt 7, '' using 1:5 ti "Weight Updates" lw 2 pt 8
unset multiplot
set out
