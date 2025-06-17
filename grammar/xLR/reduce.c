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

Accessed * XLR_Accessed_0 (Token args[], LRContext *, ErrInfo *, const Allocator *allocator) {
  ArithExpr *lhs = args[0].value;
  ArithExpr *rhs = args[2].value;

  CompExpr *expr = allocator->calloc(1, sizeof(Expr));
  expr->type = XLR_TOKEN_Accessed;
  expr->lhs = lhs;
  expr->rhs = rhs;

  return expr;
}

ActionBlock * XLR_ActionBlock_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ActionStatement * XLR_ActionStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ActionStatement * XLR_ActionStatement_1 (Token args[], LRContext *context, ErrInfo *, const Allocator *allocator) {
  REFER(ActionBlock) v_block = args[0].value;

  INDEX(ActionBlock) i_block = refer2index(v_block);
  Array_append(context->curr_block->subblocks, &i_block, 1);

  ActionStatement *statement = allocator->calloc(1, sizeof(ActionStatement));
  statement->type = XLR_TOKEN_ActionBlock;
  statement->condition = nullptr;
  statement->action = v_block;

  return statement;
}

ActionStatement * XLR_ActionStatement_2 (Token args[], LRContext *context, ErrInfo *, const Allocator *) {
  Declaration *vars = args[0].value;

  ActionBlock *block = context->curr_block;
  Array_foreach(Variable, vars, {
    REFER(Variable) v_var = AVLTree_get(block->var_tree, (uint64_t) __element->name);
    if (v_var) { return nullptr; }
    Array_append(block->var_array, __element, 1);
    v_var = Array_last_virt(block->var_array);
    AVLTree_set(block->var_tree, (uint64_t) __element->name, v_var);
  });

  return (ActionStatement *) XLR_TOKEN_ActionStatement;
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

Arguments * XLR_Arguments_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arguments * XLR_Arguments_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Arguments * XLR_Arguments_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

ArithExpr * XLR_ArithExpr_0 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

ArithExpr * XLR_ArithExpr_1 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

ArithExpr * XLR_ArithExpr_2 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
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

AssignExpr * XLR_AssignExpr_0 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *) {
  Assignable *assignable = args[0].value;
  ArithExpr *expr = args[2].value;
  switch (assignable->type) {
    case XLR_TOKEN_IDENTIFIER: {
      LRVariable *var = assignable->rhs;
      var->value = LRContext_eval(context, errInfo, expr);
      return assignable;
    }
    case XLR_TOKEN_Accessed: {}
    case XLR_TOKEN_Subscribed: {}
  }

  return nullptr;
}

AssignExpr * XLR_AssignExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

AssignExpr * XLR_AssignExpr_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

Assignable * XLR_Assignable_0 (Token args[], LRContext *context, ErrInfo *, const Allocator *allocator) {
  REFER(Identifier) v_ident = args[0].value;

  LRVariable *var = LRContext_get_variable(context, v_ident);
  if (!var) { return nullptr; }

  Assignable *assignable = allocator->calloc(1, sizeof(Assignable));
  assignable->type = XLR_TOKEN_IDENTIFIER;
  assignable->lhs = nullptr;
  assignable->rhs = var;

  return assignable;
}

Assignable * XLR_Assignable_1 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

Assignable * XLR_Assignable_2 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

CompExpr * XLR_CompExpr_0 (Token args[], LRContext *, ErrInfo *, const Allocator *allocator) {
  uint32_t optype = (uint64_t) args[0].value;
  CompExpr *rhs = args[1].value;

  Expr *expr = allocator->calloc(1, sizeof(Expr));
  expr->type = optype;
  expr->lhs = nullptr;
  expr->rhs = rhs;

  return expr;
}

CompExpr * XLR_CompExpr_1 (Token args[], LRContext *, ErrInfo *, const Allocator *allocator) {
  ArithExpr *lhs = args[0].value;
  uint32_t optype = (uint64_t) args[1].value;
  ArithExpr *rhs = args[2].value;

  CompExpr *expr = allocator->calloc(1, sizeof(Expr));
  expr->type = optype;
  expr->lhs = lhs;
  expr->rhs = rhs;

  return expr;
}

CompExpr * XLR_CompExpr_2 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[1].value;
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

  LRType type = {.type = XLR_STRUCT_ENUM, .name = v_ident, .size = 32, .refer = items};
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

  LRType type = {.type = XLR_STRUCT_ENUM, .name = v_ident, .size = size, .refer = items};
  Array_append(context->type_array, &type, 1);
  v_type = Array_last_virt(context->type_array);
  AVLTree_set(context->type_tree, (uint64_t) v_ident, v_type);

  return v_type;
}

Evaluable * XLR_Evaluable_0 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

Evaluable * XLR_Evaluable_1 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

