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
#include "category.h"
#include "stack.h"

enum CONTEXT_OBJECT_TYPE_ENUM: uint32_t {
  XLR_OBJECT_NULL,
  XLR_OBJECT_NAME,

  XLR_OBJECT_IDENT,
  XLR_OBJECT_ENUM,
  XLR_OBJECT_ATTR,
  XLR_OBJECT_TYPE,
  XLR_OBJECT_FUNC,
  XLR_OBJECT_VAR,
  XLR_OBJECT_TEXT,
  XLR_OBJECT_BLOCK,

  XLR_OBJECT_PLAIN,
  XLR_OBJECT_SYMBOL,
  XLR_OBJECT_STATE,
  XLR_OBJECT_RULE,
  XLR_OBJECT_LR_ITEM,
  XLR_OBJECT_LR_ACTION,
  XLR_OBJECT_RULE_KEY,
  XLR_OBJECT_ENV_KEY,
  XLR_OBJECT_USE_KEY,
  XLR_OBJECT_ACT_KEY,
};

typedef enum SYMBOL_TYPE_ENUM : uint8_t {
  XLR_SYMTYPE_BAD_TOKEN,
  XLR_SYMTYPE_EMPTY,
  XLR_SYMTYPE_TERMINATOR,
  XLR_SYMTYPE_TERMINAL,
  XLR_SYMTYPE_NON_TERMINAL,
} symtype;

typedef enum STATE_TYPE_ENUM : uint8_t {
  XLR_STATE_TYPE_NORMAL,
  XLR_STATE_TYPE_REPEAT,
} statype;

enum BUILTIN_SYMBOL_INDEX_ENUM {
  XLR_SYM_INDEX_EMPTY = 0,
  XLR_SYM_INDEX_TERMINATOR = 1,
  XLR_SYM_INDEX_FINIAL = 2,
};

enum BUILTIN_STATE_INDEX_ENUM {
  XLR_STA_INDEX_BAD_STATE = 0,
  XLR_STA_INDEX_INIT_STATE = 1,
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
  Dict *actions; // Dict<LRActKeyPair, LRAction>
} LRState;

typedef struct LRSymbol {
  symtype symtype;
  /*
   * offset in sym_array.
   */
  uint32_t index;
  /*
   * the type the symbol corresponds
   */
  REFER(LRType) target;
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
  Array *name_array;    // Array<char_t>
  Array *ident_array;   // Array<Identifier>
  Trie  *ident_trie;    // Trie<char_t, REFER(Identifier)>
  /*
   * AVLTree<REFER(char_t), XLR_OBJECT_TYPE_ENUM>
   * Only record
   *    XLR_OBJECT_IDENT, or
   *    XLR_OBJECT_ENUM, or
   *    XLR_OBJECT_ATTR, or
   *    XLR_OBJECT_TYPE, or
   *    XLR_OBJECT_FUNC.
   */
  AVLTree *def_tree;
  /*
   * AVLTree<REFER(char_t), REFER(LRVariable)>
   * only record builtin variables
   */
  AVLTree *builtin_var_tree;
  Array *plain_array;   // Array<uint64_t>
  AVLTree *plain_tree;  // AVLTree<uint64_t, REFER(uint64_t)>
  Array *sym_array;     // Array<LRSymbol>
  AVLTree *sym_tree;    // AVLTree<REFER(union {char_t c;uint64_t p;}), REFER(LRSymbol)>
  Array *rule_array;    // Array<LRRule>
  AVLTree *rule_tree;   // AVLTree<REFER(char_t), REFER(LRRule)>
  Array *type_array;    // Array<LRType>
  AVLTree *type_tree;   // AVLTree<REFER(char_t), REFER(LRType)>
  Array *text_array;    // Array<char_t>
  Array *block_array;   // Array<ActionBlock>
  Array *state_array;   // Array<LRState>
  Array *var_array;     // Array<LRVariable>
  AVLTree *func_tree;   // AVLTree<REFER(char_t), REFER(LRFunction)>
  Array *func_array;    // Array<LRFunction>
  Trie *expr_trie;      // Trie<REFER(_any_), REFER(LRVariable)>

  uint64_t kw_as_ident; // bits for keywords to tokenize as identifiers

  ActionBlock *curr_block;

  INDEX(LRState) state;

  bool     in_pattern;

  Stack *params_stack; // Stack<Array<LRParameter> *>
  Array *curr_params;  // Array<LRParameter>

  uint32_t error;
} LRContext;

