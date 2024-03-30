/**
 * Project Name: equivC
 * Module Name: grammar
 * Filename: lex.h
 * Creator: Yaokai Liu
 * Create Date: 2024-03-16
 * Copyright (c) 2024 Yaokai Liu. All rights reserved.
 **/

#ifndef LEX_FORMULA_PARSER_H
#define LEX_FORMULA_PARSER_H

#include "Rule.h"
#include "Object.h"
#include "ActionTable.h"

typedef struct Parser {
    uint32_t    lineno;
    uint32_t    column;
    char *      filename;
    Rule*       rules;
    const Object *  error_object;
    const char *    error_message;
} Parser;
const char * get_type_name(Object * obj);
uint32_t lex(const char * input, Object * obj);
uint32_t pass_space(const char * input, uint32_t * lineno, uint32_t * column);
uint32_t tokenize(const char *input, Object *objs[], size_t *n_objs,
                  uint32_t *lineno, uint32_t *column, const char *filename);
uint32_t parse(Parser * parser, const char * input, ActionTable * action_table);

#endif //LEX_FORMULA_PARSER_H
