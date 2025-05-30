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
 * Filename: tokenize.c
 * Creator: Yaokai Liu
 * Create Date: 2025-05-30
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include <string.h>
#include "xLR/token.h"
#include "xLR/tokens.h"
#include "string_t.h"
#include "enum.h"

#define lenof(str_literal) ((sizeof str_literal) - 1)
#define max(a, b)          ((a) > (b) ? (a) : (b))
#define min(a, b)          ((a) < (b) ? (a) : (b))

uint32_t t_IDENTIFIER(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t t_NUMBER_adic16(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t t_NUMBER_adic10(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t t_NUMBER_adic8(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t t_NUMBER_adic2(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t try_keyword_if(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);
uint32_t try_keyword_for(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);
uint32_t try_keyword_else(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);
uint32_t try_keyword_enum(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);
uint32_t try_keyword_token(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);
uint32_t try_keyword_while(const char_t *input, uint32_t offs, Terminal *result, const Allocator *allocator);

uint32_t tokenize_letter_i(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t tokenize_letter_f(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t tokenize_letter_e(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t tokenize_letter_t(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t tokenize_letter_w(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t tokenize_letter_s(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t tokenize_symbol_OR(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t tokenize_symbol_AND(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t tokenize_symbol_EQUAL(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t tokenize_symbol_GT(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t tokenize_symbol_LT(const char_t *input, Terminal *result, const Allocator *allocator);
uint32_t tokenize_symbol_NOT(const char_t *input, Terminal *result, const Allocator *allocator);


uint32_t pass_whitespace(const char *input);

#define startswithDigital(pText) ('0' <= (pText)[0] && (pText)[0] <= '9')
#define startswithLetter(pText)  (('a' <= (pText)[0] && (pText)[0] <= 'z') || ('A' <= (pText)[0] && (pText)[0] <= 'Z'))
#define isIdentCase(pText)  (startswithLetter(pText) || startswithDigital(pText) || (*pText == '_'))


inline uint32_t t_NUMBER_adic16(const char_t * const input, Terminal * const result, const Allocator * const) {
  const char_t *pText = input;
  uint64_t value = 0LL;
  while (true) {
    if ('0' <= *pText && *pText <= '9') {
      value = (value << 4) + (*pText++ - '0');
    } else if ('a' <= *pText && *pText <= 'f') {
      value = (value << 4) + (*pText++ - 'a' + 0xa);  // NOLINT(*-magic-numbers)
    } else if ('A' <= *pText && *pText <= 'F') {
      value = (value << 4) + (*pText++ - 'A' + 0xA);  // NOLINT(*-magic-numbers)
    } else if (('g' <= *pText && *pText <= 'z') || ('G' <= *pText && *pText <= 'Z') || ('_' == *pText)) {
      result->length = pText - input;
      return 0;
    } else if (*pText == '\'') {
      pText++;
    } else {
      break;
    }
  }
  result->type = XLR_TOKEN_NUMBER;
  result->value = (void *) value;
  result->length = pText - input;
  return result->length;
}

inline uint32_t t_NUMBER_adic10(const char_t * const input, Terminal * const result, const Allocator * const) {
  const char_t *pText = input;
  uint64_t value = 0;
  while (true) {
    if (startswithDigital(pText)) {
      value = (value * 10) + (*pText++ - '0');  // NOLINT(*-magic-numbers)
      continue;
    }
    if (startswithLetter(pText)) {
      result->length = pText - input;
      return 0;
    }
    if (*pText == '\'') {
      pText++;
    } else {
      break;
    }
  }
  result->type = XLR_TOKEN_NUMBER;
  result->value = (void *) value;
  result->length = pText - input;
  return result->length;
}

inline uint32_t t_NUMBER_adic8(const char_t * const input, Terminal * const result, const Allocator * const) {
  const char_t *pText = input;
  uint64_t value = 0;
  while (true) {
    if ('0' <= *pText && *pText <= '7') {
      value = (value << 3) + (*pText++ - '0');
      continue;
    }
    if (('8' == *pText) || ('9' == *pText) || startswithLetter(pText)) {
      result->length = pText - input;
      return 0;
    }
    if (*pText == '\'') {
      pText++;
    } else {
      break;
    }
  }
  result->type = XLR_TOKEN_NUMBER;
  result->value = (void *) value;
  result->length = pText - input;
  return result->length;
}

inline uint32_t t_NUMBER_adic2(const char_t * const input, Terminal * const result, const Allocator * const) {
  const char_t *pText = input;
  uint64_t value = 0;
  while (true) {
    if ('0' == *pText || *pText == '1') {
      value = (value << 1) + (*pText++ - '0');
      continue;
    }
    if (('2' <= *pText && *pText <= '9') || startswithLetter(pText)) {
      result->length = pText - input;
      return 0;
    }
    if (*pText == '\'') {
      pText++;
    } else {
      break;
    }
  }
  result->type = XLR_TOKEN_NUMBER;
  result->value = (void *) value;
  result->length = pText - input;
  return result->length;
}

inline uint32_t t_IDENTIFIER(const char_t * const input, Terminal * const result, const Allocator * const allocator) {
  const char_t *pText = input;
  if (startswithLetter(pText) || (*pText == '_')) {
    pText++;
  } else {
    result->length = pText - input;
    return 0;
  }
  while (true) {
    if (isIdentCase(pText)) { pText++; } else { break; }
  }
  const uint32_t len = pText - input;
  result->type = XLR_TOKEN_IDENTIFIER;
  result->value = allocator->calloc(len + 1, sizeof(char_t));
  allocator->memcpy(result->value, input, len);
  ((char_t *) result->value)[len] = '\0';
  result->length = pText - input;
  return result->length;
}

#define fn_try_keyword(_kw, _type)                                                       \
  inline uint32_t try_keyword_##_kw(const char_t * const input, uint32_t const offs,           \
                                    Terminal * const result, const Allocator * const allocator) {                 \
    const char_t pattern[] = string_t(#_kw);                                             \
    for (uint32_t i = offs; i < sizeof(pattern) - 1; i++) {                                 \
      if (input[i - offs] != pattern[i]) { goto __failed_kw_##_kw; }                        \
    }                                                                                    \
    const char_t * const tail = &input[sizeof(pattern) - 1 - offs];                             \
    if (startswithLetter(tail) || *tail == '_') { goto __failed_kw_##_kw; }              \
    result->type = XLR_TOKEN_##_type;                                                    \
    result->value = nullptr;                                                             \
    result->length = lenof(#_kw);                                                        \
    return lenof(#_kw);                                                                  \
    __failed_kw_##_kw : return t_IDENTIFIER(input - 2, result, allocator);               \
  }
#define fn_try_keyword_val(_kw, _type, val)                                              \
  inline uint32_t try_keyword_##_kw(const char_t * const input, uint32_t const offs,           \
                                    Terminal * const result, const Allocator * const allocator) {                 \
    const char_t pattern[] = string_t(#_kw);                                             \
    for (uint32_t i = offs; i < sizeof(pattern) - 1; i++) {                                 \
      if (input[i - offs] != pattern[i]) { goto __failed_kw_##_kw; }                        \
    }                                                                                    \
    const char_t * const tail = &input[sizeof(pattern) - 1 - offs];                             \
    if (startswithLetter(tail) || *tail == '_') { goto __failed_kw_##_kw; }              \
    result->type = XLR_TOKEN_##_type;                                                    \
    result->value = (void *) val;                                                        \
    result->length = lenof(#_kw);                                                        \
    return lenof(#_kw);                                                                  \
    __failed_kw_##_kw : return t_IDENTIFIER(input - 2, result, allocator);               \
  }

fn_try_keyword(if, IF)
fn_try_keyword(for, FOR)
fn_try_keyword(else, ELSE)
fn_try_keyword(enum, ENUM)
fn_try_keyword(token, TOKEN)
fn_try_keyword(while, WHILE)
fn_try_keyword_val(sizeof, BUILTIN_FUNCTION, XLR_BF_SIZEOF)


#define fn_fall_through(len)                                      \
  do {                                                            \
    uint32_t length = t_IDENTIFIER(input - 1, result, allocator); \
    if (length == 0) {                                            \
      result->type = XLR_TOKEN_IDENTIFIER;                        \
      result->value = allocator->calloc(len + 1, sizeof(char_t)); \
      allocator->memcpy(result->value, input, 1);                 \
      ((char_t *) result->value)[1] = '\0';                       \
      result->length = 1;                                         \
      return 1;                                                   \
    }                                                             \
    return length;                                                \
  } while (0)

struct {
  const char_t *pattern;
  uint32_t      value;
} BUILTIN_TYPES[8] = {
    {"uint64", XLR_BT_U64},
    {"uint32", XLR_BT_U32},
    {"uint16", XLR_BT_U16},
    {"uint8", XLR_BT_U8 },
    {"int64", XLR_BT_I64},
    {"int32", XLR_BT_I32},
    {"int16", XLR_BT_I16},
    {"int8", XLR_BT_I8 },
};

uint32_t try_builtin_int(
    const char_t * const input, Terminal * const result, const Allocator * const allocator
) {
  uint32_t length = t_IDENTIFIER(input, result, allocator);
  for (uint32_t i = 0; i < 8; i++) {
    if (strcmp(BUILTIN_TYPES[i].pattern, result->value) == 0) {
      result->type = XLR_TOKEN_BUILTIN_TYPE;
      allocator->free(result->value);
      result->value = (void *) (uint64_t) BUILTIN_TYPES[i].value;
      return length;
    }
  }
  return length;
}

uint32_t tokenize_number(
    const char_t * const input, Terminal * const result, const Allocator * const allocator
) {
  if ('0' == *input) {
    uint32_t length = 0;
    switch (input[1]) {
      case 'x':
      case 'X': {
        length = t_NUMBER_adic16(input + 2, result, allocator);
        result->length += 2;
        return length > 0 ? result->length : 0;
      }
      case 'o':
      case 'O': {
        length = t_NUMBER_adic8(input + 2, result, allocator);
        result->length += 2;
        return length > 0 ? result->length : 0;
      }
      case 'b':
      case 'B': {
        length = t_NUMBER_adic2(input + 2, result, allocator);
        result->length += 2;
        return length > 0 ? result->length : 0;
      }
      default: {
      }
    }
  }
  return t_NUMBER_adic10(input, result, allocator);
}

uint32_t tokenize_letter_i(
    const char_t * const input, Terminal * const result, const Allocator * const allocator
) {
  switch (*input) {
    case 'f': {
      return try_keyword_if(input + 1, 2, result, allocator);
    }
    case 'n': {
      return try_builtin_int(input - 1, result, allocator);
    }
    default: fn_fall_through(1);
  }
}
uint32_t tokenize_letter_f(
    const char_t * const input, Terminal * const result, const Allocator * const allocator
) {
  switch (*input) {
    case 'o': {
      return try_keyword_for(input + 1, 2, result, allocator);
    }
    default: fn_fall_through(1);
  }
}
uint32_t tokenize_letter_e(
    const char_t * const input, Terminal * const result, const Allocator * const allocator
) {
  switch (*input) {
    case 'l': {
      return try_keyword_else(input + 1, 2, result, allocator);
    }
    default: fn_fall_through(1);
  }
}
uint32_t tokenize_letter_t(
    const char_t * const input, Terminal * const result, const Allocator * const allocator
) {
  switch (*input) {
    case 'o': {
      return try_keyword_token(input + 1, 2, result, allocator);
    }
    default: fn_fall_through(1);
  }
}
uint32_t tokenize_letter_w(
    const char_t * const input, Terminal * const result, const Allocator * const allocator
) {
  switch (*input) {
    case 'h': {
      return try_keyword_while(input + 1, 2, result, allocator);
    }
    default: fn_fall_through(1);
  }
}
uint32_t tokenize_letter_s(
    const char_t * const input, Terminal * const result, const Allocator * const allocator
) {
  switch (*input) {
    case 'i': {
      return try_keyword_sizeof(input + 1, 2, result, allocator);
    }
    default: fn_fall_through(1);
  }
}
uint32_t tokenize_symbol_OR(const char_t *input, Terminal *result, const Allocator *) {
  const char_t *pText =input;
  if (*pText == '|') {
    result->type = XLR_TOKEN_BOOL_OR;
    result->value = nullptr;
    result->length = 2;
  } else {
    result->type = XLR_TOKEN_ARITH_2_BIN_OP;
    result->value = (void *) (uint64_t) XLR_AB_OR;
    result->length = 1;
  }
  return result->length;
}
uint32_t tokenize_symbol_AND(const char_t *input, Terminal *result, const Allocator *) {
  const char_t *pText =input;
  if (*pText == '&') {
    result->type = XLR_TOKEN_BOOL_AND;
    result->value = nullptr;
    result->length = 2;
  } else {
    result->type = XLR_TOKEN_ARITH_2_BIN_OP;
    result->value = (void *) (uint64_t) XLR_AB_AND;
    result->length = 1;
  }
  return result->length;
}
uint32_t tokenize_symbol_EQUAL(const char_t *input, Terminal *result, const Allocator *) {
  const char_t *pText =input;
  if (*pText == '=') {
    result->type = XLR_TOKEN_COND_BIN_OP;
    result->value = (void *) (uint64_t) XLR_CB_EQ;
    result->length = 2;
  } else {
    result->type = XLR_TOKEN_ASSIGNER;
    result->value = nullptr;
    result->length = 1;
  }
  return result->length;
}
uint32_t tokenize_symbol_GT(const char_t *input, Terminal *result, const Allocator *) {
  const char_t *pText =input;
  if (*pText == '=') {
    result->type = XLR_TOKEN_COND_BIN_OP;
    result->value = (void *) (uint64_t) XLR_CB_GE;
    result->length = 2;
  } else if (*pText == '>') {
    result->type = XLR_TOKEN_ARITH_2_BIN_OP;
    result->value = (void *) (uint64_t) XLR_AB_RSH;
    result->length = 2;
  } else {
    result->type = XLR_TOKEN_COND_BIN_OP;
    result->value = (void *) (uint64_t) XLR_CB_GT;
    result->length = 1;
  }
  return result->length;
}
uint32_t tokenize_symbol_LT(const char_t *input, Terminal *result, const Allocator *) {
  const char_t *pText =input;
  if (*pText == '=') {
    result->type = XLR_TOKEN_COND_BIN_OP;
    result->value = (void *) (uint64_t) XLR_CB_LE;
    result->length = 2;
  } else if (*pText == '<') {
    result->type = XLR_TOKEN_ARITH_2_BIN_OP;
    result->value = (void *) (uint64_t) XLR_AB_LSH;
    result->length = 2;
  } else {
    result->type = XLR_TOKEN_COND_BIN_OP;
    result->value = (void *) (uint64_t) XLR_CB_LT;
    result->length = 1;
  }
  return result->length;
}
uint32_t tokenize_symbol_NOT(const char_t *input, Terminal *result, const Allocator *) {
  const char_t *pText =input;
  if (*pText == '=') {
    result->type = XLR_TOKEN_COND_BIN_OP;
    result->value = (void *) (uint64_t) XLR_CB_NE;
    result->length = 2;
  } else {
    result->type = XLR_TOKEN_BOOL_NOT;
    result->value = nullptr;
    result->length = 1;
  }
  return result->length;
}

const struct {
  uint32_t t_type;
  uint32_t a_type;
} ARITH_SYM_TYPE_LITERALS[] = {
    {XLR_TOKEN_ARITH_0_BIN_OP, XLR_AB_ADD},
    {XLR_TOKEN_ARITH_0_BIN_OP, XLR_AB_SUB},
    {XLR_TOKEN_ARITH_1_BIN_OP, XLR_AB_MUL},
    {XLR_TOKEN_ARITH_1_BIN_OP, XLR_AB_DIV},
    {XLR_TOKEN_ARITH_1_BIN_OP, XLR_AB_MOD},
    {XLR_TOKEN_ARITH_2_BIN_OP, XLR_AB_XOR},
    {XLR_TOKEN_ARITH_2_SIN_OP, XLR_AS_INV},
};
uint32_t tokenize_arith_single_symbols(
    const char_t * const input, Terminal * const result, const Allocator * const
) {
  constexpr char_t ARITH_SYM_LITERALS[] = "+-*/%^~";
  uint32_t length = stridx_o(*input, ARITH_SYM_LITERALS);
  if (length < lenof(ARITH_SYM_LITERALS)) {
    result->type = ARITH_SYM_TYPE_LITERALS[length].t_type;
    result->value = (void *) (uint64_t) ARITH_SYM_TYPE_LITERALS[length].a_type;
    result->length = 1;
    return 1;
  }
  return 0;
}
constexpr uint32_t TERMINAL_TYPE_LITERALS[] = {
    XLR_TOKEN_AT,
    XLR_TOKEN_DOT,
    XLR_TOKEN_COLON,
    XLR_TOKEN_COMMA,
    XLR_TOKEN_SEMICOLON,
    XLR_TOKEN_QUESTION_MARK,

    XLR_TOKEN_LEFT_BRACKET, XLR_TOKEN_RIGHT_BRACKET,
    XLR_TOKEN_LEFT_SQUARE_BRACKET, XLR_TOKEN_RIGHT_SQUARE_BRACKET,
    XLR_TOKEN_LEFT_PARENTHESIS, XLR_TOKEN_RIGHT_PARENTHESIS,
};
uint32_t tokenize_grammar_single_symbols(
    const char_t * const input, Terminal * const result, const Allocator * const
) {
  constexpr char_t SINGLE_LITERAL[] = "@.:,;?{}[]()";
  uint32_t length = stridx_o(*input, SINGLE_LITERAL);
  if (length < lenof(SINGLE_LITERAL)) {
    result->type = TERMINAL_TYPE_LITERALS[length];
    result->value = nullptr;
    result->length = 1;
    return 1;
  }
  return 0;
}

inline uint32_t action_single_tokenize(
    const char_t * const input, Terminal * const result, const Allocator * const allocator
) {
  if (!*input) {
    result->type = XLR_TOKEN_TERMINATOR;
    result->value = nullptr;
    result->length = 0;
    return 0;
  }
  switch (*input) {
    case 'i': return tokenize_letter_i(input + 1, result, allocator);
    case 'f': return tokenize_letter_f(input + 1, result, allocator);
    case 'e': return tokenize_letter_e(input + 1, result, allocator);
    case 't': return tokenize_letter_t(input + 1, result, allocator);
    case 'w': return tokenize_letter_w(input + 1, result, allocator);
    case 's': return tokenize_letter_s(input + 1, result, allocator);
    case '|': return tokenize_symbol_OR(input + 1, result, allocator);
    case '&': return tokenize_symbol_AND(input + 1, result, allocator);
    case '=': return tokenize_symbol_EQUAL(input + 1, result, allocator);
    case '>': return tokenize_symbol_GT(input + 1, result, allocator);
    case '<': return tokenize_symbol_LT(input + 1, result, allocator);
    case '!': return tokenize_symbol_NOT(input + 1, result, allocator);
    default: {
    }
  }
  uint32_t length = 0;
  if (startswithDigital(input)) {
    length = tokenize_number(input, result, allocator);
    return length;
  }
  if (startswithLetter(input)) {
    length = t_IDENTIFIER(input, result, allocator);
    return length;
  }
  length = tokenize_grammar_single_symbols(input, result, allocator);
  if (length > 0) { return length; }
  length = tokenize_arith_single_symbols(input, result, allocator);
  if (length > 0) { return length; }
  result->type = XLR_TOKEN_BAD_TOKEN;
  result->value = nullptr;
  result->length = 0;
  return 0;
}

inline uint32_t pattern_single_tokenize(
    const char_t * const input, Terminal * const result, const Allocator * const allocator
) {
  if (!*input) {
    result->type = XLR_TOKEN_TERMINATOR;
    result->value = nullptr;
    result->length = 0;
    return 0;
  }
  switch (*input) {
    case '*': {
      result->type = XLR_TOKEN_QUANTIFIER;
      result->value = (void *) (uint64_t) XLR_QUANT_ANY_COUNT;
      result->length = 1;
      return result->length;
    }
    case '+': {
      result->type = XLR_TOKEN_QUANTIFIER;
      result->value = (void *) (uint64_t) XLR_QUANT_MORE_THAN_ONE;
      result->length = 1;
      return result->length;
    }
    case '?': {
      result->type = XLR_TOKEN_QUANTIFIER;
      result->value = (void *) (uint64_t) XLR_QUANT_LESS_THAN_ONE;
      result->length = 1;
      return result->length;
    }
    default: {}
  }
  if (!startswithLetter(input)) { return 0; }
  uint32_t length = t_IDENTIFIER(input, result, allocator);
  if (length > 0) { return length; }
  result->type = XLR_TOKEN_BAD_TOKEN;
  result->value = nullptr;
  result->length = 0;
  return 0;
}


uint32_t pass_whitespace(const char_t * const input) {
  const char_t *pText = input;
  while (*pText && stridx_o(*pText, " \t\n\f\v\r") < lenof(" \t\n\f\v\r")) { pText++; }
  return pText - input;
}
uint32_t
try_pass_comment(const char * const input, uint32_t * const lineno, uint32_t * const column) {
  const char *pText = input + 1;
  if (*pText == '/') {
    do { pText++; } while (*pText != '\n');
  } else if (*pText == '*') {
    pText++;
    do {
      while (*pText != '*') {
        if (*pText == '\n') { (*lineno)++, *column = 1; }
        pText++;
      }
    } while (*(++pText) != '/');
    pText++;
  } else {
    return 0;
  }
  *column += pText - input;
  return pText - input;
}
uint32_t pass_space(const char * const input, uint32_t * const lineno, uint32_t * const column) {
  uint32_t l = lineno ? *lineno : 0;
  uint32_t c = column ? *column : 0;
  const char *pText = input;
  while (*pText) {
    switch (*pText) {
      case '\n': {
        l++;
        c = 1;
        break;
      }
      case '\f':
      case '\r':
      case ' ':
      case '\t': {
        c++;
        break;
      }
      case '/': {
        uint32_t passed = try_pass_comment(pText, &l, &c);
        if (passed) {
          pText += passed;
          continue;
        }
      }
      default: {
        goto __return;
      }
    }
    pText++;
  }
  __return:
  lineno ? *lineno = l : 0;
  column ? *column = c : 0;
  return pText - input;
}


void terminal2Token(Terminal *terminal, Token *token) {
  token->type = terminal->type;
  token->start.lineno = terminal->location.lineno;
  token->start.offset = terminal->location.offset;
  token->start.column = terminal->location.column;
  token->end.lineno = terminal->location.lineno;
  token->end.offset = terminal->location.offset + terminal->length;
  token->end.column = terminal->location.column + terminal->length;
  token->length = terminal->length;
  token->value = terminal->value;
}
