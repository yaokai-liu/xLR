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
 * Filename: reduce.c
 * Creator: Yaokai Liu
 * Create Date: 2025-05-30
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include <stdint-gcc.h>
#include "target.h"
#include "enum.h"
#include "generated/xLR/rules.gen.h"

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

Accessed * XLR_Accessed_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ActionBlock * XLR_ActionBlock_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ActionStatement * XLR_ActionStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ActionStatement * XLR_ActionStatement_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ActionStatement * XLR_ActionStatement_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ActionStatement * XLR_ActionStatement_3 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ActionStatements * XLR_ActionStatements_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ActionStatements * XLR_ActionStatements_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ActionStatements * XLR_ActionStatements_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

AndCondExpr * XLR_AndCondExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

AndCondExpr * XLR_AndCondExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arguments * XLR_Arguments_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arguments * XLR_Arguments_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arguments * XLR_Arguments_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ArithExpr * XLR_ArithExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ArithExpr * XLR_ArithExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ArithExpr * XLR_ArithExpr_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arith_0_Expr * XLR_Arith_0_Expr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arith_0_Expr * XLR_Arith_0_Expr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arith_0_Expr * XLR_Arith_0_Expr_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arith_1_Expr * XLR_Arith_1_Expr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arith_1_Expr * XLR_Arith_1_Expr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arith_1_Expr * XLR_Arith_1_Expr_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arith_2_Expr * XLR_Arith_2_Expr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arith_2_Expr * XLR_Arith_2_Expr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arith_2_Expr * XLR_Arith_2_Expr_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arith_3_Expr * XLR_Arith_3_Expr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arith_3_Expr * XLR_Arith_3_Expr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arith_3_Expr * XLR_Arith_3_Expr_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arith_4_Expr * XLR_Arith_4_Expr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arith_4_Expr * XLR_Arith_4_Expr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

AssignExpr * XLR_AssignExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

AssignExpr * XLR_AssignExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

AssignExpr * XLR_AssignExpr_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Assignable * XLR_Assignable_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Assignable * XLR_Assignable_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Assignable * XLR_Assignable_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

CondExpr * XLR_CondExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

CondExpr * XLR_CondExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

CondStatement * XLR_CondStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

CondStatement * XLR_CondStatement_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

CondStatement * XLR_CondStatement_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

CondStatement * XLR_CondStatement_3 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Declaration * XLR_Declaration_0 (Token args[], LRContext *context, ErrInfo *, const Allocator *) {
  REFER(Identifier) type_name = args[0].value;
  VarList *variables = args[1].value;

  const REFER(LRType) type = AVLTree_get(context->type_tree, (uint64_t) type_name);
  if (!type) { return nullptr; }

  INDEX(LRType) i_type = (uint32_t) (uint64_t) type;

  Array_foreach(Variable, variables, {
    if (__element->value && __element->value->type != i_type) { return nullptr; }
    __element->type = i_type;
  });


  return variables;
}

Declarations * XLR_Declarations_0 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  Declaration *decl = args[0].value;
  Declarations *decls = args[1].value;

  Array_concat(decls, decl);
  releasePrimeArray(decl);

  return decls;
}

Declarations * XLR_Declarations_1 (Token args[], LRContext *, ErrInfo *, const Allocator *allocator) {
  Declaration *decl = args[0].value;

  Declarations *decls = Array_new(sizeof(Variable), XLR_TOKEN_Variable, allocator);

  Array_concat(decls, decl);
  releasePrimeArray(decl);

  return decls;
}

EnumItem * XLR_EnumItem_0 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  REFER(Identifier) name = args[0].value;
  ArithExpr *val = args[2].value;

  EnumItem *item = allocator->calloc(1, sizeof(EnumItem));
  item->name = name;
  item->value = LRContext_eval(context, errInfo, val);

  return item;
}

EnumItem * XLR_EnumItem_1 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  REFER(Identifier) name = args[0].value;

  EnumItem *item = allocator->calloc(1, sizeof(EnumItem));
  item->name = name;
  item->value = LRContext_last_enum_val(context, errInfo);

  return item;
}

