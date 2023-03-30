This is a C program that implements a test harness for a numerical library that calculates various mathematical constants, such as e and pi. It has various options to perform different tests and output verbose statistics if desired.

Please ensure all the following files are installed in the same directory:

batcher.c
batcher.h
shell.c
shell.h
gaps.h
heap.c
heap.h
quick.c
quick.h
set.c
set.h
stats.c
stats.h
sorting.c
Makefile
	
To use the file, open terminal and navigate to the appropriate directory

Compile all the files (make clean, make all)

Type "chmod +x {file}" into the terminal if you want to edit the file

Once compiled without errors, the following options are avaliable:

	-a : Employs all sorting algorithms.
	-h : Enables Heap Sort.
	-b : Enables Batcher Sort.
	-s : Enables Shell Sort.
	-q : Enables Quick Sort.
	-r seed : Set the random seed to seed. The default seed is 13371453.
	-n size : Set the array size to size. The default size should be 100.
	-p elements : Print out elements number of elements from the array. The default number of elements to print out should be 	100. If the size of the array is less than the specified number of elements to print, print out the entire array and nothing more.
	-H : Prints out program usage.
	
Example: ./sorting -a -n 15 -p 5 runs all sorting algorithms, sets the number of elements in each array to 15, and prints out the only first 5 elements of each array.
