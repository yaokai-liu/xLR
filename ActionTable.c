/**
 * Project Name: equivC
 * Module Name: grammar
 * Filename: lex.h
 * Creator: Yaokai Liu
 * Create Date: 2024-03-16
 * Copyright (c) 2024 Yaokai Liu. All rights reserved.
 **/

#include "ActionTable.h"
#include <stdlib.h>

#include "BinTree.h"
#include "TrieTree.h"

typedef struct BinTree CellTree;

typedef struct ActionTable {
    uint32_t    buff_len;
    uint32_t    used_len;
    TrieTree *  keys;
    CellTree ** rows;
} ActionTable;

ActionTable * newActionTable() {
    auto const table = (ActionTable *) calloc(1, sizeof(ActionTable));
    table->keys = newTrieTree();
    return table;
}

int32_t destroyActionTable(ActionTable * const table) {
    int32_t res = destroyTrie(table->keys);
    for (uint32_t i = 0; i < table->used_len; i ++) {
        res += destroyBinTree(table->rows[i], nullptr);
    }
    free(table->rows);
    free(table);
    return res;
}

int32_t pushStatus(ActionTable * action_table);

inline Action makeAction(uint64_t const _action, uint64_t const _target) {
    return ((_action << 56) | ((_target << 8) >> 8));
}

#define NULL_ACTION makeAction(reject, 0LL)

Action getAction(const ActionTable * const action_table, const uint32_t status, const char * const key) {
    if (status >= action_table->used_len) { return NULL_ACTION; }
    auto const col = (uint64_t) getTrieValue(action_table->keys, key);
    if (!col) { return NULL_ACTION; }
    auto const row = action_table->rows[status];
    Action const a = (Action) getBinTreeValue(row, col);
    return a ? a : NULL_ACTION;
}

int32_t setAction(ActionTable *action_table, uint32_t const status, const char * const key, Action const cell) {
    if (getAction(action_table, status, key) != NULL_ACTION) { return -1; }
    if (cell == NULL_ACTION) { return 0; }
    uint64_t col = (uint64_t) getTrieValue(action_table->keys, key);
    if (col == 0) {
        col = getTrieCount(action_table->keys) + 1;
        setTrieValue(action_table->keys, key, (void *) col);
    }
    while (status >= action_table->used_len) { pushStatus(action_table); }
    auto const row = action_table->rows[status];
    setBinTreeValue(row, col, (void *) cell);
    return 0;
}

#define BUFF_ALLOC_LEN  32
int32_t pushStatus(ActionTable * const action_table) {
    if (action_table->used_len >= action_table->buff_len) {
        action_table->buff_len += BUFF_ALLOC_LEN;
        auto const new_buff_len = sizeof(CellTree *) * action_table->buff_len;
        auto const new_arrays = (CellTree **) realloc(action_table->rows, new_buff_len);
        if (!new_arrays) { return -1; }
        action_table->rows = new_arrays;
    }
    action_table->rows[action_table->used_len] = newBinTree();
    action_table->used_len ++;
    return 0;
}
