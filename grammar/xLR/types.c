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
 * Filename: types.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-16
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "types.h"
#include "enum.h"

const LRType BUILTIN_TYPES[BUILTIN_TYPE_COUNT] = {
    [XLR_BUILTIN_TYPE_I32 ] = {.type = XLR_STRUCT_BUILTIN, .size = 0x04, .name = "i32", .refer = nullptr},
    [XLR_BUILTIN_TYPE_U32 ] = {.type = XLR_STRUCT_BUILTIN, .size = 0x04, .name = "u32", .refer = nullptr},
    [XLR_BUILTIN_TYPE_I64 ] = {.type = XLR_STRUCT_BUILTIN, .size = 0x08, .name = "i64", .refer = nullptr},
    [XLR_BUILTIN_TYPE_U64 ] = {.type = XLR_STRUCT_BUILTIN, .size = 0x08, .name = "u64", .refer = nullptr},
    [XLR_BUILTIN_TYPE_I128] = {.type = XLR_STRUCT_BUILTIN, .size = 0x10, .name = "i128", .refer = nullptr},
    [XLR_BUILTIN_TYPE_U128] = {.type = XLR_STRUCT_BUILTIN, .size = 0x10, .name = "u128", .refer = nullptr},
    [XLR_BUILTIN_TYPE_I256] = {.type = XLR_STRUCT_BUILTIN, .size = 0x20, .name = "i256", .refer = nullptr},
    [XLR_BUILTIN_TYPE_U256] = {.type = XLR_STRUCT_BUILTIN, .size = 0x20, .name = "u256", .refer = nullptr},
    [XLR_BUILTIN_TYPE_F32 ] = {.type = XLR_STRUCT_BUILTIN, .size = 0x04, .name = "f32", .refer = nullptr},
    [XLR_BUILTIN_TYPE_F64 ] = {.type = XLR_STRUCT_BUILTIN, .size = 0x08, .name = "f64", .refer = nullptr},
    [XLR_BUILTIN_TYPE_F128] = {.type = XLR_STRUCT_BUILTIN, .size = 0x10, .name = "f128", .refer = nullptr},
    [XLR_BUILTIN_TYPE_F256] = {.type = XLR_STRUCT_BUILTIN, .size = 0x20, .name = "f256", .refer = nullptr},
    [XLR_BUILTIN_TYPE_STR ] = {.type = XLR_STRUCT_BUILTIN, .size = 0x00, .name = "string", .refer = nullptr},

    [XLR_BUILTIN_TYPE_TOKEN   ] = {.type = XLR_STRUCT_BUILTIN, .size = 0x00, .name = "__TOKEN__", .refer = nullptr},
    [XLR_BUILTIN_TYPE_TOKENS  ] = {.type = XLR_STRUCT_BUILTIN, .size = 0x00, .name = "__TOKENS__", .refer = nullptr},
    [XLR_BUILTIN_TYPE_COMPILER] = {.type = XLR_STRUCT_BUILTIN, .size = 0x00, .name = "__COMPILER__", .refer = nullptr},
};