typedef struct LRRulePair LRRulePair;  // Pair<bool, INDEX(LRRule)>
typedef struct LREnvPair LREnvPair;  // Pair<INDEX(LRState), INDEX(LRTerminal)>
typedef struct LRActKeyPair LRActKeyPair;

uint32_t LRContext_set_rule(LRContext *context, INDEX(LRRule) i_rule, bool enable_flag);

int32_t LRAction_cmp(const LRAction *a, const LRAction *b);
uint64_t LREnvPair_hash(const LREnvPair *pair);
uint64_t LRActKeyPair_hash(const LRActKeyPair *pair);

#define LRRule_new_items() Array_new(sizeof(INDEX(LRSymbol)), XLR_OBJECT_SYMBOL, context->allocator)

#define LRSymbol_new_rules() Array_new(sizeof(LRRulePair), XLR_OBJECT_RULE, allocator)
#define LRSymbol_new_envs() Dict_new(sizeof(LREnvPair), sizeof_set, (key_t *) LREnvPair_hash, \
  XLR_OBJECT_SYMBOL, nullptr, (destruct_t *) LRRuleSet_release, context->allocator)
#define LRSymbol_new_firsts() Dict_new(sizeof(INDEX(LRTerminal)), sizeof_set, nullptr, \
  XLR_OBJECT_SYMBOL, nullptr, (destruct_t *) LRRuleSet_release, allocator)

#define LRState_new_actions() Dict_new(sizeof(LRActKeyPair), sizeof(LRAction), (key_t *) LRActKeyPair_hash, \
  XLR_OBJECT_ACT_KEY, nullptr, (destruct_t *) LRAction_release, context->allocator)
#define LRContext_new_ruleset() Set_new(sizeof(INDEX(LRRule)), \
  XLR_OBJECT_RULE_KEY, nullptr, (destruct_t *) LRRuleSet_release, context->allocator)

#define LRSymbol_new()  {                     \
  .symtype = XLR_SYMTYPE_TERMINAL,            \
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

LRVariable *LRContext_get_variable(const LRContext *context, REFER(Identifier) v_ident);

REFER(char_t) LRContent_new_text_content(LRContext *context, const char_t *text, uint32_t size);
#define LRContent_add_text_content(context, text_content, size) \
                         Array_append((context)->text_array, (text_content), (size))

REFER(LRSymbol) LRContext_plain_to_sym(LRContext *context, uint64_t plain);

#define LRContext_get_attr(name)      AVLTree_get(context->attr_tree, (uint64_t) Trie_get(context->ident_trie, name))
#define LRContext_builtin_attr(index) AVLTree_get(context->attr_tree, (uint64_t) Trie_get(context->ident_trie, BUILTIN_ATTRS[index].ident))
#define LRContext_get_type(name)      AVLTree_get(context->type_tree, (uint64_t) Trie_get(context->ident_trie, name))
#define LRContext_builtin_type(index) AVLTree_get(context->type_tree, (uint64_t) Trie_get(context->ident_trie, BUILTIN_TYPES[index].ident))
#define LRContext_get_func(name)      AVLTree_get(context->func_tree, (uint64_t) Trie_get(context->ident_trie, name))
#define LRContext_builtin_func(index) AVLTree_get(context->func_tree, (uint64_t) Trie_get(context->ident_trie, BUILTIN_FUNCS[index].ident))

#define LRContext_builtin_var(index)  AVLTree_get(context->builtin_var_tree,  (uint64_t) Trie_get(context->ident_trie, BUILTIN_VARS[index].ident))

#endif  // XLR_LR_CONTEXT_H
