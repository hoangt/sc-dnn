reset
set terminal png enhanced size 640,240
set title plotTitle
set yrange[0:100]
set xrange[0:10]
set ylabel "Percentage Sparsity"
set xlabel "Epoch Count"
set xtics 0,1,10
set style data linespoints
set key outside
#show variables inputFile
#show variables outputFile
set out outputFile
plot inputFile using 1:2 title "32-bits" lw 2 pt 5, '' using 1:3 ti "128-bits" lw 2 pt 6, '' using 1:4 ti "256-bits" lw 2 pt 7, '' using 1:5 ti "Cacheline" lw 2 pt 8
set out
