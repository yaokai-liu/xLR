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
#include "regex/tokens.h"
#include "regex/parse.h"
#include "regex/xlr.h"

#define Array_foreach(type, _array, doing)              \
  do {                                                  \
    uint32_t __n_elements = Array_length(_array);       \
    type *__elements = Array_first_real(_array);        \
    for (uint32_t __i = 0; __i < __n_elements; __i++) { \
      type *__element = &__elements[__i];               \
      doing                                             \
    }                                                   \
  } while (false)

#define LRRule_new_items() Array_new(sizeof(INDEX(LRSymbol)), XLR_TYPE_SYMBOL, context->allocator)
#define LRSymbol_new_envs() Dict_new(sizeof(LREnvPair), sizeof_set, (unikey_t *) LREnvPair_hash, \
  XLR_TYPE_SYMBOL, nullptr, (destruct_t *) LRRuleSet_release, context->allocator)
#define LRSymbol_new_firsts() Dict_new(sizeof(INDEX(LRTerminal)), sizeof_set, nullptr, \
  XLR_TYPE_SYMBOL, nullptr, (destruct_t *) LRRuleSet_release, allocator)
#define LRState_new_actions() Dict_new(sizeof(LRActKeyPair), sizeof(LRAction), (unikey_t *) LRActKeyPair_hash, \
  XLR_TYPE_ACT_KEY, nullptr, (destruct_t *) LRAction_release, context->allocator)
#define LRContext_new_ruleset() Set_new(sizeof(INDEX(LRRule)), \
  XLR_TYPE_RULE_KEY, nullptr, (destruct_t *) LRRuleSet_release, context->allocator)

REFER(LRSymbol) LRContext_add_symbol(LRContext *context);
REFER(LRSymbol) LRContext_add_named_symbol(LRContext *context, const REFER(char_t) v_ident);

inline REFER(LRSymbol) LRContext_add_symbol(LRContext *context) {
  LRSymbol sym = {
      .symtype = SYMTYPE_TERMINAL, .index = Array_length(context->sym_array),
      .rules = nullptr, .firsts = nullptr, .envs = LRSymbol_new_envs()
  };
  Array_append(context->sym_array, &sym, 1);
  return Array_last_virt(context->sym_array);
}

inline REFER(LRSymbol) LRContext_add_named_symbol(LRContext *context, const REFER(char_t) v_ident) {
  REFER(LRSymbol) v_sym = AVLTree_get(context->sym_tree, (uint64_t) v_ident);
  if (!v_sym) {
    v_sym = LRContext_add_symbol(context);
    AVLTree_set(context->sym_tree, (uint64_t) v_ident, v_sym);
  }
  return v_sym;
}

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
  LRContext_add_symbol(context);
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

INDEX(LRRule) LRContext_add_rules_from_regex(LRContext *context, const REFER(char_t) target, const Regex *regexp) {
  const Allocator * const allocator = context->allocator;

  const INDEX(LRRule) i_rule = Array_length(context->rule_array);

  LRContext_add_named_symbol(context, target);
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
      LRRule new_rule = {
          .items = Array_new(sizeof(INDEX(LRSymbol)), XLR_TYPE_SYMBOL, allocator),
          .target = i_target, .enabled = false
      };
      Array_append(context->rule_array, &new_rule, 1);
      LRRule *rule = Array_last_real(context->rule_array);
      for (uint32_t j = 0; j < n_objects; j++) {
        INDEX(LRSymbol) i_sym = 0;
        if (objects[i].type == enum_Group) {
          regexp = ((Group *) objects[i].target)->regexp;
          if (regexp && Array_length(regexp) > 0) {
            LRContext_add_symbol(context);
            i_sym = Array_length(context->sym_array) - 1;
            RegexPair sub_pair = { .target = i_sym, .regex = regexp };
            Array_append(regex_array, &sub_pair, 1);
          }
        } else if (objects[i].type == enum_SYMBOL) {
          LRContext_add_named_symbol(context, objects[i].target);
          i_sym = Array_length(context->sym_array) - 1;
        } else {
          context->error = XLR_ERROR_BAD_TOKEN;
          goto __failed_to_add_rule;
        }
        i_sym = LRRule_add_item(context, i_sym, objects[i].min_times, objects[i].max_times);
        if (!i_sym) { goto __failed_to_add_rule; }
        Array_append(rule->items, &i_sym, 1);
      }
    }
    old_count = new_count;
    new_count = Array_length(regex_array);
  }
  releasePrimeArray(regex_array);
  return i_rule;

  __failed_to_add_rule:
  uint32_t n_rules = Array_length(context->rule_array);
  for (uint32_t i = i_rule; i < n_rules; i ++) {
    LRRule_release(Array_real_addr(context->rule_array, i), context->allocator);
  }
  Array_delete(context->rule_array, i_rule, n_rules - i_rule);
  releasePrimeArray(regex_array);
  return 0;
}

