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
 * Filename: context.c
 * Creator: Yaokai Liu
 * Create Date: 2025-05-15
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "context.h"
#ifdef XLR_ENABLE_DEBUG
#include <stdio.h>
#endif
#include "xLR/char_t.h"
#include "generated/xLR/action-table.gen.h"
#include "target.h"
#include <string.h>
#include "builtin.h"
#include "enum.h"
#include "meman-utils.h"

#define Array_foreach(type, _array, doing)              \
  do {                                                  \
    uint32_t __n_elements = Array_length(_array);       \
    type *__elements = Array_first_real(_array);        \
    for (uint32_t __i = 0; __i < __n_elements; __i++) { \
      type *__element = &__elements[__i];               \
      doing                                             \
    }                                                   \
  } while (false)

#define Array_foreach_virt(type, array, doing)          \
  do {                                                  \
    uint32_t __n_elements = Array_length(array);        \
    type *__elements = Array_first_virt(array);         \
    for (uint32_t __i = 0; __i < __n_elements; __i++) { \
      type *__element = &__elements[__i];               \
      doing                                             \
    }                                                   \
  } while (false)

#define Set_foreach(type, set, doing)                   \
  do {                                                  \
    uint32_t __n_elements = Set_count(set);             \
    type *__elements = Set_data(set);                   \
    for (uint32_t __i = 0; __i < __n_elements; __i++) { \
      type *__element = &__elements[__i];               \
      doing                                             \
    }                                                   \
  } while (false)

#define Dict_foreach(key_type, ele_type, dict, doing)     \
  do {                                                    \
    uint32_t __n_keys = Dict_count(dict);                 \
    key_type *__keys = Dict_keys(dict);                   \
    for (uint32_t __i = 0; __i < __n_keys; __i++) {       \
      key_type *__key = &__keys[__i];                     \
      ele_type *__element = Dict_get(dict, &__keys[__i]); \
      doing                                               \
    }                                                     \
  } while (false)

#define Dict_foreach_key(key_type, ele_type, dict, doing) \
  do {                                                    \
    uint32_t __n_keys = Dict_count(dict);                 \
    key_type *__keys = Dict_keys(dict);                   \
    for (uint32_t __i = 0; __i < __n_keys; __i++) {       \
      key_type *__key = &__keys[__i];                     \
      doing                                               \
    }                                                     \
  } while (false)

#define Dict_foreach_ele(key_type, ele_type, dict, doing) \
  do {                                                    \
    uint32_t __n_keys = Dict_count(dict);                 \
    key_type *__keys = Dict_keys(dict);                   \
    for (uint32_t __i = 0; __i < __n_keys; __i++) {       \
      ele_type *__element = Dict_get(dict, &__keys[__i]); \
      doing                                               \
    }                                                     \
  } while (false)

static void LRSymbol_add_rule(LRContext *context, INDEX(LRSymbol) i_sym, INDEX(LRRule) i_rule, uint64_t enable);
static void LRSymbol_update_first_set(LRContext *context, INDEX(LRSymbol) i_sym, INDEX(LRRule) i_rule, uint64_t enable);
static void LRSymbol_set_env(const LRContext *context, INDEX(LRSymbol) i_sym, const LRItem *item, uint64_t enable);
static void LRSymbol_release(LRSymbol *symbol, const Allocator *allocator);
static void LRSymbol_build_first_set(LRContext *context, INDEX(LRSymbol) i_sym);

static bool LRItem_isEnd(const LRContext *context, const LRItem *item);
static void LRItem_build_closure(const LRContext *context, Array *item_array);
static INDEX(LRSymbol) LRItem_current(const LRContext *context, const LRItem *item);
static INDEX(LRSymbol) LRItem_ahead(const LRContext *context, const LRItem *item);
static void LRContext_enumerate_items(LRContext *context, INDEX(LRRule) i_rule, Array *item_array);

static LRAction *
LRState_set_reduce_action(LRContext *context, LRState *state, uint32_t key_info, const LRItem *item, uint64_t enable);
static LRAction *
LRState_set_stack_action(LRContext *context, LRState *state, uint32_t key_info, INDEX(LRSymbol) i_sym,
                                   INDEX(LRRule) i_rule, uint64_t enable);

inline int32_t LRAction_cmp(const LRAction *a, const LRAction *b) {
  return (a->acttype == b->acttype) ? (int32_t) (a->index - b->index) : (int32_t) (a->acttype - b->acttype);
}

