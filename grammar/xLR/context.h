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
 * Module Name: grammar/xLR
 * Filename: context.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-15
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_LR_CONTEXT_H
#define XLR_LR_CONTEXT_H

#include "xLR/error.h"
#include "avl-tree.h"
#include "xLR/xlr.h"
#include "array.h"
#include "trie.h"
#include "dict.h"
#include "set.h"
#include "target.h"
#include "types.h"

enum CONTEXT_OBJECT_TYPE_ENUM: uint32_t {
  OBJECT_NULL,
  OBJECT_IDENT,
  OBJECT_PLAIN,
  OBJECT_SYMBOL,
  OBJECT_STATE,
  OBJECT_RULE,
  OBJECT_TYPE,
  OBJECT_VAR,
  OBJECT_TEXT,
  OBJECT_BLOCK,
};

typedef enum SYMBOL_TYPE_ENUM : uint8_t {
  SYMTYPE_BAD_TOKEN,
  SYMTYPE_EMPTY,
  SYMTYPE_TERMINATOR,
  SYMTYPE_TERMINAL,
  SYMTYPE_NON_TERMINAL,
} symtype;

typedef enum STATE_TYPE_ENUM : uint8_t {
  STATYPE_NORMAL,
  STATYPE_REPEAT,
} statype;

enum BUILTIN_SYMBOL_INDEX_ENUM {
  SYM_INDEX_EMPTY = 0,
  SYM_INDEX_TERMINATOR = 1,
  SYM_INDEX_FINIAL = 2,
};

enum BUILTIN_STATE_INDEX_ENUM {
  STA_INDEX_BAD_STATE = 0,
  STA_INDEX_INIT_STATE = 1,
};

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

typedef struct LRAction LRAction;

typedef struct LRState {
  statype  type;
  uint32_t index;
  uint32_t count;
  /*
   * Dict<LRActKeyPair, LRAction>
   */
  Dict *actions;
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
  /*
   * attributes defined by TokenDefinition
   */
  Array *attr_array; // Array<LRVariable>
} LRSymbol, LRTerminal;

typedef struct LRRule {
  bool enabled;
  uint16_t cost;
  INDEX(LRSymbol) target;
  Array *items;  // Array<INDEX(LRSymbol)>
  ActionBlock *action;
} LRRule;

typedef struct LRItem {
  INDEX(LRState) state;
  INDEX(LRSymbol) look;
  INDEX(LRRule) rule;
  uint32_t pos;
} LRItem;

typedef struct LRContext {
  const Allocator *allocator;
  Array *ident_array;   // Array<char_t>
  Trie  *ident_trie;    // Trie<REFER(char_t)>
  Array *plain_array;   // Array<uint64_t>
  AVLTree *plain_tree;   // AVLTree<uint64_t, REFER(uint64_t)>
  Array *sym_array;     // Array<LRSymbol>
  AVLTree *sym_tree;    // AVLTree<REFER(union {char_t c;uint64_t p;}), REFER(LRSymbol)>
  Array *rule_array;    // Array<LRRule>
  AVLTree *rule_tree;   // AVLTree<REFER(char_t), REFER(LRRule)>
  Array *type_array;    // Array<LRType>
  AVLTree *type_tree;   // AVLTree<REFER(char_t), REFER(LRType)>
  Array *text_array;    // Array<char_t>
  Array *block_array;   // Array<ActionBlock>
  Array *state_array;   // Array<LRState>
  AVLTree *var_tree;  // AVLTree<REFER(char_t), REFER(Array<LRVariable>)>
  Array   *var_array; // Array<Array<LRVariable>>
  REFER(ActionBlock) curr_block;
  INDEX(LRState) state;
  bool     in_pattern;
  uint64_t kw_as_ident; // bits for keywords to tokenize as identifiers
  uint32_t error;
} LRContext;

typedef struct LRRulePair LRRulePair;  // Pair<bool, INDEX(LRRule)>
typedef struct LREnvPair LREnvPair;  // Pair<INDEX(LRState), INDEX(LRTerminal)>
typedef struct LRActKeyPair LRActKeyPair;

uint32_t LRContext_set_rule(LRContext *context, INDEX(LRRule) i_rule, bool enable_flag);

