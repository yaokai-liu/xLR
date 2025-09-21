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
 * Module Name: grammar/tokenize
 * Filename: tokenize.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-30
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_GRAMMAR_TOKENIZE_TOKENIZE_H
#define XLR_GRAMMAR_TOKENIZE_TOKENIZE_H

#include <stdint.h>
#include "xLR/char_t.h"
#include "xLR/token.h"

uint32_t action_single_tokenize(const char_t *input, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);
uint32_t pattern_single_tokenize(const char_t *input, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);
uint32_t pass_space(const char *input, uint32_t *lineno, uint32_t *column);
void terminal2Token(Terminal *terminal, Token *token);

#endif //XLR_GRAMMAR_TOKENIZE_TOKENIZE_H