inline uint64_t LREnvPair_hash(const LREnvPair *pair) {
  return ((((uint64_t) pair->state) << 32) | (pair->follow));
}

inline uint64_t LRActKeyPair_hash(const LRActKeyPair *pair) {
  return ((((uint64_t) pair->key_info) << 32) | (pair->symbol));
}

inline void LRState_release(LRState *state, const Allocator *) {
  if (state->actions) { Dict_destroy(state->actions); }
}

inline void LRSymbol_release(LRSymbol *symbol, const Allocator *) {
  if (symbol->rules) { releasePrimeArray(symbol->rules); }
  if (symbol->firsts) { Dict_destroy(symbol->firsts); }
  if (symbol->envs) { Dict_destroy(symbol->envs); }
}

inline void LRAction_release(LRAction *action, const Allocator *) {
  if (action->rules) { Set_destroy(action->rules); }
}

inline void LRRule_release(LRRule *rule, const Allocator *) {
  if (rule->items) { releasePrimeArray(rule->items); }
}

inline void LRRuleSet_release(Set *set, const Allocator *) { Set_destroy(set); }

inline void LRSymbol_build_first_set(LRContext *context, INDEX(LRSymbol) i_sym) {
  LRSymbol *symbol = Array_real_addr(context->sym_array, i_sym);
  Dict_reset(symbol->firsts);
  Array_foreach(LRRulePair, symbol->rules, {
    if (!__element->enabled) { continue; }
    LRSymbol_update_first_set(context, i_sym, __element->rule, 0);
  });
}

#define Record_set_rule(rule_set, i_rule, enable) do {   \
  if (enable) {                                          \
    Set_add(rule_set, i_rule);                           \
  } else {                                               \
    Set_remove(rule_set, i_rule);                        \
    Set_tidy(rule_set);                                  \
  }                                                      \
} while (false)

#define LRSymbol_add_using(dict, first, i_rule) do {            \
  Set *rules = Dict_get((dict), first);                         \
  if (!rules) {                                                 \
    rules = Set_new(sizeof(INDEX(LRRule)), XLR_OBJECT_RULE_KEY, \
                    nullptr, nullptr, context->allocator);      \
    Record_set_rule(rules, i_rule, enable);                     \
  }                                                             \
  Dict_set((dict), first, rules);                               \
} while (false)

void LRSymbol_add_rule(LRContext *context, INDEX(LRSymbol) i_sym, INDEX(LRRule) i_rule, uint64_t enable) {
  const Allocator *allocator = context->allocator;
  LRSymbol *sym = Array_real_addr(context->sym_array, i_sym);
  if (sym->symtype == XLR_SYMTYPE_TERMINAL) { sym->symtype = XLR_SYMTYPE_NON_TERMINAL; }
  if (!sym->rules) { sym->rules = Array_new(sizeof(LRRulePair), XLR_OBJECT_RULE_KEY, allocator); }
  else {
    Array_foreach(LRRulePair, sym->rules, {
      if (__element->rule == i_rule) {
        __element->enabled = enable;
        goto __changed_rule_enable;
      }
    });
  }
  LRRulePair pair = { .rule = i_rule, .enabled = enable };
  Array_append(sym->rules, &pair, 1);
  __changed_rule_enable:
  if (!sym->firsts) {
    sym->firsts = Dict_new(sizeof(INDEX(LRTerminal)), sizeof_set, nullptr, XLR_OBJECT_SYMBOL,
                           nullptr, (destruct_t *) LRRuleSet_release, allocator);
  }
  LRSymbol_update_first_set(context, i_sym, i_rule, enable);
}

inline void LRSymbol_update_first_set(LRContext *context, const INDEX(LRSymbol) i_sym,
                                      const INDEX(LRRule) i_rule, uint64_t enable) {
  const LRRule *rule = Array_real_addr(context->rule_array, i_rule);
  LRSymbol *sym = Array_real_addr(context->sym_array, i_sym);
  if (!rule->items || Array_length(rule->items) == 0) {
    Dict_foreach_key(LREnvPair, AVLTree, sym->envs, {
      INDEX(LRTerminal) follow = __key->follow;
      LRSymbol_add_using(sym->firsts, &follow, &i_rule);
    });
  } else {
    INDEX(LRSymbol) first = *(INDEX(LRSymbol) *) Array_first_real(rule->items);
    const LRSymbol *first_sym = Array_real_addr(context->sym_array, first);
    if (first_sym->symtype == XLR_SYMTYPE_NON_TERMINAL) {
      Dict_foreach_key(INDEX(LRSymbol), LRUsePair, first_sym->firsts, {
        LRSymbol_add_using(sym->firsts, __key, &i_rule);
      });
    } else {
      LRSymbol_add_using(sym->firsts, &first, &i_rule);
    }
  }
}

