#!/bin/gnuplot
set title graph_title
set ylabel 'SpeedUp'
set xlabel 'CPUS'
set tics nomirror out scale 0.75
set xtics ("1" 1, "2" 3, "4" 5, "8" 7, "16" 9, "32" 11, "64" 13)
set grid
set border 3
set term pngcairo enhanced
set output filename
set datafile separator ":"
set key autotitle columnhead
set key top left
#set xrange [0.9:32.1]
load 'dark2.pal'
set style fill solid
set boxwidth 0.5
plot input_csv every 3 using 1:2 with boxes title 'MareNostrum 4' ls 1, \
     input_csv every 3::1 using 1:2 with boxes title 'ARM High End' ls 2, \
     input_csv every 3::2 using 1:2 with boxes title 'ARM Low Power' ls 3
