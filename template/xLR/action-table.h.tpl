${license}

#ifndef XLR_ACTION_TABLE_GEN_H
#define XLR_ACTION_TABLE_GEN_H

#include <stdint.h>

enum XLR_STATE_ENUM {
  XLR_BAD_STATE = 0,
  ${state_enum}
};

const struct grammar_action *getParseAction(uint32_t index, uint32_t ahead);

uint32_t parseJumpState(uint32_t index, uint32_t current);

uint32_t getParseStateCurrentTokenType(int32_t state);

#endif  // XLR_ACTION_TABLE_GEN_H