inline void LRSymbol_set_env(const LRContext *context, INDEX(LRSymbol) i_sym, const LRItem *item, uint64_t enable) {
  LRSymbol *sym = Array_real_addr(context->sym_array, i_sym);
  INDEX(LRSymbol) look = LRItem_ahead(context, item);
  const LRSymbol *look_sym = Array_real_addr(context->sym_array, look);
  if (look_sym->symtype == XLR_SYMTYPE_NON_TERMINAL) {
    Dict_foreach_key(INDEX(LRSymbol), LRUsePair, look_sym->firsts, {
      LREnvPair pair = {};
      pair.state = item->state;
      pair.follow = *__key;
      LRSymbol_add_using(sym->envs, &pair, &item->rule);
    });
  } else {
    LREnvPair pair = {.state = item->state, .follow = look};
    LRSymbol_add_using(sym->envs, &pair, &item->rule);
  }
}

inline bool LRItem_isEnd(const LRContext *context, const LRItem *item) {
  const LRRule * const rule = Array_real_addr(context->rule_array, item->rule);
  return (!rule->items) || (item->pos >= Array_length(rule->items));
}

inline INDEX(LRSymbol) LRItem_current(const LRContext *context, const LRItem *item) {
  const LRRule * const rule = Array_real_addr(context->rule_array, item->rule);
  return *(INDEX(LRSymbol) *) Array_real_addr(rule->items, item->pos);
}

inline INDEX(LRSymbol) LRItem_ahead(const LRContext *context, const LRItem *item) {
  const LRRule * const rule = Array_real_addr(context->rule_array, item->rule);
  if (item->pos + 1 >= Array_length(rule->items)) { return item->look; }
  return *(INDEX(LRSymbol) *) Array_real_addr(rule->items, item->pos + 1);
}

inline void LRItem_build_closure(const LRContext *context, Array *item_array) {
  uint32_t old_item_count = 0, new_item_count = Array_length(item_array);
  while (new_item_count > old_item_count) {
    for (uint32_t i = old_item_count; i < new_item_count; i++) {
      const LRItem * const item = Array_real_addr(item_array, i);
      if (LRItem_isEnd(context, item)) { continue; }
      const INDEX(LRSymbol) cur_sym_index = LRItem_current(context, item);
      const LRSymbol * const cur_sym = Array_real_addr(context->sym_array, cur_sym_index);
      if (cur_sym->symtype == XLR_SYMTYPE_NON_TERMINAL) {
        INDEX(LRSymbol) look = LRItem_ahead(context, item);
        LRItem sub_item = {.state = item->state, .look = look, .pos = 0};
        Array_foreach(LRRulePair, cur_sym->rules, {
          if (!__element->enabled) { continue; }
          sub_item.rule = __element->rule;
          Array_append(item_array, &sub_item, 1);
        });
      }
    }
    old_item_count = new_item_count;
    new_item_count = Array_length(item_array);
  }
}

inline LRAction *
LRState_set_reduce_action(LRContext *context, LRState *state, uint32_t key_info,
                          const LRItem *item, uint64_t enable) {
  const LRActKeyPair key = {.key_info = key_info, .symbol = item->look};
  LRAction *action = Dict_get(state->actions, &key);
  if (!action) {
    LRAction new_action = {.acttype = ACTTYPE_REDUCE, .rules = LRContext_new_ruleset(), .index = item->rule};
    Dict_set(state->actions, &key, &new_action);
    action = Dict_get(state->actions, &key);
  }
  if (action->acttype != ACTTYPE_REDUCE) {
    context->error = XLR_ERROR_REDUCE_STACK_CONFLICT;
    return nullptr;
  }
  if (action->index != item->rule) {
    context->error = XLR_ERROR_REDUCE_REDUCE_CONFLICT;
    return nullptr;
  }
  Record_set_rule(action->rules, &item->rule, enable);
  return action;
}

