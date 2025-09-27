${license}

#include "action.h"
#include "generated/tokens.gen.h"
#include "generated/xLR/rules.gen.h"
#include "generated/xLR/action-table.gen.h"

struct state {
  const uint16_t ndx_base;
  const uint16_t goto_base;
  const uint16_t token_base;
  const uint16_t n_tokens;
};
struct unit {
  uint8_t type;
  uint8_t offset;
};

const struct grammar_action XLR_ACTIONS[];
const uint16_t XLR_JUMPS[];
const struct unit XLR_UNITS[];
const struct state XLR_STATES[];
const uint32_t XLR_CURRENT_TOKENS[];

const struct unit *getParseUnit(const state *state, uint32_t look);

const struct grammar_action XLR_ACTIONS[] = {
  ${actions}
};

const uint16_t XLR_JUMPS[] = {
  ${jumps}
};

const struct unit XLR_UNITS[] = {
  ${units}
};

const struct state XLR_STATES[] = {
  ${states}
};

const uint32_t XLR_CURRENT_TOKENS[] = {
  ${currents}
};

inline const struct unit *getParseUnit(const state *state, uint32_t look) {
  const struct unit *unit, *base = &XLR_UNITS[state->token_base];
  int32_t left = 0, right = state->n_tokens - 1;
  int32_t u_idx = (left + right) / 2;
  unit = &base[u_idx];
  while (unit->type != look && left < right) {
    if (unit->type < look) {
      left = u_idx + 1;
    } else {
      right = u_idx - 1;
    }
    u_idx = (left + right) / 2;
    unit = &base[u_idx];
  }
  if (unit->type != look) { return nullptr; }
  return unit;
}


inline const struct grammar_action *getParseAction(uint32_t index, uint32_t ahead) {
    const state *state = &XLR_STATES[index];
    const struct unit *unit = getParseUnit(state, ahead);
    if (!unit) { return nullptr; }
    const struct grammar_action *act = &XLR_ACTIONS[state->ndx_base + unit->offset];
    return act;
}

inline uint32_t parseJumpState(uint32_t index, uint32_t current) {
    const state *state = &XLR_STATES[index];
    const struct unit *unit = getParseUnit(state, current);
    if (!unit) { return XLR_BAD_STATE; }
    return XLR_JUMPS[state->goto_base + unit->offset];
}

inline uint32_t getParseStateCurrentTokenType(int32_t state) {
    return XLR_CURRENT_TOKENS[state];
}