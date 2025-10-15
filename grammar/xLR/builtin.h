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
 * MERCHANTARITH_BINILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *
 * Project Name: xLR
 * Module Name: grammar/xLR
 * Filename: builtin.h
 * Creator: Yaokai Liu
 * Create Date: 2025-09-19
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_GRAMMAR_BUILTIN_H
#define XLR_GRAMMAR_BUILTIN_H

#include "category.h"

enum XLR_BUILTIN_TYPE_ENUM {
  XLR_BUILTIN_TYPE_I32,
  XLR_BUILTIN_TYPE_U32,
  XLR_BUILTIN_TYPE_I64,
  XLR_BUILTIN_TYPE_U64,
  XLR_BUILTIN_TYPE_I128,
  XLR_BUILTIN_TYPE_U128,
  XLR_BUILTIN_TYPE_I256,
  XLR_BUILTIN_TYPE_U256,
  XLR_BUILTIN_TYPE_F32,
  XLR_BUILTIN_TYPE_F64,
  XLR_BUILTIN_TYPE_F128,
  XLR_BUILTIN_TYPE_F256,
  XLR_BUILTIN_TYPE_STR,

  XLR_BUILTIN_TYPE_TOKEN,
  XLR_BUILTIN_TYPE_TOKENS,
  XLR_BUILTIN_TYPE_COMPILER,
};

enum XLR_BUILTIN_FUNC_ENUM {
  XLR_BUILTIN_FUNC_ACCESS,
  XLR_BUILTIN_FUNC_ATTRIBUTE,
  XLR_BUILTIN_FUNC_SUBSCRIBE,
  XLR_BUILTIN_FUNC_INTEGRATE_INC,
  XLR_BUILTIN_FUNC_INTEGRATE_DEC,
  XLR_BUILTIN_FUNC_ARITH_BIN_ADD,
  XLR_BUILTIN_FUNC_ARITH_BIN_SUB,
  XLR_BUILTIN_FUNC_ARITH_BIN_MUL,
  XLR_BUILTIN_FUNC_ARITH_BIN_DIV,
  XLR_BUILTIN_FUNC_ARITH_BIN_MOD,
  XLR_BUILTIN_FUNC_BIT_BIN_OR,
  XLR_BUILTIN_FUNC_BIT_BIN_AND,
  XLR_BUILTIN_FUNC_BIT_BIN_XOR,
  XLR_BUILTIN_FUNC_BIT_BIN_LSH,
  XLR_BUILTIN_FUNC_BIT_BIN_RSH,
  XLR_BUILTIN_FUNC_BIT_SIN_INV,
  XLR_BUILTIN_FUNC_BOOL_BIN_AND,
  XLR_BUILTIN_FUNC_BOOL_BIN_OR,
  XLR_BUILTIN_FUNC_BOOL_SIN_NOT,
  XLR_BUILTIN_FUNC_COMPARE_LT,
  XLR_BUILTIN_FUNC_COMPARE_LE,
  XLR_BUILTIN_FUNC_COMPARE_GT,
  XLR_BUILTIN_FUNC_COMPARE_GE,
  XLR_BUILTIN_FUNC_COMPARE_EQ,
  XLR_BUILTIN_FUNC_COMPARE_NE,
};

#define BUILTIN_VAR_COUNT       2
extern const LRVariable BUILTIN_VARS[BUILTIN_VAR_COUNT];
#define BUILTIN_TYPE_COUNT      16
extern const LRType     BUILTIN_TYPES[BUILTIN_TYPE_COUNT];
#define BUILTIN_FUNC_COUNT      25
extern const LRFunction BUILTIN_FUNCS[BUILTIN_FUNC_COUNT];


#endif //XLR_GRAMMAR_BUILTIN_H