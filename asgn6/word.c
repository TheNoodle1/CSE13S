#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <locale.h>

#include "trie.h"
#include "word.h"
#include "io.h"
#include "code.h"
#include "endian.h"

Word *word_create(uint8_t *syms, uint32_t len) {
    Word *w = (Word *) malloc(sizeof(Word));
    if (w != NULL) {
        w->syms = (uint8_t *) malloc(len * sizeof(uint8_t));
        if (w->syms != NULL) {
            for (uint8_t i = 0; i < len; i++) {
                w->syms[i] = syms[i];
            }
            w->len = len;
        } else {
            free(w);
            w = NULL;
        }
    }
    return w;
}

Word *word_append_sym(Word *w, uint8_t sym) {
    uint32_t new_len = w->len + 1;
    uint8_t *new_syms = (uint8_t *) malloc(new_len);
    memcpy(new_syms, w->syms, w->len);
    new_syms[w->len] = sym;
    Word *new_word = word_create(new_syms, new_len);
    free(new_syms);
    return new_word;
}

void word_delete(Word *w) {
    if (w != NULL) {
        if (w->syms != NULL) {
            free(w->syms);
            w->syms = NULL;
        }
        free(w);
        w = NULL;
    }
}

WordTable *wt_create(void) {
    WordTable *wt = (WordTable *) malloc((UINT16_MAX + 1) * sizeof(Word *));
    if (wt != NULL) {
        Word *empty_word = word_create(NULL, 0);
        if (empty_word == NULL) {
            free(wt);
            wt = NULL;
        } else {
            for (int i = 0; i <= UINT16_MAX; i++) {
                wt[i] = NULL;
            }
            wt[EMPTY_CODE] = empty_word;
        }
    }
    return wt;
}

void wt_reset(WordTable *wt) {
    if (wt != NULL) {
        for (int i = 0; i <= UINT16_MAX; i++) {
            if (wt[i] != NULL) {
                word_delete(wt[i]);
                wt[i] = NULL;
            }
        }
        Word *empty_word = word_create(NULL, 0);
        if (empty_word != NULL) {
            wt[EMPTY_CODE] = empty_word;
        }
    }
}

void wt_delete(WordTable *wt) {
    if (wt != NULL) {
        wt_reset(wt);
        Word *empty_word = wt[EMPTY_CODE];
        free(wt);
        wt = NULL;
        if (empty_word != NULL) {
            word_delete(empty_word);
        }
    }
}
