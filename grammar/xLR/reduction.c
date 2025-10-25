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
 * Filename: reduction.c
 * Creator: Yaokai Liu
 * Create Date: 2025-05-30
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include <math.h>

#include "target.h"
#include "enum.h"
#include "builtin.h"
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

inline Accessed * XLR_Accessed_0 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *) {
  const IntegratedExpr *integrated_expr = args[0].value;
  const Identifier *field_name = args[2].value;

  const REFER(LRType) v_type = integrated_expr->type;
  const LRType *type = Array_virt2real(context->type_array, v_type);
  if (type->cat != XLR_CATEGORY_STRUCT && type->cat != XLR_CATEGORY_UNION) {
    errInfo->code = XLR_ERROR_ACCESS_NONE_FIELD_TYPE;
    errInfo->start = args[1].start;
    errInfo->end = args[1].end;
    return nullptr;
  }
  const Array *field_array = type->refer;
  const uint32_t field_count = Array_length(field_array);
  const LRVariable *fields = Array_first_real(field_array);
  for (uint32_t i = 0; i < field_count; i++) {
    if (fields[i].ident == field_name) {
      const AVLTree *field_mapping = integrated_expr->refer;
      const REFER(LRVariable) v_var = AVLTree_get(field_mapping, (uint64_t) field_name);
      return Array_virt2real(context->var_array, v_var);
    }
  } /* field not found */ {
    errInfo->code = XLR_ERROR_NO_SUCH_FIELD;
    errInfo->start = args[2].start;
    errInfo->end = args[2].end;
    return nullptr;
  }
}

inline ActionBlock * XLR_ActionBlock_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return (ActionBlock *) (uint64_t) XLR_TOKEN_ActionBlock;
}

inline ActionBlock * XLR_ActionBlock_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return (ActionBlock *) (uint64_t) XLR_TOKEN_ActionBlock;
}

inline ActionStatement * XLR_ActionStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return (ActionStatement *) (uint64_t) XLR_TOKEN_ActionStatement;
}

inline ActionStatement * XLR_ActionStatement_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return (ActionStatement *) (uint64_t) XLR_TOKEN_ActionStatement;
}

inline ActionStatement * XLR_ActionStatement_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return (ActionStatement *) (uint64_t) XLR_TOKEN_ActionStatement;
}

inline ActionStatement * XLR_ActionStatement_3 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return (ActionStatement *) (uint64_t) XLR_TOKEN_ActionStatement;
}

inline ActionStatements * XLR_ActionStatements_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return (ActionStatements *) (uint64_t) XLR_TOKEN_ActionStatements;
}

inline ActionStatements * XLR_ActionStatements_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return (ActionStatements *) (uint64_t) XLR_TOKEN_ActionStatements;
}

inline Argument * XLR_Argument_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return (Argument *) (uint64_t) XLR_TOKEN_Argument;
}

inline Argument * XLR_Argument_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return (Argument *) (uint64_t) XLR_TOKEN_Argument;
}

inline Arguments * XLR_Arguments_0 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  Arguments *arguments = args[0].value;
  Argument  *argument  = args[2].value;

  Array_append(arguments, argument, 1);

  return arguments;
}

inline Arguments * XLR_Arguments_1 (Token args[], LRContext *, ErrInfo *, const Allocator *allocator) {
  const Argument  *argument  = args[0].value;

  Arguments *arguments = Array_new(sizeof(Argument), XLR_OBJECT_VAR, allocator);
  Array_append(arguments, argument, 1);

  return arguments;
}

inline Arguments * XLR_Arguments_2 (Token [], LRContext *, ErrInfo *, const Allocator *allocator) {
  return Array_new(sizeof(Argument), XLR_OBJECT_VAR, allocator);;
}

inline ArithExpr * XLR_ArithExpr_0 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

inline ArithExpr * XLR_ArithExpr_1 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

inline ArithExpr * XLR_ArithExpr_2 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[1].value;
}

