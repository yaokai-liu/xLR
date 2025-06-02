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
 * Filename: target.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-29
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_GRAMMAR_XLR_TARGET_H
#define XLR_GRAMMAR_XLR_TARGET_H

#include "types.h"
#include "avl-tree.h"

typedef char_t Identifier;

typedef struct GrammarEntry {

} GrammarEntry;

typedef Array ActionStatements;

typedef const void GrammarItem; // REFER(const LRSymbol)
typedef Array GrammarItems; // Array<GrammarItem>
typedef GrammarItems GrammarPattern;

typedef Array Arguments, EnumItems;

typedef struct {
  REFER(Identifier) name;
  LRValue *         value;
} EnumItem;

typedef LRType EnumDeclaration, TokenDefinition;

typedef LRRule RuleDefinition;

typedef struct Expression Expr, CondExpr, SingleCondExpr;
typedef struct Expression ArithExpr, Arith_0_Expr, Arith_1_Expr, Arith_2_Expr, Arith_3_Expr, Arith_4_Expr;
typedef struct Expression AssignExpr, Assignable, Accessed, Subscribed, Subscriber;
typedef struct Expression Evaluable, IntegratedExpr, FunctionCall;
typedef struct Expression OptionalAssignExpr, OptionalCondExpr;

typedef LRVariable Variable;
typedef Array VarList, Declaration, Declarations; // Array<LRVariable>

typedef struct ActionBlock ActionBlock;
struct ActionBlock {
  REFER(ActionBlock) parent;
  AVLTree *var_tree;  // AVLTree<REFER(Identifier), REFER(LRVariable)>
  Array   *var_array; // Array<LRVariable>
  Array   *subblocks;  // Array<INDEX(ActionBlock)>
  Array   *commands;
};

typedef struct ActionStatement ActionStatement, CondStatement;

typedef struct ActionStatement IfStatement, ForStatement, WhileStatement;

struct ActionStatement {
  uint32_t type;
  CondExpr *condition;
  void *action;
};

typedef CondExpr IfCondition, ForCondition;

struct Expression {
  uint32_t type;
  void *lhs;
  void *rhs;
};

ActionBlock *ActionBlock_new(const Allocator *allocator);

void releaseActionBlock(ActionBlock *, const Allocator *);
void releaseRuleDefinition(RuleDefinition *, const Allocator *);
void releaseDeclaration(Declaration *, const Allocator *);
void releaseTokenDefinition(TokenDefinition *, const Allocator *);
void releaseActionStatement(ActionStatement *, const Allocator *);
void releaseGrammarEntry(GrammarEntry *, const Allocator *);
void releaseIfCondition(IfCondition *, const Allocator *);
void releaseForStatement(ForStatement *, const Allocator *);
void releaseGrammarItem(GrammarItem *, const Allocator *);
void releaseIfStatement(IfStatement *, const Allocator *);
void releaseVariable(Variable *, const Allocator *);
void releaseWhileStatement(WhileStatement *, const Allocator *);
void releaseEnumItem(EnumItem *, const Allocator *);
void releaseCondStatement(CondStatement *, const Allocator *);
void releaseGrammarPattern(GrammarPattern *, const Allocator *);
void releaseEnumDeclaration(EnumDeclaration *, const Allocator *);
void releaseForCondition(ForCondition *, const Allocator *);
void releaseExpr(Expr *, const Allocator *);

#endif //XLR_GRAMMAR_XLR_TARGET_H
