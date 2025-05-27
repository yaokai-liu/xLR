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
 * Filename: LRContext.c
 * Creator: Yaokai Liu
 * Create Date: 2025-05-15
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "LRContext.h"

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

void LRSymbol_add_rule(LRContext *context, INDEX(LRSymbol) i_sym, INDEX(LRRule) i_rule, uint64_t enable);
void LRSymbol_update_first_set(LRContext *context, INDEX(LRSymbol) i_sym, INDEX(LRRule) i_rule, uint64_t enable);
void LRSymbol_set_env(const LRContext *context, INDEX(LRSymbol) i_sym, const LRItem *item, uint64_t enable);
void LRSymbol_release(LRSymbol *symbol, const Allocator *allocator);
#define LRSymbol_new_envs() Dict_new(sizeof(LREnvPair), sizeof_set, \
  (unikey_t *) LREnvPair_hash, XLR_TYPE_SYMBOL, context->allocator)


bool LRItem_isEnd(const LRContext *context, const LRItem *item);
void LRItem_build_closure(const LRContext *context, Array *item_array);
INDEX(LRSymbol) LRItem_current(const LRContext *context, const LRItem *item);
INDEX(LRSymbol) LRItem_ahead(const LRContext *context, const LRItem *item);
void LRContext_enumerate_items(LRContext *context, INDEX(LRRule) i_rule, Array *item_array);

LRAction *
LRState_set_reduce_action(LRContext *context, LRState *state, uint32_t key_info, const LRItem *item, uint64_t enable);
LRAction *LRState_set_stack_action(LRContext *context, LRState *state, uint32_t key_info, INDEX(LRSymbol) i_sym,
                                   INDEX(LRRule) i_rule, uint64_t enable);
void LRState_release(LRState *state, const Allocator *allocator);

void LRAction_release(LRAction *action, const Allocator *allocator);
void LRRule_release(LRRule *rule, const Allocator *allocator);

int32_t LRAction_cmp(const LRAction *a, const LRAction *b) {
  return (a->acttype == b->acttype) ? (int32_t) (a->index - b->index) : (int32_t) (a->acttype - b->acttype);
}

uint64_t LREnvPair_hash(const LREnvPair *pair) {
  return ((((uint64_t) pair->state) << 32) | (pair->follow));
}

uint64_t LRActKeyPair_hash(const LRActKeyPair *pair) {
  return ((((uint64_t) pair->key_info) << 32) | (pair->symbol));
}

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
    rules = Set_new(sizeof(INDEX(LRRule)), XLR_TYPE_RULE_KEY,   \
                    nullptr, context->allocator);               \
    Record_set_rule(rules, i_rule, enable);                     \
  }                                                             \
  Dict_set((dict), first, rules);                               \
} while (false)

void LRSymbol_add_rule(LRContext *context, INDEX(LRSymbol) i_sym, INDEX(LRRule) i_rule, uint64_t enable) {
  const Allocator *allocator = context->allocator;
  LRSymbol *sym = Array_real_addr(context->sym_array, i_sym);
  if (sym->symtype == SYMTYPE_TERMINAL) { sym->symtype = SYMTYPE_NON_TERMINAL; }
  if (!sym->rules) { sym->rules = Array_new(sizeof(LRRulePair), XLR_TYPE_RULE_KEY, allocator); }
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
    sym->firsts = Dict_new(sizeof(INDEX(LRTerminal)), sizeof_set,
                           nullptr, XLR_TYPE_SYMBOL, allocator);
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
    if (first_sym->symtype == SYMTYPE_TERMINAL) {
      LRSymbol_add_using(sym->firsts, &first, &i_rule);
    } else {
      Dict_foreach_key(INDEX(LRSymbol), LRUsePair, first_sym->firsts, {
        LRSymbol_add_using(sym->firsts, __key, &i_rule);
      });
    }
  }
}