inline Arith_0_Expr * XLR_Arith_0_Expr_0 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *) {
  const Arith_0_Expr *v_lhs = args[0].value;
  const uint32_t builtin_op = (uint64_t) args[1].value;
  const Arith_0_Expr *v_rhs = args[2].value;

  const Arith_0_Expr *lhs = Array_virt2real(context->var_array, v_lhs);
  const Arith_0_Expr *rhs = Array_virt2real(context->var_array, v_rhs);

  if (lhs->type != rhs->type) {
    errInfo->code = XLR_ERROR_TYPE_MISMATCH;
    errInfo->start = args[0].start;
    errInfo->end = args[2].end;
    return nullptr;
  }
  const LRFunction *func = Array_real_addr(context->func_array,  builtin_op);
  const LRParameter *params = Array_real_addr(func->params, 0);
  if (lhs->type != params[0].type || rhs->type != params[1].type) {
    errInfo->code = XLR_ERROR_TYPE_MISMATCH;
    errInfo->start = args[0].start;
    errInfo->end = args[2].end;
    return nullptr;
  }
  // TODO: generate instruction for the builtin operator, and the result's attributes and value.
  const LRVariable res = { .type = func->restype, .ident = nullptr, .parent = nullptr, .attrs = nullptr, .refer = nullptr };

  Array_append(context->var_array, &res, 1);
  return Array_last_virt(context->var_array);
}

inline Arith_0_Expr * XLR_Arith_0_Expr_1 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *) {
  const uint32_t builtin_op = (uint64_t) args[0].value;
  const Arith_0_Expr *v_rhs = args[1].value;

  const Arith_0_Expr *rhs = Array_virt2real(context->var_array, v_rhs);
  const LRFunction *func = Array_real_addr(context->func_array,  builtin_op);
  const LRParameter *params = Array_real_addr(func->params, 0);
  if (rhs->type != params[0].type) {
    errInfo->code = XLR_ERROR_TYPE_MISMATCH;
    errInfo->start = args[0].start;
    errInfo->end = args[2].end;
    return nullptr;
  }
  // TODO: generate instruction for the builtin operator, and the result's attributes and value.

  const LRVariable res = { .type = rhs->type, .ident = nullptr, .parent = nullptr, .attrs = nullptr, .refer = nullptr };
  Array_append(context->var_array, &res, 1);

  return Array_last_virt(context->var_array);
}

inline Arith_0_Expr * XLR_Arith_0_Expr_2 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

inline Arith_1_Expr * XLR_Arith_1_Expr_0 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  return XLR_Arith_0_Expr_0(args, context, errInfo, allocator);
}

inline Arith_1_Expr * XLR_Arith_1_Expr_1 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  return XLR_Arith_0_Expr_1(args, context, errInfo, allocator);
}

inline Arith_1_Expr * XLR_Arith_1_Expr_2 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

inline Arith_2_Expr * XLR_Arith_2_Expr_0 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  return XLR_Arith_0_Expr_0(args, context, errInfo, allocator);
}

inline Arith_2_Expr * XLR_Arith_2_Expr_1 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  return XLR_Arith_0_Expr_1(args, context, errInfo, allocator);
}

inline Arith_2_Expr * XLR_Arith_2_Expr_2 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

inline Arith_3_Expr * XLR_Arith_3_Expr_0 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  return XLR_Arith_0_Expr_0(args, context, errInfo, allocator);
}

inline Arith_3_Expr * XLR_Arith_3_Expr_1 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  return XLR_Arith_0_Expr_1(args, context, errInfo, allocator);
}

inline Arith_3_Expr * XLR_Arith_3_Expr_2 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

inline Arith_4_Expr * XLR_Arith_4_Expr_0 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  return XLR_Arith_0_Expr_1(args, context, errInfo, allocator);
}

inline Arith_4_Expr * XLR_Arith_4_Expr_1 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

inline AssignExpr * XLR_AssignExpr_0 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *) {
  const REFER(Assignable) v_assignable = args[0].value;
  REFER(ArithExpr) v_expr = args[2].value;

  const ArithExpr *expr = Array_virt2real(context->var_array, v_expr);
  const Assignable *assignable = Array_virt2real(context->var_array, v_assignable);
  const LRVariable *parent = Array_virt2real(context->var_array, assignable->parent);
  if (!parent) {
    const REFER(LRVariable) var = LRContext_get_variable(context, assignable->ident);
    if (expr->type != var->type) {
      errInfo->code = XLR_ERROR_TYPE_MISMATCH;
      errInfo->start = args[0].start;
      errInfo->end = args[2].end;
      return nullptr;
    }
    AVLTree_set(context->curr_block->var_tree, (uint64_t) assignable->ident, v_expr);
  } else {
    const REFER(LRType) parent_type = Array_virt2real(context->type_array, parent->type);
    switch (parent_type->cat) {
      case XLR_CATEGORY_UNION:
      case XLR_CATEGORY_STRUCT: {
        const LRVariable *field = AVLTree_get(parent_type->refer, (uint64_t) assignable->ident);
        if (expr->type != field->type) {
          errInfo->code = XLR_ERROR_TYPE_MISMATCH;
          errInfo->start = args[0].start;
          errInfo->end = args[2].end;
          return nullptr;
        }
        // TODO: attribute check
        AVLTree_set(parent->refer, (uint64_t) assignable->ident, v_expr);
        break;
      }
      case XLR_CATEGORY_ARRAY: {
        if (expr->type != (REFER(LRType)) parent_type->refer) {
          errInfo->code = XLR_ERROR_TYPE_MISMATCH;
          errInfo->start = args[0].start;
          errInfo->end = args[2].end;
          return nullptr;
        }
        // TODO: attribute check
        // TODO: generate instruction for set array element.
      }
      default: ;
    }
  }
  return v_expr;
}

