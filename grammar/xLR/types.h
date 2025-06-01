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
 * Filename: types.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-31
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_GRAMMAR_XLR_TYPES_H
#define XLR_GRAMMAR_XLR_TYPES_H

#include "xLR/char_t.h"
#include "xLR/xlr.h"
#include "array.h"

enum XLR_TYPE_ENUM {
  XLR_TYPE_BAD_TYPE = 0,
  XLR_TYPE_CHAR,
  XLR_TYPE_ENUM,
  XLR_TYPE_TYPE,
  XLR_TYPE_SYMBOL,
  XLR_TYPE_RULE,
  XLR_TYPE_ITEM,
  XLR_TYPE_STATE,
  XLR_TYPE_ACTION,
  XLR_TYPE_RULE_KEY,
  XLR_TYPE_ENV_KEY,
  XLR_TYPE_USE_KEY,
  XLR_TYPE_ACT_KEY,
};

typedef struct LRType {
  uint32_t      type;
  uint32_t      size;
  // name of attribute
  REFER(char_t) name;
  // if type
  // is XLR_STRUCT_BUILTIN:     nullptr
  // is XLR_STRUCT_TOKEN:       Array<LRAttr>
  // is XLR_STRUCT_ENUM:        REFER(Enum)
  void         *refer;
} LRType;

typedef struct LRValue {
  INDEX(LRType) type;
  uint32_t      size;
  void *        bytes;
} LRValue;

typedef struct LRAttr {
  // type of attribute
  INDEX(LRType) type;
  // name of attribute
  REFER(char_t) name;
  LRValue *     count;
  LRValue *     value;
} LRAttr;

#endif //XLR_GRAMMAR_XLR_TYPES_H
