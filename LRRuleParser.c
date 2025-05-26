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
 * Filename: LRParser.c
 * Creator: Yaokai Liu
 * Create Date: 2025-05-21
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "LRRuleParser.h"
#include "xlr.h"
#include "tokens.h"

#define Array_foreach(type, _array, doing)              \
  do {                                                  \
    uint32_t __n_elements = Array_length(_array);       \
    type *__elements = Array_first_real(_array);        \
    for (uint32_t __i = 0; __i < __n_elements; __i++) { \
      type *__element = &__elements[__i];               \
      doing                                             \
    }                                                   \
  } while (false)

#define LRSymbol_new_envs() Dict_new(sizeof(LREnvPair), sizeof_set, \
  (unikey_t *) LREnvPair_hash, XLR_TYPE_SYMBOL, context->allocator)

#define LRContext_add_symbol() do {                                         \
  LRSymbol sym = {                                                          \
    .symtype = SYMTYPE_TERMINAL, .index = Array_length(context->sym_array), \
    .rules = nullptr, .firsts = nullptr, .envs = LRSymbol_new_envs()        \
  };                                                                        \
  Array_append(context->sym_array, &sym, 1);                                \
} while (false)

#define LRContext_add_named_symbol(i_ident) do {                      \
  REFER(LRSymbol) v_sym = AVLTree_get(context->sym_tree, (i_ident));  \
  if (!v_sym) {                                                       \
    LRContext_add_symbol();                                           \
    v_sym = Array_last_virt(context->sym_array);                      \
    AVLTree_set(context->sym_tree, (i_ident), v_sym);                 \
  }                                                                   \
} while (false)

typedef struct RegexPair {
  INDEX(LRSymbol) target;
  const Regex *   regex;
} RegexPair;

enum QUANT_TYPE_ENUM {
  QUANT_KLEENE_STAR = 0,
  QUANT_ONE_OR_LESS = 1,
  QUANT_KLEENE_PLUS = 2,
  QUANT_DEFAULT = 3,
  QUANT_SPECIFIED = 3,
};

INDEX(LRSymbol) LRRule_add_item(LRContext *context, const INDEX(LRSymbol) i_sym,
                                uint32_t min_times, uint32_t max_times) {
  const Allocator * const allocator = context->allocator;
  if (min_times == 1 && max_times == 1) { return i_sym; }
  LRContext_add_symbol();
  INDEX(LRSymbol) i_target = Array_length(context->sym_array) - 1;
  LRRule rules[3] = {
      { .items = Array_new(sizeof(INDEX(LRSymbol)), XLR_TYPE_SYMBOL, allocator),
          .target = i_target, .enabled = false },
      { .items = Array_new(sizeof(INDEX(LRSymbol)), XLR_TYPE_SYMBOL, allocator),
          .target = i_target, .enabled = false },
      { .items = nullptr, .target = i_target, .enabled = false },
  };
  if (min_times == 0 && max_times == 1) {
    Array_append(rules[0].items, &i_sym, 1);
    Array_append(context->rule_array, &rules[1], 2);
    return i_target;
  }
  if (min_times == 1 && max_times == 0) {
    Array_append(rules[0].items, &i_target, 1);
    Array_append(rules[0].items, &i_sym, 1);
    Array_append(rules[1].items, &i_sym, 1);
    Array_append(context->rule_array, rules, 2);
    return i_target;
  }
  if (min_times == 0 && max_times == 0) {
    Array_append(rules[0].items, &i_target, 1);
    Array_append(rules[0].items, &i_sym, 1);
    Array_append(rules[1].items, &i_sym, 1);
    Array_append(context->rule_array, rules, 3);
    return i_target;
  }
  context->error = XLR_ERROR_BAD_QUANTIFIER;
  return 0;
}

uint32_t LRRule_from_regex(LRContext *context, REFER(void) target, const Regex *regexp) {
  const Allocator * const allocator = context->allocator;
  LRContext_add_named_symbol((uint64_t) target);
  INDEX(LRSymbol) i_target = Array_length(context->sym_array) - 1;
  Array *regex_array =  Array_new(sizeof(RegexPair), enum_Regex, allocator);
  RegexPair pair = { .target = i_target, .regex = regexp };
  Array_append(regex_array, &pair, 1);
  uint32_t old_count = 0, new_count = Array_length(regex_array);
  while (old_count < new_count) {
    uint32_t n_branches = Array_length(regexp);
    for (uint32_t i = 0; i < n_branches; i++) {
      const Branch *branch = Array_real_addr(regexp, i);
      uint32_t n_objects = Array_length(branch);
      const Object *objects = Array_real_addr(branch, 0);
      LRRule rule = {
          .items = Array_new(sizeof(INDEX(LRSymbol)), XLR_TYPE_SYMBOL, allocator),
          .target = i_target, .enabled = false
      };
      for (uint32_t j = 0; j < n_objects; j++) {
        INDEX(LRSymbol) i_sym = 0;
        if (objects[i].type == enum_Group) {
          regexp = ((Group *) objects[i].target)->regexp;
          if (regexp && Array_length(regexp) > 0) {
            LRContext_add_symbol();
            i_sym = Array_length(context->sym_array) - 1;
            RegexPair sub_pair = { .target = i_sym, .regex = regexp };
            Array_append(regex_array, &sub_pair, 1);
          }
        } else if (objects[i].type == enum_SYMBOL) {
          LRContext_add_named_symbol((uint64_t) objects[i].target);
          i_sym = Array_length(context->sym_array) - 1;
        } else {
          releasePrimeArray(rule.items);
          releasePrimeArray(regex_array);
          context->error = XLR_ERROR_BAD_TOKEN;
          return context->error;
        }
        i_sym = LRRule_add_item(context, i_sym, objects[i].min_times, objects[i].max_times);
        if (!i_sym) { return context->error; }
        Array_append(rule.items, &i_sym, 1);
      }
    }
    old_count = new_count;
    new_count = Array_length(regex_array);
  }
  releasePrimeArray(regex_array);
  return XLR_SUCCESS;
}
