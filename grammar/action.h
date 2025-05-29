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
 * Module Name: grammar
 * Filename: action.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-29
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_GRAMMAR_ACTION_H
#define XLR_GRAMMAR_ACTION_H

#include <stdint.h>

typedef struct state state;
struct grammar_action {
  enum : uint8_t {
    XLR_action_reject = 0,
    XLR_action_stack = 1,
    XLR_action_reduce = 2
  } action      : 4;
  uint8_t count : 4;
  uint8_t type;
  const uint16_t offset;
};

#endif //XLR_GRAMMAR_ACTION_H
