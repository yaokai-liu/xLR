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

const LRType BUILTIN_TYPES[] = {
    [XLR_BUILTIN_TYPE_I32 ] = {.type = XLR_STRUCT_BUILTIN, .size = 0x04, .name = ((void *)  1), .refer = nullptr},
    [XLR_BUILTIN_TYPE_U32 ] = {.type = XLR_STRUCT_BUILTIN, .size = 0x04, .name = ((void *)  5), .refer = nullptr},
    [XLR_BUILTIN_TYPE_I64 ] = {.type = XLR_STRUCT_BUILTIN, .size = 0x08, .name = ((void *)  9), .refer = nullptr},
    [XLR_BUILTIN_TYPE_U64 ] = {.type = XLR_STRUCT_BUILTIN, .size = 0x08, .name = ((void *) 13), .refer = nullptr},
    [XLR_BUILTIN_TYPE_I128] = {.type = XLR_STRUCT_BUILTIN, .size = 0x10, .name = ((void *) 17), .refer = nullptr},
    [XLR_BUILTIN_TYPE_U128] = {.type = XLR_STRUCT_BUILTIN, .size = 0x10, .name = ((void *) 22), .refer = nullptr},
    [XLR_BUILTIN_TYPE_I256] = {.type = XLR_STRUCT_BUILTIN, .size = 0x20, .name = ((void *) 27), .refer = nullptr},
    [XLR_BUILTIN_TYPE_U256] = {.type = XLR_STRUCT_BUILTIN, .size = 0x20, .name = ((void *) 32), .refer = nullptr},
    [XLR_BUILTIN_TYPE_F32 ] = {.type = XLR_STRUCT_BUILTIN, .size = 0x04, .name = ((void *) 37), .refer = nullptr},
    [XLR_BUILTIN_TYPE_F64 ] = {.type = XLR_STRUCT_BUILTIN, .size = 0x08, .name = ((void *) 41), .refer = nullptr},
    [XLR_BUILTIN_TYPE_F128] = {.type = XLR_STRUCT_BUILTIN, .size = 0x10, .name = ((void *) 45), .refer = nullptr},
    [XLR_BUILTIN_TYPE_F256] = {.type = XLR_STRUCT_BUILTIN, .size = 0x20, .name = ((void *) 50), .refer = nullptr},
};
