#include "batcher.h"
#include "gaps.h"
#include "heap.h"
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

#define OPTIONS "asbhq r:n:p: H"

int main(int argc, char **argv) {
    int opt = 0;
    uint32_t array_size = 100;
    uint32_t element_amount = 100;
    uint32_t seed = 13371453;
    char *seed_pointer;
    Set setter = set_empty();

    Stats stats;
    reset(&stats);

    bool a_option = false; //all		t
    bool s_option = false; // heap		t
    bool b_option = false; //batcher	t
    bool h_option = false; //shell		t
    bool q_option = false; //quick		t
    bool r_option = false; // seed		t
    bool n_option = false; //size		t
    bool p_option = false; // elements	t
    bool H_option = false; //help		t

    // Use getopt to parse the command-line options
    while ((opt = getopt(argc, argv, "asbhqr:n:p:H")) != -1) {
        switch (opt) {
        case 'a':
            // Run all tests
            a_option = true;
            break;
        case 's': s_option = true; break;
        case 'b': b_option = true; break;
        case 'h': h_option = true; break;
        case 'q': q_option = true; break;
        case 'r':
            r_option = true;
            seed = strtoul(optarg, &seed_pointer, 10);
            seed = atoi(optarg);
            break;
        case 'n':
            n_option = true;
            array_size = atoi(optarg);
            break;
        case 'p':
            p_option = true;
            element_amount = atoi(optarg);
            break;

        case 'H':
            printf("SYNOPSIS\n");
            printf("   A collection of comparison-based sorting algorithms.\n");
            printf("\nUSAGE\n");
            printf("   ./sorting_arm64 [-Hasbhqn:p:r:] [-n length] [-p elements] [-r seed]\n");
            printf("\nOPTIONS\n");
            printf("   -H              Display program help and usage.\n");
            printf("   -a              Enable all sorts.\n");
            printf("   -s              Enable Shell Sort.\n");
            printf("   -b              Enable Batcher Sort.\n");
            printf("   -h              Enable Heap Sort.\n");
            printf("   -q              Enable Quick Sort.\n");
            printf("   -n length       Specify number of array elements (default: 100).\n");
            printf("   -p elements     Specify number of elements to print (default: 100).\n");
            printf("   -r seed         Specify random seed (default: 13371453).\n");
            break;
        default:
            printf("SYNOPSIS\n");
            printf("   A collection of comparison-based sorting algorithms.\n");
            printf("\nUSAGE\n");
            printf("   ./sorting_arm64 [-Hasbhqn:p:r:] [-n length] [-p elements] [-r seed]\n");
            printf("\nOPTIONS\n");
            printf("   -H              Display program help and usage.\n");
            printf("   -a              Enable all sorts.\n");
            printf("   -s              Enable Shell Sort.\n");
            printf("   -b              Enable Batcher Sort.\n");
            printf("   -h              Enable Heap Sort.\n");
            printf("   -q              Enable Quick Sort.\n");
            printf("   -n length       Specify number of array elements (default: 100).\n");
            printf("   -p elements     Specify number of elements to print (default: 100).\n");
            printf("   -r seed         Specify random seed (default: 13371453).\n");
            break;
        }
    }

    if (a_option == true) {
        setter = set_insert(setter, 0);
        setter = set_insert(setter, 1);
        setter = set_insert(setter, 2);
        setter = set_insert(setter, 3);
        setter = set_insert(setter, 4);

        // paste everything for all
        // later, use if statement set_member to print all, paste each one so -a arrays will use the same array
    }

    if (s_option == true) {
        setter = set_insert(setter, 1);
    }

    if (b_option == true) {
        setter = set_insert(setter, 2);
    }

    if (h_option == true) {
        setter = set_insert(setter, 3);
    }

    if (q_option == true) {
        setter = set_insert(setter, 4);
    }

    if (r_option == true) {
        setter = set_insert(setter, 5);
    }

    if (n_option == true) {
        setter = set_insert(setter, 6);
    }

    if (p_option == true) {
        setter = set_insert(setter, 7);
    }

    if (H_option == true) {
        setter = set_insert(setter, 8);
    }

    srandom(seed);

    uint32_t *all_array = (uint32_t *) calloc(array_size, sizeof(uint32_t));
    uint32_t *shell_array = (uint32_t *) calloc(array_size, sizeof(uint32_t));
    uint32_t *batcher_array = (uint32_t *) calloc(array_size, sizeof(uint32_t));
    uint32_t *heap_array = (uint32_t *) calloc(array_size, sizeof(uint32_t));
    uint32_t *quick_array = (uint32_t *) calloc(array_size, sizeof(uint32_t));

    for (uint32_t i = 0; i < array_size; i++) {
        int randomize = random() & 0x3FFFFFFF;
        all_array[i] = randomize;
    }

    for (uint32_t i = 0; i < array_size; i++) {
        shell_array[i] = all_array[i];
    }

    for (uint32_t i = 0; i < array_size; i++) {
        batcher_array[i] = all_array[i];
    }

    for (uint32_t i = 0; i < array_size; i++) {
        heap_array[i] = all_array[i];
    }

    for (uint32_t i = 0; i < array_size; i++) {
        quick_array[i] = all_array[i];
    }

    if (set_member(setter, 1) == 1) {
        if (element_amount > array_size) {
            element_amount = array_size;
        }

        shell_sort(&stats, shell_array, array_size);
        printf("Shell Sort, %d elements, %lu moves, %lu compares\n", array_size, stats.moves,
            stats.compares);

        for (uint32_t i = 0; i < element_amount; i++) {
            if ((i != 0) && (i % 5 == 0)) {
                printf("\n");
            }
            printf("%13" PRIu32 " ", shell_array[i]);
        }

        if (element_amount != 0) {
            printf("\n");
        }

        reset(&stats);
        free(shell_array);
    }

    if (set_member(setter, 2) == 1) {
        if (element_amount > array_size) {
            element_amount = array_size;
        }

        batcher_sort(&stats, batcher_array, array_size);
        printf("Batcher Sort, %d elements, %lu moves, %lu compares\n", array_size, stats.moves,
            stats.compares);

        for (uint32_t i = 0; i < element_amount; i++) {
            if ((i != 0) && (i % 5 == 0)) {
                printf("\n");
            }
            printf("%13" PRIu32 " ", batcher_array[i]);
        }

        if (element_amount != 0) {
            printf("\n");
        }

        reset(&stats);
        free(batcher_array);
    }

    if (set_member(setter, 3) == 1) {
        if (element_amount > array_size) {
            element_amount = array_size;
        }

        heap_sort(&stats, heap_array, array_size);
        printf("Heap Sort, %d elements, %lu moves, %lu compares\n", array_size, stats.moves,
            stats.compares);

        for (uint32_t i = 0; i < element_amount; i++) {
            if ((i != 0) && (i % 5 == 0)) {
                printf("\n");
            }
            printf("%13" PRIu32 " ", heap_array[i]);
        }

        if (element_amount != 0) {
            printf("\n");
        }

        reset(&stats);
        free(heap_array);
    }

    if (set_member(setter, 4) == 1) {
        if (element_amount > array_size) {
            element_amount = array_size;
        }

        quick_sort(&stats, quick_array, array_size);
        printf("Quick Sort, %d elements, %lu moves, %lu compares\n", array_size, stats.moves,
            stats.compares);

        for (uint32_t i = 0; i < element_amount; i++) {
            if ((i != 0) && (i % 5 == 0)) {
                printf("\n");
            }
            printf("%13" PRIu32 " ", quick_array[i]);
        }

        if (element_amount != 0) {
            printf("\n");
        }

        reset(&stats);
        free(quick_array);
    }

    free(all_array);
    return 0;
}