int32_t LRAction_cmp(const LRAction *a, const LRAction *b);
uint64_t LREnvPair_hash(const LREnvPair *pair);
uint64_t LRActKeyPair_hash(const LRActKeyPair *pair);

#define LRRule_new_items() Array_new(sizeof(INDEX(LRSymbol)), XLR_TYPE_SYMBOL, context->allocator)

#define LRSymbol_new_rules() Array_new(sizeof(LRRulePair), XLR_TYPE_RULE, allocator)
#define LRSymbol_new_envs() Dict_new(sizeof(LREnvPair), sizeof_set, (unikey_t *) LREnvPair_hash, \
  XLR_TYPE_SYMBOL, nullptr, (destruct_t *) LRRuleSet_release, context->allocator)
#define LRSymbol_new_firsts() Dict_new(sizeof(INDEX(LRTerminal)), sizeof_set, nullptr, \
  XLR_TYPE_SYMBOL, nullptr, (destruct_t *) LRRuleSet_release, allocator)

#define LRState_new_actions() Dict_new(sizeof(LRActKeyPair), sizeof(LRAction), (unikey_t *) LRActKeyPair_hash, \
  XLR_TYPE_ACT_KEY, nullptr, (destruct_t *) LRAction_release, context->allocator)
#define LRContext_new_ruleset() Set_new(sizeof(INDEX(LRRule)), \
  XLR_TYPE_RULE_KEY, nullptr, (destruct_t *) LRRuleSet_release, context->allocator)

#define LRSymbol_new()  {                     \
  .symtype = SYMTYPE_TERMINAL,                \
  .index = Array_length(context->sym_array),  \
  .rules = nullptr,                           \
  .firsts = nullptr,                          \
  .envs = LRSymbol_new_envs()                 \
}

void LRState_release(LRState *state, const Allocator *allocator);
void LRAction_release(LRAction *action, const Allocator *allocator);
void LRRule_release(LRRule *rule, const Allocator *allocator);
void LRRuleSet_release(Set *set, const Allocator *allocator);

void LRContext_init(LRContext *context);
void LRContext_destroy(LRContext *context);
LRContext *LRContext_new(const Allocator *allocator);
uint32_t LRContext_enable_rule(LRContext *context, INDEX(LRRule) i_rule);
uint32_t LRContext_disable_rule(LRContext *context, INDEX(LRRule) i_rule);
INDEX(LRRule) LRContext_add_rule(LRContext *context, ErrInfo *errInfo, const char_t *string);
INDEX(LRSymbol) LRContext_add_target(LRContext *context, ErrInfo *errInfo, const char_t *string);

void LRContext_state_action(LRContext *context, uint32_t state, Token *, const Allocator *allocator);

LRValue *LRContext_last_enum_val(LRContext *context, ErrInfo *errInfo);
INDEX(LRType) LRContext_typeof(LRContext *context, ErrInfo *errInfo, Expr *expr);
bool LRContext_subtype(LRContext *context, ErrInfo *errInfo, Expr *expr, LRType);
LRValue *LRContext_eval(LRContext *context, ErrInfo *errInfo, Expr *expr);

LRVariable *LRContext_get_variable(LRContext *context, REFER(Identifier) v_ident);

REFER(char_t) LRContent_new_text_content(LRContext *context, const char_t *text, uint32_t size);
#define LRContent_add_text_content(context, text_content, size) \
                         Array_append((context)->text_array, (text_content), (size))

REFER(LRSymbol) LRContext_plain_to_sym(LRContext *context, uint64_t plain);

#define LRContext_get_type(name)      AVLTree_get(context->type_tree, (uint64_t) Trie_get(context->ident_trie, name))
#define LRContext_builtin_type(index) AVLTree_get(context->type_tree, (uint64_t) Trie_get(context->ident_trie, BUILTIN_TYPES[index].name))
#define LRContext_get_var(name)       AVLTree_get(context->var_tree, (uint64_t) Trie_get(context->ident_trie, name))
#define LRContext_builtin_var(index)  AVLTree_get(context->var_tree, (uint64_t) Trie_get(context->ident_trie, BUILTIN_TYPES[index].name))

#endif  // XLR_LR_CONTEXT_H
