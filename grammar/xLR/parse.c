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
  uint32_t status = XLRTokenizer_next(tokenizer, &token, errInfo, allocator);
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
      status = XLRTokenizer_next(tokenizer, &token, errInfo, allocator);
      if (status != XLR_SUCCESS) {
        return clean_parse_stack(state_stack, token_stack, allocator);
      }
      fn_ctx_act *ctxAct = getLRContextAction(state);
      if (ctxAct) { ctxAct(context, &token); }
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
      fn_ctx_act *ctxAct = getLRContextAction(state);
      if (ctxAct) { ctxAct(context, &token); }
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

#define releaseArrayCase(array, ele)                        \
  case XLR_TOKEN_##array: {                                 \
    Array_reset(token->value, (destruct_t *) release##ele); \
    Array_destroy(token->value);                            \
    break;                                                  \
  }
#define releaseTokenCase(t, r)           \
  case XLR_TOKEN_##t: {                  \
    release##r(token->value, allocator); \
    break;                               \
  }

void releaseToken(Token *token, const Allocator *allocator) {
  switch (token->type) {
    // token array
    releaseArrayCase(Declarations, Declaration)
    releaseArrayCase(ActionStatements, ActionStatement)
    releaseArrayCase(GrammarItems, GrammarItem)
    releaseArrayCase(Arguments, Expr)
    releaseArrayCase(EnumAssignments, EnumAssignment)
    // token
    releaseTokenCase(ActionBlock, ActionBlock)
    releaseTokenCase(RuleDefinition, RuleDefinition)
    releaseTokenCase(Declaration, Declaration)
    releaseTokenCase(TokenDefinition, TokenDefinition)
    releaseTokenCase(ActionStatement, ActionStatement)
    releaseTokenCase(GrammarEntry, GrammarEntry)
    releaseTokenCase(IfCondition, IfCondition)
    releaseTokenCase(ForStatement, ForStatement)
    releaseTokenCase(GrammarItem, GrammarItem)
    releaseTokenCase(IfStatement, IfStatement)
    releaseTokenCase(DecItem, DecItem)
    releaseTokenCase(WhileStatement, WhileStatement)
    releaseTokenCase(EnumAssignment, EnumAssignment)
    releaseTokenCase(CondStatement, CondStatement)
    releaseTokenCase(GrammarPattern, GrammarPattern)
    releaseTokenCase(EnumDeclaration, EnumDeclaration)
    releaseTokenCase(GrammarAction, ActionBlock)
    releaseTokenCase(ForCondition, ForCondition)
    case XLR_TOKEN_CondExpr:
    case XLR_TOKEN_AndCondExpr:
    case XLR_TOKEN_SingleCondExpr:
    case XLR_TOKEN_ArithExpr:
    case XLR_TOKEN_Arith_0_Expr:
    case XLR_TOKEN_Arith_1_Expr:
    case XLR_TOKEN_Arith_2_Expr:
    case XLR_TOKEN_Arith_3_Expr:
    case XLR_TOKEN_Arith_4_Expr:
    case XLR_TOKEN_AssignExpr:
    case XLR_TOKEN_Assignable:
    case XLR_TOKEN_Accessed:
    case XLR_TOKEN_Subscribed:
    case XLR_TOKEN_Evaluable:
    case XLR_TOKEN_IntegratedExpr:
    case XLR_TOKEN_FunctionCall:
    case XLR_TOKEN_OptionalAssignExpr:
    case XLR_TOKEN_OptionalCondExpr: {
      releaseExpr(token->value, allocator);
      break;
    }
    default: {}
  }
}