inline void LRSymbol_set_env(const LRContext *context, INDEX(LRSymbol) i_sym, const LRItem *item, uint64_t enable) {
  LRSymbol *sym = Array_real_addr(context->sym_array, i_sym);
  INDEX(LRSymbol) look = LRItem_ahead(context, item);
  const LRSymbol *look_sym = Array_real_addr(context->sym_array, look);
  if (look_sym->symtype == SYMTYPE_TERMINAL) {
    LREnvPair pair = {.state = item->state, .follow = look};
    LRSymbol_add_using(sym->envs, &pair, &item->rule);
  } else {
    Dict_foreach_key(INDEX(LRSymbol), LRUsePair, look_sym->firsts, {
      LREnvPair pair = {};
      pair.state = item->state;
      pair.follow = *__key;
      LRSymbol_add_using(sym->envs, &pair, &item->rule);
    });
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
      const INDEX(LRSymbol) cur_sym_index = LRItem_current(context, item);
      const LRSymbol * const cur_sym = Array_real_addr(context->sym_array, cur_sym_index);
      if (cur_sym->symtype == SYMTYPE_NON_TERMINAL) {
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
    action = context->allocator->calloc(1, sizeof(LRAction));
    Set *rules = Set_new(sizeof(INDEX(LRRule)), XLR_TYPE_RULE_KEY, nullptr, context->allocator);
    *action = (LRAction) {.acttype = ACTTYPE_REDUCE, .rules = rules, .index = item->rule};
    Dict_set(state->actions, &key, action);
  }
  if (action->acttype != ACTTYPE_REDUCE) {
    context->error = XLR_ERROR_RS_CONFLICT;
    return nullptr;
  }
  if (action->index != item->rule) {
    context->error = XLR_ERROR_RR_CONFLICT;
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
    Dict *actions = Dict_new(sizeof(LRActKeyPair), sizeof(REFER(LRAction)),
                             (unikey_t *) LRActKeyPair_hash, XLR_TYPE_ACT_KEY, context->allocator);
    Set *rules = Set_new(sizeof(INDEX(LRRule)), XLR_TYPE_RULE_KEY, nullptr, context->allocator);
    LRState new_state = {.index = index, .actions = actions};
    action = context->allocator->calloc(1, sizeof(LRAction));
    *action = (LRAction) { .acttype = ACTTYPE_STACK, .rules = rules, .index = new_state.index };
    Dict_set(state->actions, &key, action);
  } else if (action->acttype != ACTTYPE_STACK) {
    context->error = XLR_ERROR_SR_CONFLICT;
    return nullptr;
  }
  Record_set_rule(action->rules, &i_rule, enable);
  return action;
}

constexpr uint32_t SYM_INDEX_EMPTY = 0;
constexpr uint32_t SYM_INDEX_TERMINATOR = 1;
constexpr uint32_t SYM_INDEX_EXTEND = 2;

const LRSymbol DEFAULT_SYMBOLS[] = {
    { .symtype = SYMTYPE_EMPTY, .index = SYM_INDEX_EMPTY, .rules = nullptr, .firsts = nullptr, .envs = nullptr },
    { .symtype = SYMTYPE_TERMINATOR, .index = SYM_INDEX_TERMINATOR, .rules = nullptr, .firsts = nullptr, .envs = nullptr }
};

LRContext *LRContext_new(const Allocator *allocator) {
  LRContext *context = allocator->calloc(1, sizeof(LRContext));
  context->allocator = allocator;
  context->state_array = Array_new(sizeof(LRState), XLR_TYPE_STATE, allocator);
  context->sym_array = Array_new(sizeof(LRSymbol), XLR_TYPE_SYMBOL, allocator);
  context->ident_array = Array_new(sizeof(uint8_t), XLR_TYPE_CHAR, allocator);
  context->rule_array = Array_new(sizeof(LRRule), XLR_TYPE_RULE, allocator);
  context->sym_tree = AVLTree_new(allocator, nullptr);

  Array_append(context->ident_array, "", 1);
  const LRSymbol extend_symbol = {
    .symtype = SYMTYPE_NON_TERMINAL, .index = SYM_INDEX_EXTEND,
    .rules = Array_new(sizeof(LRRulePair), XLR_TYPE_RULE_KEY, allocator),
    .firsts = Dict_new(sizeof(INDEX(LRTerminal)), sizeof_set,
                       nullptr, XLR_TYPE_SYMBOL, allocator),
    .envs = LRSymbol_new_envs()
  };
  Array_append(context->sym_array, DEFAULT_SYMBOLS, 2);
  Array_append(context->sym_array, &extend_symbol, 1);
  return context;
}

void LRContext_destroy(LRContext *context) {
  Array_reset(context->state_array, (destruct_t *) LRState_release);
  Array_reset(context->sym_array, (destruct_t *) LRSymbol_release);
  Array_reset(context->rule_array, (destruct_t *) LRRule_release);
  AVLTree_destroy(context->sym_tree, nullptr);
  Array_destroy(context->state_array);
  Array_destroy(context->rule_array);
  Array_destroy(context->sym_array);
  releasePrimeArray(context->ident_array);
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

inline uint32_t LRContext_set_rule(LRContext *context, const INDEX(LRRule) i_rule, uint64_t enable_flag) {
  enable_flag = enable_flag ? 1 : 0;
  const uint32_t action_flag = enable_flag ? 1 : -1;
  LRRule * const rule = Array_real_addr(context->rule_array, i_rule);
  if (rule->enabled == enable_flag) {
    context->error = XLR_ERROR_DUPLICATED_SET_RULE;
    return context->error;
  }
  Array *item_array = Array_new(sizeof(LRItem), XLR_TYPE_ITEM, context->allocator);
  LRContext_enumerate_items(context, i_rule, item_array);
  while (Array_length(item_array) > 0) {
    Array *next_item_array = Array_new(sizeof(LRItem), XLR_TYPE_ITEM, context->allocator);
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