#define token_check(expr) do {            \
  if (!(expr)) {                          \
    errInfo->code = XLR_UNEXPECTED_TOKEN; \
    errInfo->pos = token.location;        \
    errInfo->token = token.type;          \
    errInfo->state = 0;                   \
    return 0;                             \
  }                                       \
} while (false)

INDEX(LRRule) LRContext_add_rule(LRContext *context, ErrInfo *errInfo, const char_t *string) {
  const Allocator *const allocator = context->allocator;
  XLRTokenizer * tokenizer = XLRTokenizer_new(string, context->ident_array, context->ident_trie, allocator);
  Token token = {};
  uint32_t result = REGEX_ERROR_UNRECOGNIZED_SYMBOL;

  result = tokenizer->SUPER.next((Tokenizer *) tokenizer, &token, errInfo, allocator);
  if (result != REGEX_SUCCESS) { return 0; }
  token_check(token.type == enum_SYMBOL);

  REFER(char_t) target = token.value;

  result = tokenizer->SUPER.next((Tokenizer *) tokenizer, &token, errInfo, allocator);
  if (result != REGEX_SUCCESS) { return 0; }
  token_check(token.type == enum_ASSIGNER);

  Regex *regex = parse((Tokenizer *) tokenizer, errInfo, allocator);
  if (!regex) { return 0; }

  result = tokenizer->SUPER.next((Tokenizer *) tokenizer, &token, errInfo, allocator);
  if (result != REGEX_SUCCESS) { return 0; }
  token_check(token.type == enum_SEMICOLON || token.type == enum_TERMINATOR);

  INDEX(LRRule) i_rule = LRContext_add_rules_from_regex(context, target, regex);
  XLRTokenizer_destroy(tokenizer);
  if (!i_rule) { errInfo->code = context->error; }
  return i_rule;
}

inline INDEX(LRSymbol) LRContext_add_target(LRContext *context, ErrInfo *errInfo, const char_t *string) {
  const Allocator *const allocator = context->allocator;
  XLRTokenizer * tokenizer = XLRTokenizer_new(string, context->ident_array, context->ident_trie, allocator);
  Token token = {};
  uint32_t result = REGEX_ERROR_UNRECOGNIZED_SYMBOL;

  result = tokenizer->SUPER.next((Tokenizer *) tokenizer, &token, errInfo, allocator);
  if (result != REGEX_SUCCESS) { return 0; }
  token_check(token.type == enum_SYMBOL);

  REFER(char_t) v_ident = token.value;

  result = tokenizer->SUPER.next((Tokenizer *) tokenizer, &token, errInfo, allocator);
  if (result != REGEX_SUCCESS) { return 0; }
  token_check(token.type == enum_TERMINATOR);

  REFER(LRSymbol) v_sym = AVLTree_get(context->sym_tree, (uint64_t) v_ident);
  if (!v_sym) {
    context->error = XLR_ERROR_UNKNOWN_TARGET;
    errInfo->code = context->error;
    return 0;
  }
  LRSymbol *sym = Array_virt2real(context->sym_array, v_sym);
  INDEX(LRSymbol) i_sym = (uint32_t) (uint64_t) v_sym;

  LRRule rule = {.items = LRRule_new_items(), .target = SYM_INDEX_EXTEND, false};
  Array_append(rule.items, &i_sym, 1);

  INDEX(LRRule) i_rule = Array_length(context->rule_array);
  Array_append(context->rule_array, &rule, 1);

  LREnvPair pair = { .state = 0, .follow = SYM_INDEX_TERMINATOR };
  Set *rule_set = Set_new(sizeof(INDEX(LRRule)), XLR_TYPE_RULE, nullptr, nullptr, allocator);
  Set_add(rule_set, &i_rule);

  Dict_set(sym->envs, &pair, rule_set);

  result = LRContext_set_rule(context, i_rule, true);
  if (result != XLR_SUCCESS) { errInfo->code = context->error; return 0; }
  return i_sym;
}
