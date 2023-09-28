#!/bin/gnuplot
set title 'Total number of memory requests to DRAM'
set ylabel 'Number of requests'
set xlabel 'Number of cores'
set tics nomirror out scale 0.7
set xtics ("1" 0, "2" 1, "4" 2, "8" 3, "16" 4)
set grid
set border 3
set term pngcairo enhanced
set output filename
set datafile separator ":"
set key autotitle columnhead
set key center rmargin
set xrange [-0.5:4.5]
set yrange [0:]
load 'dark2.pal'
plot for [i=2:columns] input_csv using 0:i with linespoints
