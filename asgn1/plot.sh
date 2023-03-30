#!/bin/bash

make clean && make monte_carlo 	# Rebuild the monte_carlo executable.
(./monte_carlo -n 10000) > monte_carlo.dat 	# Place the data points into a file.

tail -n +2 monte_carlo.dat > monte_carlo_1.dat 	# Copies the contents of the original monte_carlo.dat into another file without the first row

awk '{print $3, $4, $5}' monte_carlo_1.dat > monte_carlo_2.dat 	# Repeats the previous step but with columns 3-5 instead

awk '{if($3==0) print $1, $2 > "monte_carlo_red.dat"; else print $1, $2 > "monte_carlo_blue.dat"}' monte_carlo_2.dat 	# Separates them into files based on their third column value



./monte_carlo -n 65536 -r $RANDOM | tail -n +2 | awk '{print $1, $2 - 3.1415926535897932}' > monte_carlo_error.dat 	# Repeats 65536 times with random xy coordinates
./monte_carlo -n 65536 -r $RANDOM | tail -n +2 | awk '{print $1, $2 - 3.1415926535897932}' > monte_carlo_error_1.dat 	# tail again only copies everything starting from the second row
./monte_carlo -n 65536 -r $RANDOM | tail -n +2 | awk '{print $1, $2 - 3.1415926535897932}' > monte_carlo_error_2.dat 	# awk prints only the first and second column into files with $2 being subtracted by pi so we can get the error 
./monte_carlo -n 65536 -r $RANDOM | tail -n +2 | awk '{print $1, $2 - 3.1415926535897932}' > monte_carlo_error_3.dat 	# Repeated four times for four individual files so when graphed, there will be four independent lines



# This is the here - document that is sent to gnuplot .
gnuplot <<END



set terminal pdf 	# Creates the file type, in this case, pdf
set output "monte_carlo.pdf"	 # Names the file

set xrange [0:1]	 # Sets the domain of the graph
set yrange [0:1]	 # Sets the range of the graph
set size ratio 1	 # Makes the graph square 

set object circle at 0,0 size 1 fc rgb "black"	 # Makes the quarter circle starting with the center at the origin with a radius of 1 in the color black

plot "monte_carlo_red.dat" with dots title "" lc "red", "monte_carlo_blue.dat" with dots title "" lc "blue" 	# Plots one file with red dots and another with blue, the files were previously separated by the value of the third column.

END

gnuplot <<END

set terminal pdf 	# Creates the file type, in this case, pdf
set output "Monte Carlo Error Estimation.pdf" 	# Names the file
set title "Monte Carlo Error Estimation" 	# Create the title of the graph
set grid xtics ytics	 # Makes the grid marks where the x and y tics are on the graph
set ylabel "Error"	 # Label the y-axis with "Error" as seen on the example
set yrange [-1:1]	 # Sets the range
set ytics -1, .5, 1	 # Creates the y tics
set logscale x 	# Scales the graph logarithmically on the x-axis
set xtics (1, 4, 16, 64, 256, 1024, 4094, 16384) 	# Sets the x tics as seen in the example graph

plot "monte_carlo_error.dat" with lines title "", "monte_carlo_error_1.dat" with lines title "", "monte_carlo_error_2.dat" with lines title "", "monte_carlo_error_3.dat" with lines title ""	 # Graphs all four lines in another file.



END
