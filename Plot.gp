set terminal pngcairo size 800, 600 enhanced font 'Arial,12'
set output 'plot1.png'
set title 'Как же мне поебать' font 'Arial,16'
set xlabel 'x' font 'Arial,14'
set ylabel 'f(x)' font 'Arial,14'
set grid lt 0 lw 1 lc rgb '#DDDDDD'
set key outside right top box
set yrange [-1:1]
set object circle at 0,0 size 0.1 fillcolor rgb "black" fillstyle solid noborder 
plot \
  (cos(x))*(sin(x)) with lines lw 2 lc rgb 'red' title 'func', \
  (((sin(x))*(-1))*(sin(x)))+((cos(x))*(cos(x))) with lines lw 2 lc rgb 'blue' title 'diff', \
  ((x)**(1))+((-0.666667)*((x)**(3))) with lines lw 2 lc rgb 'green' title 'taylor'
