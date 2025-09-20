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
 * Filename: enum.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-30
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_GRAMMAR_ENUM_H
#define XLR_GRAMMAR_ENUM_H

#include "xLR/tokens.h"

enum XLR_OP_ENUM {
  XLR_AB_ADD = XLR_MAX_TOTAL_TOKEN + 1,
  XLR_IA_INC,
  XLR_IA_DEC,
  XLR_AB_SUB,
  XLR_AB_MUL,
  XLR_AB_DIV,
  XLR_AB_MOD,
  XLR_AB_OR,
  XLR_AB_AND,
  XLR_AB_XOR,
  XLR_AB_LSH,
  XLR_AB_RSH,
  XLR_AS_INV,
  XLR_CB_AND,
  XLR_CB_OR,
  XLR_CS_NOT,
  XLR_COMP_LT,
  XLR_COMP_LE,
  XLR_COMP_GT,
  XLR_COMP_GE,
  XLR_COMP_EQ,
  XLR_COMP_NE,
  XLR_AS_ID,
  RECURSIVE_OP_MAX = XLR_AS_ID,
};

enum XLR_BUILTIN_FUNCTION_ENUM {
  XLR_FUN_SIZEOF,
};

enum XLR_STRUCT_TYPE_ENUM {
  XLR_STRUCT_BUILTIN,
  XLR_STRUCT_TOKEN,
  XLR_STRUCT_ENUM,
};

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

enum XLR_QUANT_TYPE_ENUM {
  XLR_QUANT_ANY_COUNT,
  XLR_QUANT_LESS_THAN_ONE,
  XLR_QUANT_MORE_THAN_ONE,
};

enum XLR_VAL_LITERAL_TYPE_ENUM {
  XLR_VAL_LITERAL_NULL,

  XLR_VAL_LITERAL_INT,
  XLR_VAL_LITERAL_UINT,
  XLR_VAL_LITERAL_FLOAT,
  XLR_VAL_LITERAL_STRING,
  XLR_VAL_LITERAL_BOOLEAN,

  XLR_VAL_LITERAL_UNINITIALIZED,
};

#endif //XLR_GRAMMAR_ENUM_H
