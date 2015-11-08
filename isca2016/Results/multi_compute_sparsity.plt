reset
unset multiplot
show variables outFile
show variables forwardFile
show variables backwardFile
show variables weightFile
set terminal png enhanced size 1280,300
set yrange[0:100]
set xrange[0:10]
set ylabel "Percentage Sparsity"
set xlabel "Epoch Count"
set xtics 0,1,10
set style data linespoints
set key outside
set out outFile

wordKey='Word'
sseKey='4-Words'
avxKey='8-Words'
cacheKey='16-Words'

set multiplot layout 1,4 rowsfirst
set label 11 center at graph 0.5,char 1 "(a)" 
set bmargin 5

set title 'FeedForward Evaluation'
unset key
plot forwardFile using 1:2 title wordKey lw 2, '' using 1:3 ti sseKey lw 2, '' using 1:4 ti avxKey lw 2 , '' using 1:5 ti cacheKey lw 2

set label 11 "(b)"
set title 'BackPropagation'
unset key
plot backwardFile using 1:2 title wordKey lw 2, '' using 1:3 ti sseKey lw 2, '' using 1:4 ti avxKey lw 2 , '' using 1:5 ti cacheKey lw 2

set label 11 "(c)"
set title 'Weight Update'
plot weightFile using 1:2 title wordKey lw 2, '' using 1:3 ti sseKey lw 2, '' using 1:4 ti avxKey lw 2 , '' using 1:5 ti cacheKey lw 2

set border 0
unset label
unset title
unset tics
unset xlabel
unset ylabel
set yrange[0:1]
set key lmargin
plot 2 title wordKey lw 2, '' ti sseKey lw 2, '' ti avxKey lw 2 , '' ti cacheKey lw 2

unset multiplot
set out