inline LRAction *
LRState_set_stack_action(LRContext *context, LRState *state, uint32_t key_info, INDEX(LRSymbol) i_sym,
                         INDEX(LRRule) i_rule, uint64_t enable) {
  const LRActKeyPair key = {.key_info = key_info, .symbol = i_sym};
  LRAction *action = Dict_get(state->actions, &key);
  if (!action) {
    const INDEX(LRState) index = Array_length(context->state_array);
    LRState new_state = { .type = XLR_STATE_TYPE_NORMAL, .count = 0, .index = index, .actions = LRState_new_actions() };
    Array_append(context->state_array, &new_state, 1);
    LRAction new_action = { .acttype = ACTTYPE_STACK, .rules = LRContext_new_ruleset(), .index = new_state.index };
    Dict_set(state->actions, &key, &new_action);
    action = Dict_get(state->actions, &key);
  } else if (action->acttype != ACTTYPE_STACK) {
    context->error = XLR_ERROR_STACK_REDUCE_CONFLICT;
    return nullptr;
  }
  Record_set_rule(action->rules, &i_rule, enable);
  return action;
}

LRContext *LRContext_new(const Allocator *allocator) {
  LRContext *context = allocator->calloc(1, sizeof(LRContext));
  context->allocator = allocator;
  context->name_array = Array_new(sizeof(char_t), XLR_OBJECT_NAME, allocator);
  context->ident_trie = Trie_new(sizeof(char_t), (key_t *) char2u64, allocator);
  context->ident_array = Array_new(sizeof(Identifier), XLR_OBJECT_IDENT, allocator);
  context->plain_array = Array_new(sizeof(uint64_t), XLR_OBJECT_PLAIN, allocator);
  context->plain_tree = AVLTree_new(allocator, nullptr);
  context->sym_array = Array_new(sizeof(LRSymbol), XLR_OBJECT_SYMBOL, allocator);
  context->sym_tree = AVLTree_new(allocator, nullptr);
  context->rule_array = Array_new(sizeof(LRRule), XLR_OBJECT_RULE, allocator);
  context->rule_tree = AVLTree_new(allocator, nullptr);
  context->type_array = Array_new(sizeof(LRType), XLR_OBJECT_TYPE, allocator);
  context->type_tree = AVLTree_new(allocator, nullptr);
  context->var_array = Array_new(sizeof(LRVariable), XLR_OBJECT_VAR, allocator);
  context->var_tree = AVLTree_new(allocator, nullptr);
  context->func_array = Array_new(sizeof(LRVariable), XLR_OBJECT_FUNC, allocator);
  context->func_tree = AVLTree_new(allocator, nullptr);
  context->text_array = Array_new(sizeof(char_t), XLR_OBJECT_TEXT, allocator);
  context->block_array = Array_new(sizeof(ActionBlock), XLR_OBJECT_BLOCK, allocator);
  context->state_array = Array_new(sizeof(LRState), XLR_OBJECT_STATE, allocator);
  context->expr_trie = Trie_new(sizeof(REFER(void)), (key_t *)refer2u64, allocator);
  context->in_pattern = false;

  LRContext_init(context);

  return context;
}

