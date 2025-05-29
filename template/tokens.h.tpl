${license}

#ifndef XLR_TOKENS_H
#define XLR_TOKENS_H

#include "xLR/char_t.h"
#include <stdint.h>

enum TOKEN_TYPE_ENUM: uint32_t {
  ${enums},
  enum_PLACE_HOLDER,
  MAX_TOTAL_TOKEN,
  enum_BAD_TOKEN = UINT32_MAX
};
extern const char_t *TOKEN_NAMES[];
#endif  // XLR_TOKENS_H
