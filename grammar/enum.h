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

#include "generated/tokens.gen.h"

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

enum XLR_KEYWORD_BIT_ENUM: uint64_t {
  XLR_KW_IF        = 0b00000000'00000001LLU,
  XLR_KW_FOR       = 0b00000000'00000010LLU,
  XLR_KW_ELSE      = 0b00000000'00000100LLU,
  XLR_KW_WHILE     = 0b00000000'00001000LLU,

  XLR_KW_ENUM      = 0b00000001'00000000LLU,
  XLR_KW_UNION     = 0b00000010'00000000LLU,
  XLR_KW_STRUCT    = 0b00000100'00000000LLU,
  XLR_KW_TYPEDEF   = 0b00001000'00000000LLU,
  XLR_KW_ATTRIBUTE = 0b00010000'00000000LLU,

  XLR_KW_AS_IDENT  = 0b00011111'00000000LLU
};

#endif //XLR_GRAMMAR_ENUM_H
