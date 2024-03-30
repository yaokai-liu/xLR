/**
 * Project Name: equivC
 * Module Name: grammar
 * Filename: lex.h
 * Creator: Yaokai Liu
 * Create Date: 2024-03-16
 * Copyright (c) 2024 Yaokai Liu. All rights reserved.
 **/

#ifndef LEX_OBJECT_H
#define LEX_OBJECT_H
#ifdef __cplusplus
#include <cstdint>
#include <cstddef>
extern "C" {
#else
#include <stdint.h>
#include <stddef.h>
#endif
typedef void * Any;
typedef struct Object {
    uint32_t        typeid;
    uint64_t        length;
    Any             value;
    uint32_t        lineno;
    uint32_t        column;
    const char*     filename;
} Object;

#ifdef __cplusplus
};
#endif

#endif //LEX_OBJECT_H
