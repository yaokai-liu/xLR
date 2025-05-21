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
  EMPTY,
  TERMINATOR,
  TERMINAL,
  NON_TERMINAL,
} symtype;
typedef enum ACT_ENUM : uint8_t {
  STACK = 1,
  REDUCE = 2
} acttype;

typedef struct RulePair {
  bool        enabled;
  INDEX(Rule) rule;
} RulePair;  // Pair<bool, INDEX(Rule)>

typedef struct EnvPair {
  INDEX(State) state;
  INDEX(Terminal) follow;
} EnvPair;  // Pair<INDEX(State), INDEX(Terminal)>

typedef struct UsePair {
  uint32_t       count;
  INDEX(AVLTree) tree;
} UsePair;  // Pair<count, INDEX(AVLTree<INDEX(rule), bool>)>

typedef struct Action {
  acttype acttype;
  UsePair using;
  /*
   * if acttype:
   * is STACK:      next state index;
   * is REDUCE:     reduce rule index;
   */
  uint32_t index;
} Action;

typedef struct State {
  uint32_t index;
  /*
   * AVLTree<INDEX(Symbol), REFER(Action)>
   */
  AVLTree *actions;
} State;

typedef struct Symbol {
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
   * is NON_TERMINAL:   Array<RulePair>;
   * otherwise:         nullptr.
   */
  Array *pop_rules;
  /*
   * if symtype
   * is NON_TERMINAL:   Dict<Terminal, UsePair>;
   * otherwise:         nullptr.
   */
  Dict *first_set;
  /*
   * if symtype
   * is TERMINAL:       Dict<EnvPair, UsePair>;
   * is NON_TERMINAL:   Dict<EnvPair, UsePair>;
   * otherwise:         nullptr.
   */
  Dict *envs;
} Symbol, Terminal, NonTerminal;

typedef struct Rule {
  Array *items;  // Array<INDEX(Symbol)>
  INDEX(Symbol) target;
  bool  enabled;
} Rule;

typedef struct LrItem {
  INDEX(State) state;
  INDEX(Symbol) look;
  INDEX(Rule) rule;
  uint32_t pos;
} LrItem;

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

#define Action_toUint64(act)   ((((uint64_t) (act).acttype) << 32) | ((act).index))
#define Action_fromUint64(u64) {.acttype = (u64) >> 32, .index = ((u64) | 0xFFFFFFFF)}

void Symbol_update_first_set(LRContext *context, INDEX(Symbol) i_sym, const Rule *rule, uint64_t enable);
void Symbol_set_env(const LRContext *context, INDEX(Symbol) i_sym, const LrItem *item, uint64_t enable);

bool LrItem_isEnd(const LRContext *context, const LrItem *item);
void LrItem_build_closure(const LRContext *context, Array *item_array);
INDEX(Symbol) LrItem_current(const LRContext *context, const LrItem *item);
INDEX(Symbol) LrItem_ahead(const LRContext *context, const LrItem *item);
void LRContext_enumerate_items(LRContext *context, INDEX(Rule) i_rule, Array *item_array);

Action *State_set_reduce_action(LRContext *context, State *state, const LrItem *item);
Action *State_set_stack_action(LRContext *context, State *state, INDEX(Symbol) i_sym);

uint32_t LRContext_add_rule(LRContext *context, INDEX(Rule) i_rule);


int32_t Action_cmp(const Action *a, const Action *b) {
  return (a->acttype == b->acttype) ? (int32_t) (a->index - b->index) : (int32_t) (a->acttype - b->acttype);
}

uint64_t EnvPair_hash(const EnvPair *pair) {
  return ((((uint64_t) pair->state) << 32) | (pair->follow));
}

inline bool Rule_length(const LRContext *context, INDEX(Rule) i_rule) {
  const Rule *p_rule = Array_real_addr(context->rule_array, i_rule);
  return Array_length(p_rule->items);
}

inline const Symbol *Rule_get(const LRContext *context, const Rule *rule, uint32_t index) {
  REFER(Symbol) symbol = Array_real_addr(rule->items, index);
  return Array_virt2real(context->sym_array, symbol);
}

inline INDEX(Symbol) Rule_target(const LRContext *context, INDEX(Rule) i_rule) {
  const Rule *p_rule = Array_real_addr(context->rule_array, i_rule);
  return p_rule->target;
}

inline void Symbol_build_first_set(LRContext *context, INDEX(Symbol) i_sym) {
  Symbol *symbol = Array_real_addr(context->sym_array, i_sym);
  Dict_reset(symbol->first_set);
  Array_foreach(RulePair, symbol->pop_rules, {
    if (!__element->enabled) { continue; }
    const Rule *rule = Array_real_addr(context->rule_array, __element->rule);
    Symbol_update_first_set(context, i_sym, rule, 0);
  });
}

