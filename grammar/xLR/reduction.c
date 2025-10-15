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

Accessed * XLR_Accessed_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

ActionBlock * XLR_ActionBlock_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

ActionBlock * XLR_ActionBlock_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

ActionStatement * XLR_ActionStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

ActionStatement * XLR_ActionStatement_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

ActionStatement * XLR_ActionStatement_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

ActionStatement * XLR_ActionStatement_3 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

ActionStatements * XLR_ActionStatements_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

ActionStatements * XLR_ActionStatements_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Argument * XLR_Argument_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Argument * XLR_Argument_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arguments * XLR_Arguments_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arguments * XLR_Arguments_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arguments * XLR_Arguments_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

ArithExpr * XLR_ArithExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

ArithExpr * XLR_ArithExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

ArithExpr * XLR_ArithExpr_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arith_0_Expr * XLR_Arith_0_Expr_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arith_0_Expr * XLR_Arith_0_Expr_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arith_0_Expr * XLR_Arith_0_Expr_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arith_1_Expr * XLR_Arith_1_Expr_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arith_1_Expr * XLR_Arith_1_Expr_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arith_1_Expr * XLR_Arith_1_Expr_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arith_2_Expr * XLR_Arith_2_Expr_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arith_2_Expr * XLR_Arith_2_Expr_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arith_2_Expr * XLR_Arith_2_Expr_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arith_3_Expr * XLR_Arith_3_Expr_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arith_3_Expr * XLR_Arith_3_Expr_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arith_3_Expr * XLR_Arith_3_Expr_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arith_4_Expr * XLR_Arith_4_Expr_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Arith_4_Expr * XLR_Arith_4_Expr_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

AssignExpr * XLR_AssignExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

AssignExpr * XLR_AssignExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

AssignExpr * XLR_AssignExpr_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Assignable * XLR_Assignable_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Assignable * XLR_Assignable_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Assignable * XLR_Assignable_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Assignable * XLR_Assignable_3 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

AttrDefinition * XLR_AttrDefinition_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

AttrDefinition * XLR_AttrDefinition_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

AttrList * XLR_AttrList_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

AttrList * XLR_AttrList_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

AttrList * XLR_AttrList_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

AttrList * XLR_AttrList_3 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

AttrList * XLR_AttrList_4 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

AttrRenaming * XLR_AttrRenaming_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

AttrRenaming * XLR_AttrRenaming_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Attributed * XLR_Attributed_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

CompExpr * XLR_CompExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

CompExpr * XLR_CompExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

CompExpr * XLR_CompExpr_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

CondExpr * XLR_CondExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

CondExpr * XLR_CondExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

CondStatement * XLR_CondStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

CondStatement * XLR_CondStatement_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

CondStatement * XLR_CondStatement_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

CondStatement * XLR_CondStatement_3 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Declaration * XLR_Declaration_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Declaration * XLR_Declaration_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Declarations * XLR_Declarations_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Declarations * XLR_Declarations_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

EnumDeclaration * XLR_EnumDeclaration_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

EnumDeclaration * XLR_EnumDeclaration_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

EnumItem * XLR_EnumItem_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

EnumItem * XLR_EnumItem_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

EnumItems * XLR_EnumItems_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

EnumItems * XLR_EnumItems_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Evaluable * XLR_Evaluable_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Evaluable * XLR_Evaluable_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Evaluable * XLR_Evaluable_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Evaluable * XLR_Evaluable_3 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

ForCondition * XLR_ForCondition_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

ForStatement * XLR_ForStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

FuncDefinition * XLR_FuncDefinition_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

FunctionCall * XLR_FunctionCall_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

GrammarEntry * XLR_GrammarEntry_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

GrammarEntry * XLR_GrammarEntry_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

GrammarEntry * XLR_GrammarEntry_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

GrammarEntry * XLR_GrammarEntry_3 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

GrammarEntry * XLR_GrammarEntry_4 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

GrammarEntry * XLR_GrammarEntry_5 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

GrammarEntry * XLR_GrammarEntry_6 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

GrammarEntry * XLR_GrammarEntry_EXT (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

IfCondition * XLR_IfCondition_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

IfStatement * XLR_IfStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

InitializerHinter * XLR_InitializerHinter_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

InitializerHinter * XLR_InitializerHinter_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

InitializerHinter * XLR_InitializerHinter_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

InitializerHinter * XLR_InitializerHinter_3 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

IntegratedExpr * XLR_IntegratedExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

IntegratedExpr * XLR_IntegratedExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

IntegratedExpr * XLR_IntegratedExpr_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LRPattern * XLR_Pattern_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LRPattern * XLR_Pattern_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LRToken * XLR_Token_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LRToken * XLR_Token_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LRToken * XLR_Token_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LRToken * XLR_Token_3 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LRTokens * XLR_Tokens_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LRTokens * XLR_Tokens_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LRTokens * XLR_Tokens_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LRTokens * XLR_Tokens_3 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LRTokens * XLR_Tokens_4 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LiteralTexts * XLR_LiteralTexts_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LiteralTexts * XLR_LiteralTexts_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LiteralValue * XLR_LiteralValue_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LiteralValue * XLR_LiteralValue_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

LiteralValue * XLR_LiteralValue_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

OptionalLoopCondExpr * XLR_OptionalLoopCondExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

OptionalLoopCondExpr * XLR_OptionalLoopCondExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

OptionalLoopInitExpr * XLR_OptionalLoopInitExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

OptionalLoopInitExpr * XLR_OptionalLoopInitExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

OptionalLoopInitExpr * XLR_OptionalLoopInitExpr_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

OptionalLoopUpdateExpr * XLR_OptionalLoopUpdateExpr_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

OptionalLoopUpdateExpr * XLR_OptionalLoopUpdateExpr_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Parameter * XLR_Parameter_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Parameter * XLR_Parameter_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Parameter * XLR_Parameter_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Parameter * XLR_Parameter_3 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Parameters * XLR_Parameters_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Parameters * XLR_Parameters_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

RuleDefinition * XLR_RuleDefinition_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

StructuredInitializer * XLR_StructuredInitializer_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

StructuredInitializer * XLR_StructuredInitializer_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Subscribed * XLR_Subscribed_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Subscriber * XLR_Subscriber_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Type * XLR_Type_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Type * XLR_Type_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

TypeDefinition * XLR_TypeDefinition_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

TypeRenaming * XLR_TypeRenaming_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

TypeRenaming * XLR_TypeRenaming_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

TypeRenaming * XLR_TypeRenaming_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

VarList * XLR_VarList_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

VarList * XLR_VarList_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Variable * XLR_Variable_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Variable * XLR_Variable_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Variable * XLR_Variable_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

Variable * XLR_Variable_3 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

VariableInitializer * XLR_VariableInitializer_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

VariableInitializer * XLR_VariableInitializer_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

VariableInitializerList * XLR_VariableInitializerList_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

VariableInitializerList * XLR_VariableInitializerList_1 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

VariableInitializerList * XLR_VariableInitializerList_2 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

VariableInitializerList * XLR_VariableInitializerList_3 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}

WhileStatement * XLR_WhileStatement_0 (Token [], LRContext *, ErrInfo *, const Allocator *){
  return nullptr;
}
