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
 * Filename: category.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-31
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_GRAMMAR_CATEGORY_H
#define XLR_GRAMMAR_CATEGORY_H

#include "xLR/char_t.h"
#include "xLR/extint.h"
#include "array.h"
#include "xLR/extfloat.h"

enum XLR_TYPE_CATEGORY_ENUM: uint32_t {
  XLR_CATEGORY_BUILTIN,
  XLR_CATEGORY_STRUCT,
  XLR_CATEGORY_UNION,
  XLR_CATEGORY_ARRAY,
  XLR_CATEGORY_ENUM,
};

typedef struct {
  uint32_t      type; // XLR_CATEGORY_ENUM
  REFER(char_t) name;
} Identifier;

typedef struct LRType {
  uint32_t      cat;
  /*
   * if cat
   * is XLR_CATEGORY_ARRAY:    count of elements
   * otherwise:                size of the type
   */
  uint32_t      size;
  // name of the type
  REFER(Identifier) ident;
  /*
   * if cat
   * is XLR_CATEGORY_ENUM:        Array<EnumItem>
   * is XLR_CATEGORY_ARRAY:       REFER(LRType)
   * is XLR_CATEGORY_UNION:       Array<LRVariable>
   * is XLR_CATEGORY_STRUCT:      Array<LRVariable>
   * is XLR_CATEGORY_BUILTIN:     nullptr
   */
  void *        refer;
  Array *       attrs;  // Array<LRAttribute>
} LRType;

typedef struct LRValue {
  REFER(LRType) type;
  uint64_t      size;
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

typedef struct LRVariable LRVariable;
typedef struct LRVariable {
  // type of variable
  REFER(LRType) type;
  REFER(Identifier) ident;
  /*
   * if is a field or element of another variable, then
   *    refers to the parent variable;
   * else
   *    set as nullptr.
   */
  REFER(LRVariable) parent;
  Array * attrs;  // Array<LRAttribute>
  /*
   * if type.cat
   * is XLR_CATEGORY_BUILTIN:     nullptr
   * is XLR_CATEGORY_STRUCT:      AVLTree<Identifier, REFER(LRVariable)>
   * is XLR_CATEGORY_UNION:       AVLTree<Identifier, REFER(LRVariable)>
   * is XLR_CATEGORY_ENUM:        REFER(LRVariable<EnumItem>)
   * is XLR_CATEGORY_ARRAY:       Array<REFER(LRVariable)>
   */
  void *        refer;
} LRVariable;

typedef struct LRArgument {
  // type of variable
  REFER(LRType) type;
  Array * attrs; // Array<LRAttribute>
} LRArgument;

typedef struct LRAttribute {
  // name of attribute
  REFER(Identifier) ident;
  Array            *args; // Array<Argument>
} LRAttribute;

typedef struct Parameter {
  REFER(LRType)     type;
  REFER(Identifier) ident;
  Array            *attrs; // Array<LRAttribute>
} LRParameter;

typedef struct LRFunction {
  REFER(Identifier)   ident;
  REFER(LRType)       restype;
  Array              *params; // Array<Parameter>
  Array              *attrs; // Array<LRAttribute>
  Array              *commands; // Array<uint8_t>
} LRFunction;

typedef struct LRSymbol LRSymbol;
typedef struct LRState LRState;
typedef struct LRItem LRItem;
typedef struct LRRule LRRule;

#endif //XLR_GRAMMAR_CATEGORY_H