EnumItems * XLR_EnumItems_0 (Token args[], LRContext *, ErrInfo *, const Allocator *allocator) {
  EnumItem *item = args[0].value;
  EnumItems *items = args[1].value;

  Array_append(items, item, 1);
  allocator->free(item);

  return items;
}

EnumItems * XLR_EnumItems_1 (Token args[], LRContext *, ErrInfo *, const Allocator *allocator) {
  EnumItem *item = args[0].value;

  EnumItems *items = Array_new(sizeof(EnumItem), XLR_TOKEN_EnumItem, allocator);

  Array_append(items, item, 1);
  allocator->free(item);

  return items;
}

EnumDeclaration * XLR_EnumDeclaration_0 (Token args[], LRContext *context, ErrInfo *, const Allocator *) {
  REFER(Identifier) v_ident = args[1].value;
  EnumItems *items = args[3].value;

  REFER(LRType) v_type = AVLTree_get(context->type_tree, (uint64_t) v_ident);
  if (v_type) { return nullptr; }

  Array_append(context->enum_array, items, 1);
  REFER(Array) v_items = Array_last_virt(context->enum_array);
  LRType type = {.type = XLR_STRUCT_ENUM, .name = v_ident, .size = 32, .refer = v_items};
  Array_append(context->type_array, &type, 1);
  v_type = Array_last_virt(context->type_array);
  AVLTree_set(context->type_tree, (uint64_t) v_ident, v_type);

  return v_type;
}

EnumDeclaration * XLR_EnumDeclaration_1 (Token args[], LRContext *context, ErrInfo *, const Allocator *) {
  REFER(Identifier) v_ident = args[1].value;
  REFER(Identifier) t_name = args[3].value;
  EnumItems *items = args[5].value;

  REFER(LRType) v_base = AVLTree_get(context->type_tree, (uint64_t) t_name);
  if (!v_base) { return nullptr; }
  const LRType *base = Array_virt2real(context->type_array, v_base);
  if (base->type != XLR_STRUCT_BUILTIN && base->type != XLR_STRUCT_ENUM) { return nullptr; }
  const uint32_t size = base->size;

  REFER(LRType) v_type = AVLTree_get(context->type_tree, (uint64_t) v_ident);
  if (v_type) { return nullptr; }

  Array_append(context->enum_array, items, 1);
  REFER(Array) v_items = Array_last_virt(context->enum_array);
  LRType type = {.type = XLR_STRUCT_ENUM, .name = v_ident, .size = size, .refer = v_items};
  Array_append(context->type_array, &type, 1);
  v_type = Array_last_virt(context->type_array);
  AVLTree_set(context->type_tree, (uint64_t) v_ident, v_type);

  return v_type;
}

Evaluable * XLR_Evaluable_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Evaluable * XLR_Evaluable_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Evaluable * XLR_Evaluable_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Evaluable * XLR_Evaluable_3 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ForCondition * XLR_ForCondition_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ForStatement * XLR_ForStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

FunctionCall * XLR_FunctionCall_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

GrammarAction * XLR_GrammarAction_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

GrammarEntry * XLR_GrammarEntry_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

GrammarEntry * XLR_GrammarEntry_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

GrammarEntry * XLR_GrammarEntry_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

