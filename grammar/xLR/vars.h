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
 * Filename: vars.h
 * Creator: Yaokai Liu
 * Create Date: 2025-09-19
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_VARS_H
#define XLR_VARS_H

#include "types.h"

#define BUILTIN_VAR_COUNT   2
#define BUILTIN_VAR_NAMES   "CONTEXT\0TOKENS\0"

extern const LRVariable BUILTIN_VARS[BUILTIN_VAR_COUNT];

#endif //XLR_VARS_H