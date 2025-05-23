/* License
 *
 * xLR - Dynamic LR(1) Grammar Parser
 * Copyright (C) 2025 Yaokai Liu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *
 * Project Name: xLR
 * Module Name:
 * Filename: LRContext.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-15
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_LR_CONTEXT_H
#define XLR_LR_CONTEXT_H

#include "array.h"
#include "avl-tree.h"
#include "set.h"

enum XLR_ERROR_CODE_ENUM {
  SUCCESS,
  XLR_ERROR_SR_CONFLICT,
  XLR_ERROR_RS_CONFLICT,
  XLR_ERROR_RR_CONFLICT,

  XLR_ERROR_TARGET_MISMATCH,
  XLR_ERROR_DUPLICATED_SET_RULE,
  XLR_ERROR_MULTI_EMPTY_RULE,
};

enum XLR_TYPE_ENUM {
  XLR_TYPE_BAD_TYPE = 0,
  XLR_TYPE_SYMBOL,
  XLR_TYPE_RULE,
  XLR_TYPE_ITEM,
  XLR_TYPE_STATE,
  XLR_TYPE_ACTION,
  XLR_TYPE_RULE_KEY,
  XLR_TYPE_ENV_KEY,
  XLR_TYPE_USE_KEY,
  XLR_TYPE_ACT_KEY,
};

typedef struct LRAction LRAction;
typedef struct LRState LRState;
typedef struct LRSymbol LRSymbol;
typedef struct LRItem LRItem;
typedef struct LRRule LRRule;
#define INDEX(o) uint32_t

typedef struct LRContext {
  const Allocator *allocator;
  Array *sym_array;  // Array<LRSymbol>
  Array *rule_array;  // Array<LRRule>
  Array *state_array;  // Array<LRState>
  uint32_t error;
} LRContext;

typedef struct LRRulePair LRRulePair;  // Pair<bool, INDEX(LRRule)>
typedef struct LREnvPair LREnvPair;  // Pair<INDEX(LRState), INDEX(LRTerminal)>
typedef struct LRActKeyPair LRActKeyPair;

int32_t LRAction_cmp(const LRAction *a, const LRAction *b);
uint64_t LREnvPair_hash(const LREnvPair *pair);
uint64_t LRActKeyPair_hash(const LRActKeyPair *pair);

uint32_t LRContext_set_rule(LRContext *context, INDEX(LRRule) i_rule, uint64_t enable_flag);

#endif  // XLR_LR_CONTEXT_H
