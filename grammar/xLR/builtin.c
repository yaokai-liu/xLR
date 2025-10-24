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
 * Filename: builtin.c
 * Creator: Yaokai Liu
 * Create Date: 2025-09-19
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "builtin.h"
#include "enum.h"

const LRType BUILTIN_TYPES[BUILTIN_TYPE_COUNT] = {
    // basic types
    [XLR_BUILTIN_TYPE_I32 ] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x04, .ident = (Identifier *) "i32",    .refer = nullptr},
    [XLR_BUILTIN_TYPE_U32 ] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x04, .ident = (Identifier *) "u32",    .refer = nullptr},
    [XLR_BUILTIN_TYPE_I64 ] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x08, .ident = (Identifier *) "i64",    .refer = nullptr},
    [XLR_BUILTIN_TYPE_U64 ] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x08, .ident = (Identifier *) "u64",    .refer = nullptr},
    [XLR_BUILTIN_TYPE_I128] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x10, .ident = (Identifier *) "i128",   .refer = nullptr},
    [XLR_BUILTIN_TYPE_U128] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x10, .ident = (Identifier *) "u128",   .refer = nullptr},
    [XLR_BUILTIN_TYPE_I256] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x20, .ident = (Identifier *) "i256",   .refer = nullptr},
    [XLR_BUILTIN_TYPE_U256] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x20, .ident = (Identifier *) "u256",   .refer = nullptr},
    [XLR_BUILTIN_TYPE_F32 ] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x04, .ident = (Identifier *) "f32",    .refer = nullptr},
    [XLR_BUILTIN_TYPE_F64 ] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x08, .ident = (Identifier *) "f64",    .refer = nullptr},
    [XLR_BUILTIN_TYPE_F128] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x10, .ident = (Identifier *) "f128",   .refer = nullptr},
    [XLR_BUILTIN_TYPE_F256] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x20, .ident = (Identifier *) "f256",   .refer = nullptr},
    [XLR_BUILTIN_TYPE_STR ] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x00, .ident = (Identifier *) "string", .refer = nullptr},

    // hidden types
    [XLR_BUILTIN_TYPE_TOKEN   ] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x00, .ident = (Identifier *) "__BUILTIN__::TOKEN",    .refer = nullptr},
    [XLR_BUILTIN_TYPE_TOKENS  ] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x00, .ident = (Identifier *) "__BUILTIN__::TOKENS",   .refer = nullptr},
    [XLR_BUILTIN_TYPE_COMPILER] = {.cat = XLR_CATEGORY_BUILTIN, .size = 0x00, .ident = (Identifier *) "__BUILTIN__::COMPILER", .refer = nullptr},
};

const LRVariable BUILTIN_VARS[BUILTIN_VAR_COUNT] = {
    {.type = (REFER(LRType)) XLR_BUILTIN_TYPE_COMPILER, .ident = (Identifier *) "$COMPILER"},
    {.type = (REFER(LRType)) XLR_BUILTIN_TYPE_TOKENS, .ident = (Identifier *) "$TOKENS"},
};

const LRFunction BUILTIN_FUNCS[BUILTIN_FUNC_COUNT] = {
    [XLR_BUILTIN_FUNC_ATTRS_GET]      = {.ident = (Identifier *) "__BUILTIN__::ATTRS_GET",     .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_ATTRS_SET]      = {.ident = (Identifier *) "__BUILTIN__::ATTRS_SET",     .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_ARRAY_GET]      = {.ident = (Identifier *) "__BUILTIN__::ARRAY_GET",     .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_ARRAY_SET]      = {.ident = (Identifier *) "__BUILTIN__::ARRAY_SET",     .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_INTEGRATE_INC]  = {.ident = (Identifier *) "__BUILTIN__::INTEGRATE_INC", .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_INTEGRATE_DEC]  = {.ident = (Identifier *) "__BUILTIN__::INTEGRATE_DEC", .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_ARITH_BIN_ADD]  = {.ident = (Identifier *) "__BUILTIN__::ARITH_BIN_ADD", .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_ARITH_BIN_SUB]  = {.ident = (Identifier *) "__BUILTIN__::ARITH_BIN_SUB", .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_ARITH_BIN_MUL]  = {.ident = (Identifier *) "__BUILTIN__::ARITH_BIN_MUL", .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_ARITH_BIN_DIV]  = {.ident = (Identifier *) "__BUILTIN__::ARITH_BIN_DIV", .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_ARITH_BIN_MOD]  = {.ident = (Identifier *) "__BUILTIN__::ARITH_BIN_MOD", .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_BIT_BIN_OR]     = {.ident = (Identifier *) "__BUILTIN__::BIT_BIN_OR",    .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_BIT_BIN_AND]    = {.ident = (Identifier *) "__BUILTIN__::BIT_BIN_AND",   .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_BIT_BIN_XOR]    = {.ident = (Identifier *) "__BUILTIN__::BIT_BIN_XOR",   .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_BIT_BIN_LSH]    = {.ident = (Identifier *) "__BUILTIN__::BIT_BIN_LSH",   .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_BIT_BIN_RSH]    = {.ident = (Identifier *) "__BUILTIN__::BIT_BIN_RSH",   .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_BIT_SIN_INV]    = {.ident = (Identifier *) "__BUILTIN__::BIT_SIN_INV",   .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_BOOL_BIN_AND]   = {.ident = (Identifier *) "__BUILTIN__::BOOL_BIN_AND",  .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_BOOL_BIN_OR]    = {.ident = (Identifier *) "__BUILTIN__::BOOL_BIN_OR",   .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_BOOL_SIN_NOT]   = {.ident = (Identifier *) "__BUILTIN__::BOOL_SIN_NOT",  .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_COMPARE_LT]     = {.ident = (Identifier *) "__BUILTIN__::COMPARE_LT",    .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_COMPARE_LE]     = {.ident = (Identifier *) "__BUILTIN__::COMPARE_LE",    .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_COMPARE_GT]     = {.ident = (Identifier *) "__BUILTIN__::COMPARE_GT",    .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_COMPARE_GE]     = {.ident = (Identifier *) "__BUILTIN__::COMPARE_GE",    .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_COMPARE_EQ]     = {.ident = (Identifier *) "__BUILTIN__::COMPARE_EQ",    .restype = nullptr, .params = nullptr},
    [XLR_BUILTIN_FUNC_COMPARE_NE]     = {.ident = (Identifier *) "__BUILTIN__::COMPARE_NE",    .restype = nullptr, .params = nullptr},
};
