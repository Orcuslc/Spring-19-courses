# plot file for hw6b.1

set style data lines
plot 'hw6_2.dat' u 2:3
replot 'hw6_2.dat' u 8:9

set xlabel "x"
set ylabel "y"
set term png
set output "hw6b_2.1.png"
replot
set term x11
