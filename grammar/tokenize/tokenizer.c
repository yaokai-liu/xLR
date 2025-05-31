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
#include "tokenize.h"
#include "xLR/tokens.h"
#include "generated/xLR/action-table.gen.h"

typedef struct Tokenizer {
  const Allocator *allocator;
  const char_t *src;
  uint32_t offset;
  uint32_t lineno;
  uint32_t column;
  Array *  ident_array;  // Array<Identifier>
  Trie *   ident_trie;   // Trie<char_t, Identifier>
} Tokenizer;

Tokenizer *XLRTokenizer_new(const char_t *src, Array *ident_array, Trie *ident_trie, const Allocator *allocator) {
  Tokenizer *tokenizer = allocator->calloc(1, sizeof(Tokenizer));
  tokenizer->ident_trie = ident_trie;
  tokenizer->allocator = allocator;
  tokenizer->ident_array = ident_array;
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
uint32_t
XLRTokenizer_next(Tokenizer *tokenizer, bool in_pattern, Token *token, ErrInfo *errInfo, const Allocator *allocator) {
  tokenizer->offset += pass_space(pText, &tokenizer->lineno, &tokenizer->column);
  Terminal terminal = {};
  terminal.type = XLR_ERROR_BAD_TOKEN;
  terminal.location.lineno = tokenizer->lineno;
  terminal.location.column = tokenizer->column;
  terminal.location.offset = tokenizer->offset;
  const uint32_t length = (in_pattern)
                        ? pattern_single_tokenize(pText, &terminal, allocator)
                        : action_single_tokenize(pText, &terminal, allocator);
  if (terminal.type == XLR_TOKEN_BAD_TOKEN) {
    errInfo->pos.lineno = tokenizer->lineno;
    errInfo->pos.column = tokenizer->column;
    errInfo->pos.offset = tokenizer->offset;
    errInfo->code = XLR_ERROR_UNRECOGNIZED_SYMBOL;
    return errInfo->code;
  }
  if (terminal.type == XLR_TOKEN_IDENTIFIER) {
    REFER(char_t) v_ident = Trie_get(tokenizer->ident_trie, terminal.value);
    if (!v_ident) {
      v_ident = Array_last_virt(tokenizer->ident_array) + 1;
      Array_append(tokenizer->ident_array, terminal.value, terminal.length + 1);
      Trie_set(tokenizer->ident_trie, terminal.value, v_ident);
    }
    allocator->free(terminal.value);
    terminal.value = v_ident;
  }
  terminal2Token(&terminal, token);
  tokenizer->offset += length;
  tokenizer->column += length;
  return XLR_SUCCESS;
}
