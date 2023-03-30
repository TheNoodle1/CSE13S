#include "universe.h"

#include <inttypes.h>
#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "ts n:i:o: h"
#define SLEEP   50000

typedef struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
} Universe;

int main(int argc, char **argv) {
    int opt = 0;
    uint32_t generations = 100;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    char *input_file_name;
    char *output_file_name;
    bool toroidal = false;
    bool s_loud = true;

    bool s_option = false;
    bool i_option = false;
    bool o_option = false;
    bool h_option = false;

    while ((opt = getopt(argc, argv, "ts n:i:o: h")) != -1) {
        switch (opt) {
        case 't': toroidal = true; break;
        case 's': s_option = true; break;
        case 'n': generations = atoi(optarg); break;
        case 'i':
            i_option = true;
            input_file_name = optarg;
            infile = fopen(optarg, "r");
            i_option = true;
            break;
        case 'o':
            outfile = fopen(optarg, "w");
            output_file_name = optarg;
            o_option = true;
            break;
        case 'h': h_option = true; break;
        default: h_option = true; break;
        }
    }

    if (s_option == true) {
        s_loud = false;
    }

    if (i_option == true) {
        if (infile == NULL) {
            printf("Error opening %s\n", input_file_name);
            exit(1);
        }
    }

    if (o_option == true) {
        if (outfile == NULL) {
            printf("Error opening %s\n", output_file_name);
            exit(1);
        }
    }

    if (h_option == true) {
        printf("SYNOPSIS\n");
        printf("    Conway's Game of Life\n");
        printf("\nUSAGE\n");
        printf("    ./life tsn:i:o:h\n");
        printf("\nOPTIONS\n");
        printf("    -t             Create your universe as a toroidal\n");
        printf("    -s             Silent - do not use animate the evolution\n");
        printf("    -n {number}    Number of generations [default: 100]\n");
        printf("    -i {file}      Input file [default: stdin]\n");
        printf("    -o {file}      Output file [default: stdout]\n");
    }

    // Opening the input file for universe dimensions
    uint32_t width, height;
    fscanf(infile, "%u %u", &width, &height);

    // Creating universe A and B and setting the initial conditions
    Universe *universe_A = uv_create(width, height, toroidal);
    Universe *universe_B = uv_create(width, height, toroidal);
    uv_populate(universe_A, infile);
    if (s_loud == true) {
        initscr();
        curs_set(FALSE);
    }

    for (uint32_t gen = 0; gen < generations; gen++) {
        for (uint32_t i = 0; i < universe_A->rows; i++) {
            for (uint32_t j = 0; j < universe_A->cols; j++) {
                uint32_t neighbors = uv_census(universe_A, i, j);
                if (uv_get_cell(universe_A, i, j) == 1) {
                    if (neighbors == 2 || neighbors == 3) {
                        uv_live_cell(universe_B, i, j);
                    } else {
                        uv_dead_cell(universe_B, i, j);
                    }
                } else {
                    if (neighbors == 3) {
                        uv_live_cell(universe_B, i, j);
                    } else {
                        uv_dead_cell(universe_B, i, j);
                    }
                }
            }
        }
        if (s_loud == true) {
            clear();
            clear();
            for (uint32_t row = 0; row < universe_A->rows; row++) {
                for (uint32_t col = 0; col < universe_A->cols; col++) {
                    if (uv_get_cell(universe_A, row, col)) {
                        mvprintw(row, col, "o");
                    }
                }
            }
            refresh();
            usleep(SLEEP);
        }
        for (uint32_t i = 0; i < universe_A->rows; i++) {
            for (uint32_t j = 0; j < universe_A->cols; j++) {
                // Use a temporary variable to store the value of universe_A
                bool temp = universe_A->grid[i][j];

                // Copy the value of universe_B to universe_A
                universe_A->grid[i][j] = universe_B->grid[i][j];

                // Copy the value of the temporary variable to universe_B
                universe_B->grid[i][j] = temp;
            }
        }
    }
    if (s_loud == true) {
        endwin();
    }

    uv_print(universe_A, outfile);
    uv_delete(universe_A);
    uv_delete(universe_B);

    return 0;
}
