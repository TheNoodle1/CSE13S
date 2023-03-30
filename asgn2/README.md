This is a C program that implements a test harness for a numerical library that calculates various mathematical constants, such as e and pi. It has various options to perform different tests and output verbose statistics if desired.

Please ensure all the following files are installed in the same directory:

	bbp.c: This contains the implementation of the Bailey-Borwein-Plouffe formula to approximate π and the function to return 			the number of computed terms.

	e.c: This contains the implementation of the Taylor series to approximate Euler’s number e and the function to return the number of computed terms.

	euler.c: This contains the implementation of Euler’s solution used to approximate π and the function to return the number of computed terms.

	madhava.c: This contains the implementation of the Madhava series to approximate π and the function to return the number of computed terms.
	
	mathlib-test.c: This contains the main() function which tests each of the math library functions.

	mathlib.h: This contains the interface for the math library.

	newton.c: This contains the implementation of the square root approximation using Newton’s method and the function to return the number of computed iterations.

	viete.c: This contains the implementation of Viète’s formula to approximate π and the function to return
the number of computed factors


	
To use the file, open terminal and navigate to the appropriate directory

Compile all the files (make clean, make all)

Type "chmod +x {file}" into the terminal if you want to edit the file

Once compiled without errors, the following options are avaliable:

	-a Runs all tests.
	-e Runs e test.
	-b Runs BBP pi test.
	-m Runs Madhava pi test.
	-r Runs Euler pi test.
	-v Runs Viete pi test.
	-n Runs Newton square root tests.
	-s Print verbose statistics.
	-h Display program synopsis and usage.
	
Example: ./mathlib-test.c -a runs all tests and prints statistics.
