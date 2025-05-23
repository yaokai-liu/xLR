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
#include "avl-tree.h"
#include "dict.h"

typedef enum SYMBOL_TYPE_ENUM : uint8_t {
  SYMTYPE_EMPTY,
  SYMTYPE_BAD_TOKEN,
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
   * if symtype:
   * is EMPTY:          EMPTY;
   * is TERMINATOR:     TERMINATOR;
   * otherwise:         offset in sym_array.
   */
  uint32_t index;
  /*
   * if symtype
   * is SYMTYPE_NON_TERMINAL:   Array<LRRulePair>;
   * otherwise:         nullptr.
   */
  Array *pop_rules;
  /*
   * if symtype
   * is SYMTYPE_NON_TERMINAL:   Dict<LRTerminal, Set>;
   * otherwise:         nullptr.
   */
  Dict *first_set;
  /*
   * if symtype
   * is SYMTYPE_TERMINAL:       Dict<LREnvPair, Set>;
   * is SYMTYPE_NON_TERMINAL:   Dict<LREnvPair, Set>;
   * otherwise:         nullptr.
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

void LRSymbol_update_first_set(LRContext *context, INDEX(LRSymbol) i_sym, INDEX(LRRule) i_rule, uint64_t enable);
void LRSymbol_set_env(const LRContext *context, INDEX(LRSymbol) i_sym, const LRItem *item, uint64_t enable);

bool LRItem_isEnd(const LRContext *context, const LRItem *item);
void LRItem_build_closure(const LRContext *context, Array *item_array);
INDEX(LRSymbol) LRItem_current(const LRContext *context, const LRItem *item);
INDEX(LRSymbol) LRItem_ahead(const LRContext *context, const LRItem *item);
void LRContext_enumerate_items(LRContext *context, INDEX(LRRule) i_rule, Array *item_array);

LRAction *
LRState_set_reduce_action(LRContext *context, LRState *state, uint32_t key_info, const LRItem *item, uint64_t enable);
LRAction *LRState_set_stack_action(LRContext *context, LRState *state, uint32_t key_info, INDEX(LRSymbol) i_sym,
                                   INDEX(LRRule) i_rule, uint64_t enable);

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
  Dict_reset(symbol->first_set);
  Array_foreach(LRRulePair, symbol->pop_rules, {
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

inline void LRSymbol_update_first_set(LRContext *context, const INDEX(LRSymbol) i_sym,
                                      const INDEX(LRRule) i_rule, uint64_t enable) {
const LRRule *rule = Array_real_addr(context->rule_array, i_rule);
  if (rule->target != i_sym) { context->error = XLR_ERROR_TARGET_MISMATCH; return; }
  LRSymbol *sym = Array_real_addr(context->sym_array, i_sym);
  if (!rule->items || Array_length(rule->items) == 0) {
    Dict_foreach_key(LREnvPair, AVLTree, sym->envs, {
      INDEX(LRTerminal) follow = __key->follow;
      LRSymbol_add_using(sym->first_set, &follow, &i_rule);
    });
  } else {
    INDEX(LRSymbol) first = *(INDEX(LRSymbol) *) Array_first_real(rule->items);
    const LRSymbol *first_sym = Array_real_addr(context->sym_array, first);
    if (first_sym->symtype == SYMTYPE_TERMINAL) {
      LRSymbol_add_using(sym->first_set, &first, &i_rule);
    } else {
      Dict_foreach_key(INDEX(LRSymbol), LRUsePair, first_sym->first_set, {
        LRSymbol_add_using(sym->first_set, __key, &i_rule);
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
    Dict_foreach_key(INDEX(LRSymbol), LRUsePair, look_sym->first_set, {
      LREnvPair pair = {};
      pair.state = item->state;
      pair.follow = *__key;
      LRSymbol_add_using(sym->envs, &pair, &item->rule);
    });
  }
}

inline bool LRItem_isEnd(const LRContext *context, const LRItem *item) {
  const LRRule * const rule = Array_real_addr(context->rule_array, item->rule);
  return item->pos >= Array_length(rule->items);
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
        Array_foreach(LRRulePair, cur_sym->pop_rules, {
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
LRState_set_reduce_action(LRContext *context, LRState *state, uint32_t key_info, const LRItem *item, uint64_t enable) {
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

inline void LRContext_enumerate_items(LRContext *context, const INDEX(LRRule) i_rule, Array *item_array) {
  const LRRule * const rule = Array_real_addr(context->rule_array, i_rule);
  LRSymbol *target = Array_real_addr(context->sym_array, rule->target);
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
      LRItem next_item = {.state = action->index, .rule = item->rule, .look = item->look, .pos = item->pos + 1};
      Array_append(next_item_array, &next_item, 1);
      LRItem use_item = {.state = item->state, .rule = i_rule, .look = item->look, .pos = 0};
      LRSymbol_set_env(context, i_cur_sym, &use_item, enable_flag);
    }
    releasePrimeArray(item_array);
    LRItem_build_closure(context, next_item_array);
    item_array = next_item_array;
  }
  releasePrimeArray(item_array);
  LRSymbol_update_first_set(context, rule->target, i_rule, 0);
  rule->enabled = enable_flag;
  context->error = SUCCESS;
  return context->error;
}
