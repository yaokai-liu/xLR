/**
 * Project Name: equivC
 * Module Name: grammar
 * Filename: lex.h
 * Creator: Yaokai Liu
 * Create Date: 2024-03-16
 * Copyright (c) 2024 Yaokai Liu. All rights reserved.
 **/

#ifndef LEX_TRIE_H
#define LEX_TRIE_H

#include <stdint.h>

typedef struct TrieTree TrieTree;

TrieTree * newTrieTree();
int32_t destroyTrie(TrieTree * tree);

uint64_t getTrieCount(const TrieTree * tree);
void * getTrieValue(const TrieTree * tree, const char * key);
int32_t setTrieValue(TrieTree * tree, const char * key, void * value);
int32_t delTrieValue(TrieTree * tree, const char * key);

#endif //LEX_TRIE_H