void LRContext_init(LRContext *context) {
  const Allocator *allocator = context->allocator;

  // Builtin Types
  for (uint32_t i = 0; i < BUILTIN_TYPE_COUNT; i ++) {
    const LRType *builtin_type = &BUILTIN_TYPES[i];
    const char_t *name = (char_t *) builtin_type->ident;
    REFER(Identifier) v_ident = Trie_get(context->ident_trie, name);
    if (!v_ident) {
      REFER(char_t) v_name = Array_last_virt(context->name_array) + 1;
      Array_append(context->name_array, name, strlen(name) + 1);
      Identifier ident = {.type = XLR_OBJECT_TYPE, .name = v_name};
      Array_append(context->ident_array, &ident, 1);
      v_ident = Array_last_virt(context->ident_array);
      Trie_set(context->ident_trie, name, v_ident);
    }
    LRType type = {
      .cat = builtin_type->cat, .size = builtin_type->size,
      .ident = v_ident, .refer = builtin_type->refer
    };
    Array_append(context->type_array, &type, 1);
    REFER(LRType) v_type = Array_last_virt(context->type_array);
    AVLTree_set(context->type_tree, (uint64_t) v_ident, v_type);
  }
  // Builtin Variables
  for (uint32_t i = 0; i < BUILTIN_VAR_COUNT; i ++) {
    const LRVariable *builtin_var = &BUILTIN_VARS[i];
    const char_t *name = (char_t *) builtin_var->ident;
    REFER(Identifier) v_ident = Trie_get(context->ident_trie, name);
    if (!v_ident) {
      REFER(char_t) v_name = Array_last_virt(context->name_array) + 1;
      Array_append(context->name_array, name, strlen(name) + 1);
      Identifier ident = {.type = XLR_OBJECT_VAR, .name = v_name};
      Array_append(context->ident_array, &ident, 1);
      v_ident = Array_last_virt(context->ident_array);
      Trie_set(context->ident_trie, name, v_ident);
    }
    REFER(char_t) type_name = Trie_get(context->ident_trie, BUILTIN_TYPES[(uint64_t) builtin_var->type].ident);
    REFER(LRType) type = AVLTree_get(context->type_tree, (uint64_t) type_name);
    LRVariable var = { .type = type, .ident = v_ident };
    Array_append(context->var_array, &var, 1);
    REFER(LRVariable) v_var = Array_last_virt(context->var_array);
    AVLTree_set(context->var_tree, (uint64_t) v_ident, v_var);
  }
  // Builtin Functions
  for (uint32_t i = 0; i < BUILTIN_FUNC_COUNT; i ++) {
    const LRFunction *builtin_func = &BUILTIN_FUNCS[i];
    const char_t *name = (char_t *) builtin_func->ident;
    REFER(Identifier) v_ident = Trie_get(context->ident_trie, name);
    if (!v_ident) {
      REFER(char_t) v_name = Array_last_virt(context->name_array) + 1;
      Array_append(context->name_array, name, strlen(name) + 1);
      Identifier ident = {.type = XLR_OBJECT_VAR, .name = v_name};
      Array_append(context->ident_array, &ident, 1);
      v_ident = Array_last_virt(context->ident_array);
      Trie_set(context->ident_trie, name, v_ident);
    }
    LRFunction func = { .ident = v_ident, .params = nullptr, .restype = nullptr, .attrs = nullptr };
    Array_append(context->func_array, &func, 1);
    REFER(LRVariable) v_func = Array_last_virt(context->func_array);
    AVLTree_set(context->func_tree, (uint64_t) v_ident, v_func);
  }

  // Builtin Pattern Symbols
  LRSymbol BUILTIN_SYMBOLS[] = {
    // SYM EMPTY
    [XLR_SYM_INDEX_EMPTY] = { .symtype = XLR_SYMTYPE_EMPTY, .index = XLR_SYM_INDEX_EMPTY, .rules = nullptr, .firsts = nullptr, .envs = nullptr },
    // SYM TERMINATOR
    [XLR_SYM_INDEX_TERMINATOR] = { .symtype = XLR_SYMTYPE_TERMINATOR, .index = XLR_SYM_INDEX_TERMINATOR, .rules = nullptr, .firsts = nullptr, .envs = nullptr },
    // SYM FINIAL
    [XLR_SYM_INDEX_FINIAL] = {
        .symtype = XLR_SYMTYPE_NON_TERMINAL, .index = XLR_SYM_INDEX_FINIAL,
        .rules = Array_new(sizeof(LRRulePair), XLR_OBJECT_RULE_KEY, allocator),
        .firsts = LRSymbol_new_firsts(), .envs = LRSymbol_new_envs()
    }
  };
  LREnvPair pair = { .state = XLR_STA_INDEX_INIT_STATE, .follow = XLR_SYM_INDEX_TERMINATOR };
  Set *rule_set = LRContext_new_ruleset();
  Dict_set(BUILTIN_SYMBOLS[XLR_SYM_INDEX_FINIAL].envs, &pair, rule_set);
  Array_append(context->sym_array, BUILTIN_SYMBOLS, 3);

  // Builtin States
  const LRState BAD_STATE = {};
  Array_append(context->state_array, &BAD_STATE, 1);
  const LRState INIT_STATE = { .type = XLR_STATE_TYPE_NORMAL, .index = XLR_STA_INDEX_INIT_STATE,
                                .count = 1, .actions = LRState_new_actions() };
  Array_append(context->state_array, &INIT_STATE, 1);
  context->state = XLR_STA_INDEX_INIT_STATE;

  // Builtin Rules
  const LRRule BAD_RULE = { .items = nullptr, .target = XLR_SYM_INDEX_EMPTY, .enabled = false };
  Array_append(context->rule_array, &BAD_RULE, 1);
}

