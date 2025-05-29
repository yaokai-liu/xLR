${license}

#ifndef XLR_RULES_H
#define XLR_RULES_H

#include "xLR/target.h"
#include "xLR/context.h"

typedef void *fn_xlr_reduce(Token argv[], LRContext *, ErrInfo *, const Allocator *);

enum XLR_PRODUCT_RULE_ENUM {
  ${enum_reduces}
};

extern fn_xlr_reduce * const XLR_PRODUCTS[];

${reduces}

#endif  // XLR_RULES_H
