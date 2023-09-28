#!/bin/gnuplot
set title graph_title
set ylabel 'SpeedUp'
set xlabel 'cpus'
set tics nomirror out scale 0.75
set grid
set border 3
set term pngcairo enhanced
set output filename
set datafile separator ":"
set key autotitle columnhead
set key top left
#set xrange [0.9:32.1]
load 'dark2.pal'
plot input_csv using 1:2 with linespoints title 'Native' ls 1 lw 2 pt 1, \
     input_csv using 1:3 with linespoints title 'Burst' ls 2 lw 2 pt 2, \
     input_csv using 1:4 with linespoints title 'Memory' ls 3 lw 2 pt 3
