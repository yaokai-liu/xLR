${license}

#include "xLR/token.h"
#include "generated/tokens.gen.h"
const char_t XLR_TERMINALS[] = {
  ${terminals}
};

const char_t * XLR_TERMINAL_STRINGS[] = {
  ${strings}
};

const uint32_t XLR_TERMINAL_STRING_LENS[] = {
  ${string_lens}
};

const int32_t N_XLR_TERMINAL = sizeof(XLR_TERMINALS);