inline AssignExpr * XLR_AssignExpr_1 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *) {
  uint32_t integrate_op = (uint64_t) args[0].value;
  REFER(IntegratedExpr) v_expr = args[1].value;

  const IntegratedExpr *expr = Array_virt2real(context->var_array, v_expr);
  const LRFunction *func = Array_real_addr(context->func_array, integrate_op);
  const LRParameter *param = Array_real_addr(func->params, 0);
  if (expr->type != param->type) {
    errInfo->code = XLR_ERROR_TYPE_MISMATCH;
    errInfo->start = args[0].start;
    errInfo->end = args[1].end;
    return nullptr;
  }
  // TODO: generate instruction for set array element.
  const LRVariable res = { .type = func->restype, .ident = nullptr, .parent = nullptr, .attrs = nullptr, .refer = nullptr };

  Array_append(context->var_array, &res, 1);

  return Array_last_virt(context->var_array);
}

inline AssignExpr * XLR_AssignExpr_2 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *) {
  REFER(IntegratedExpr) v_expr = args[0].value;
  uint32_t integrate_op = (uint64_t) args[1].value;

  const IntegratedExpr *expr = Array_virt2real(context->var_array, v_expr);
  const LRFunction *func = Array_real_addr(context->func_array, integrate_op);
  const LRParameter *param = Array_real_addr(func->params, 0);
  if (expr->type != param->type) {
    errInfo->code = XLR_ERROR_TYPE_MISMATCH;
    errInfo->start = args[0].start;
    errInfo->end = args[1].end;
    return nullptr;
  }
  // TODO: generate instruction for set array element.
  const LRVariable res = { .type = func->restype, .ident = nullptr, .parent = nullptr, .attrs = nullptr, .refer = nullptr };

  Array_append(context->var_array, &res, 1);

  return v_expr;
}

inline Assignable * XLR_Assignable_0 (Token args[], LRContext *context, ErrInfo *errInfo, const Allocator *) {
  REFER(Identifier) ident = args[0].value;

  REFER(LRVariable) var = LRContext_get_variable(context, ident);
  if (!var) {
    errInfo->code = XLR_ERROR_NO_SUCH_VARIABLE;
    errInfo->start = args[0].start;
    errInfo->end = args[0].end;
    return nullptr;
  }

  return var;
}

inline Assignable * XLR_Assignable_1 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

inline Assignable * XLR_Assignable_2 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

inline Assignable * XLR_Assignable_3 (Token args[], LRContext *, ErrInfo *, const Allocator *) {
  return args[0].value;
}

