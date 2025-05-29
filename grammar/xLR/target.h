/* License
 *
 * ${PROJ_DESCRIPTION}
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
 * Filename: target.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-29
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_GRAMMAR_XLR_TARGET_H
#define XLR_GRAMMAR_XLR_TARGET_H

#include "array.h"

typedef void GrammarEntry;

typedef Array Declarations, GrammarItems, ActionStatements, Arguments;
typedef ActionStatements ActionBlock, GrammarAction;
typedef GrammarItems GrammarPattern;

typedef struct TokenDefinition {

} TokenDefinition;

typedef struct RuleDefinition {

} RuleDefinition;

typedef struct ActionStatement {

} ActionStatement, CondStatement;
typedef struct ActionStatement IfStatement, ForStatement, WhileStatement;

typedef struct Condition {

} IfCondition, ForCondition;

typedef struct Declaration {

} Declaration;

typedef struct DecItem {

} DecItem;

typedef struct GrammarItem {

} GrammarItem;


struct Expression {
  uint32_t type;
  void *lhs;
  void *rhs;
};

typedef struct Expression CondExpr, AndCondExpr, SingleCondExpr;
typedef struct Expression ArithExpr, Arith_0_Expr, Arith_1_Expr, Arith_2_Expr, Arith_3_Expr, Arith_4_Expr;
typedef struct Expression AssignExpr, Assignable, Accessed, Subscribed;
typedef struct Expression Evaluable, IntegratedExpr, FunctionCall;
typedef struct Expression OptionalAssignExpr, OptionalCondExpr;


#endif //XLR_GRAMMAR_XLR_TARGET_H
