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

TrieNode *trie_node_create(uint16_t index) {
    TrieNode *node = (TrieNode *) malloc(sizeof(TrieNode));
    node->code = index;
    for (int i = 0; i < ALPHABET; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void trie_node_delete(TrieNode *n) {
    if (n == NULL)
        return;
    for (int i = 0; i < ALPHABET; i++) {
        trie_node_delete(n->children[i]);
    }
    free(n);
}

TrieNode *trie_create(void) {
    TrieNode *root = trie_node_create(EMPTY_CODE);
    if (root == NULL) {
        return NULL;
    }
    return root;
}

void trie_reset(TrieNode *root) {
    for (int i = 0; i < ALPHABET; i++) {
        if (root->children[i] != NULL) {
            trie_delete(root->children[i]);
            root->children[i] = NULL;
        }
    }
}

void trie_delete(TrieNode *n) {
    if (n == NULL) {
        return;
    }

    for (int i = 0; i < ALPHABET; i++) {
        trie_delete(n->children[i]);
        n->children[i] = NULL;
    }

    trie_node_delete(n);
}

TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    return n->children[sym];
}
