#include "universe.h"

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
} Universe;

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *myUni = (Universe *) calloc(1, sizeof(Universe));
    myUni->rows = rows;
    myUni->cols = cols;
    myUni->grid = calloc(rows, sizeof(bool *));
    for (uint32_t i = 0; i < rows; i++) {
        myUni->grid[i] = calloc(cols, sizeof(bool));
    }
    myUni->toroidal = toroidal;
    return myUni;
}

void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < u->rows; i++) {
        free(u->grid[i]);
    }
    free(u->grid);
    free(u);
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = true;
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = false;
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= u->rows || c >= u->cols) {
        return false;
    }
    return u->grid[r][c];
}

bool uv_populate(Universe *u, FILE *infile) {
    uint32_t i = 0;
    uint32_t r, c;
    while (fscanf(infile, "%u %u", &r, &c) == 2) {
        i++;
        if (r >= u->rows || c >= u->cols) {
            return false;
        }
        if (i > 0) {
            uv_live_cell(u, r, c);
        }
    }
    return true;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t count = 0;
    int dr[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dc[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    for (uint32_t i = 0; i < 8; i++) {
        uint32_t row = r + dr[i];
        uint32_t col = c + dc[i];
        if (!u->toroidal) {
            if (row >= 0 && row < u->rows && col >= 0 && col < u->cols) {
                if (u->grid[row][col]) {
                    count++;
                }
            }
        } else {
            row = (row + u->rows) % u->rows;
            col = (col + u->cols) % u->cols;
            if (u->grid[row][col]) {
                count++;
            }
        }
    }
    return count;
}

void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t r = 0; r < u->rows; r++) {
        for (uint32_t c = 0; c < u->cols; c++) {
            if (u->grid[r][c]) {
                fputc('o', outfile);
            } else {
                fputc('.', outfile);
            }
        }
        fputc('\n', outfile);
    }
}
