/**
 * Project Name: equivC
 * Module Name: grammar
 * Filename: lex.h
 * Creator: Yaokai Liu
 * Create Date: 2024-03-16
 * Copyright (c) 2024 Yaokai Liu. All rights reserved.
 **/

#include <stdio.h>
#include "Parser.h"
#include "Set.h"


extern const char * TOKEN_NAMES[];
typedef struct LrItem {
    const char *name;
    Set *       first_set;
    Set *       follow_set;
} LrItem;

int main() {
    Object * objs; size_t n_objs;
    char * input = "iaA90 := bbs+ (std)* e3? \"a|z\" ;\n"
                   "iaA90 += aldjkn ;"
                   "!iaA90;"
                   "~iaA90;";
    uint32_t l, c;
    uint32_t n = tokenize(input, &objs, &n_objs, &l, &c, nullptr);
    if (!n) {
        printf("line %d, col %d: Can't be recognized.\n", l + 1, c + 1);
        return -1;
    }
    for (int i = 0; i < n_objs; i ++){
        printf("(%s, \"%s\")\n", get_type_name(&objs[i]), (char *) objs[i].value);
    }
    return 0;
}
