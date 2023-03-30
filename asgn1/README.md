Please ensure all the following files are installed in the same directory:
	plot.sh
	monte_carlo.c
	Makefile

To use the file, open terminal and navigate to the appropriate directory

Complie monte_carlo.c ("gcc monte_carlo.c")

Type "chmod +x plot.sh" into the terminal, this is only neccesary the first time

To launch the script, use "./plot.sh", the script will automate the creation of two graphs which is computed by monte_carlo.c and graphed by gnuplot.

Some files should be created in the folder including various .dat files, ignore these

The graphs created are in pdf files titled monte_carlo.pdf and Monte Carlo Error Estimation.pdf

The program plots tens of thousands of points in either graph. In the first graph, blue points are in the circle while red points are outside of it. The color is determined by the distance from the origin which indirectly places blue dots inside the circle since it's radius is 1. The second graph shows the error estimation for four independent runs of the program. The error should decrease as it continues.
