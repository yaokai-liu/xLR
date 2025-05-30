${license}

#ifndef XLR_TOKENS_H
#define XLR_TOKENS_H

#include "xLR/char_t.h"
#include <stdint.h>

enum TOKEN_TYPE_ENUM: uint32_t {
  ${enums},
  XLR_TOKEN_PLACE_HOLDER,
  XLR_MAX_TOTAL_TOKEN,
  XLR_TOKEN_BAD_TOKEN = UINT32_MAX
};
extern const char_t *TOKEN_NAMES[];
#endif  // XLR_TOKENS_H
