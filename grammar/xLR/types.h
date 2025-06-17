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
#include "xLR/extint.h"
#include "xLR/xlr.h"
#include "array.h"
#include "xLR/extfloat.h"

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
  // is XLR_STRUCT_TOKEN:       Array<LRVariable>
  // is XLR_STRUCT_ENUM:        Array<EnumItem>
  void         *refer;
} LRType;

typedef struct LRValue {
  INDEX(LRType) type;
  uint32_t      size;
  union {
    int32_t       I32;
    uint32_t      U32;
    int64_t       I64;
    uint64_t      U64;
    int128_t      I128;
    uint128_t     U128;
    float32_t     F32;
    float64_t     F64;
    float128_t    F128;
    char_t *      STRING;
    bool          BOOLEAN;
    void *        STRUCT;
  } val;
} LRValue;

typedef struct LRVariable {
  // type of variable
  INDEX(LRType) type;
  // name of variable
  REFER(char_t) name;
  LRValue *     count;
  LRValue *     value;
} LRVariable;

typedef struct LROperate {
  uint32_t    opcode;
  LRVariable *result;
  LRVariable *operand[2];
} LROperate;

typedef struct LRSymbol LRSymbol;
typedef struct LRState LRState;
typedef struct LRItem LRItem;
typedef struct LRRule LRRule;

#define BUILTIN_TYPE_NAMES "\0i32\0u32\0i64\0u64\0i128\0u128\0i256\0u256\0f32\0f64\0f128\0f256\0string\0"
#define BUILTIN_TYPE_COUNT 13
extern const LRType BUILTIN_TYPES[BUILTIN_TYPE_COUNT];

#endif //XLR_GRAMMAR_XLR_TYPES_H
