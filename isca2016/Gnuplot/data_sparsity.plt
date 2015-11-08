reset
set terminal png enhanced size 640,240
set yrange[0:100]
set xrange[0:10]
set ylabel "Percentage Sparsity"
set xlabel "Epoch Count"
set xtics 0,1,10
set style data linespoints
set key outside
set out 'cifar-10_data_sparsity.png'
plot 'cifar-10_data_sparsity.txt' using 1:2 title "Activations" lw 2, '' using 1:3 ti "Errors" lw 2, '' using 1:4 ti "Weight Deltas" lw 2 , '' using 1:5 ti "Weights" lw 2 
set out
