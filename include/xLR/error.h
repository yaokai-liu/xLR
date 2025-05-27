/* License
 *
 * ${PROJ_DESCRIPTION}
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
 * Module Name: include
 * Filename: error.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-27
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_INCLUDE_ERROR_H
#define XLR_INCLUDE_ERROR_H

#include "regex/error.h"

enum XLR_ERROR_CODE_ENUM {
  XLR_SUCCESS = REGEX_SUCCESS,
  XLR_ERROR_UNRECOGNIZED_SYMBOL = REGEX_ERROR_UNRECOGNIZED_SYMBOL,
  XLR_ERROR_SR_CONFLICT,
  XLR_ERROR_RS_CONFLICT,
  XLR_ERROR_RR_CONFLICT,

  XLR_ERROR_TARGET_MISMATCH,
  XLR_ERROR_DUPLICATED_SET_RULE,
  XLR_ERROR_MULTI_EMPTY_RULE,

  XLR_ERROR_BAD_QUANTIFIER,
  XLR_ERROR_UNKNOWN_TARGET,
  XLR_UNEXPECTED_TOKEN,
  XLR_ERROR_BAD_TOKEN,
  XLR_ERROR_BAD_RULE,
};

#endif //XLR_INCLUDE_ERROR_H