inline void Symbol_update_first_set(LRContext *context, INDEX(Symbol) i_sym, const Rule *rule, uint64_t enable) {
  if (rule->target != i_sym) { context->error = XLR_ERROR_TARGET_MISMATCH; return; }
  Symbol *sym = Array_real_addr(context->sym_array, i_sym);
  if (!rule->items || Array_length(rule->items) == 0) {
    Dict_foreach_key(EnvPair, AVLTree, sym->envs, {
      INDEX(Terminal) follow = __key->follow;
      UsePair *using = Dict_get(sym->first_set, &follow);
      if (using) {context->error = XLR_ERROR_MULTI_EMPTY_RULE; return; }
      UsePair new_using = { };
      new_using.count = 1;
      new_using.tree = -1; // TODO: if debug enable, create a new AVLTree here.
      Dict_set(sym->first_set, &follow, &new_using);
    });
  } else {
    INDEX(Symbol) first = *(INDEX(Symbol) *) Array_first_real(rule->items);
    const Symbol *first_sym = Array_real_addr(context->sym_array, first);
    if (first_sym->symtype == TERMINAL) {
      UsePair new_using = { .count = 0, .tree = -1 }; // TODO: if debug enable, create a new AVLTree here.
      Dict_set(sym->first_set, &first, &new_using);
    } else {
      Dict_foreach_key(INDEX(Symbol), UsePair, first_sym->first_set, {
        UsePair *using = Dict_get(sym->first_set, __key);
        if (!using) {
          UsePair new_using = { };
          new_using.count = 0;
          new_using.tree = -1; // TODO: if debug enable, create a new AVLTree here.
          Dict_set(sym->first_set, __key, &new_using);
          using = Dict_get(sym->envs, __key);
        }
        using->count += enable ? 1 : -1;
        // TODO: if debug enable, set rule's flag in `using->tree`.
      });
    }
  }
}

inline void Symbol_set_env(const LRContext *context, INDEX(Symbol) i_sym, const LrItem *item, uint64_t enable) {
  Symbol *sym = Array_real_addr(context->sym_array, i_sym);
  INDEX(Symbol) look = LrItem_ahead(context, item);
  const Symbol *look_sym = Array_real_addr(context->sym_array, look);
  if (look_sym->symtype == TERMINAL) {
    EnvPair pair = { .state = item->state, .follow = look };
    UsePair *using = Dict_get(sym->envs, &pair);
    if (!using) {
      UsePair new_using = { .count = 0, .tree = -1 }; // TODO: if debug enable, create a new AVLTree here.
      Dict_set(sym->envs, &pair, &new_using);
      using = Dict_get(sym->envs, &pair);
    }
    using->count += enable ? 1 : -1;
    // TODO: if debug enable, set rule's flag in `using->tree`.
  } else {
    Dict_foreach_key(INDEX(Symbol), UsePair, look_sym->first_set, {
      EnvPair pair = { };
      pair.state = item->state;
      pair.follow = *__key;
      UsePair *using = Dict_get(sym->envs, &pair);
      if (!using) {
        UsePair new_using = { };
        new_using.count = 0;
        new_using.tree = -1; // TODO: if debug enable, create a new AVLTree here.
        Dict_set(sym->envs, &pair, &new_using);
        using = Dict_get(sym->envs, &pair);
      }
      using->count += enable ? 1 : -1;
      // TODO: if debug enable, set rule's flag in `using->tree`.
    });
  }
}

inline bool LrItem_isEnd(const LRContext *context, const LrItem *item) {
  const Rule * const rule = Array_real_addr(context->rule_array, item->rule);
  return item->pos >= Array_length(rule->items);
}

inline INDEX(Symbol) LrItem_current(const LRContext *context, const LrItem *item) {
  const Rule * const rule = Array_real_addr(context->rule_array, item->rule);
  return *(INDEX(Symbol) *) Array_real_addr(rule->items, item->pos);
}

inline INDEX(Symbol) LrItem_ahead(const LRContext *context, const LrItem *item) {
  const Rule * const rule = Array_real_addr(context->rule_array, item->rule);
  if (item->pos + 1 >= Array_length(rule->items)) { return item->look; }
  return *(INDEX(Symbol) *) Array_real_addr(rule->items, item->pos + 1);
}