GrammarEntry * XLR_GrammarEntry_EXT (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

GrammarItem * XLR_GrammarItem_0 (Token args[], LRContext *context, ErrInfo *, const Allocator *) {
  GrammarItems *syms = args[1].value;
  const LRSymbol target = LRSymbol_new();
  Array_append(context->sym_array, &target, 1);
  const REFER(LRSymbol) v_target = Array_last_virt(context->sym_array);
  const LRRule rule = {.items = LRRule_new_items(), .target = refer2index(v_target), .enabled = false};

  Array_foreach(REFER(LRSymbol), syms, {
    const INDEX(LRSymbol) i_sym = refer2index(*__element);
    Array_append(rule.items, &i_sym, 1);
  });
  Array_append(context->rule_array, &rule, 1);

  releasePrimeArray(syms);

  return v_target;
}

GrammarItem * XLR_GrammarItem_1 (Token args[], LRContext *context, ErrInfo *, const Allocator *) {
  const REFER(GrammarItem) v_sym = args[0].value;
  const uint32_t quant = (uint64_t) args[1].value;

  const INDEX(LRSymbol) i_sym = refer2index(v_sym);
  const LRSymbol target = LRSymbol_new();
  Array_append(context->sym_array, &target, 1);
  const REFER(LRSymbol) v_target = Array_last_virt(context->sym_array);
  const INDEX(LRSymbol) i_target = refer2index(v_target);

  switch (quant) {
    case XLR_QUANT_ANY_COUNT: {
      const LRRule RULES[3] = {
          {.items = LRRule_new_items(), .target = refer2index(v_target), .enabled = false},
          {.items = LRRule_new_items(), .target = refer2index(v_target), .enabled = false},
          {.items = LRRule_new_items(), .target = refer2index(v_target), .enabled = false},
      };
      Array_append(RULES[0].items, &i_target, 1);
      Array_append(RULES[0].items, &i_sym, 1);
      Array_append(RULES[1].items, &i_sym, 1);
      Array_append(context->rule_array, RULES, 3);
      break;
    }
    case XLR_QUANT_MORE_THAN_ONE: {
      const LRRule RULES[2] = {
          {.items = LRRule_new_items(), .target = refer2index(v_target), .enabled = false},
          {.items = LRRule_new_items(), .target = refer2index(v_target), .enabled = false},
      };
      Array_append(RULES[0].items, &i_sym, 1);
      Array_append(RULES[1].items, &i_target, 1);
      Array_append(RULES[1].items, &i_sym, 1);
      Array_append(context->rule_array, RULES, 2);
      break;
    }
    case XLR_QUANT_LESS_THAN_ONE: {
      const LRRule RULES[2] = {
          {.items = LRRule_new_items(), .target = refer2index(v_target), .enabled = false},
          {.items = LRRule_new_items(), .target = refer2index(v_target), .enabled = false},
      };
      Array_append(RULES[0].items, &i_sym, 1);
      Array_append(context->rule_array, RULES, 2);
      break;
    }
    default:{}
  }
  return v_target;
}

GrammarItem * XLR_GrammarItem_2 (Token args[], LRContext *context, ErrInfo *, const Allocator *) {
  const REFER(Identifier) ident = args[0].value;
  REFER(LRSymbol) v_sym = AVLTree_get(context->sym_tree, (uint64_t) ident);
  if (!v_sym) {
    LRSymbol sym = LRSymbol_new();
    Array_append(context->sym_array, &sym, 1);
    v_sym = Array_last_virt(context->sym_array);
    AVLTree_set(context->sym_tree, (uint64_t) ident, v_sym);
  }
  return v_sym;
}

GrammarItems * XLR_GrammarItems_0 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  GrammarItems *items = args[0].value;
  REFER(GrammarItem) *item = args[1].value;

  Array_append(items, item, 1);

  return items;
}

GrammarItems * XLR_GrammarItems_1 (Token args[], LRContext *, ErrInfo *, const Allocator *allocator) {
  REFER(GrammarItem) *item = args[0].value;

  GrammarItems *items = Array_new(sizeof(REFER(GrammarItem)), XLR_TYPE_SYMBOL, allocator);
  Array_append(items, item, 1);

  return items;
}

GrammarItems * XLR_GrammarItems_2 (Token [], LRContext *, ErrInfo *, const Allocator *allocator) {
  return Array_new(sizeof(REFER(GrammarItem)), XLR_TYPE_SYMBOL, allocator);
}

GrammarPattern * XLR_GrammarPattern_0 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  GrammarItems *syms = args[1].value;
  return syms;
}

IfCondition * XLR_IfCondition_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

IfStatement * XLR_IfStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

IntegratedExpr * XLR_IntegratedExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

IntegratedExpr * XLR_IntegratedExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

OptionalAssignExpr * XLR_OptionalAssignExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

