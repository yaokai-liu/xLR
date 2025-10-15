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

#include "category.h"
#include "avl-tree.h"

typedef struct GrammarEntry {

} GrammarEntry;

typedef Array ActionStatements;

typedef const void LRToken; // REFER(const LRSymbol)
typedef Array LRTokens; // Array<Token>
typedef struct LRPattern {
  uint16_t  cost;
  LRTokens *tokens;
} LRPattern;

typedef struct Argument {

} Argument;
typedef Array Arguments; // Array<Argument>

typedef struct {
  REFER(Identifier) ident;
  LRValue *     value;
} EnumItem;

typedef Array EnumItems; // Array<EnumItem>

typedef LRType EnumDeclaration, TypeDefinition, Type, TypeRenaming;

typedef struct AttrDefinition {

} AttrDefinition, AttrRenaming;

typedef LRRule RuleDefinition;
typedef LRFunction FuncDefinition;

typedef LRVariable Expression;

typedef Expression Expr, CondExpr, CompExpr;
typedef Expression ArithExpr, Arith_0_Expr, Arith_1_Expr, Arith_2_Expr, Arith_3_Expr, Arith_4_Expr;
typedef Expression AssignExpr, Assignable, Accessed, Attributed, Subscribed, Subscriber;
typedef Expression OptionalLoopInitExpr, OptionalLoopUpdateExpr, OptionalLoopCondExpr;
typedef Expression Evaluable, IntegratedExpr, FunctionCall;

typedef AssignExpr ArrayInitExpr;

typedef LRVariable Variable;
typedef Array VarList, Declaration, Declarations; // Array<LRVariable>
typedef Array ArrayInitExprList; // Array<ArithExpr>

typedef Array AttrList; // Array<LRAttribute>

typedef LRParameter Parameter;
typedef Array   Parameters; // Array<Parameter>

typedef struct InitializerHinter {
  REFER(LRVariable)     habitat;
  uint32_t              offset;
  // count of elements if is an array
  uint32_t              count;
  REFER(LRType)         type;
} InitializerHinter;

typedef struct VariableInitializer {

} VariableInitializer;
typedef VariableInitializer StructuredInitializer;
typedef Array VariableInitializerList;

typedef struct ActionBlock ActionBlock;
struct ActionBlock {
  REFER(ActionBlock) parent;
  AVLTree *var_tree;  // AVLTree<REFER(Identifier), REFER(Array<LRVariable>)>
  Array   *var_array; // Array<Array<LRVariable>>
  Array   *subblocks;  // Array<INDEX(ActionBlock)>
  Array   *commands;
};

typedef struct ActionStatement {
  uint32_t type;
  CondExpr *condition;
  void *action;
} ActionStatement;

typedef ActionStatement IfStatement, ForStatement, WhileStatement, CondStatement;

typedef CondExpr IfCondition, ForCondition;

typedef LRValue LiteralValue, LiteralTexts;

typedef struct WrapperedText {
  uint16_t n_pred;
  uint16_t n_succ;
  uint32_t length;
  char_t * content;
} WrapperedText;

ActionBlock *ActionBlock_new(const Allocator *allocator);
#define AttrList_new(allocator) Array_new(sizeof(LRAttribute), XLR_TOKEN_ATTRIBUTE, allocator)

void releaseActionBlock(ActionBlock *, const Allocator *);
void releaseRuleDefinition(RuleDefinition *, const Allocator *);
void releaseDeclaration(Declaration *, const Allocator *);
void releaseTypeDefinition(TypeDefinition *, const Allocator *);
void releaseActionStatement(ActionStatement *, const Allocator *);
void releaseGrammarEntry(GrammarEntry *, const Allocator *);
void releaseIfCondition(IfCondition *, const Allocator *);
void releaseForStatement(ForStatement *, const Allocator *);
void releaseLRToken(LRToken *, const Allocator *);
void releaseIfStatement(IfStatement *, const Allocator *);
void releaseVariable(Variable *, const Allocator *);
void releaseWhileStatement(WhileStatement *, const Allocator *);
void releaseEnumItem(EnumItem *, const Allocator *);
void releaseCondStatement(CondStatement *, const Allocator *);
void releasePattern(LRPattern *, const Allocator *);
void releaseEnumDeclaration(EnumDeclaration *, const Allocator *);
void releaseForCondition(ForCondition *, const Allocator *);
void releaseExpr(Expr *, const Allocator *);

#endif //XLR_GRAMMAR_XLR_TARGET_H