inline void LrItem_build_closure(const LRContext *context, Array *item_array) {
  uint32_t old_item_count = 0, new_item_count = Array_length(item_array);
  while (new_item_count > old_item_count) {
    for (uint32_t i = old_item_count; i < new_item_count; i ++) {
      const LrItem *const item = Array_real_addr(item_array, i);
      const INDEX(Symbol) cur_sym_index = LrItem_current(context, item);
      const Symbol *const cur_sym = Array_real_addr(context->sym_array, cur_sym_index);
      if (cur_sym->symtype == NON_TERMINAL) {
        INDEX(Symbol) look = LrItem_ahead(context, item);
        LrItem sub_item = { .state = item->state, .look = look, .pos = 0 };
        Array_foreach(RulePair, cur_sym->pop_rules, {
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

inline Action *State_set_reduce_action(LRContext *context, State *state, const LrItem *item) {
  Action *action = AVLTree_get(state->actions, item->look);
  if (!action) {
    action = context->allocator->calloc(1, sizeof(Action));
    AVLTree_set(state->actions, item->look, action);
    *action = (Action) {.acttype = REDUCE, .using = { .count = 0, .tree = -1 }, .index = item->rule};
  }
  if (action->acttype != REDUCE) {
    context->error = XLR_ERROR_RS_CONFLICT;
    return nullptr;
  }
  if (action->index != item->rule) {
    context->error = XLR_ERROR_RR_CONFLICT;
    return nullptr;
  }
  return action;
}

inline Action *State_set_stack_action(LRContext *context, State *state, INDEX(Symbol) i_sym) {
  Action *action = AVLTree_get(state->actions, i_sym);
  if (!action) {
    const INDEX(State) index = Array_length(context->state_array);
    AVLTree *actions = AVLTree_new(context->allocator, nullptr);
    State new_state = {.index = index, .actions = actions};
    action = context->allocator->calloc(1, sizeof(Action));
    AVLTree_set(state->actions, i_sym, action);
    *action = (Action) { .acttype = STACK, .using = { .count = 0, .tree = -1 }, .index = new_state.index };
  } else if (action->acttype != STACK) {
    context->error = XLR_ERROR_SR_CONFLICT;
    return nullptr;
  }
  return action;
}

inline void LRContext_enumerate_items(LRContext *context, const INDEX(Rule) i_rule, Array *item_array) {
  const Rule * const rule = Array_real_addr(context->rule_array, i_rule);
  Symbol *target = Array_real_addr(context->sym_array, rule->target);
  LrItem item = { .rule = i_rule, .pos = 0 };
  Dict_foreach_key(EnvPair, AVLTree, target->envs, {
    item.state = __key->state;
    item.look = __keys->follow;
    Array_append(item_array, &item, 1);
  });
  LrItem_build_closure(context, item_array);
}

inline uint32_t LRContext_set_rule(LRContext *context, const INDEX(Rule) i_rule, uint64_t enable_flag) {
  enable_flag = enable_flag ? 1 : 0;
  const uint32_t action_flag = enable_flag ? 1 : -1;
  Rule *const rule = Array_real_addr(context->rule_array, i_rule);
  if (rule->enabled == enable_flag) {
    context->error = XLR_ERROR_DUPLICATED_SET_RULE;
    return context->error;
  }
  Array *item_array = Array_new(sizeof(LrItem), XLR_TYPE_ITEM, context->allocator);
  LRContext_enumerate_items(context, i_rule, item_array);
  while (Array_length(item_array) > 0) {
    Array *next_item_array = Array_new(sizeof(LrItem), XLR_TYPE_ITEM, context->allocator);
    const uint32_t n_items = Array_length(item_array);
    const LrItem * const items = Array_first_real(item_array);
    for (uint32_t i = 0; i < n_items; i++) {
      const LrItem * const item = &items[i];
      State * const state = Array_real_addr(context->state_array, item->state);
      if (LrItem_isEnd(context, item)) {
        Action *action = State_set_reduce_action(context, state, item);
        if (!action) { return context->error; }
        action->using.count += action_flag;
        continue;
      }
      const INDEX(Symbol) i_cur_sym = LrItem_current(context, item);
      Action *action = State_set_stack_action(context, state, i_cur_sym);
      if (!action) { return context->error; }
      LrItem next_item = { .state = action->index, .rule = item->rule, .look = item->look, .pos = item->pos + 1 };
      Array_append(next_item_array, &next_item, 1);
      action->using.count += action_flag;
      LrItem use_item = { .state = item->state, .rule = i_rule, .look = item->look, .pos = 0 };
      Symbol_set_env(context, i_cur_sym, &use_item, enable_flag);
    }
    releasePrimeArray(item_array);
    LrItem_build_closure(context, next_item_array);
    item_array = next_item_array;
  }
  releasePrimeArray(item_array);
  Symbol_update_first_set(context, rule->target, rule, 0);
  rule->enabled = enable_flag;
  context->error = SUCCESS;
  return context->error;
}
