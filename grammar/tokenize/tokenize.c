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

#include "enum.h"
#include "string_t.h"
#include "xLR/token.h"
#include "generated/tokens.gen.h"
#include "xLR/category.h"
#include "xLR/builtin.h"
#include "xLR/extfloat.h"
#include "xLR/target.h"
#include <tgmath.h>

#define lenof(str_literal) ((sizeof str_literal) - 1)
#define max(a, b)          ((a) > (b) ? (a) : (b))
#define min(a, b)          ((a) < (b) ? (a) : (b))

static uint32_t t_IDENTIFIER(const char_t *input, Terminal *result, const Allocator *allocator);
static uint32_t t_NUMBER(const char_t *input, Terminal *result, bool negative, uint32_t adic, const Allocator *allocator);
static uint32_t t_INT_DIGITS_adic16(const char_t *input, uint32_t *effective_length, uint256_t *value);
static uint32_t t_INT_DIGITS_adic10(const char_t *input, uint32_t *effective_length, uint256_t *value);
static uint32_t t_INT_DIGITS_adic8 (const char_t *input, uint32_t *effective_length, uint256_t *value);
static uint32_t t_INT_DIGITS_adic2 (const char_t *input, uint32_t *effective_length, uint256_t *value);
static uint32_t t_FRAC_DIGITS_adic16(const char_t *input, uint32_t *effective_length, uint256_t *value);
static uint32_t t_FRAC_DIGITS_adic10(const char_t *input, uint32_t *effective_length, uint256_t *value);
static uint32_t t_FRAC_DIGITS_adic8 (const char_t *input, uint32_t *effective_length, uint256_t *value);
static uint32_t t_FRAC_DIGITS_adic2 (const char_t *input, uint32_t *effective_length, uint256_t *value);

static uint32_t tokenize_number(const char_t * input, Terminal *  result, const Allocator * allocator);
static uint32_t tokenize_single_char(const char_t *input, Terminal *result, const Allocator *allocator);
static uint32_t tokenize_text(const char_t *input, uint32_t n_pred, const char_t *succ, uint32_t n_succ, Terminal *result, const Allocator *allocator);

static uint32_t try_keyword_if(const char_t *input, uint32_t offs, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);
static uint32_t try_keyword_for(const char_t *input, uint32_t offs, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);
static uint32_t try_keyword_else(const char_t *input, uint32_t offs, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);
static uint32_t try_keyword_enum(const char_t *input, uint32_t offs, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);
static uint32_t try_keyword_attribute(const char_t *input, uint32_t offs, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);
static uint32_t try_keyword_union(const char_t *input, uint32_t offs, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);
static uint32_t try_keyword_struct(const char_t *input, uint32_t offs, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);
static uint32_t try_keyword_while(const char_t *input, uint32_t offs, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);
static uint32_t try_keyword_typedef(const char_t *input, uint32_t offs, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);

static uint32_t tokenize_letter_i(const char_t *input, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);
static uint32_t tokenize_letter_f(const char_t *input, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);
static uint32_t tokenize_letter_e(const char_t *input, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);
static uint32_t tokenize_letter_t(const char_t *input, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);
static uint32_t tokenize_letter_w(const char_t *input, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);
static uint32_t tokenize_letter_s(const char_t *input, Terminal *result, uint64_t kw_as_ident, const Allocator *allocator);

static uint32_t tokenize_symbol_OR(const char_t *input, Terminal *result, const Allocator *allocator);
static uint32_t tokenize_symbol_AND(const char_t *input, Terminal *result, const Allocator *allocator);
static uint32_t tokenize_symbol_EQUAL(const char_t *input, Terminal *result, const Allocator *allocator);
static uint32_t tokenize_symbol_GT(const char_t *input, Terminal *result, const Allocator *allocator);
static uint32_t tokenize_symbol_LT(const char_t *input, Terminal *result, const Allocator *allocator);
static uint32_t tokenize_symbol_NOT(const char_t *input, Terminal *result, const Allocator *allocator);
static uint32_t tokenize_symbol_PLUS(const char_t *input, Terminal *result, const Allocator *allocator);
static uint32_t tokenize_symbol_MINUS(const char_t *input, Terminal *result, const Allocator *allocator);

static uint32_t try_pass_comment(const char *input, uint32_t *lineno, uint32_t *column);

#define isBuiltinIdentChar(pText)     ((*(pText) == '$'))
#define isIdentHeader(pText)      (startswithLetter(pText) || (*(pText) == '_'))
#define isIdentChar(pText)        (startswithLetter(pText) || isDecDigital(pText) || (*(pText) == '_'))

#define startswithLetter(pText)   (('a' <= *(pText) && *(pText) <= 'z') || ('A' <= *(pText) && *(pText) <= 'Z'))
#define isHexLetter(pText)        (('a' <= *(pText) && *(pText) <= 'f') || ('A' <= *(pText) && *(pText) <= 'F'))

