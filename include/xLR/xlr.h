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
 * Module Name:
 * Filename: xlr.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-27
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_H
#define XLR_H

#include <stdint.h>
#include "allocator.h"
#include "xLR/char_t.h"
#include "set.h"


typedef enum ACTION_TYPE_ENUM : uint8_t {
  ACTTYPE_REJECT,
  ACTTYPE_STACK,
  ACTTYPE_REDUCE,
} acttype;

typedef struct LRAction {
  acttype acttype;
  /*
   * if acttype:
   * is ACTTYPE_STACK:        next state index;
   * is ACTTYPE_REDUCE:       reduce rule index;
   * is TRANSFORM:            target symbol index;
   */
  uint32_t index;
  // rules that involve this action
  Set * rules;
} LRAction;

typedef struct LRContext LRContext;
#define INDEX(o) uint32_t

#endif //XLR_H
