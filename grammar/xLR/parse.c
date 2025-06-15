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
 * Filename: XLR_parse.c
 * Creator: Yaokai Liu
 * Create Date: 2025-05-30
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "parse.h"
#include "action.h"
#include "target.h"
#include "tokenize/tokenizer.h"
#include "generated/xLR/action-table.gen.h"
#include "generated/xLR/rules.gen.h"
#include "xLR/tokens.h"

#define MAX_ARGC       16

GrammarEntry *parse(Tokenizer *tokenizer, LRContext *context, ErrInfo *errInfo, const Allocator *allocator) {
  Token token = {};
  Token args[MAX_ARGC] = {};
  Stack *state_stack = Stack_new(allocator);
  Stack *token_stack = Stack_new(allocator);
  uint32_t state = XLR_state_;
  Stack_push(state_stack, &state, sizeof(int32_t));
  uint32_t status = XLRTokenizer_next(tokenizer, context->in_pattern, &token, errInfo, allocator);
  if (status != XLR_SUCCESS) { return nullptr; }
  while (true) {
    const struct grammar_action *act = getParseAction(state, token.type);
    if (!act) {
      return clean_parse_stack(state_stack, token_stack, allocator);
    }
    if (act->action == XLR_action_stack) {
      state = act->offset;
      Stack_push(token_stack, &token, sizeof(Token));
      Stack_push(state_stack, &state, sizeof(uint32_t));
      status = XLRTokenizer_next(tokenizer, context->in_pattern, &token, errInfo, allocator);
      if (status != XLR_SUCCESS) {
        return clean_parse_stack(state_stack, token_stack, allocator);
      }
      LRContext_state_action(context, state, &token, allocator);
    } else if (act->action == XLR_action_reduce) {
      Stack_pop(token_stack, args, act->count * sizeof(Token));
      Stack_pop(state_stack, nullptr, act->count * sizeof(uint32_t));
      Stack_top(state_stack, (uint32_t *) &state, sizeof(uint32_t));
      fn_xlr_reduce *func = XLR_PRODUCTS[act->offset];
      token.type = act->type;
      token.start.offset = args[0].start.offset;
      token.start.lineno = args[0].start.lineno;
      token.start.column = args[0].start.column;
      token.end.offset = args[act->count - 1].end.offset;
      token.end.lineno = args[act->count - 1].end.lineno;
      token.end.column = args[act->count - 1].end.column;
      token.length = token.end.offset - token.start.offset;
      token.value = func(args, context, errInfo, allocator);
      if (!token.value) {
        return failed_to_produce(state_stack, token_stack, args, act->count, allocator);
      }
      state = parseJumpState(state, act->type);
      if (state == XLR_BAD_STATE) {
        return failed_to_get_next_state(state_stack, token_stack, &token, allocator);
      }
      Stack_push(token_stack, &token, sizeof(Token));
      Stack_push(state_stack, &state, sizeof(uint32_t));
      LRContext_state_action(context, state, &token, allocator);
      if (act->offset == XLR_RULE_GrammarEntry_EXT) { break; }
    } else {
      // never be touched
    }
  }
  Stack_clear(token_stack);
  Stack_clear(state_stack);
  allocator->free(token_stack);
  allocator->free(state_stack);
  return token.value;
}


GrammarEntry *failed_to_get_next_state(
    Stack *state_stack, Stack *token_stack, Token *token, const Allocator *allocator
) {
  int32_t state = 0;
  Stack_top(state_stack, (&state), sizeof(int32_t));
  releaseToken(token, allocator);
  allocator->free(token);
  return clean_parse_stack(state_stack, token_stack, allocator);
}

GrammarEntry  *failed_to_produce(
    Stack *state_stack, Stack *token_stack, Token args[], uint32_t argc, const Allocator *allocator
) {
  for (uint32_t i = 0; i < argc; i++) { releaseToken(&args[i], allocator); }
  return clean_parse_stack(state_stack, token_stack, allocator);
}

GrammarEntry  *clean_parse_stack(Stack *state_stack, Stack *token_stack, const Allocator *allocator) {
  Token token = {};
  while (!Stack_empty(token_stack)) {
    Stack_pop(token_stack, &token, sizeof(Token));
    releaseToken(&token, allocator);
  }
  Stack_clear(token_stack);
  Stack_clear(state_stack);
  allocator->free(token_stack);
  allocator->free(state_stack);
  return nullptr;
}
