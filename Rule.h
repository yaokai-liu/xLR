/**
 * Project Name: equivC
 * Module Name: grammar
 * Filename: lex.h
 * Creator: Yaokai Liu
 * Create Date: 2024-03-16
 * Copyright (c) 2024 Yaokai Liu. All rights reserved.
 **/

#ifndef LEX_RULE_H
#define LEX_RULE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Object.h"
typedef struct LrItem LrItem;
typedef struct {
    LrItem *    target;
    uint32_t    order;
    uint32_t    argc;
    Object * (*produce) (...);
} Rule;

#ifdef __cplusplus
};
#endif

#endif //LEX_RULE_H
