This C program implements John Conway's Game of Life. The Game of Life is a zero-player game since it only requires initial user input; the rules are few and simple:

1. Any live cell with two or three live neighbors survives.
2. Any dead cell with exactly three live neighbors becomes a live cell.
3. All other cells die, either due to loneliness or overcrowding.

The program requires an initial user input which may be done either through stdin or an input file. The output is also similar with the default being stdout or the user can select an output file. 

Please ensure all the following files are correctly downloaded in the same directory:

* universe.c - Contains various functions required to create a universe and modify, delete, and access it. 
* universe.h - The companion header file to universe.c above.
* life.c - Contains the main function from where the functions from universe.c is called. 
* Makefile 

The following files offer greater detail about the program(s):

* DESIGN.pdf
* WRITEUP.pdf
* README.md

To use the file, open terminal and navigate to the appropriate directory

Compile all the files (make clean, make all)

Type "chmod +x {file}" into the terminal if you want to edit the file

Once compiled without errors, the following options are avaliable:

    -t : Specify that the Game of Life is to be played on a toroidal universe.
    -s : Silence ncurses. Enabling this option means that nothing should be displayed by ncurses.
    -n generations : Specify the number of generations that the universe goes through. The default number of generations is 100.
    -i input : Specify the input file to read in order to populate the universe. By default the input should be stdin.
    -o output : Specify the output file to print the final state of the universe to. By default the output should be stdout.
    -h : Display program synopsis and usage.
    
Example: *./life -i lists/101.txt -s -n 500* would use 101.txt as the input, silence ncurses, and go through 500 generations.
