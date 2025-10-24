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
 * Filename: tokenizer.c
 * Creator: Yaokai Liu
 * Create Date: 2025-05-30
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "tokenizer.h"

#include "avl-tree.h"
#include "tokenize.h"
#include "generated/tokens.gen.h"
#include "xLR/category.h"

typedef struct Tokenizer {
  const Allocator *allocator;
  const char_t *src;
  uint32_t offset;
  uint32_t lineno;
  uint32_t column;
  LRContext *context;
} Tokenizer;

Tokenizer *XLRTokenizer_new(const char_t *src, LRContext *context, const Allocator *allocator) {
  Tokenizer *tokenizer = allocator->calloc(1, sizeof(Tokenizer));
  tokenizer->allocator = allocator;
  tokenizer->context = context;
  tokenizer->lineno = 1;
  tokenizer->column = 1;
  tokenizer->src = src;
  tokenizer->offset = 0;
  return tokenizer;
}

void XLRTokenizer_destroy(Tokenizer *tokenizer) {
  tokenizer->allocator->free(tokenizer);
}

#define pText (tokenizer->src + tokenizer->offset)
#define CONTEXT (tokenizer->context)
uint32_t XLRTokenizer_next(Tokenizer *tokenizer, Token *token, ErrInfo *errInfo,
                           const Allocator *allocator) {
  tokenizer->offset += pass_space(pText, &tokenizer->lineno, &tokenizer->column);
  Terminal terminal = {};
  terminal.type = XLR_TOKEN_BAD_TOKEN;
  terminal.location.lineno = tokenizer->lineno;
  terminal.location.column = tokenizer->column;
  terminal.location.offset = tokenizer->offset;
  const uint32_t length = (CONTEXT->in_pattern)
                        ? pattern_single_tokenize(pText, &terminal, CONTEXT->kw_as_ident, allocator)
                        : action_single_tokenize(pText, &terminal, CONTEXT->kw_as_ident, allocator);
  if (terminal.type == XLR_TOKEN_BAD_TOKEN) {
    errInfo->start.lineno = tokenizer->lineno;
    errInfo->start.column = tokenizer->column;
    errInfo->start.offset = tokenizer->offset;
    errInfo->end = errInfo->start;
    errInfo->code = XLR_ERROR_UNRECOGNIZED_SYMBOL;
    return errInfo->code;
  }
  if (terminal.type == XLR_TOKEN_IDENTIFIER || terminal.type == XLR_TOKEN_BUILTIN_IDENTIFIER) {
    REFER(Identifier) v_ident = Trie_get(CONTEXT->ident_trie, terminal.value);
    if (!v_ident) {
      REFER(char_t) v_name = Array_last_virt(CONTEXT->name_array) + 1;
      Array_append(CONTEXT->name_array, terminal.value, terminal.length + 1);
      Identifier ident = {.type = XLR_OBJECT_IDENT, .name = v_name};
      Array_append(CONTEXT->ident_array, &ident, 1);
      v_ident = Array_last_virt(CONTEXT->ident_array);
      Trie_set(CONTEXT->ident_trie, terminal.value, v_ident);
    } else if (!CONTEXT->in_pattern) {
      const Identifier *ident = Array_virt2real(CONTEXT->ident_array, v_ident);
      switch (ident->type) {
        case XLR_OBJECT_ATTR: { terminal.type = XLR_TOKEN_ATTRNAME;  break;   }
        case XLR_OBJECT_TYPE: { terminal.type = XLR_TOKEN_TYPENAME;  break;   }
        case XLR_OBJECT_RULE: { terminal.type = XLR_TOKEN_RULENAME;  break;   }
        case XLR_OBJECT_FUNC: { terminal.type = XLR_TOKEN_FUNCNAME;  break;   }
        case XLR_OBJECT_ENUM: { terminal.type = XLR_TOKEN_ENUM_GROUP;  break; }
        default:;
      }
    }
    allocator->free(terminal.value);
    terminal.value = v_ident;
  }
  terminal2Token(&terminal, token);
  tokenizer->offset += length;
  tokenizer->column += length;
  return XLR_SUCCESS;
}
