#include "shell.h"

#include "gaps.h"
#include "set.h"
#include "stats.h"

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void shell_sort(Stats *stats, uint32_t *arr, uint32_t length) {
    for (uint32_t g = 0; g < GAPS; g++) {
        uint32_t gapper = gaps[g];

        for (uint32_t i = gapper; i < length; i++) {
            uint32_t j = i;
            uint32_t temp = arr[i];
            move(stats, arr[i]);

            while (j >= gapper && cmp(stats, temp, arr[j - gapper]) == -1) {
                arr[j] = arr[j - gapper];
                move(stats, arr[j - gapper]);
                j -= gapper;
            }

            arr[j] = temp;
            move(stats, arr[j]);
        }
    }
}
