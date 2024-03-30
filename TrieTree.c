/**
 * Project Name: equivC
 * Module Name: grammar
 * Filename: lex.h
 * Creator: Yaokai Liu
 * Create Date: 2024-03-16
 * Copyright (c) 2024 Yaokai Liu. All rights reserved.
 **/

#include "TrieTree.h"
#include <stdlib.h>
#include "BinTree.h"

typedef void * Any;

typedef struct TrieNode TrieNode;

typedef struct TrieNode {
    Any         value;
    BinTree     *children;
} TrieNode;

typedef struct TrieTree {
    uint64_t    count;
    TrieNode    *root;
} TrieTree;

TrieTree * newTrieTree() {
    TrieNode * node = calloc(1, sizeof(TrieNode));
    TrieTree * tree = calloc(1, sizeof(TrieTree));
    node->children = newBinTree();
    tree->root = node;
    return tree;
}
uint64_t getTrieCount(const TrieTree * const tree) {
    return tree->count;
}
Any getTrieValue(const TrieTree * const tree, const char * key) {
    const TrieNode * trie_node = tree->root;
    for (int i = 0; key[i]; i++) {
        if (!trie_node->children) { return nullptr; }
        trie_node = getBinTreeValue(trie_node->children, key[i]);
        if (!trie_node) { return nullptr; }
    }
    return trie_node->value;
}

int32_t setTrieValue(TrieTree * tree, const char * key, Any const value) {
    TrieNode * trie_node = tree->root;
    for (int i = 0; key[i]; i++)  {
        auto node = (TrieNode *) getBinTreeValue(trie_node->children, key[i]);
        if (!node) {
            node = calloc(1, sizeof(TrieNode));
            node->children = newBinTree();
            if (-1 == setBinTreeValue(trie_node->children, key[i], node)) return -1;
        }
        trie_node = node;
    }
    if (!trie_node->value) tree->count ++;
    trie_node->value = value;
    return 0;
}

int32_t delTrieValue(TrieTree * tree, const char * key) {
    TrieNode * trie_node = tree->root;
    for (int i = 0; key[i]; i++) {
        if (!trie_node->children) { return -1; }
        auto const node =  (TrieNode *) getBinTreeValue(trie_node->children, key[i]);
        if (!node) { return -1; }
        trie_node = node;
    }
    if (!trie_node->value) { return -1; }
    trie_node->value = nullptr; tree->count--;
    return 0;
}
int32_t delTrieNode(TrieNode * trie_node);
int32_t destroyTrie(TrieTree * tree) {
    if (! tree) { return -1; }
    int32_t const res = delTrieNode(tree->root);
    free(tree);
    return res;
}


int32_t delTrieNode(TrieNode * trie_node) {
    if (!trie_node) { return 0; }
    int32_t const res = destroyBinTree(
        trie_node->children,
        (int32_t (*)(void *)) delTrieNode
    );
    free(trie_node);
    return res;
}
