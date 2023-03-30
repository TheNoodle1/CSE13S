#include "quick.h"

#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int partition(Stats *stats, uint32_t *quick_array, int lo, int hi) {
    int i = lo - 1;

    for (int j = lo; j <= hi - 1; j++) {
        if (cmp(stats, quick_array[j - 1], quick_array[hi - 1]) < 0) {
            i++;
            swap(stats, &quick_array[j - 1], &quick_array[i - 1]);
        }
    }

    swap(stats, &quick_array[i], &quick_array[hi - 1]);
    return i + 1;
}

void quick_sorter(Stats *stats, uint32_t *quick_array, int lo, int hi) {
    if (lo < hi) {
        int p = partition(stats, quick_array, lo, hi);
        quick_sorter(stats, quick_array, lo, p - 1);
        quick_sorter(stats, quick_array, p + 1, hi);
    }
}

void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n);
}
