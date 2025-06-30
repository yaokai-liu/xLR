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
 * Module Name: grammar
 * Filename: token.c
 * Creator: Yaokai Liu
 * Create Date: 2025-05-29
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "allocator.h"
#include "array.h"
#include "xLR/target.h"
#include "xLR/tokens.h"
#include "xLR/token.h"

const char_t *get_name(uint16_t type) {
  return TOKEN_NAMES[type];
}


#define releaseArrayCase(array, ele)                        \
  case XLR_TOKEN_##array: {                                 \
    Array_reset(token->value, (destruct_t *) release##ele); \
    Array_destroy(token->value);                            \
    break;                                                  \
  }
#define releaseTokenCase(t, r)           \
  case XLR_TOKEN_##t: {                  \
    release##r(token->value, allocator); \
    break;                               \
  }

void releaseToken(Token *token, const Allocator *allocator) {
  switch (token->type) {
    // token array
    releaseArrayCase(Declarations, Declaration)
    releaseArrayCase(ActionStatements, ActionStatement)
    releaseArrayCase(Tokens, LRToken)
    releaseArrayCase(Arguments, Expr)
    releaseArrayCase(EnumItems, EnumItem)
    // token
    releaseTokenCase(ActionBlock, ActionBlock)
    releaseTokenCase(RuleDefinition, RuleDefinition)
    releaseTokenCase(Declaration, Declaration)
    releaseTokenCase(TokenDefinition, TokenDefinition)
    releaseTokenCase(ActionStatement, ActionStatement)
    releaseTokenCase(GrammarEntry, GrammarEntry)
    releaseTokenCase(IfCondition, IfCondition)
    releaseTokenCase(ForStatement, ForStatement)
    releaseTokenCase(Token, LRToken)
    releaseTokenCase(IfStatement, IfStatement)
    releaseTokenCase(WhileStatement, WhileStatement)
    releaseTokenCase(EnumItem, EnumItem)
    releaseTokenCase(CondStatement, CondStatement)
    releaseTokenCase(Pattern, Pattern)
    releaseTokenCase(EnumDeclaration, EnumDeclaration)
    releaseTokenCase(ForCondition, ForCondition)
    releaseTokenCase(Variable, Variable)
    case XLR_TOKEN_CondExpr:
    case XLR_TOKEN_CompExpr:
    case XLR_TOKEN_ArithExpr:
    case XLR_TOKEN_Arith_0_Expr:
    case XLR_TOKEN_Arith_1_Expr:
    case XLR_TOKEN_Arith_2_Expr:
    case XLR_TOKEN_Arith_3_Expr:
    case XLR_TOKEN_Arith_4_Expr:
    case XLR_TOKEN_AssignExpr:
    case XLR_TOKEN_Assignable:
    case XLR_TOKEN_Accessed:
    case XLR_TOKEN_Subscribed:
    case XLR_TOKEN_Evaluable:
    case XLR_TOKEN_IntegratedExpr:
    case XLR_TOKEN_FunctionCall:
    case XLR_TOKEN_OptionalAssignExpr:
    case XLR_TOKEN_OptionalCondExpr: {
      releaseExpr(token->value, allocator);
      break;
    }
    default: {}
  }
}