void LRContext_destroy(LRContext *context) {
  Array_reset(context->state_array, (destruct_t *) LRState_release);
  Array_reset(context->sym_array, (destruct_t *) LRSymbol_release);
  Array_reset(context->rule_array, (destruct_t *) LRRule_release);
  AVLTree_destroy(context->sym_tree, nullptr);
  Array_destroy(context->state_array);
  Array_destroy(context->rule_array);
  Array_destroy(context->sym_array);
  Trie_destroy(context->ident_trie);
}

inline void LRContext_enumerate_items(LRContext *context, const INDEX(LRRule) i_rule, Array *item_array) {
  const LRRule * const rule = Array_real_addr(context->rule_array, i_rule);
  const LRSymbol *target = Array_real_addr(context->sym_array, rule->target);
  LRItem item = {.rule = i_rule, .pos = 0};
  Dict_foreach_key(LREnvPair, AVLTree, target->envs, {
    item.state = __key->state;
    item.look = __keys->follow;
    Array_append(item_array, &item, 1);
  });
  LRItem_build_closure(context, item_array);
}

inline uint32_t LRContext_set_rule(LRContext *context, INDEX(LRRule) i_rule, bool enable_flag) {
  enable_flag = enable_flag ? 1 : 0;
  const uint32_t action_flag = enable_flag ? 1 : -1;
  if (!i_rule) { context->error = XLR_ERROR_BAD_RULE; return context->error; }
  LRRule * const rule = Array_real_addr(context->rule_array, i_rule);
  if (!rule) { context->error = XLR_ERROR_BAD_RULE; return context->error; }
  if (rule->enabled == enable_flag) {
    context->error = XLR_ERROR_DUPLICATED_SET_RULE;
    return context->error;
  }
  Array *item_array = Array_new(sizeof(LRItem), XLR_OBJECT_LR_ITEM, context->allocator);
  LRContext_enumerate_items(context, i_rule, item_array);
  while (Array_length(item_array) > 0) {
    Array *next_item_array = Array_new(sizeof(LRItem), XLR_OBJECT_LR_ITEM, context->allocator);
    const uint32_t n_items = Array_length(item_array);
    const LRItem * const items = Array_first_real(item_array);
    for (uint32_t i = 0; i < n_items; i++) {
      const LRItem * const item = &items[i];
      LRState * const state = Array_real_addr(context->state_array, item->state);
      if (LRItem_isEnd(context, item)) {
        LRAction *action = LRState_set_reduce_action(context, state, 0, item, action_flag);
        if (!action) { return context->error; }
        continue;
      }
      const INDEX(LRSymbol) i_cur_sym = LRItem_current(context, item);
      LRAction *action = LRState_set_stack_action(context, state, 0, i_cur_sym, i_rule, action_flag);
      if (!action) { return context->error; }
      const LRItem next_item = {.state = action->index, .rule = item->rule, .look = item->look, .pos = item->pos + 1};
      Array_append(next_item_array, &next_item, 1);
      const LRItem use_item = {.state = item->state, .rule = i_rule, .look = item->look, .pos = 0};
      LRSymbol_set_env(context, i_cur_sym, &use_item, enable_flag);
    }
    releasePrimeArray(item_array);
    LRItem_build_closure(context, next_item_array);
    item_array = next_item_array;
  }
  releasePrimeArray(item_array);
  LRSymbol_add_rule(context, rule->target, i_rule, enable_flag);
  rule->enabled = enable_flag;
  context->error = XLR_SUCCESS;
  return context->error;
}

uint32_t LRContext_enable_rule(LRContext *context, INDEX(LRRule) i_rule) {
  return (LRContext_set_rule(context, i_rule, true) == XLR_SUCCESS) ? i_rule : 0;
}

uint32_t LRContext_disable_rule(LRContext *context, INDEX(LRRule) i_rule) {
  return (LRContext_set_rule(context, i_rule, false) == XLR_SUCCESS) ? i_rule : 0;
}

LRValue *LRContext_last_enum_val(LRContext *, ErrInfo *) {
  return nullptr;
}

