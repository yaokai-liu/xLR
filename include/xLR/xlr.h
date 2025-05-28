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
 * Module Name: include/xLR
 * Filename: parse.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-27
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_INCLUDE_XLR_PARSE_H
#define XLR_INCLUDE_XLR_PARSE_H

#include <stdint.h>
#include "allocator.h"
#include "regex/char_t.h"
#include "xLR/error.h"
typedef struct LRContext LRContext;
typedef struct LRAction LRAction;
typedef struct LRSymbol LRSymbol;
typedef struct LRState LRState;
typedef struct LRItem LRItem;
typedef struct LRRule LRRule;
#define INDEX(o) uint32_t

typedef INDEX(LRSymbol) trans_t(INDEX(LRSymbol));

void LRContext_destroy(LRContext *context);
LRContext *LRContext_new(const Allocator *allocator);
uint32_t LRContext_set_rule(LRContext *context, INDEX(LRRule) i_rule, uint64_t enable_flag);
INDEX(LRRule) LRContext_add_rule(LRContext *context, ErrInfo *errInfo, const char_t *string);
INDEX(LRSymbol) LRContext_add_target(LRContext *context, ErrInfo *errInfo, const char_t *string);

#endif //XLR_INCLUDE_XLR_PARSE_H
