/**
 * Project Name: xLR
 * Module Name: 
 * Filename: Exceptions.h
 * Creator: Yaokai Liu
 * Create Date: 2024-03-17
 * Copyright (c) 2024 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_EXCEPTIONS_H
#define XLR_EXCEPTIONS_H

#include <stdint.h>

enum __BNF_ERROR_CODE_ENUM__ : uint32_t { // NOLINT(*-reserved-identifier)
    NO_ERROR = 0,
    UNEXPECTED_OBJECT,
    UNKNOWN_SYMBOL,
    SENTENCE_TOO_SHORT,
};

extern const char * BNF_PARSE_ERRORS[];

#endif //XLR_EXCEPTIONS_H