INDEX(LRType) LRContext_typeof(LRContext *, ErrInfo *, Expr *) {
  return 0;
}

bool LRContext_subtype(LRContext *, ErrInfo *, Expr *, LRType) {
  return false;
}

LRValue * LRContext_eval(LRContext *, ErrInfo *, Expr *) {
  return nullptr;
}

LRVariable *LRContext_get_variable(LRContext *context, REFER(Identifier) v_ident) {
  const REFER(LRVariable) v_var = nullptr;
  const ActionBlock *curr_block = context->curr_block;
  while (curr_block) {
    v_var = AVLTree_get(curr_block->var_tree, (uint64_t) v_ident);
    curr_block = Array_virt2real(context->block_array, curr_block->parent);
    if (v_var) { break; }
  }
  if (!curr_block) { return nullptr; }
  return Array_virt2real(curr_block->var_array, v_var);
}

inline REFER(char_t) LRContent_new_text_content(LRContext *context, const char_t *text, uint32_t size) {
  Array_append(context->text_array, "\0", 1);
  REFER(char_t) v_content = Array_last_virt(context->text_array) + 1;
  Array_append(context->text_array, text, size);
  return v_content;
}

inline REFER(LRSymbol) LRContext_plain_to_sym(LRContext *context, uint64_t plain) {
  REFER(uint64_t) v_plain = AVLTree_get(context->plain_tree, plain);
  if (!v_plain) {
    Array_append(context->plain_array, &plain, 1);
    v_plain = Array_last_virt(context->plain_array);
    AVLTree_set(context->plain_tree, (uint64_t) plain, v_plain);
  }
  REFER(LRSymbol) v_sym = AVLTree_get(context->sym_tree, (uint64_t) v_plain);
  if (!v_sym) {
    LRSymbol sym = LRSymbol_new();
    Array_append(context->sym_array, &sym, 1);
    v_sym = Array_last_virt(context->sym_array);
    AVLTree_set(context->sym_tree, (uint64_t) v_plain, v_sym);
  }
  return v_sym;
}


#define IN_RULE(a) XLR_state_Type_AttrList_RULE_IDENTIFIER_##a
#define IN_ACTION_BLOCK(a) XLR_state_AttrList_ATTRIBUTE_IDENTIFIER_LEFT_BRACKET_##a

void LRContext_state_action(LRContext *context, uint32_t state, Token *, const Allocator *allocator) {
  switch (state) {
      case IN_RULE(LEFT_PARENTHESIS): {
      context->in_pattern = true; return;
    }
    case IN_RULE(LEFT_PARENTHESIS_Pattern): {
      context->in_pattern = false; return;
    }
    case XLR_state_AttrList_ATTRIBUTE_IDENTIFIER_LEFT_BRACKET:
    case IN_RULE(LEFT_PARENTHESIS_Pattern_RIGHT_PARENTHESIS_LEFT_BRACKET): {
      const ActionBlock *block = ActionBlock_new(allocator);
      Array_append(context->block_array, block, 1);
      context->curr_block = Array_last_real(context->block_array);
      context->kw_as_ident = XLR_KW_AS_IDENT;
      return;
    }
    case IN_ACTION_BLOCK(LEFT_BRACKET):
    case IN_ACTION_BLOCK(IF_IfCondition_LEFT_BRACKET): {
      ActionBlock *block = ActionBlock_new(allocator);
      block->parent = Array_real2virt(context->block_array, context->curr_block);
      Array_append(context->block_array, block, 1);
      context->curr_block = Array_last_real(context->block_array);
      return;
    }
    case IN_ACTION_BLOCK(ActionBlock):
    case IN_ACTION_BLOCK(IF_IfCondition_ActionBlock): {
      context->curr_block = Array_virt2real(context->block_array, context->curr_block->parent);
      return;
    }
    case XLR_state_AttrList_ATTRIBUTE_IDENTIFIER_ActionBlock:
    case IN_RULE(LEFT_PARENTHESIS_Pattern_RIGHT_PARENTHESIS_ActionBlock): {
      context->kw_as_ident = XLR_KW_AS_IDENT;
      context->curr_block = nullptr;
      return;
    }
    default: {
#ifdef XLR_ENABLE_DEBUG
        printf("nothing context to do at this state: %d.\n", state);
#endif
      }
  }
}
