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
 * Module Name: test/tokenize
 * Filename: test-tokenize.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-03
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "test-tokenize.h"
#include "test/test.h"
#include "tokenize/tokenize.h"
#include "xLR/tokens.h"
#include "string_t.h"
#include "xLR/types.h"
#include "enum.h"
#include <math.h>
#include <string.h>

typedef uint32_t tokenize_t(const char_t *, Terminal *, const Allocator *);

#define NEW_TEST(test_name) uint32_t test_name(tokenize_t *tokenize, const char_t *__tokeinze_name)
#define TEST_START(_input)                                    \
  uint32_t n_failed = 0;;                                     \
  const char_t *const input = _input;                         \
  const uint32_t input_len = strlen(input);                   \
  Terminal result = {};                                       \
  uint32_t length = tokenize(input, &result, &STDAllocator);  \

#define TEST_END \
  if (result.type == XLR_TOKEN_IDENTIFIER && result.value) {  \
  STDAllocator.free(result.value);                            \
  }                                                           \
  if (!n_failed) {                                            \
    fprintf(stdout, "test for '%s' passed.\n", __FUNCTION__); \
  }                                                           \
  return n_failed;

#define test_assert(expr)                               \
  do {                                                  \
    if (!(expr)) {                                      \
      fprintf(stderr, "assert failed in %s:%u, "        \
                      "by tokenizer '%s'\n",            \
              __FILE__, __LINE__, __tokeinze_name);     \
      fprintf(stderr, "when try check: '%s'\n", #expr); \
      n_failed++;                                       \
    }                                                   \
  } while (false)

#define test_assert_tokenize_success() test_assert((length == input_len) && result.type != XLR_TOKEN_BAD_TOKEN)


NEW_TEST(IDENTIFIER_lower_letters) {
  TEST_START("acdoqbdpgqdfnpoqibda")
  test_assert_tokenize_success();
  test_assert(result.length == length);
  test_assert(result.type == XLR_TOKEN_IDENTIFIER);
  test_assert(strcmp_o(result.value, input) == input_len);
  TEST_END
}


NEW_TEST(IDENTIFIER_upper_letters) {
  TEST_START("AOUBNPQUVBNKSDF")
  test_assert_tokenize_success();
  test_assert(result.length == length);
  test_assert(result.type == XLR_TOKEN_IDENTIFIER);
  test_assert(strcmp_o(result.value, input) == input_len);
  TEST_END
}


NEW_TEST(IDENTIFIER_tailed_numbers_0) {
  TEST_START("adfa123456")
  test_assert_tokenize_success();
  test_assert(result.length == length);
  test_assert(result.type == XLR_TOKEN_IDENTIFIER);
  test_assert(strcmp_o(result.value, input) == input_len);
  TEST_END
}
NEW_TEST(IDENTIFIER_tailed_numbers_1) {
  TEST_START("UIBNP123456")
  test_assert_tokenize_success();
  test_assert(result.length == length);
  test_assert(result.type == XLR_TOKEN_IDENTIFIER);
  test_assert(strcmp_o(result.value, input) == input_len);
  TEST_END
}

NEW_TEST(IDENTIFIER_startswith_dash_0) {
  TEST_START("_alkjHPNqg")
  test_assert_tokenize_success();
  test_assert(result.length == length);
  test_assert(result.type == XLR_TOKEN_IDENTIFIER);
  test_assert(strcmp_o(result.value, input) == input_len);
  TEST_END
}
NEW_TEST(IDENTIFIER_startswith_dash_1) {
  TEST_START("_12345")
  test_assert_tokenize_success();
  test_assert(result.length == length);
  test_assert(result.type == XLR_TOKEN_IDENTIFIER);
  test_assert(strcmp_o(result.value, input) == input_len);
  TEST_END
}

#define NEW_INTEGER_TEST(sig, adic, _size, str, _value, suffix) \
NEW_TEST(INTEGER_##sig##_##_size##_##adic##_##suffix) {         \
  TEST_START(str)                                               \
  test_assert_tokenize_success();                               \
  test_assert(result.length == length);                         \
  test_assert(result.type == XLR_TOKEN_INTEGER);                \
  LRValue *val = result.value;                                  \
  test_assert(val->type == XLR_VAL_INTEGER_##sig);              \
  test_assert(val->size == _size);                              \
  test_assert(((uint64_t) val->bytes) == _value);               \
  TEST_END                                                      \
}
NEW_INTEGER_TEST(  SIGNED, adic16, 4, "0x12AB5" , 0x12AB5 , 0)
NEW_INTEGER_TEST(  SIGNED, adic16, 4, "0X12345" , 0x12345 , 1)
NEW_INTEGER_TEST(  SIGNED, adic10, 4, "1234567" , 1234567 , 0)
NEW_INTEGER_TEST(  SIGNED, adic8 , 4, "0o12345" ,  012345 , 0)
NEW_INTEGER_TEST(  SIGNED, adic8 , 4, "0O12345" ,  012345 , 1)
NEW_INTEGER_TEST(  SIGNED, adic8 , 4, "012345"  ,  012345 , 2)
NEW_INTEGER_TEST(  SIGNED, adic2 , 4, "0b01001" , 0b01001 , 0)
NEW_INTEGER_TEST(  SIGNED, adic2 , 4, "0B01001" , 0b01001 , 1)
NEW_INTEGER_TEST(UNSIGNED, adic16, 4, "0x12ab5u", 0x12ab5u, 0)
NEW_INTEGER_TEST(UNSIGNED, adic10, 4, "1234567u", 1234567u, 0)
NEW_INTEGER_TEST(UNSIGNED, adic8 , 4, "0o12345u",  012345u, 0)
NEW_INTEGER_TEST(UNSIGNED, adic2 , 4, "0B01001u", 0b01001u, 0)
NEW_INTEGER_TEST(UNSIGNED, adic16, 4, "0x12345U", 0x12345u, 1)
NEW_INTEGER_TEST(UNSIGNED, adic10, 4, "1234567U", 1234567u, 1)
NEW_INTEGER_TEST(UNSIGNED, adic8 , 4, "0o12345U",  012345u, 1)
NEW_INTEGER_TEST(UNSIGNED, adic2 , 4, "0B01001U", 0b01001u, 1)
NEW_INTEGER_TEST(  SIGNED, adic16, 8, "0x12AB5l" , 0x12AB5l, 0)
NEW_INTEGER_TEST(  SIGNED, adic16, 8, "0X12345l" , 0x12345l, 1)
NEW_INTEGER_TEST(  SIGNED, adic10, 8, "1234567l" , 1234567l, 0)
NEW_INTEGER_TEST(  SIGNED, adic8 , 8, "0o12345l" ,  012345l, 0)
NEW_INTEGER_TEST(  SIGNED, adic8 , 8, "0O12345l" ,  012345l, 1)
NEW_INTEGER_TEST(  SIGNED, adic8 , 8, "012345L"  ,  012345l, 2)
NEW_INTEGER_TEST(  SIGNED, adic2 , 8, "0b01001L" , 0b01001l, 0)
NEW_INTEGER_TEST(  SIGNED, adic2 , 8, "0B01001L" , 0b01001l, 1)
NEW_INTEGER_TEST(UNSIGNED, adic16, 8, "0x12ab5lu", 0x12ab5u, 0)
NEW_INTEGER_TEST(UNSIGNED, adic10, 8, "1234567lu", 1234567u, 0)
NEW_INTEGER_TEST(UNSIGNED, adic8 , 8, "0o12345lu",  012345u, 0)
NEW_INTEGER_TEST(UNSIGNED, adic2 , 8, "0B01001Lu", 0b01001u, 0)
NEW_INTEGER_TEST(UNSIGNED, adic16, 8, "0x12345lU", 0x12345u, 1)
NEW_INTEGER_TEST(UNSIGNED, adic10, 8, "1234567lU", 1234567u, 1)
NEW_INTEGER_TEST(UNSIGNED, adic8 , 8, "0o12345lU",  012345u, 1)
NEW_INTEGER_TEST(UNSIGNED, adic2 , 8, "0B01001lU", 0b01001u, 1)

#define NEW_FLOAT_TEST(_size, str, _value, suffix)  \
NEW_TEST(FLOAT_##_size##_##suffix) {                \
  TEST_START(str)                                   \
  test_assert_tokenize_success();                   \
  test_assert(result.length == length);             \
  test_assert(result.type == XLR_TOKEN_FLOAT);      \
  LRValue *val = result.value;                      \
  test_assert(val->type == XLR_VAL_FLOAT);          \
  test_assert(val->size == _size);                  \
  long double value = _value;                       \
  test_assert(val->bytes = *(void **) &value);      \
  TEST_END                                          \
}

NEW_FLOAT_TEST(4 , "912.128f", 912.128L, 0)
NEW_FLOAT_TEST(8 , "912.1281934876", 912.1281934876L, 0)
NEW_FLOAT_TEST(16, "912.128193981734134134876L", 912.128193981734134134876L, 0)
NEW_FLOAT_TEST(4 , "3.402823E+38F", 3.402823E+38L, 1)
NEW_FLOAT_TEST(8 , "0xc12.128d3be19p56", 0xc12.128d3be19p56L, 1)
NEW_FLOAT_TEST(16, "0xc12.1281939817p78l", 0xc12.1281939817p78L, 1)
NEW_FLOAT_TEST(4 , "912.128e-21f", 912.128e-21L, 2)
NEW_FLOAT_TEST(8 , "0Xc12.128d3be19p-48", 0xc12.128d3be19p-48L, 2)
NEW_FLOAT_TEST(16, "0xc12.1281939817p-87l", 0xc12.1281939817p-87L, 2)

#define NEW_SYMBOL_TEST(name, str, val, suffix) \
NEW_TEST(SYMBOL_##name##_##suffix) {            \
  TEST_START(str)                               \
  test_assert_tokenize_success();               \
  test_assert(result.length == length);         \
  test_assert(result.type == XLR_TOKEN_##name); \
  test_assert(result.value == (val));           \
  TEST_END                                      \
}

NEW_SYMBOL_TEST(IF, "if", nullptr, 0)
NEW_SYMBOL_TEST(FOR, "for", nullptr, 0)
NEW_SYMBOL_TEST(ELSE, "else", nullptr, 0)
NEW_SYMBOL_TEST(ENUM, "enum", nullptr, 0)
NEW_SYMBOL_TEST(TOKEN, "token", nullptr, 0)
NEW_SYMBOL_TEST(WHILE, "while", nullptr, 0)

NEW_SYMBOL_TEST(AT, "@", nullptr, 0)
NEW_SYMBOL_TEST(DOT, ".", nullptr, 0)
NEW_SYMBOL_TEST(COMMA, ",", nullptr, 0)
NEW_SYMBOL_TEST(COLON, ":", nullptr, 0)
NEW_SYMBOL_TEST(ASSIGNER, "=", nullptr, 0)
NEW_SYMBOL_TEST(SEMICOLON, ";", nullptr, 0)
NEW_SYMBOL_TEST(QUESTION_MARK, "?", nullptr, 0)

NEW_SYMBOL_TEST(LEFT_BRACKET, "{", nullptr, 0)
NEW_SYMBOL_TEST(RIGHT_BRACKET, "}", nullptr, 0)
NEW_SYMBOL_TEST(LEFT_PARENTHESIS, "(", nullptr, 0)
NEW_SYMBOL_TEST(RIGHT_PARENTHESIS, ")", nullptr, 0)
NEW_SYMBOL_TEST(LEFT_SQUARE_BRACKET, "[", nullptr, 0)
NEW_SYMBOL_TEST(RIGHT_SQUARE_BRACKET, "]", nullptr, 0)


#define NEW_OPERATOR_TEST(_type, val, str, suffix)      \
NEW_TEST(OPERATOR_##_type##_##suffix) {        \
  TEST_START(str)                                       \
  test_assert_tokenize_success();                       \
  test_assert(result.length == length);                 \
  test_assert(result.type == XLR_TOKEN_##_type);        \
  test_assert(result.value = (void *) (uint64_t) val);  \
  TEST_END                                              \
}

NEW_OPERATOR_TEST(COND_BIN_OP, XLR_CB_OR , "||", OR)
NEW_OPERATOR_TEST(COND_BIN_OP, XLR_CB_AND, "&&", AND)
NEW_OPERATOR_TEST(COND_SIN_OP, XLR_CS_NOT, "!", NOT)
NEW_OPERATOR_TEST(COMPARE_OP, XLR_COMP_EQ, "==", EQ)
NEW_OPERATOR_TEST(COMPARE_OP, XLR_COMP_NE, "!=", NE)
NEW_OPERATOR_TEST(COMPARE_OP, XLR_COMP_GE, ">=", GE)
NEW_OPERATOR_TEST(COMPARE_OP, XLR_COMP_LE, "<=", LE)
NEW_OPERATOR_TEST(COMPARE_OP, XLR_COMP_GT, ">", GT)
NEW_OPERATOR_TEST(COMPARE_OP, XLR_COMP_LT, "<", LT)
NEW_OPERATOR_TEST(ARITH_0_OP, XLR_AB_ADD, "+", ADD)
NEW_OPERATOR_TEST(ARITH_0_OP, XLR_AB_SUB, "-", SUB)
NEW_OPERATOR_TEST(ARITH_1_BIN_OP, XLR_AB_MUL, "*", MUL)
NEW_OPERATOR_TEST(ARITH_1_BIN_OP, XLR_AB_DIV, "/", DIV)
NEW_OPERATOR_TEST(ARITH_1_BIN_OP, XLR_AB_MOD, "%", MOD)
NEW_OPERATOR_TEST(ARITH_2_BIN_OP, XLR_AB_XOR, "^", XOR)
NEW_OPERATOR_TEST(ARITH_2_SIN_OP, XLR_AS_INV, "~", INV)
NEW_OPERATOR_TEST(ARITH_2_BIN_OP, XLR_AB_OR , "|", OR )
NEW_OPERATOR_TEST(ARITH_2_BIN_OP, XLR_AB_AND, "&", AND)
NEW_OPERATOR_TEST(ARITH_2_BIN_OP, XLR_AB_RSH, ">>", RSH)
NEW_OPERATOR_TEST(ARITH_2_BIN_OP, XLR_AB_LSH, "<<", LSH)
NEW_OPERATOR_TEST(INTEGRATED_OP, XLR_IA_INC, "++", INC)
NEW_OPERATOR_TEST(INTEGRATED_OP, XLR_IA_DEC, "--", DEC)

NEW_SYMBOL_TEST(BUILTIN_FUNCTION, "sizeof", XLR_FUN_SIZEOF, SIZEOF)

uint32_t test_action_tokenize() {
  uint32_t n_failed = 0;

  add_test(IDENTIFIER_lower_letters, action_single_tokenize);
  add_test(IDENTIFIER_upper_letters, action_single_tokenize);
  add_test(IDENTIFIER_tailed_numbers_0, action_single_tokenize);
  add_test(IDENTIFIER_tailed_numbers_1, action_single_tokenize);
  add_test(IDENTIFIER_startswith_dash_0, action_single_tokenize);
  add_test(IDENTIFIER_startswith_dash_1, action_single_tokenize);
  add_test(  INTEGER_SIGNED_4_adic16_0, action_single_tokenize);
  add_test(  INTEGER_SIGNED_4_adic16_1, action_single_tokenize);
  add_test(  INTEGER_SIGNED_4_adic10_0, action_single_tokenize);
  add_test(  INTEGER_SIGNED_4_adic8_0 , action_single_tokenize);
  add_test(  INTEGER_SIGNED_4_adic8_1 , action_single_tokenize);
  add_test(  INTEGER_SIGNED_4_adic8_2 , action_single_tokenize);
  add_test(  INTEGER_SIGNED_4_adic2_0 , action_single_tokenize);
  add_test(  INTEGER_SIGNED_4_adic2_1 , action_single_tokenize);
  add_test(INTEGER_UNSIGNED_4_adic16_0, action_single_tokenize);
  add_test(INTEGER_UNSIGNED_4_adic16_1, action_single_tokenize);
  add_test(INTEGER_UNSIGNED_4_adic10_0, action_single_tokenize);
  add_test(INTEGER_UNSIGNED_4_adic10_1, action_single_tokenize);
  add_test(INTEGER_UNSIGNED_4_adic8_0 , action_single_tokenize);
  add_test(INTEGER_UNSIGNED_4_adic8_1 , action_single_tokenize);
  add_test(INTEGER_UNSIGNED_4_adic2_0 , action_single_tokenize);
  add_test(INTEGER_UNSIGNED_4_adic2_1 , action_single_tokenize);
  add_test(  INTEGER_SIGNED_8_adic16_0, action_single_tokenize);
  add_test(  INTEGER_SIGNED_8_adic16_1, action_single_tokenize);
  add_test(  INTEGER_SIGNED_8_adic10_0, action_single_tokenize);
  add_test(  INTEGER_SIGNED_8_adic8_0 , action_single_tokenize);
  add_test(  INTEGER_SIGNED_8_adic8_1 , action_single_tokenize);
  add_test(  INTEGER_SIGNED_8_adic8_2 , action_single_tokenize);
  add_test(  INTEGER_SIGNED_8_adic2_0 , action_single_tokenize);
  add_test(  INTEGER_SIGNED_8_adic2_1 , action_single_tokenize);
  add_test(INTEGER_UNSIGNED_8_adic16_0, action_single_tokenize);
  add_test(INTEGER_UNSIGNED_8_adic16_1, action_single_tokenize);
  add_test(INTEGER_UNSIGNED_8_adic10_0, action_single_tokenize);
  add_test(INTEGER_UNSIGNED_8_adic10_1, action_single_tokenize);
  add_test(INTEGER_UNSIGNED_8_adic8_0 , action_single_tokenize);
  add_test(INTEGER_UNSIGNED_8_adic8_1 , action_single_tokenize);
  add_test(INTEGER_UNSIGNED_8_adic2_0 , action_single_tokenize);
  add_test(INTEGER_UNSIGNED_8_adic2_1 , action_single_tokenize);

  add_test(FLOAT_4_0, action_single_tokenize);
  add_test(FLOAT_4_1, action_single_tokenize);
  add_test(FLOAT_4_2, action_single_tokenize);
  add_test(FLOAT_8_0, action_single_tokenize);
  add_test(FLOAT_8_1, action_single_tokenize);
  add_test(FLOAT_8_2, action_single_tokenize);
  add_test(FLOAT_16_0, action_single_tokenize);
  add_test(FLOAT_16_1, action_single_tokenize);
  add_test(FLOAT_16_2, action_single_tokenize);
  add_test(SYMBOL_IF_0, action_single_tokenize);
  add_test(SYMBOL_FOR_0, action_single_tokenize);
  add_test(SYMBOL_ELSE_0, action_single_tokenize);
  add_test(SYMBOL_ENUM_0, action_single_tokenize);
  add_test(SYMBOL_TOKEN_0, action_single_tokenize);
  add_test(SYMBOL_WHILE_0, action_single_tokenize);
  add_test(SYMBOL_AT_0, action_single_tokenize);
  add_test(SYMBOL_DOT_0, action_single_tokenize);
  add_test(SYMBOL_COMMA_0, action_single_tokenize);
  add_test(SYMBOL_COLON_0, action_single_tokenize);
  add_test(SYMBOL_ASSIGNER_0, action_single_tokenize);
  add_test(SYMBOL_SEMICOLON_0, action_single_tokenize);
  add_test(SYMBOL_QUESTION_MARK_0, action_single_tokenize);
  add_test(SYMBOL_LEFT_BRACKET_0, action_single_tokenize);
  add_test(SYMBOL_RIGHT_BRACKET_0, action_single_tokenize);
  add_test(SYMBOL_LEFT_PARENTHESIS_0, action_single_tokenize);
  add_test(SYMBOL_RIGHT_PARENTHESIS_0, action_single_tokenize);
  add_test(SYMBOL_LEFT_SQUARE_BRACKET_0, action_single_tokenize);
  add_test(SYMBOL_RIGHT_SQUARE_BRACKET_0, action_single_tokenize);

  add_test(OPERATOR_COND_BIN_OP_OR, action_single_tokenize);
  add_test(OPERATOR_COND_BIN_OP_AND, action_single_tokenize);
  add_test(OPERATOR_COND_SIN_OP_NOT, action_single_tokenize);
  add_test(OPERATOR_COMPARE_OP_EQ, action_single_tokenize);
  add_test(OPERATOR_COMPARE_OP_NE, action_single_tokenize);
  add_test(OPERATOR_COMPARE_OP_GE, action_single_tokenize);
  add_test(OPERATOR_COMPARE_OP_LE, action_single_tokenize);
  add_test(OPERATOR_COMPARE_OP_GT, action_single_tokenize);
  add_test(OPERATOR_COMPARE_OP_LT, action_single_tokenize);
  add_test(OPERATOR_ARITH_0_OP_ADD, action_single_tokenize);
  add_test(OPERATOR_ARITH_0_OP_SUB, action_single_tokenize);
  add_test(OPERATOR_ARITH_1_BIN_OP_MUL, action_single_tokenize);
  add_test(OPERATOR_ARITH_1_BIN_OP_DIV, action_single_tokenize);
  add_test(OPERATOR_ARITH_1_BIN_OP_MOD, action_single_tokenize);
  add_test(OPERATOR_ARITH_2_BIN_OP_XOR, action_single_tokenize);
  add_test(OPERATOR_ARITH_2_SIN_OP_INV, action_single_tokenize);
  add_test(OPERATOR_ARITH_2_BIN_OP_OR, action_single_tokenize );
  add_test(OPERATOR_ARITH_2_BIN_OP_AND, action_single_tokenize);
  add_test(OPERATOR_ARITH_2_BIN_OP_RSH, action_single_tokenize);
  add_test(OPERATOR_ARITH_2_BIN_OP_LSH, action_single_tokenize);
  add_test(OPERATOR_INTEGRATED_OP_INC, action_single_tokenize);
  add_test(OPERATOR_INTEGRATED_OP_DEC, action_single_tokenize);

  add_test(SYMBOL_BUILTIN_FUNCTION_SIZEOF, action_single_tokenize);

  return n_failed;
}

uint32_t test_pattern_tokenize() {
  uint32_t n_failed = 0;
  add_test(IDENTIFIER_lower_letters, pattern_single_tokenize);
  add_test(IDENTIFIER_upper_letters, pattern_single_tokenize);
  add_test(IDENTIFIER_tailed_numbers_0, pattern_single_tokenize);
  add_test(IDENTIFIER_tailed_numbers_1, pattern_single_tokenize);
  add_test(IDENTIFIER_startswith_dash_0, pattern_single_tokenize);
  add_test(IDENTIFIER_startswith_dash_1, pattern_single_tokenize);
  return n_failed;
}
