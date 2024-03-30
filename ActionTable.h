/**
 * Project Name: equivC
 * Module Name: grammar
 * Filename: lex.h
 * Creator: Yaokai Liu
 * Create Date: 2024-03-16
 * Copyright (c) 2024 Yaokai Liu. All rights reserved.
 **/

#ifndef LEX_ACTIONTABLE_H
#define LEX_ACTIONTABLE_H

#include <stdint.h>

enum action : uint64_t {
    reject = 0LL, shift = 1LL, jump = 2LL, reduce = 3LL, accept = 4LL
};


// action :     high 8 bits
// target :     low 56 bits
typedef uint64_t Action;
Action makeAction(uint64_t _action, uint64_t _target);

typedef struct ActionTable ActionTable;

ActionTable * newActionTable();
int32_t destroyActionTable(ActionTable * table);

Action getAction(const ActionTable *action_table, uint32_t status, const char * key);
int32_t setAction(ActionTable *action_table, uint32_t status, const char * key, Action cell);

#endif //LEX_ACTIONTABLE_H