#define isSign(pText)             ((*(pText) == '-') || (*(pText) == '+'))
#define isBinDigital(pText)       ('0' == *(pText) || *(pText) == '1')
#define isOctDigital(pText)       ('0' <= *(pText) && *(pText) <= '7')
#define isDecDigital(pText)       ('0' <= *(pText) && *(pText) <= '9')
#define isHexDigital(pText)       (isDecDigital(pText) || isHexLetter(pText))


// [a-fA-F0-9]+
inline uint32_t t_INT_DIGITS_adic16(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pZero = input;
  if (!isHexDigital(pZero)) { return 0; }
  while ('0' == *pZero) { pZero++; }
  const char_t *pText = pZero;
  uint32_t n_separator = 0;
  while (*pText) {
    if ('0' <= *pText && *pText <= '9') {
      *value = (*value << 4) + (*pText - '0');
    } else if ('a' <= *pText && *pText <= 'f') {
      *value = (*value << 4) + (*pText - 'a' + 0xa);
    } else if ('A' <= *pText && *pText <= 'F') {
      *value = (*value << 4) + (*pText - 'A' + 0xA);
    } else if (*pText == '\'') {
      if (!isHexDigital(pText + 1)) { break; }
      n_separator ++;
    } else { break; }
    pText ++;
  }
  if (effective_length) { *effective_length = pText - pZero - n_separator; }
  return pText - input;
}
// [0-9]+
inline uint32_t t_INT_DIGITS_adic10(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pZero = input;
  if (!isDecDigital(pZero)) { return 0; }
  while ('0' == *pZero) { pZero++; }
  const char_t *pText = pZero;
  uint32_t n_separator = 0;
  while (*pText) {
    if (isDecDigital(pText)) {
      *value = (*value) * 10 + (*pText - '0');
    } else if (*pText == '\'') {
      if (!isDecDigital(pText + 1)) { break; }
      n_separator ++;
    } else { break; }
    pText ++;
  }
  if (effective_length) { *effective_length = pText - pZero - n_separator; }
  return pText - input;
}
// [0-7]+
inline uint32_t t_INT_DIGITS_adic8(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pZero = input;
  if (!isOctDigital(pZero)) { return 0; }
  while ('0' == *pZero) { pZero++; }
  const char_t *pText = pZero;
  uint32_t n_separator = 0;
  while (*pText) {
    if (isOctDigital(pText)) {
      *value = (*value << 3) + (*pText - '0');
    } else if (*pText == '\'') {
      if (!isOctDigital(pText + 1)) { break; }
      n_separator ++;
    } else { break; }
    pText ++;
  }
  if (effective_length) { *effective_length = pText - pZero - n_separator; }
  return pText - input;
}
// [01]+
inline uint32_t t_INT_DIGITS_adic2(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pZero = input;
  if (!isBinDigital(pZero)) { return 0; }
  while ('0' == *pZero) { pZero++; }
  const char_t *pText = pZero;
  uint32_t n_separator = 0;
  while (*pText) {
    if (isOctDigital(pText)) {
      *value = (*value << 1) + (*pText - '0');
    } else if (*pText == '\'') {
      if (!isBinDigital(pText + 1)) { break; }
      n_separator ++;
    } else { break; }
    pText ++;
  }
  if (effective_length) { *effective_length = pText - pZero - n_separator; }
  return pText - input;
}
// (?=\.)[a-fA-F0-9]+
inline uint32_t t_FRAC_DIGITS_adic16(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pText = input;
  if (!isHexDigital(pText)) { return 0; }
  uint32_t trialing_zeros = 0;
  uint32_t n_separator = 0;
  while (*pText) {
    if (*pText == '\'') {
      if (!isHexDigital(pText + 1)) { break; }
      n_separator ++; pText ++; continue;
    }
    if ('0' == *pText) { trialing_zeros ++; continue; }
    uint32_t shift_count = 4 * (1 + trialing_zeros);
    if ('1' <= *pText && *pText <= '9') {
      *value = (*value << shift_count) + (*pText - '0');
    } else if ('a' <= *pText && *pText <= 'f') {
      *value = (*value << shift_count) + (*pText - 'a' + 0xa);
    } else if ('A' <= *pText && *pText <= 'F') {
      *value = (*value << shift_count) + (*pText - 'A' + 0xA);
    } else { break; }
    trialing_zeros = 0;
    pText ++;
  }
  uint32_t length = pText - input;
  if (effective_length) { *effective_length = length - trialing_zeros - n_separator; }
  return length;
}
// (?=\.)[0-9]+
inline uint32_t t_FRAC_DIGITS_adic10(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pText = input;
  if (!isDecDigital(pText)) { return 0; }
  uint32_t trialing_zeros = 0;
  uint32_t n_separator = 0;
  while (*pText) {
    if (*pText == '\'') {
      if (!isDecDigital(pText + 1)) { break; }
      n_separator ++; pText ++; continue;
    }
    if ('0' == *pText) { trialing_zeros ++; continue; }
    uint32_t rate = 10 * (1 + trialing_zeros);
    if ('1' <= *pText && *pText <= '9') {
      *value = ((*value) * rate) + (*pText - '0');
    } else { break; }
    trialing_zeros = 0;
    pText ++;
  }
  uint32_t length = pText - input;
  if (effective_length) { *effective_length = length - trialing_zeros - n_separator; }
  return length;
}
// (?=\.)[0-7]+
inline uint32_t t_FRAC_DIGITS_adic8(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pText = input;
  if (!isOctDigital(pText)) { return 0; }
  uint32_t trialing_zeros = 0;
  uint32_t n_separator = 0;
  while (*pText) {
    if (*pText == '\'') {
      if (!isOctDigital(pText + 1)) { break; }
      n_separator ++; pText ++; continue;
    }
    if ('0' == *pText) { trialing_zeros ++; continue; }
    uint32_t shift_count = 3 * (1 + trialing_zeros);
    if ('1' <= *pText && *pText <= '7') {
      *value = ((*value) << shift_count) + (*pText - '0');
    } else { break; }
    trialing_zeros = 0;
    pText ++;
  }
  uint32_t length = pText - input;
  if (effective_length) { *effective_length = length - trialing_zeros - n_separator; }
  return length;
}
// (?=\.)[01]+
inline uint32_t t_FRAC_DIGITS_adic2(const char_t *const input, uint32_t *effective_length, uint256_t *value) {
  const char_t *pText = input;
  if (!isBinDigital(pText)) { return 0; }
  uint32_t trialing_zeros = 0;
  uint32_t n_separator = 0;
  while (*pText) {
    if (*pText == '\'') {
      if (!isBinDigital(pText + 1)) { break; }
      n_separator ++; pText ++; continue;
    }
    if ('0' == *pText) { trialing_zeros ++; continue; }
    uint32_t shift_count = 1 + trialing_zeros;
    if ('1' == *pText) {
      *value = ((*value) << shift_count) + (*pText - '0');
    } else { break; }
    trialing_zeros = 0;
    pText ++;
  }
  uint32_t length = pText - input;
  if (effective_length) { *effective_length = length - trialing_zeros - n_separator; }
  return length;
}

