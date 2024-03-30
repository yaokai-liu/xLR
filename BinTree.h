/**
 * Project Name: equivC
 * Module Name: grammar
 * Filename: lex.h
 * Creator: Yaokai Liu
 * Create Date: 2024-03-16
 * Copyright (c) 2024 Yaokai Liu. All rights reserved.
 **/


#ifndef BINTREE_H
#define BINTREE_H

#include <stdint.h>

typedef struct BinTree BinTree;

BinTree * newBinTree();
int32_t destroyBinTree(BinTree * tree, int32_t (*del_value)(void *));

uint64_t getBinTreeHeight(const BinTree * tree);
void * getBinTreeValue(const BinTree * tree, uint64_t key);
int32_t setBinTreeValue(BinTree * tree, uint64_t key, void * value);

#endif //BINTREE_H
