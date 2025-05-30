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

enum XLR_OP_ENUM {
  XLR_AB_ADD = XLR_MAX_TOTAL_TOKEN + 1,
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
  XLR_CB_LT,
  XLR_CB_LE,
  XLR_CB_GT,
  XLR_CB_GE,
  XLR_CB_EQ,
  XLR_CB_NE,
  XLR_AS_ID,
  RECURSIVE_OP_MAX = XLR_AS_ID,
};

enum XLR_BUILTIN_FUNCTION_ENUM {
  XLR_BF_SIZEOF,
};

enum XLR_BUILTIN_TYPE_ENUM {
  XLR_BT_U64, XLR_BT_U32, XLR_BT_U16, XLR_BT_U8,
  XLR_BT_I64, XLR_BT_I32, XLR_BT_I16, XLR_BT_I8,
};

enum XLR_QUANT_TYPE_ENUM {
  XLR_QUANT_ANY_COUNT,
  XLR_QUANT_LESS_THAN_ONE,
  XLR_QUANT_MORE_THAN_ONE,
};

#endif //XLR_GRAMMAR_ENUM_H
