/**
 * Project Name: equivC
 * Module Name: grammar
 * Filename: lex.h
 * Creator: Yaokai Liu
 * Create Date: 2024-03-16
 * Copyright (c) 2024 Yaokai Liu. All rights reserved.
 **/

#include <stdlib.h>
#include <string.h>
#include "Parser.h"
#include "ActionTable.h"
#include "BinTree.h"
#include "Set.h"

#define len(_expr) ((sizeof _expr) / (sizeof _expr[0]))

uint32_t t_IDENTIFIER(const char * input, Object * obj);
uint32_t t_LITERAL(const char * input, Object * obj);
uint32_t t_ASSIGNER(const char * input, Object * obj);
uint32_t t_APPENDER(const char * input, Object * obj);
uint32_t t_SPLITER(const char * input, Object * obj);
uint32_t t_BEGIN_GROUP(const char * input, Object * obj);
uint32_t t_END_GROUP(const char * input, Object * obj);
uint32_t t_CLOSURE(const char * input, Object * obj);
uint32_t t_POSURE(const char * input, Object * obj);
uint32_t t_BOOLURE(const char * input, Object * obj);
uint32_t t_EOL(const char * input, Object * obj);
uint32_t t_DISCARD(const char * input, Object * obj);
uint32_t t_DESTROY(const char * input, Object * obj);

enum __TOKEN_NAME_ENUM__ : uint32_t { // NOLINT(*-reserved-identifier)
    __END__ = 0, // NOLINT(*-reserved-identifier)
    IDENTIFIER = 1,
    LITERAL,
    ASSIGNER,
    APPENDER,
    SPLITER,
    BEGIN_GROUP,
    END_GROUP,
    CLOSURE,
    POSURE,
    BOOLURE,
    EOL,
    DISCARD,
    DESTROY,
};

typedef struct LrItem {
    const char *name;
    Set *       first_set;
    Set *       follow_set;
} LrItem;

typedef struct {
    const Rule *    rule;
    const LrItem *  lookahead;
    uint64_t        position;
} ExtRule;

static const char * const BNF_TOKEN_NAMES[] = {
    [__END__] = nullptr,
    [IDENTIFIER] = "IDENTIFIER",
    [LITERAL] = "LITERAL",
    [ASSIGNER] = "ASSIGNER",
    [APPENDER] = "APPENDER",
    [SPLITER] = "SPLITER",
    [BEGIN_GROUP] = "BEGIN_GROUP",
    [END_GROUP] = "END_GROUP",
    [CLOSURE] = "CLOSURE",
    [POSURE] = "POSURE",
    [BOOLURE] = "BOOLURE",
    [EOL] = "EOL",
    [DISCARD] = "DISCARD",
    [DESTROY] = "DESTROY",
};


static uint32_t (* const BNF_LEX_RULES[])(const char * input, Object * obj) = {
        [__END__] = nullptr,
        [IDENTIFIER] = t_IDENTIFIER,
        [ASSIGNER] = t_ASSIGNER,
        [APPENDER] = t_APPENDER,
        [LITERAL] = t_LITERAL,
        [SPLITER] = t_SPLITER,
        [BEGIN_GROUP] = t_BEGIN_GROUP,
        [END_GROUP] = t_END_GROUP,
        [CLOSURE] = t_CLOSURE,
        [POSURE] = t_POSURE,
        [BOOLURE] = t_BOOLURE,
        [EOL] = t_EOL,
        [DISCARD] = t_DISCARD,
        [DESTROY] = t_DESTROY,
};

uint32_t lex(const char * const input, Object * const obj) {
    uint32_t n_matched = 0;
    for (int i = 0; i < len(BNF_LEX_RULES); i ++) {
        if (!BNF_LEX_RULES[i]) { continue; }
        n_matched = BNF_LEX_RULES[i](input, obj);
        if (n_matched != 0) { return n_matched; }
    }
    return n_matched;
}

/*
 *
 * Implications for Token Functions.
 *
 */

#define salloc(len) malloc((((sizeof(char) * ((len) + 1)) >> 3) + 1) << 3)

uint32_t str_match_d(const char * restrict a, const char * restrict b) {
    const char * s = a;
    while (*a) { if (*a != *b) break; a++, b++; } return a - s;
}

#define literal(_name, _pattern) \
uint32_t t_##_name(const char * const input, Object * const obj) { \
    if (str_match_d(input, _pattern) >= (sizeof _pattern) - 1) { \
        obj->typeid = _name; \
        obj->length = (sizeof _pattern) - 1; \
        obj->value = _pattern; \
        return (sizeof _pattern) - 1; \
    } else { \
        obj->typeid = __END__; \
        return 0; \
    } \
}

literal(ASSIGNER, ":=")

literal(APPENDER, "+=")

literal(SPLITER, "|")