Evaluable * XLR_Evaluable_2 (Token args[], LRContext *context, ErrInfo *, const Allocator *allocator) {
  REFER(Identifier) v_type_name = args[0].value;
  REFER(Identifier) v_enum_name = args[2].value;

  REFER(LRType) v_type = AVLTree_get(context->type_tree, (uint64_t) v_type_name);
  if (!v_type) { return nullptr; }
  const LRType *type = Array_virt2real(context->type_array, v_type);
  if (type->type != XLR_STRUCT_ENUM) { return nullptr; }
  LRValue *val = nullptr;
  Array_foreach(EnumItem, type->refer, {
    if (__element->name == v_enum_name) { val = __element->value; break; }
  });
  if (!val) { return nullptr; }

  Evaluable *evaluable = allocator->calloc(1, sizeof(Evaluable));
  evaluable->type = XLR_TOKEN_VAL_LITERAL;
  evaluable->lhs = nullptr;
  evaluable->rhs = val;

  return args[0].value;
}

Evaluable * XLR_Evaluable_3 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

Evaluable * XLR_Evaluable_4 (Token args[], LRContext *, ErrInfo *, const Allocator *allocator) {
  LRValue *val = args[0].value;

  Evaluable *evaluable = allocator->calloc(1, sizeof(Evaluable));
  evaluable->type = XLR_TOKEN_VAL_LITERAL;
  evaluable->lhs = nullptr;
  evaluable->rhs = val;

  return evaluable;
}
Evaluable * XLR_Evaluable_5 (Token args[], LRContext *, ErrInfo *, const Allocator * allocator) {
  uint64_t the_char = (uint64_t) args[0].value;

  LRValue *val = allocator->calloc(1, sizeof(LRValue));
  val->type = XLR_BUILTIN_TYPE_U64;
  val->size = 8;
  val->val.U64 = the_char;

  Evaluable *evaluable = allocator->calloc(1, sizeof(Evaluable));
  evaluable->type = XLR_TOKEN_VAL_LITERAL;
  evaluable->lhs = nullptr;
  evaluable->rhs = val;

  return evaluable;
}

Evaluable * XLR_Evaluable_6 (Token args[], LRContext *, ErrInfo *, const Allocator * allocator) {
  LRValue *texts = args[0].value;

  Evaluable *evaluable = allocator->calloc(1, sizeof(Evaluable));
  evaluable->type = XLR_TOKEN_LiteralTexts;
  evaluable->lhs = nullptr;
  evaluable->rhs = texts;

  return evaluable;
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

GrammarEntry * XLR_GrammarEntry_0 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

GrammarEntry * XLR_GrammarEntry_1 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

GrammarEntry * XLR_GrammarEntry_2 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

GrammarEntry * XLR_GrammarEntry_EXT (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
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
  const uint64_t plain = (uint64_t) args[0].value;

  return LRContext_plain_to_sym(context, plain);
}

GrammarItem * XLR_GrammarItem_3 (Token args[], LRContext *context, ErrInfo *, const Allocator * allocator) {
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
  REFER(GrammarItem) item = args[1].value;

  INDEX(LRSymbol) i_item = refer2index(item);
  Array_append(items, &i_item, 1);

  return items;
}

GrammarItems * XLR_GrammarItems_1 (Token args[], LRContext *, ErrInfo *, const Allocator *allocator) {
  REFER(GrammarItem) item = args[0].value;

  GrammarItems *items = Array_new(sizeof(INDEX(GrammarItem)), XLR_TYPE_SYMBOL, allocator);

  INDEX(LRSymbol) i_item = refer2index(item);
  Array_append(items, &i_item, 1);

  return items;
}

GrammarItems * XLR_GrammarItems_2 (Token [], LRContext *, ErrInfo *, const Allocator *allocator) {
  return Array_new(sizeof(INDEX(GrammarItem)), XLR_TYPE_SYMBOL, allocator);
}

GrammarPattern * XLR_GrammarPattern_0 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  GrammarItems *syms = args[1].value;
  return syms;
}

GrammarPattern * XLR_GrammarPattern_1 (Token args[], LRContext *context, ErrInfo *, const Allocator *allocator) {
  LiteralTexts *texts = args[0].value;

  GrammarItems *items = Array_new(sizeof(INDEX(GrammarItem)), XLR_TYPE_SYMBOL, allocator);

  const char_t *const plains = Array_virt2real(context->text_array, texts->val.STRING);
  for (uint32_t i = 0; i < texts->size; i++) {
    const uint64_t plain = plains[i];
    const REFER(LRSymbol) v_sym = LRContext_plain_to_sym(context, plain);
    Array_append(items, v_sym, 1);
  }
  return items;
}

IfCondition * XLR_IfCondition_0 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

