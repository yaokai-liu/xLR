/**
 * Project Name: equivC
 * Module Name: grammar
 * Filename: lex.h
 * Creator: Yaokai Liu
 * Create Date: 2024-03-16
 * Copyright (c) 2024 Yaokai Liu. All rights reserved.
 **/
#include <stdio.h>

#include "ActionTable.h"
#include "TrieTree.h"

int main() {
    TrieTree * tree = newTrieTree();
    setTrieValue(tree, "abc", (void *) 123);
    auto const p = getTrieValue(tree, "aaa");
    if (p) printf("value for key '%s' is %p\n", "abc", p);
    else printf("no value for key '%s'\n", "aaa");
    auto const q = getTrieValue(tree, "abc");
    if (q) printf("value for key '%s' is %p\n", "abc", q);
    else printf("no value for key '%s'\n", "abc");
    destroyTrie(tree);

    auto const table = newActionTable();
    printf("action of \"accc\" in status 12 is: 0x%llx\n", getAction(table, 12, "accc"));
    setAction(table, 0, "accc", makeAction(reject, 12LL));
    printf("action of \"accc\" in status 0 is: 0x%llx\n", getAction(table, 0, "accc"));
    destroyActionTable(table);
    return 0;
}