#define ADIC_TYPE_16   0
#define ADIC_TYPE_10   1
#define ADIC_TYPE_8    2
#define ADIC_TYPE_2    3
#define INT_DIGITAL_FUNC      0
#define FRAC_DIGITAL_FUNC     1

typedef uint32_t tokenize_t(const char_t *, uint32_t *, uint256_t *);
static tokenize_t *const DIGITAL_FUNC_TOOLS[4][2] = {
    [ADIC_TYPE_16] = { [INT_DIGITAL_FUNC] = t_INT_DIGITS_adic16, [FRAC_DIGITAL_FUNC] = t_FRAC_DIGITS_adic16},
    [ADIC_TYPE_10] = { [INT_DIGITAL_FUNC] = t_INT_DIGITS_adic10, [FRAC_DIGITAL_FUNC] = t_FRAC_DIGITS_adic10},
    [ADIC_TYPE_8 ] = { [INT_DIGITAL_FUNC] = t_INT_DIGITS_adic8 , [FRAC_DIGITAL_FUNC] = t_FRAC_DIGITS_adic8 },
    [ADIC_TYPE_2 ] = { [INT_DIGITAL_FUNC] = t_INT_DIGITS_adic2 , [FRAC_DIGITAL_FUNC] = t_FRAC_DIGITS_adic2 },
};
static constexpr uint32_t ADIC_BASE[] = {
    [ADIC_TYPE_16] = 16,
    [ADIC_TYPE_10] = 10,
    [ADIC_TYPE_8 ] = 8,
    [ADIC_TYPE_2 ] = 2,
};

/*
 * if adic
 * is ADIC_TYPE_16:    [a-fA-F0-9]+((\.[a-fA-F0-9]+([pP][+-]?[0-9]+)?)?[lL]{0,2})|[lL]{0,2}[uU]?)
 * is ADIC_TYPE_10:    [01]+((\.[01]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2})|[lL]{0,2}[uU]?)
 * is ADIC_TYPE_8:     [0-7]+((\.[0-7]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2}|[lL]{0,2}[uU]?)
 * is ADIC_TYPE_2:     [0-9]+((\.[0-9]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2}|[lL]{0,2}[uU]?)
 */