IfStatement * XLR_IfStatement_0 (Token args[], LRContext *, ErrInfo *, const Allocator *allocator) {
  CondExpr *condition = args[1].value;
  ActionStatement *action = args[2].value;

  IfStatement *statement = allocator->calloc(1, sizeof(IfStatement));
  statement->condition = condition;
  statement->action = action;

  return statement;
}

IntegratedExpr * XLR_IntegratedExpr_0 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

IntegratedExpr * XLR_IntegratedExpr_1 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

LiteralTexts * XLR_LiteralTexts_0 (Token args[], LRContext *context, ErrInfo *, const Allocator *) {
  LiteralTexts *texts = args[0].value;
  WrapperedText *text = args[1].value;

  LRContent_add_text_content(context, text->content, text->length);
  texts->size += text->length;

  return texts;

}

LiteralTexts * XLR_LiteralTexts_1 (Token args[], LRContext *context, ErrInfo *, const Allocator *allocator) {
  WrapperedText *text = args[0].value;

  LRValue *texts = allocator->calloc(1, sizeof(LRValue));
  texts->type = XLR_BUILTIN_TYPE_STR;
  texts->size = text->length;
  texts->val.STRING = LRContent_new_text_content(context, text->content, text->length);

  return texts;
}

OptionalAssignExpr * XLR_OptionalAssignExpr_0 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

OptionalAssignExpr * XLR_OptionalAssignExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return (OptionalAssignExpr *) XLR_TOKEN_OptionalAssignExpr;
}

OptionalCondExpr * XLR_OptionalCondExpr_0 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

OptionalCondExpr * XLR_OptionalCondExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return (OptionalCondExpr *) XLR_TOKEN_OptionalCondExpr;
}

RuleDefinition * XLR_RuleDefinition_0 (Token args[], LRContext *context, ErrInfo *, const Allocator *) {
  REFER(LRType) *v_type = args[0].value;
  REFER(Identifier) *v_ident = args[1].value;
  GrammarPattern *pattern = args[2].value;
  ActionBlock *action = args[3].value;

  REFER(LRRule) v_rule = AVLTree_get(context->rule_tree, (uint64_t) v_ident);
  if (v_rule) { return nullptr; }

  const LRType *type = Array_virt2real(context->type_array, v_type);
  REFER(LRSymbol) v_sym = AVLTree_get(context->sym_tree, (uint64_t) type->name);
  LRRule rule = { .enabled = false, .target = refer2index(v_sym),
                  .items = pattern, .action = action };
  Array_append(context->rule_array, &rule, 1);
  v_rule = Array_last_virt(context->rule_array);
  AVLTree_set(context->rule_tree, (uint64_t) v_ident, v_rule);

  return v_rule;
}

RuleDefinition * XLR_RuleDefinition_1 (Token args[], LRContext *context, ErrInfo *, const Allocator *) {
  REFER(Identifier) *v_type_name = args[0].value;
  REFER(Identifier) *v_rule_name = args[1].value;
  GrammarPattern *pattern = args[2].value;
  ActionBlock *block = args[3].value;

  REFER(LRType) v_type = AVLTree_get(context->type_tree, (uint64_t) v_type_name);
  if (v_type) { return nullptr; }
  REFER(LRRule) v_rule = AVLTree_get(context->rule_tree, (uint64_t) v_rule_name);
  if (v_rule) { return nullptr; }

  REFER(LRSymbol) v_sym = AVLTree_get(context->sym_tree, (uint64_t) v_type_name);
  LRRule rule = { .enabled = false, .target = refer2index(v_sym),
      .items = pattern, .action = block };
  Array_append(context->rule_array, &rule, 1);
  v_rule = Array_last_virt(context->rule_array);
  AVLTree_set(context->rule_tree, (uint64_t) v_rule_name, v_rule);

  return v_rule;
}

Subscribed * XLR_Subscribed_0 (Token args[], LRContext *, ErrInfo *, const Allocator *allocator) {
  IntegratedExpr *lhs = args[0].value;
  Subscriber     *rhs = args[1].value;

  Expr *expr = allocator->calloc(1, sizeof(Expr));
  expr->type = XLR_TOKEN_Subscribed;
  expr->lhs = lhs;
  expr->rhs = rhs;

  return expr;
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
  Array_foreach(LRVariable, attrs, {
    const LRType *type = Array_real_addr(context->type_array, __element->type);
    size += type->size * (uint32_t) (uint64_t) __element->count->val.U32;
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
  ArithExpr *value_expr = args[2].value;

  // TODO: check multi-define for variable
  Variable *item = allocator->calloc(1, sizeof(Variable));
  item->type = -1;
  item->name = attr_name;
  item->count = nullptr;
  item->value = LRContext_eval(context, errInfo, value_expr);

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
