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
 * Filename: string_t.h
 * Creator: Yaokai Liu
 * Create Date: 2024-12-05
 * Copyright (c) 2024 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_STRING_T_H
#define XLR_STRING_T_H

#include "xLR/char_t.h"
#include <stdint.h>

uint32_t stridx_o(char_t chr, const char_t *str);

uint32_t strcmp_o(const char_t *str1, const char_t *str2);

#endif  // XLR_STRING_T_H
