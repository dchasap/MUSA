#!/bin/gnuplot
set title plottitle
set ylabel 'Bandwidth [GB/s]'
set xlabel 'Number of cores'
set tics nomirror out scale 0.7
set xtics ("1" 0, "2" 1, "4" 2, "8" 3, "16" 4)
set grid
set border 3
set term pngcairo enhanced
set output filename
set datafile separator ":"
set key autotitle columnhead
set key top left
set xrange [-0.5:4.5]
load 'dark2.pal'
plot for [i=2:columns] input_csv using 0:i with linespoints
