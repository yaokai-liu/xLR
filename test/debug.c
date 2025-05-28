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
 * Module Name: test
 * Filename: debug.c
 * Creator: Yaokai Liu
 * Create Date: 2025-05-27
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "xLR/xlr.h"


int main() {
  const char_t *string = "Range = SYMBOL MINUS SYMBOL";
  LRContext *context = LRContext_new(&STDAllocator);
  ErrInfo errInfo = {};
  INDEX(LRRule) i_rule = LRContext_add_rule(context, &errInfo, string);
  if (!i_rule) { return (int) errInfo.code; }
  INDEX(LRSymbol) i_sym = LRContext_add_target(context, &errInfo, "Range");
  if (!i_sym) { return (int) errInfo.code; }
  uint32_t result = LRContext_set_rule(context, i_rule, true);
  if (result != XLR_SUCCESS) { return (int) result; }
  return 0;
}