inline AttrDefinition * XLR_AttrDefinition_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline AttrDefinition * XLR_AttrDefinition_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline AttrList * XLR_AttrList_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline AttrList * XLR_AttrList_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline AttrList * XLR_AttrList_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline AttrList * XLR_AttrList_3 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline AttrList * XLR_AttrList_4 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline AttrRenaming * XLR_AttrRenaming_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline AttrRenaming * XLR_AttrRenaming_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Attributed * XLR_Attributed_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline CompExpr * XLR_CompExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline CompExpr * XLR_CompExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline CompExpr * XLR_CompExpr_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline CondExpr * XLR_CondExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline CondExpr * XLR_CondExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline CondStatement * XLR_CondStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline CondStatement * XLR_CondStatement_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline CondStatement * XLR_CondStatement_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline CondStatement * XLR_CondStatement_3 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline ControlStatement * XLR_ControlStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline ControlStatement * XLR_ControlStatement_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline ControlStatement * XLR_ControlStatement_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Declaration * XLR_Declaration_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Declaration * XLR_Declaration_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Declarations * XLR_Declarations_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Declarations * XLR_Declarations_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline EnumDeclaration * XLR_EnumDeclaration_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline EnumDeclaration * XLR_EnumDeclaration_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline EnumItem * XLR_EnumItem_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline EnumItem * XLR_EnumItem_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline EnumItems * XLR_EnumItems_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline EnumItems * XLR_EnumItems_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Evaluable * XLR_Evaluable_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Evaluable * XLR_Evaluable_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Evaluable * XLR_Evaluable_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Evaluable * XLR_Evaluable_3 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline ForCondition * XLR_ForCondition_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline ForStatement * XLR_ForStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline FuncDefinition * XLR_FuncDefinition_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline FunctionCall * XLR_FunctionCall_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline GrammarEntry * XLR_GrammarEntry_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline GrammarEntry * XLR_GrammarEntry_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline GrammarEntry * XLR_GrammarEntry_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline GrammarEntry * XLR_GrammarEntry_3 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline GrammarEntry * XLR_GrammarEntry_4 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline GrammarEntry * XLR_GrammarEntry_5 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline GrammarEntry * XLR_GrammarEntry_6 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

GrammarEntry * XLR_GrammarEntry_EXT (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline IfCondition * XLR_IfCondition_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline IfStatement * XLR_IfStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline InitializerHinter * XLR_InitializerHinter_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline InitializerHinter * XLR_InitializerHinter_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline InitializerHinter * XLR_InitializerHinter_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline InitializerHinter * XLR_InitializerHinter_3 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline IntegratedExpr * XLR_IntegratedExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline IntegratedExpr * XLR_IntegratedExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline IntegratedExpr * XLR_IntegratedExpr_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LRPattern * XLR_Pattern_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LRPattern * XLR_Pattern_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LRToken * XLR_Token_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LRToken * XLR_Token_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LRToken * XLR_Token_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LRToken * XLR_Token_3 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LRTokens * XLR_Tokens_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LRTokens * XLR_Tokens_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LRTokens * XLR_Tokens_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LRTokens * XLR_Tokens_3 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LRTokens * XLR_Tokens_4 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LiteralTexts * XLR_LiteralTexts_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LiteralTexts * XLR_LiteralTexts_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LiteralValue * XLR_LiteralValue_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LiteralValue * XLR_LiteralValue_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline LiteralValue * XLR_LiteralValue_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline OptionalLoopCondExpr * XLR_OptionalLoopCondExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline OptionalLoopCondExpr * XLR_OptionalLoopCondExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline OptionalLoopInitExpr * XLR_OptionalLoopInitExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline OptionalLoopInitExpr * XLR_OptionalLoopInitExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline OptionalLoopInitExpr * XLR_OptionalLoopInitExpr_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline OptionalLoopUpdateExpr * XLR_OptionalLoopUpdateExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline OptionalLoopUpdateExpr * XLR_OptionalLoopUpdateExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Parameter * XLR_Parameter_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Parameter * XLR_Parameter_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Parameter * XLR_Parameter_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Parameter * XLR_Parameter_3 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Parameters * XLR_Parameters_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Parameters * XLR_Parameters_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline RuleDefinition * XLR_RuleDefinition_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline StructuredInitializer * XLR_StructuredInitializer_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline StructuredInitializer * XLR_StructuredInitializer_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Subscribed * XLR_Subscribed_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Subscriber * XLR_Subscriber_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Type * XLR_Type_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Type * XLR_Type_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline TypeDefinition * XLR_TypeDefinition_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline TypeRenaming * XLR_TypeRenaming_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline TypeRenaming * XLR_TypeRenaming_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline TypeRenaming * XLR_TypeRenaming_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline VarList * XLR_VarList_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline VarList * XLR_VarList_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Variable * XLR_Variable_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Variable * XLR_Variable_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Variable * XLR_Variable_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline Variable * XLR_Variable_3 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline VariableInitializer * XLR_VariableInitializer_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline VariableInitializer * XLR_VariableInitializer_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline VariableInitializerList * XLR_VariableInitializerList_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline VariableInitializerList * XLR_VariableInitializerList_1 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline VariableInitializerList * XLR_VariableInitializerList_2 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline VariableInitializerList * XLR_VariableInitializerList_3 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}

inline WhileStatement * XLR_WhileStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *) {
  return nullptr;
}