uint32_t t_NUMBER(const char_t *const input, Terminal *const result,
                  const bool negative , const uint32_t adic, const Allocator *const allocator) {
  const char_t *pText = input;

  uint32_t size = 4;
  uint256_t integer = 0;
  uint256_t exponent = 0;
  uint32_t int_eff_length = 0;
  uint32_t frac_eff_length = 0;
  bool exp_negative = false;
  enum XLR_VAL_LITERAL_TYPE_ENUM type = XLR_VAL_LITERAL_INT;

  // TODO: supporting for special numbers: inf, Inf, INF, nan, Nan, NaN, NAN
  uint32_t length = DIGITAL_FUNC_TOOLS[adic][INT_DIGITAL_FUNC](pText, &int_eff_length, &integer);
  if (!length) { return 0; } else { pText += length; }
  if (*pText == '.') {
    type = XLR_VAL_LITERAL_FLOAT; size = 4; pText ++;
    length = DIGITAL_FUNC_TOOLS[adic][FRAC_DIGITAL_FUNC](pText, &frac_eff_length, &integer);
    if (!length) { return 0; }
    pText += length;
  }
  if (*pText == 'p' || *pText == 'P' || *pText == 'e' || *pText == 'E') {
    pText ++;
    exp_negative = (*pText == '-');
    if (isSign(pText)) { pText++; }
    length = DIGITAL_FUNC_TOOLS[ADIC_TYPE_10][INT_DIGITAL_FUNC](pText, nullptr, &exponent);
    if (!length) { return 0; }
    pText += length;
  }
  exponent = exp_negative ? -exponent : exponent;
  exponent += int_eff_length;
  exponent -= frac_eff_length;
  if ((*pText == 'l') || (*pText == 'L')) { size *= 2; pText++; }
  if ((*pText == 'l') || (*pText == 'L')) { size *= 2; pText++; }
  size = min(size, 16);
  if ((*pText == 'u') || (*pText == 'U')) {
    if (type == XLR_VAL_LITERAL_FLOAT) { return 0; }
    type = XLR_VAL_LITERAL_UINT; pText++;
  }
  if (isIdentChar(pText) || *pText == '.') { return 0; }

  LRValue *value = allocator->calloc(1, sizeof(LRValue));
  value->size = size;
  // TODO:
  //  The value obtained in this way is not accurate enough.
  //  Please try to improve the algorithm.
  if (type == XLR_VAL_LITERAL_FLOAT) {
    const uint32_t exponent_base = ADIC_BASE[adic];
    if (size == 4 ) {
      float32_t real = ((float32_t) (uint32_t) integer);
      float32_t exp = (float32_t) pow((float32_t) exponent_base, (int32_t) exponent - 1);
      value->val.F32 = (negative) ? -real * exp : real * exp;
      value->type = (REFER(LRType)) XLR_BUILTIN_TYPE_F32;
    } else if (size == 8 ) {
      float64_t real = ((float64_t) (uint64_t) integer);
      float64_t exp = (float64_t) pow((float64_t) exponent_base, (int64_t) exponent - 1);
      value->val.F64 = (negative) ? -real * exp : real * exp;
      value->type = (REFER(LRType)) XLR_BUILTIN_TYPE_F64;
    } else if (size == 16) {
      float128_t real = ((float128_t) (uint128_t) integer);
      float128_t exp = (float128_t) pow((float128_t) exponent_base, (int128_t) exponent - 1);
      value->val.F128 = (negative) ? -real * exp : real * exp;
      value->type = (REFER(LRType)) XLR_BUILTIN_TYPE_F128;
    } else { return 0; }
  } else if (type == XLR_VAL_LITERAL_UINT) {
    if (negative) { integer = -integer; }
    if (size == 4) { value->val.U32 = integer; value->type = (REFER(LRType)) XLR_BUILTIN_TYPE_U32; }
    else if (size == 8) { value->val.U64 = integer; value->type = (REFER(LRType)) XLR_BUILTIN_TYPE_U64; }
    else if (size == 16) { value->val.U128 = integer; value->type = (REFER(LRType)) XLR_BUILTIN_TYPE_U128; }
    else { return 0; }
  } else {
    if (negative) { integer = -integer; }
    if (size == 4) { value->val.I32 = integer; value->type = (REFER(LRType)) XLR_BUILTIN_TYPE_I32; }
    else if (size == 8) { value->val.I64 = integer; value->type = (REFER(LRType)) XLR_BUILTIN_TYPE_I64; }
    else if (size == 16) { value->val.I128 = integer; value->type = (REFER(LRType)) XLR_BUILTIN_TYPE_I128; }
    else { return 0; }
  }

  result->type = XLR_TOKEN_VAL_LITERAL;
  result->length = pText - input;
  result->value = value;
  return result->length;
}

uint32_t t_IDENTIFIER(const char_t * const input, Terminal * const result, const Allocator * const allocator) {
  const char_t *pText = input;
  bool builtin_ident = false;
  if (isIdentHeader(pText)) {
    pText++;
  } else if (isBuiltinIdentChar(pText)) {
    pText++; builtin_ident = true;
  } else {
    result->length = pText - input;
    return 0;
  }
  while (true) {
    if (isIdentChar(pText)) { pText++; }
    else if (isBuiltinIdentChar(pText)) { pText++; builtin_ident = true; }
    else { break; }
  }
  const uint32_t len = pText - input;
  result->type = builtin_ident ? XLR_TOKEN_IDENTIFIER : XLR_TOKEN_BUILTIN_IDENTIFIER;
  result->value = allocator->calloc(len + 1, sizeof(char_t));
  allocator->memcpy(result->value, input, len);
  ((char_t *) result->value)[len] = '\0';
  result->length = pText - input;
  return result->length;
}

