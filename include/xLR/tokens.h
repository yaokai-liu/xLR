/**
 * License
 *
 * xLR - Dynamic LR(1) Grammar Parser
 * Copyright (C) 2024 Yaokai Liu
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
 * Project Name: xLR
 * Module Name: grammar
 * Filename: tokens.gen.h
 * Copyright (c) 2024 Yaokai Liu. All rights reserved.
 **/


#ifndef XLR_TOKENS_H
#define XLR_TOKENS_H

#include "xLR/char_t.h"
#include <stdint.h>

enum TOKEN_TYPE_ENUM: uint32_t {
  XLR_TOKEN_ARITH_0_BIN_OP = 1,
  XLR_TOKEN_ARITH_0_SIN_OP = 2,
  XLR_TOKEN_ARITH_1_BIN_OP = 3,
  XLR_TOKEN_ARITH_1_SIN_OP = 4,
  XLR_TOKEN_ARITH_2_BIN_OP = 5,
  XLR_TOKEN_ARITH_2_SIN_OP = 6,
  XLR_TOKEN_ARITH_3_BIN_OP = 7,
  XLR_TOKEN_ARITH_3_SIN_OP = 8,
  XLR_TOKEN_ARITH_4_SIN_OP = 9,
  XLR_TOKEN_ASSIGNER = 10,
  XLR_TOKEN_Accessed = 11,
  XLR_TOKEN_ActionBlock = 12,
  XLR_TOKEN_ActionStatement = 13,
  XLR_TOKEN_ActionStatements = 14,
  XLR_TOKEN_AndCondExpr = 15,
  XLR_TOKEN_Arguments = 16,
  XLR_TOKEN_ArithExpr = 17,
  XLR_TOKEN_Arith_0_Expr = 18,
  XLR_TOKEN_Arith_1_Expr = 19,
  XLR_TOKEN_Arith_2_Expr = 20,
  XLR_TOKEN_Arith_3_Expr = 21,
  XLR_TOKEN_Arith_4_Expr = 22,
  XLR_TOKEN_AssignExpr = 23,
  XLR_TOKEN_Assignable = 24,
  XLR_TOKEN_BOOL_AND = 25,
  XLR_TOKEN_BOOL_NOT = 26,
  XLR_TOKEN_BOOL_OR = 27,
  XLR_TOKEN_BUILTIN_TYPE = 28,
  XLR_TOKEN_BUILTIN_VARIABLE = 29,
  XLR_TOKEN_COMMA = 30,
  XLR_TOKEN_COND_BIN_OP = 31,
  XLR_TOKEN_COND_SIN_OP = 32,
  XLR_TOKEN_CondExpr = 33,
  XLR_TOKEN_CondStatement = 34,
  XLR_TOKEN_DOT = 35,
  XLR_TOKEN_DecItem = 36,
  XLR_TOKEN_Declaration = 37,
  XLR_TOKEN_Declarations = 38,
  XLR_TOKEN_ELSE = 39,
  XLR_TOKEN_Evaluable = 40,
  XLR_TOKEN_FOR = 41,
  XLR_TOKEN_ForCondition = 42,
  XLR_TOKEN_ForStatement = 43,
  XLR_TOKEN_FunctionCall = 44,
  XLR_TOKEN_GrammarAction = 45,
  XLR_TOKEN_GrammarEntry = 46,
  XLR_TOKEN_GrammarItem = 47,
  XLR_TOKEN_GrammarItems = 48,
  XLR_TOKEN_GrammarPattern = 49,
  XLR_TOKEN_IDENTIFIER = 50,
  XLR_TOKEN_IF = 51,
  XLR_TOKEN_INTEGRATED_AHEAD_OP = 52,
  XLR_TOKEN_INTEGRATED_FOLLOW_OP = 53,
  XLR_TOKEN_IfCondition = 54,
  XLR_TOKEN_IfStatement = 55,
  XLR_TOKEN_IntegratedExpr = 56,
  XLR_TOKEN_LEFT_BRACKET = 57,
  XLR_TOKEN_LEFT_PARENTHESIS = 58,
  XLR_TOKEN_LEFT_SQUARE_BRACKET = 59,
  XLR_TOKEN_NUMBER = 60,
  XLR_TOKEN_OptionalAssignExpr = 61,
  XLR_TOKEN_OptionalCondExpr = 62,
  XLR_TOKEN_QUANTIFIER = 63,
  XLR_TOKEN_RIGHT_BRACKET = 64,
  XLR_TOKEN_RIGHT_PARENTHESIS = 65,
  XLR_TOKEN_RIGHT_SQUARE_BRACKET = 66,
  XLR_TOKEN_RuleDefinition = 67,
  XLR_TOKEN_SEMICOLON = 68,
  XLR_TOKEN_SingleCondExpr = 69,
  XLR_TOKEN_Subscribed = 70,
  XLR_TOKEN_TERMINATOR = 71,
  XLR_TOKEN_TOKEN = 72,
  XLR_TOKEN_TokenDefinition = 73,
  XLR_TOKEN_WHILE = 74,
  XLR_TOKEN_WhileStatement = 75,
  MAX_REAL_TOKEN = 76,
  enum_PLACE_HOLDER,
  MAX_TOTAL_TOKEN,
  enum_BAD_TOKEN = UINT32_MAX
};
extern const char_t *TOKEN_NAMES[];
#endif  // XLR_TOKENS_H
