reset
set terminal png enhanced size 640,240
set yrange[0:100]
set xrange[0:10]
set ylabel "Percentage Sparsity"
set xlabel "Epoch Count"
set xtics 0,1,10
set style data linespoints
set key outside
set out 'cifar-10_compute_sparsity.png'
plot 'cifar-10_compute_sparsity.txt' using 1:2 title "Feed-forward Eval." lw 2 pt 5, '' using 1:3 ti "Back-propagation" lw 2 pt 6, '' using 1:4 ti "Delta Computation" lw 2 pt 7, '' using 1:5 ti "Weight Updates" lw 2 pt 8
set out