#define fn_try_keyword(_kw, _type)                                                                      \
  uint32_t try_keyword_##_kw(const char_t * const input, uint32_t const offs, Terminal * const result,  \
                             uint64_t kw_as_ident, const Allocator * const allocator) {                 \
    if (kw_as_ident & XLR_KW_##_type) { goto __failed_kw_##_kw; }                                       \
    const char_t pattern[] = string_t(#_kw);                                                            \
    for (uint32_t i = offs; i < sizeof(pattern) - 1; i++) {                                             \
      if (input[i - offs] != pattern[i]) { goto __failed_kw_##_kw; }                                    \
    }                                                                                                   \
    const char_t * const tail = &input[sizeof(pattern) - 1 - offs];                                     \
    if (startswithLetter(tail) || *tail == '_') { goto __failed_kw_##_kw; }                             \
    result->type = XLR_TOKEN_##_type;                                                                   \
    result->value = nullptr;                                                                            \
    result->length = lenof(#_kw);                                                                       \
    return lenof(#_kw);                                                                                 \
    __failed_kw_##_kw : return t_IDENTIFIER(input - offs, result, allocator);                           \
  }
#define fn_try_keyword_val(_kw, _kw_type, _type, val)                                                   \
  uint32_t try_keyword_##_kw(const char_t * const input, uint32_t const offs, Terminal * const result,  \
                             uint64_t kw_as_ident, const Allocator * const allocator) {                 \
    if (kw_as_ident & XLR_KW_##_kw_type) { goto __failed_kw_##_kw; }                                    \
    const char_t pattern[] = string_t(#_kw);                                                            \
    for (uint32_t i = offs; i < sizeof(pattern) - 1; i++) {                                             \
      if (input[i - offs] != pattern[i]) { goto __failed_kw_##_kw; }                                    \
    }                                                                                                   \
    const char_t * const tail = &input[sizeof(pattern) - 1 - offs];                                     \
    if (startswithLetter(tail) || *tail == '_') { goto __failed_kw_##_kw; }                             \
    result->type = XLR_TOKEN_##_type;                                                                   \
    result->value = (void *) val;                                                                       \
    result->length = lenof(#_kw);                                                                       \
    return lenof(#_kw);                                                                                 \
    __failed_kw_##_kw : return t_IDENTIFIER(input - offs, result, allocator);                           \
  }

fn_try_keyword(if, IF)
fn_try_keyword(for, FOR)
fn_try_keyword(else, ELSE)
fn_try_keyword(enum, ENUM)
fn_try_keyword(rule, RULE)
fn_try_keyword(break, BREAK)
fn_try_keyword(while, WHILE)
fn_try_keyword(return, RETURN)
fn_try_keyword(typedef, TYPEDEF)
fn_try_keyword(continue, CONTINUE)
fn_try_keyword(attribute, ATTRIBUTE)
fn_try_keyword_val(union, UNION, TYPEHINT, XLR_CATEGORY_UNION)
fn_try_keyword_val(struct, STRUCT, TYPEHINT, XLR_CATEGORY_STRUCT)

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

#define tokenize_adic_number(text_off, adic_type) do {            \
  uint32_t length = t_NUMBER(pText + (text_off), result,          \
                             negative, (adic_type), allocator);   \
  result->length += pText - input + text_off;                     \
  return length ? result->length : 0;                             \
} while(false)

/*
 * [+-]?0[xX][a-fA-F0-9]+((\.[a-fA-F0-9]+([pP][+-]?[0-9]+)?)?[lL]{0,2})|[lL]{0,2}[uU]?) |
 * [+-]?0[bB][01]+((\.[01]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2})|[lL]{0,2}[uU]?) |
 * [+-]?0[oO]?[0-7]+((\.[0-7]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2}|[lL]{0,2}[uU]?) |
 * [+-]?[0-9]+((\.[0-9]+([pPeE][+-]?[0-9]+)?)?[lL]{0,2}|[lL]{0,2}[uU]?)
 */
uint32_t tokenize_number(const char_t * const input, Terminal * const result, const Allocator * const allocator) {
  const char_t *pText = input;
  bool negative = ('-' == *pText);
  if (isSign(pText)) { pText++; }
  if ('0' == *pText) {
    switch (pText[1]) {
      case 'x':
      case 'X': {
        tokenize_adic_number(2, ADIC_TYPE_16);
      }
      case 'o':
      case 'O': {
        tokenize_adic_number(2, ADIC_TYPE_8);
      }
      case 'b':
      case 'B': {
        tokenize_adic_number(2, ADIC_TYPE_2);
      }
      default: {}
    }
  }
  tokenize_adic_number(0, ADIC_TYPE_10);
}

// '.|\\[0-9a-zA-Z]+|\\.'
uint32_t tokenize_single_char(const char_t *const input, Terminal *const result, const Allocator *) {
  constexpr char_t SINGLE_CHAR_LITERALS[] = "nrtv";
  constexpr char_t SINGLE_CHAR_VALUE[] = "\n\r\t\v";
  constexpr uint32_t SINGLE_CHAR_COUNT = lenof(SINGLE_CHAR_LITERALS);
  const char_t *pText = input;
  if (!*pText || *pText == '\'') { return 0; }
  uint256_t the_char = 0;
  if (*pText == '\\') {
    pText++;
    uint32_t length = t_INT_DIGITS_adic16(pText, nullptr, &the_char);
    if (!length) {
      uint32_t index = stridx_o(*pText, SINGLE_CHAR_LITERALS);
      the_char = (index < SINGLE_CHAR_COUNT) ? SINGLE_CHAR_VALUE[index] : *pText;
      pText++;
    } else { pText += length; }
  } else {
    the_char = *pText; pText++;
  }
  if (*pText != '\'') { result->length = pText - input; return 0; } else { pText++; }

  result->value = (void *) (uint64_t) the_char;
  result->type = XLR_TOKEN_SINGLE_CHAR;
  result->length = pText - input + 1;
  return result->length;
}

// ${pred}.*${succ}
uint32_t tokenize_text(const char_t *const input, const uint32_t n_pred,
                       const char_t *const succ, const uint32_t n_succ,
                       Terminal *const result, const Allocator *const allocator) {
  const char_t *pText = input;
  while (*pText) {
    if (*pText == '\\') {
      if (!*pText++) { return 0; }
      pText++;
    }
    uint32_t length = strcmp_o(pText, succ);
    if (length == n_succ) { break; }
    if (!pText[length]) { return 0; }
    pText ++;
  }

  uint32_t content_length = pText - input;
  const uint32_t cost_length = n_pred + content_length + n_succ;
  WrapperedText *text = allocator->calloc(1, sizeof(WrapperedText));
  text->content = allocator->malloc((content_length + 1) * sizeof(char_t));
  text->length = 0;
  for (uint32_t i = 0; i < content_length; i++, text->length++) {
    // TODO: support for escape operations
    if (input[i] == '\\') { i++; }
    text->content[text->length] = input[i];
  }
  text->content[text->length] = '\0';
  text->n_pred = n_pred;
  text->n_succ = n_succ;

  result->type = XLR_TOKEN_TEXT;
  result->length = cost_length;
  result->value = text;

  return cost_length;
}

uint32_t tokenize_letter_i(const char_t * const input, Terminal * const result, uint64_t kw_as_ident, const Allocator * const allocator) {
  switch (*input) {
    case 'f': {
      return try_keyword_if(input + 1, 2, result, kw_as_ident, allocator);
    }
    default: fn_fall_through(1);
  }
}
uint32_t tokenize_letter_f(const char_t * const input, Terminal * const result, uint64_t kw_as_ident, const Allocator * const allocator) {
  switch (*input) {
    case 'o': {
      return try_keyword_for(input + 1, 2, result, kw_as_ident, allocator);
    }
    default: fn_fall_through(1);
  }
}
uint32_t tokenize_letter_e(const char_t * const input, Terminal * const result, uint64_t kw_as_ident, const Allocator * const allocator) {
  switch (*input) {
    case 'n': {
      return try_keyword_enum(input + 1, 2, result, kw_as_ident, allocator);
    }
    case 'l': {
      return try_keyword_else(input + 1, 2, result, kw_as_ident, allocator);
    }
    default: fn_fall_through(1);
  }
}
uint32_t tokenize_letter_t(const char_t * const input, Terminal * const result, uint64_t kw_as_ident, const Allocator * const allocator) {
  switch (*input) {
    case 'o': {
      return try_keyword_typedef(input + 1, 2, result, kw_as_ident, allocator);
    }
    default: fn_fall_through(1);
  }
}
uint32_t tokenize_letter_w(const char_t * const input, Terminal * const result, uint64_t kw_as_ident, const Allocator * const allocator) {
  switch (*input) {
    case 'h': {
      return try_keyword_while(input + 1, 2, result, kw_as_ident, allocator);
    }
    default: fn_fall_through(1);
  }
}
uint32_t tokenize_letter_s(const char_t * const input, Terminal * const result, uint64_t kw_as_ident, const Allocator * const allocator) {
  switch (*input) {
    case 't': {
      return try_keyword_struct(input + 1, 2, result, kw_as_ident, allocator);
    }
    default: fn_fall_through(1);
  }
}

uint32_t tokenize_symbol_OR(const char_t *input, Terminal *result, const Allocator *) {
  const char_t *pText = input;
  if (*pText == '|') {
    result->type = XLR_TOKEN_COND_BIN_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_BIT_BIN_OR;
    result->length = 2;
  } else {
    result->type = XLR_TOKEN_ARITH_2_BIN_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_BOOL_BIN_OR;
    result->length = 1;
  }
  return result->length;
}
uint32_t tokenize_symbol_AND(const char_t *input, Terminal *result, const Allocator *) {
  const char_t *pText = input;
  if (*pText == '&') {
    result->type = XLR_TOKEN_COND_BIN_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_BIT_BIN_AND;
    result->length = 2;
  } else {
    result->type = XLR_TOKEN_ARITH_2_BIN_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_BOOL_BIN_AND;
    result->length = 1;
  }
  return result->length;
}
uint32_t tokenize_symbol_EQUAL(const char_t *input, Terminal *result, const Allocator *) {
  const char_t *pText = input;
  if (*pText == '=') {
    result->type = XLR_TOKEN_COMPARE_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_COMPARE_EQ;
    result->length = 2;
  } else {
    result->type = XLR_TOKEN_ASSIGNER;
    result->value = nullptr;
    result->length = 1;
  }
  return result->length;
}
uint32_t tokenize_symbol_GT(const char_t *input, Terminal *result, const Allocator *) {
  const char_t *pText = input;
  if (*pText == '=') {
    result->type = XLR_TOKEN_COMPARE_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_COMPARE_GE;
    result->length = 2;
  } else if (*pText == '>') {
    result->type = XLR_TOKEN_ARITH_2_BIN_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_BIT_BIN_RSH;
    result->length = 2;
  } else {
    result->type = XLR_TOKEN_COMPARE_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_COMPARE_GT;
    result->length = 1;
  }
  return result->length;
}
uint32_t tokenize_symbol_LT(const char_t *input, Terminal *result, const Allocator *) {
  const char_t *pText = input;
  if (*pText == '=') {
    result->type = XLR_TOKEN_COMPARE_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_COMPARE_LE;
    result->length = 2;
  } else if (*pText == '<') {
    result->type = XLR_TOKEN_ARITH_2_BIN_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_BIT_BIN_LSH;
    result->length = 2;
  } else {
    result->type = XLR_TOKEN_COMPARE_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_COMPARE_LT;
    result->length = 1;
  }
  return result->length;
}
uint32_t tokenize_symbol_NOT(const char_t *input, Terminal *result, const Allocator *) {
  const char_t *pText = input;
  if (*pText == '=') {
    result->type = XLR_TOKEN_COMPARE_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_COMPARE_NE;
    result->length = 2;
  } else {
    result->type = XLR_TOKEN_COND_SIN_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_BOOL_SIN_NOT;
    result->length = 1;
  }
  return result->length;
}
uint32_t tokenize_symbol_PLUS(const char_t *input, Terminal *result, const Allocator *) {
  const char_t *pText = input;
  if (*pText == '+') {
    result->type = XLR_TOKEN_INTEGRATED_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_INTEGRATE_INC;
    result->length = 2;
  } else {
    result->type = XLR_TOKEN_ARITH_0_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_ARITH_BIN_ADD;
    result->length = 1;
  }
  return result->length;
}
uint32_t tokenize_symbol_MINUS(const char_t *input, Terminal *result, const Allocator *) {
  const char_t *pText = input;
  if (*pText == '-') {
    result->type = XLR_TOKEN_INTEGRATED_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_INTEGRATE_DEC;
    result->length = 2;
  } else {
    result->type = XLR_TOKEN_ARITH_0_OP;
    result->value = (void *) (uint64_t) XLR_BUILTIN_FUNC_ARITH_BIN_SUB;
    result->length = 1;
  }
  return result->length;
}
uint32_t tokenize_symbol_COLON(const char_t *input, Terminal *result, const Allocator *) {
  const char_t *pText = input;
  if (*pText == ':') {
    result->type = XLR_TOKEN_DOUBLE_COLON;
    result->value = nullptr;
    result->length = 2;
  } else {
    result->type = XLR_TOKEN_COLON;
    result->value = nullptr;
    result->length = 1;
  }
  return result->length;
}

const struct {
  uint32_t t_type;
  uint32_t a_type;
} ARITH_SYM_TYPE_LITERALS[] = {
  {XLR_TOKEN_ARITH_1_BIN_OP, XLR_BUILTIN_FUNC_ARITH_BIN_MUL},
  {XLR_TOKEN_ARITH_1_BIN_OP, XLR_BUILTIN_FUNC_ARITH_BIN_DIV},
  {XLR_TOKEN_ARITH_1_BIN_OP, XLR_BUILTIN_FUNC_ARITH_BIN_MOD},
  {XLR_TOKEN_ARITH_2_BIN_OP, XLR_BUILTIN_FUNC_BIT_BIN_XOR},
  {XLR_TOKEN_ARITH_2_SIN_OP, XLR_BUILTIN_FUNC_BIT_SIN_INV},
};
uint32_t tokenize_arith_single_symbols(const char_t * const input, Terminal * const result, const Allocator * const) {
  constexpr char_t ARITH_SYM_LITERALS[] = "*/%^~";
  uint32_t length = stridx_o(*input, ARITH_SYM_LITERALS);
  if (length < lenof(ARITH_SYM_LITERALS)) {
    result->type = ARITH_SYM_TYPE_LITERALS[length].t_type;
    result->value = (void *) (uint64_t) ARITH_SYM_TYPE_LITERALS[length].a_type;
    result->length = 1;
    return 1;
  }
  return 0;
}
constexpr uint32_t ACTION_TERMINAL_TYPE_LITERALS[] = {
  XLR_TOKEN_AT,
  XLR_TOKEN_DOT,
  XLR_TOKEN_COMMA,
  XLR_TOKEN_SEMICOLON,
  XLR_TOKEN_QUESTION_MARK,

  XLR_TOKEN_LEFT_BRACKET,
  XLR_TOKEN_RIGHT_BRACKET,
  XLR_TOKEN_LEFT_SQUARE_BRACKET,
  XLR_TOKEN_RIGHT_SQUARE_BRACKET,
  XLR_TOKEN_LEFT_PARENTHESIS,
  XLR_TOKEN_RIGHT_PARENTHESIS,
};
uint32_t tokenize_action_single_symbol(const char_t * const input, Terminal * const result, const Allocator * const) {
  constexpr char_t ACTION_SINGLE_LITERAL[] = "@.,;?{}[]()";
  uint32_t length = stridx_o(*input, ACTION_SINGLE_LITERAL);
  if (length < lenof(ACTION_SINGLE_LITERAL)) {
    result->type = ACTION_TERMINAL_TYPE_LITERALS[length];
    result->value = nullptr;
    result->length = 1;
    return 1;
  }
  return 0;
}
constexpr uint32_t PATTERN_TERMINAL_TYPE_LITERALS[] = {
  XLR_TOKEN_LEFT_BRACKET,
  XLR_TOKEN_RIGHT_BRACKET,
  XLR_TOKEN_LEFT_SQUARE_BRACKET,
  XLR_TOKEN_RIGHT_SQUARE_BRACKET,
  XLR_TOKEN_LEFT_PARENTHESIS,
  XLR_TOKEN_RIGHT_PARENTHESIS,
};
uint32_t tokenize_pattern_single_symbol(const char_t * const input, Terminal * const result, const Allocator * const) {
  constexpr char_t PATTERN_SINGLE_LITERAL[] = "{}[]()";
  uint32_t length = stridx_o(*input, PATTERN_SINGLE_LITERAL);
  if (length < lenof(PATTERN_SINGLE_LITERAL)) {
    result->type = PATTERN_TERMINAL_TYPE_LITERALS[length];
    result->value = nullptr;
    result->length = 1;
    return 1;
  }
  return 0;
}

uint32_t action_single_tokenize(const char_t * const input, Terminal * const result,
                                const uint64_t kw_as_ident, const Allocator * const allocator) {
  if (!*input) {
    result->type = XLR_TOKEN_TERMINATOR;
    result->value = nullptr;
    result->length = 0;
    return 0;
  }
  switch (*input) {
    case 'a': { return try_keyword_attribute(input + 1, 1, result, kw_as_ident, allocator); }
    case 'e': { return tokenize_letter_e(input + 1, result, kw_as_ident, allocator); }
    case 'f': { return tokenize_letter_f(input + 1, result, kw_as_ident, allocator); }
    case 'i': { return tokenize_letter_i(input + 1, result, kw_as_ident, allocator); }
    case 't': { return tokenize_letter_t(input + 1, result, kw_as_ident, allocator); }
    case 's': { return tokenize_letter_s(input + 1, result, kw_as_ident, allocator); }
    case 'u': { return try_keyword_union(input + 1, 1, result, kw_as_ident, allocator); }
    case 'w': { return tokenize_letter_w(input + 1, result, kw_as_ident, allocator); }
    case '|': { return tokenize_symbol_OR(input + 1, result, allocator); }
    case '&': { return tokenize_symbol_AND(input + 1, result, allocator); }
    case '=': { return tokenize_symbol_EQUAL(input + 1, result, allocator); }
    case '>': { return tokenize_symbol_GT(input + 1, result, allocator); }
    case '<': { return tokenize_symbol_LT(input + 1, result, allocator); }
    case '!': { return tokenize_symbol_NOT(input + 1, result, allocator); }
    case '+': { return tokenize_symbol_PLUS(input + 1, result, allocator); }
    case '-': { return tokenize_symbol_MINUS(input + 1, result, allocator); }
    case ':': { return tokenize_symbol_COLON(input + 1, result, allocator); }
    case '"': { return tokenize_text(input + 1, 1, "\"", 1, result, allocator); }
    case '\'':{ return tokenize_single_char(input + 1, result, allocator); }
    default: {}
  }
  uint32_t length = 0;
  if (isDecDigital(input)) {
    length = tokenize_number(input, result, allocator);
    return length;
  }
  length = tokenize_action_single_symbol(input, result, allocator);
  if (length > 0) { return length; }
  length = tokenize_arith_single_symbols(input, result, allocator);
  if (length > 0) { return length; }
  length = t_IDENTIFIER(input, result, allocator);
  if (length > 0) { return length; }
  result->type = XLR_TOKEN_BAD_TOKEN;
  result->value = nullptr;
  result->length = 0;
  return 0;
}

uint32_t pattern_single_tokenize(const char_t * const input, Terminal * const result,
                                 const uint64_t, const Allocator * const allocator) {
  if (!*input) {
    result->type = XLR_TOKEN_TERMINATOR;
    result->value = nullptr;
    result->length = 0;
    return 0;
  }
  switch (*input) {
    case '\'': return tokenize_single_char(input + 1, result, allocator);
    case '"': return tokenize_text(input + 1, 1, "\"", 1, result, allocator);
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
  uint32_t length = tokenize_pattern_single_symbol(input, result, allocator);
  if (length > 0) { return length; }
  length = t_IDENTIFIER(input, result, allocator);
  if (length > 0) { return length; }
  result->type = XLR_TOKEN_BAD_TOKEN;
  result->value = nullptr;
  result->length = 0;
  return 0;
}

uint32_t try_pass_comment(const char * const input, uint32_t * const lineno, uint32_t * const column) {
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

void terminal2Token(const Terminal *terminal, Token *token) {
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
