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
 * Filename: error.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-27
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_ERROR_H
#define XLR_ERROR_H

#include "token.h"

enum XLR_ERROR_CODE_ENUM {
  XLR_SUCCESS,
  XLR_ERROR_UNRECOGNIZED_SYMBOL,
  XLR_ERROR_STACK_REDUCE_CONFLICT,
  XLR_ERROR_REDUCE_STACK_CONFLICT,
  XLR_ERROR_REDUCE_REDUCE_CONFLICT,

  XLR_ERROR_TARGET_MISMATCH,
  XLR_ERROR_DUPLICATED_SET_RULE,
  XLR_ERROR_MULTI_EMPTY_RULE,

  XLR_ERROR_BAD_QUANTIFIER,
  XLR_ERROR_UNKNOWN_TARGET,
  XLR_ERROR_UNEXPECTED_TOKEN,
  XLR_ERROR_BAD_TOKEN,
  XLR_ERROR_BAD_RULE,

  XLR_ERROR_ACCESS_NONE_FIELD_TYPE,
  XLR_ERROR_NO_SUCH_FIELD,
};


typedef struct ErrInfo {
  Location start;
  Location end;
  uint32_t code;
  uint32_t state;
  uint32_t token;
} ErrInfo;

#endif //XLR_ERROR_H
