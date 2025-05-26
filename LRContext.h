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
#include "dict.h"

enum XLR_ERROR_CODE_ENUM {
  XLR_SUCCESS,
  XLR_ERROR_SR_CONFLICT,
  XLR_ERROR_RS_CONFLICT,
  XLR_ERROR_RR_CONFLICT,

  XLR_ERROR_TARGET_MISMATCH,
  XLR_ERROR_DUPLICATED_SET_RULE,
  XLR_ERROR_MULTI_EMPTY_RULE,

  XLR_ERROR_BAD_QUANTIFIER,
  XLR_ERROR_BAD_TOKEN,
};

enum XLR_TYPE_ENUM {
  XLR_TYPE_BAD_TYPE = 0,
  XLR_TYPE_CHAR,
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

typedef enum SYMBOL_TYPE_ENUM : uint8_t {
  SYMTYPE_BAD_TOKEN,
  SYMTYPE_EMPTY,
  SYMTYPE_TERMINATOR,
  SYMTYPE_TERMINAL,
  SYMTYPE_NON_TERMINAL,
} symtype;

typedef enum ACTION_TYPE_ENUM : uint8_t {
  ACTTYPE_REJECT,
  ACTTYPE_STACK,
  ACTTYPE_REDUCE,
  ACTTYPE_REPEAT,
} acttype;

typedef enum STATE_TYPE_ENUM : uint8_t {
  STATYPE_NORMAL,
  STATYPE_REPEAT,
} statype;

typedef INDEX(LRSymbol) trans_t(INDEX(LRSymbol));

typedef struct LRRulePair {
  bool enabled;
  INDEX(LRRule) rule;
} LRRulePair;  // Pair<bool, INDEX(LRRule)>

typedef struct LREnvPair {
  // state that will accept the symbol (or rule)
  INDEX(LRState) state;
  // symbols that after the symbol (or rule) in the state
  INDEX(LRTerminal) follow;
} LREnvPair;

typedef struct LRActKeyPair {
  // the symbol determines the action
  INDEX(LRTerminal) symbol;
  // other info to distinguish same symbol according the state
  uint32_t      key_info;
} LRActKeyPair;

typedef struct LRAction {
  acttype acttype;
  // rules that rules this action
  Set * rules;
  /*
   * if acttype:
   * is ACTTYPE_STACK:        next state index;
   * is ACTTYPE_REDUCE:       reduce rule index;
   * is TRANSFORM:    target symbol index;
   */
  uint32_t index;
} LRAction;

typedef struct LRState {
  statype  type;
  uint32_t count;
  uint32_t index;
  /*
   * Dict<LRActKeyPair, REFER(LRAction)>
   */
  Dict *actions;
  trans_t *fn_convert;
} LRState;

typedef struct LRSymbol {
  symtype symtype;
  /*
   * offset in sym_array.
   */
  uint32_t index;
  /*
   * if symtype
   * is SYMTYPE_NON_TERMINAL:   Array<LRRulePair>;
   * otherwise:                 nullptr.
   */
  Array *rules;
  /*
   * if symtype
   * is SYMTYPE_NON_TERMINAL:   Dict<LRTerminal, Set>;
   * otherwise:                 nullptr.
   */
  Dict *firsts;
  /*
   * if symtype
   * is SYMTYPE_TERMINAL:       Dict<LREnvPair, Set>;
   * is SYMTYPE_NON_TERMINAL:   Dict<LREnvPair, Set>;
   * otherwise:                 nullptr.
   */
  Dict *envs;
} LRSymbol, LRTerminal;

typedef struct LRRule {
  Array *items;  // Array<INDEX(LRSymbol)>
  INDEX(LRSymbol) target;
  bool enabled;
} LRRule;

typedef struct LRItem {
  INDEX(LRState) state;
  INDEX(LRSymbol) look;
  INDEX(LRRule) rule;
  uint32_t pos;
} LRItem;

typedef struct LRContext {
  const Allocator *allocator;
  Array *ident_array; // Array<char_t>
  AVLTree *sym_tree; // Array<REFER(char_t), REFER(LRSymbol)>
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
LRContext *LRContext_new(const Allocator *allocator);
void LRContext_destroy(LRContext *context);

#endif  // XLR_LR_CONTEXT_H