literal(BEGIN_GROUP, "(")

literal(END_GROUP, ")")

literal(CLOSURE, "*")

literal(POSURE, "+")

literal(BOOLURE, "?")

literal(EOL, ";")

literal(DISCARD, "!")

literal(DESTROY, "~")

//token(IDENTIFIER, "([a-zA-Z][a-zA-Z0-9_]*)([a-zA-Z][a-zA-Z0-9_]+)?")
uint32_t t_IDENTIFIER(const char *const input, Object * const obj) {
    obj->typeid = __END__;
    const char * sp = input;
    if ('z' < *sp || *sp < 'A') { return 0; }
    if ('Z' < *sp && *sp < 'a') { return 0; }
    sp ++;
    while (*sp) {
//        if (*sp == ':' && *(sp -1) != ':') { sp++; continue; }
        if ('z' < *sp || *sp < '0') { break; }
        if ('Z' < *sp && *sp < 'a') { break; }
        if ('9' < *sp && *sp < 'A') { break; }
        sp ++;
    }
    if (*(sp - 1) == ':') { return 0; }

    obj->typeid = IDENTIFIER;
    obj->length = sp - input;
    obj->value = salloc(obj->length);
    memcpy(obj->value, input, obj->length);
    ((char *)(obj->value))[obj->length] = '\0';

    return sp - input;
}

//token(LITERAL, '".*"')
uint32_t t_LITERAL(const char * const input, Object * const obj) {
    obj->typeid = __END__;
    const char * sp = input;
    if (*sp != '"') { return 0; }
    sp ++; obj->length = 0;
    while (*sp && *sp != '"' && *sp != '\n') {
        if (*sp == '\\') { sp ++; }
        sp ++; obj->length ++;
    }
    if (*sp != '"') { return 0; } else { sp++; }

    obj->typeid = LITERAL;
    obj->value = salloc(obj->length);
    for (int i = 1, j = 0; j < obj->length; i ++) {
        if (input[i] == '\\') { continue; }
        ((char *)obj->value)[j++] =  input[i];
    }
    ((char *)obj->value)[obj->length] = '\0';

    return sp - input;
}

/*
 * Assistant Functions
 */

const char * get_type_name(Object * obj) {
    return BNF_TOKEN_NAMES[obj->typeid];
}


/*
 * Generate Parser
 */

uint32_t pass_space(const char * const input, uint32_t * const lineno, uint32_t * const column) {
    uint32_t l = lineno ? *lineno : 0;
    uint32_t c = column ? *column : 0;
    const char * sp = input;
    while (*sp) {
        switch (*sp) {
            case '\n': { l ++; c = 0; break; }
            case ' ':
            case '\t': { c ++; break; }
            default: { goto __return; }
        }
        sp ++;
    }
    __return:
    lineno ? *lineno = l: 0;
    column ? *column = c: 0;
    return sp - input;
}

#define ALLOC_SIZE 32
uint32_t tokenize(const char * const input, Object *objs[], size_t *const n_objs,
                  uint32_t *const lineno, uint32_t *const column, const char * const filename) {
    uint32_t l = lineno ? *lineno : 0; uint32_t c = column ? *column : 0;
    const char * sp = input;
    uint32_t length = ALLOC_SIZE, used_len = 0;
    Object * obj_array = malloc(sizeof(Object) * length);
    sp += pass_space(sp, &l, &c);
    while (*sp) {
        uint32_t const steps = lex(sp, obj_array + used_len);
        if (steps) {
            sp += steps; c += steps;
            obj_array[used_len].lineno = l;
            obj_array[used_len].column = c;
            obj_array[used_len].filename = filename;
        } else { goto __failed_to_parse; }
        sp += pass_space(sp, &l, &c);
        if (used_len >= length) {
            length += ALLOC_SIZE;
            Object * p_temp = realloc(obj_array, length);
            if (p_temp) { obj_array = p_temp; } else { goto __failed_to_parse; }
        }
        used_len ++;
    }
    *objs = obj_array;
    *n_objs = used_len;

    lineno ? *lineno = l: 0;
    column ? *column = c: 0;
    return sp - input;

    __failed_to_parse:
    for (int i = 0; i < used_len; i ++) {
        if(obj_array[i].typeid == IDENTIFIER)  { free(obj_array[i].value); }
        if(obj_array[i].typeid == LITERAL)     { free(obj_array[i].value); }
    }
    free(obj_array); *n_objs = 0;
    lineno ? *lineno = l: 0;
    column ? *column = c: 0;
    return 0;
}

void parser_error_at(Parser * parser, const Object *obj, uint32_t error_code);

#include "Exceptions.h"
const char * BNF_PARSE_ERRORS[] = {
        [NO_ERROR] = nullptr,
        [UNEXPECTED_OBJECT] = "Unexpected Object.",
        [UNKNOWN_SYMBOL] = "Unknown Symbol or Character.",
        [SENTENCE_TOO_SHORT] = "Sentence is too short for a rule."
};

