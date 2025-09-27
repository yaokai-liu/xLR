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
 * Filename: target.c
 * Creator: Yaokai Liu
 * Create Date: 2025-05-30
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "target.h"
#include "generated/tokens.gen.h"

ActionBlock *ActionBlock_new(const Allocator *allocator) {
  ActionBlock *block = allocator->calloc(1, sizeof(ActionBlock));
  block->var_tree = AVLTree_new(allocator, nullptr);
  block->var_array = Array_new(sizeof(Variable), XLR_TOKEN_Variable, allocator);
  block->subblocks = Array_new(sizeof(INDEX(ActionBlock)), XLR_TOKEN_ActionBlock, allocator);
  return block;
}

void releaseActionBlock(ActionBlock *, const Allocator *) {

}

void releaseRuleDefinition(RuleDefinition *, const Allocator *) {

}

void releaseDeclaration(Declaration *, const Allocator *) {

}

void releaseTokenDefinition(TokenDefinition *, const Allocator *) {

}

void releaseActionStatement(ActionStatement *, const Allocator *) {

}

void releaseGrammarEntry(GrammarEntry *, const Allocator *) {

}

void releaseIfCondition(IfCondition *, const Allocator *) {

}

void releaseForStatement(ForStatement *, const Allocator *) {

}

void releaseLRToken(LRToken *, const Allocator *) {

}

void releaseIfStatement(IfStatement *, const Allocator *) {

}

void releaseVariable(Variable *, const Allocator *) {

}

void releaseWhileStatement(WhileStatement *, const Allocator *) {

}

void releaseEnumItem(EnumItem *, const Allocator *) {

}

void releaseCondStatement(CondStatement *, const Allocator *) {

}

void releasePattern(LRPattern *, const Allocator *) {

}

void releaseEnumDeclaration(EnumDeclaration *, const Allocator *) {

}

void releaseForCondition(ForCondition *, const Allocator *) {

}

void releaseExpr(Expr *, const Allocator *) {

}

