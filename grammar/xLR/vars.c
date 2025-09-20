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
 * Filename: vars.c
 * Creator: Yaokai Liu
 * Create Date: 2025-09-19
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "vars.h"

#include "enum.h"

const LRVariable BUILTIN_VARS[BUILTIN_VAR_COUNT] = {
    {.type = (REFER(LRType)) XLR_BUILTIN_TYPE_COMPILER, .name = "COMPILER", .count = nullptr, .value = nullptr},
    {.type = (REFER(LRType)) XLR_BUILTIN_TYPE_TOKENS, .name = "TOKENS", .count = nullptr, .value = nullptr},
};