void parser_error_at(Parser * parser, const Object *obj, uint32_t error_code) {
    parser->error_object = obj;
    parser->error_message = BNF_PARSE_ERRORS[error_code];
}

uint32_t parse_rule(Parser *parser, const Object *objs, size_t n_objs, ActionTable * action_table);
uint32_t parse_append_rule(Parser *parser, const Object *objs, size_t n_objs, ActionTable * action_table);
uint32_t parse_define_rule(Parser *parser, const Object *objs, size_t n_objs, ActionTable * action_table);
uint32_t parse_discard_rule(Parser *parser, const Object *objs, size_t n_objs, ActionTable * action_table);
uint32_t parse_destroy_rule(Parser *parser, const Object *objs, size_t n_objs, ActionTable * action_table);

uint32_t parse(Parser * parser, const char * const input, ActionTable * action_table) {
    uint32_t l = parser->lineno; uint32_t c = parser->column;
    Object * objs; size_t n_objs;
    uint32_t const _cost = tokenize(input, &objs, &n_objs, &l, &c, parser->filename);
    if (!_cost) {
        parser->lineno = l; parser->column = c;
        parser_error_at(parser, nullptr, UNKNOWN_SYMBOL) ;
        return 0;
    }
    size_t i = 0; uint32_t n_parsed = 0;
    while (i < n_objs) {
        switch (objs[i].typeid) {
            case DISCARD:
                n_parsed = parse_discard_rule(parser, objs + i, n_objs - i, action_table);
                break;
            case DESTROY:
                n_parsed = parse_destroy_rule(parser, objs + i, n_objs - i, action_table);
                break;
            default:
                n_parsed = parse_rule(parser, objs + i, n_objs - i, action_table);
        }
        if (n_parsed) { i += n_parsed; } else { goto __failed_to_parse;}
    }
    parser->lineno = l; parser->column = c;
    return n_objs;

    __failed_to_parse:
    for (size_t j = 0; j < n_objs; j ++) {
        if(objs[i].typeid == IDENTIFIER)  { free(objs[i].value); }
        if(objs[i].typeid == LITERAL)     { free(objs[i].value); }
    }
    free(objs);
    return 0;
}

#define EXPECT(_type, _obj) do { if ((_obj).typeid != (_type)) { \
    parser_error_at(parser, &(_obj), UNEXPECTED_OBJECT); return 0; \
} } while (false)

uint32_t parse_rule(Parser *parser, const Object *objs, size_t n_objs, ActionTable * action_table) {
    EXPECT(IDENTIFIER, objs[0]);
    switch (objs[1].typeid) {
        case ASSIGNER: return parse_define_rule(parser, objs, n_objs, action_table);
        case APPENDER: return parse_append_rule(parser, objs, n_objs, action_table);
        default: {
            parser_error_at(parser, &objs[1], UNEXPECTED_OBJECT);
            return 0;
        }
    }
}

uint32_t parse_define_rule(Parser *parser, const Object *objs, size_t n_objs, ActionTable * action_table) {
    const Object * target = &objs[0];
    uint32_t const order = 0;
    Rule rule = {.order=order, .target=objs[0].value};
    size_t n_parsed = 2; LrItem * pre_item = nullptr;
    for (; n_parsed < n_objs; n_parsed ++) {
        auto p = objs[n_parsed].typeid;
        switch (p) {
            case LITERAL: {/*TODO*/}
            case IDENTIFIER: {/*TODO*/}
            case BEGIN_GROUP: {/*TODO*/}
            case END_GROUP: {/*TODO*/}
            case CLOSURE: {/*TODO*/}
            case POSURE: {/*TODO*/}
            case BOOLURE: {/*TODO*/}
            case SPLITER: {/*TODO*/}
            case EOL: {/*TODO*/}
            case DESTROY: {/*TODO*/}
            case DISCARD: {/*TODO*/}
            default: {/*TODO*/}
        }
        if (pre_item) {
//            add(pre_item->follow_set, &BNF_TOKEN_NAMES[p]);
        }
        pre_item = nullptr; // TODO:
    }

    return n_parsed;
}

uint32_t parse_append_rule(Parser *parser, const Object *objs, size_t n_objs, ActionTable * action_table) {

}

uint32_t parse_discard_rule(Parser *parser, const Object *objs, size_t n_objs, ActionTable * action_table) {
    const Object * target = &objs[1];
    EXPECT(EOL, objs[2]);

}

uint32_t parse_destroy_rule(Parser *parser, const Object *objs, size_t n_objs, ActionTable * action_table) {
    const Object * target = &objs[1];
    EXPECT(EOL, objs[2]);


}