OptionalAssignExpr * XLR_OptionalAssignExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

OptionalCondExpr * XLR_OptionalCondExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

OptionalCondExpr * XLR_OptionalCondExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

RuleDefinition * XLR_RuleDefinition_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

RuleDefinition * XLR_RuleDefinition_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

SingleCondExpr * XLR_SingleCondExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

SingleCondExpr * XLR_SingleCondExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

SingleCondExpr * XLR_SingleCondExpr_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

SingleCondExpr * XLR_SingleCondExpr_3 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Subscribed * XLR_Subscribed_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Subscriber * XLR_Subscriber_0 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[1].value;
}

TokenDefinition * XLR_TokenDefinition_0 (Token args[], LRContext *context, ErrInfo *, const Allocator *) {
  REFER(Identifier) v_ident = args[1].value;
  Declarations *attrs = args[3].value;

  REFER(LRType) v_type = AVLTree_get(context->type_tree, (uint64_t) v_ident);
  if (v_type) { return nullptr; }

  uint32_t size = 0;
  Array_foreach(LRAttr, attrs, {
    const LRType *type = Array_real_addr(context->type_array, __element->type);
    size += type->size * (uint32_t) (uint64_t) __element->count->bytes;
  });

  LRType type = { .type = XLR_STRUCT_TOKEN, .size = size, .name = v_ident, .refer = attrs };
  Array_append(context->type_array, &type, 1);
  v_type = Array_last_virt(context->type_array);
  AVLTree_set(context->type_tree, (uint64_t) v_ident, v_type);

  return v_type;
}

VarList * XLR_VarList_0 (Token args[], LRContext *, ErrInfo *, const Allocator * allocator) {
  VarList *list = args[0].value;
  Variable *var = args[2].value;

  Array_append(list, var, 1);
  allocator->free(var);

  return list;
}

VarList * XLR_VarList_1 (Token args[], LRContext *, ErrInfo *, const Allocator * allocator) {
  Variable *var = args[0].value;

  VarList *list = Array_new(sizeof(Variable), XLR_TOKEN_Variable, allocator);

  Array_append(list, var, 1);
  allocator->free(var);

  return list;
}

Variable * XLR_Variable_0 (Token args[], LRContext *, ErrInfo *, const Allocator *allocator) {
  REFER(Identifier) attr_name = args[0].value;

  // TODO: check multi-define for variable
  Variable *item = allocator->calloc(1, sizeof(Variable));
  item->type = -1;
  item->name = attr_name;
  item->count = nullptr;
  item->value = nullptr;

  return item;
}

Variable * XLR_Variable_1 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  REFER(Identifier) attr_name = args[0].value;
  ArithExpr *count_expr = args[1].value;

  // TODO: check multi-define for variable
  Variable *item = allocator->calloc(1, sizeof(Variable));
  item->type = -1;
  item->name = attr_name;
  item->count = LRContext_eval(context, errInfo, count_expr);
  item->value = nullptr;

  return (errInfo->code == XLR_SUCCESS) ? item : nullptr;
}

Variable * XLR_Variable_2 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  REFER(Identifier) attr_name = args[0].value;
  ArithExpr *value = args[2].value;

  // TODO: check multi-define for variable
  Variable *item = allocator->calloc(1, sizeof(Variable));
  item->type = -1;
  item->name = attr_name;
  item->count = nullptr;
  item->value = LRContext_eval(context, errInfo, value);

  return item;
}

Variable * XLR_Variable_3 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  REFER(Identifier) attr_name = args[0].value;
  ArithExpr *count_expr = args[1].value;
  ArithExpr *value_expr = args[3].value;

  // TODO: check multi-define for variable
  Variable *item = allocator->calloc(1, sizeof(Variable));
  item->type = -1;
  item->name = attr_name;
  item->count = LRContext_eval(context, errInfo, count_expr);
  item->value = LRContext_eval(context, errInfo, value_expr);

  return (errInfo->code == XLR_SUCCESS) ? item : nullptr;
}

WhileStatement * XLR_WhileStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}
