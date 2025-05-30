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
 * Filename: parse.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-30
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_GRAMMAR_XLR_PARSE_H
#define XLR_GRAMMAR_XLR_PARSE_H

#include "stack.h"
#include "tokenize/tokenizer.h"
#include "context.h"
#include "target.h"

GrammarEntry *parse(Tokenizer *tokenizer, LRContext *context, ErrInfo *errInfo, const Allocator *allocator);

GrammarEntry *failed_to_get_next_state(
    Stack *state_stack, Stack *token_stack, Token *token, const Allocator *allocator
);
GrammarEntry *failed_to_produce(Stack *state_stack, Stack *token_stack, Token *, uint32_t,
                                const Allocator *allocator);
GrammarEntry  *clean_parse_stack(Stack *state_stack, Stack *token_stack, const Allocator *allocator);


#endif //XLR_GRAMMAR_XLR_PARSE_H
