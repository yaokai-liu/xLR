/**
 * Project Name: equivC
 * Module Name: grammar
 * Filename: lex.h
 * Creator: Yaokai Liu
 * Create Date: 2024-03-16
 * Copyright (c) 2024 Yaokai Liu. All rights reserved.
 **/


#include "BinTree.h"
#include <stdlib.h>

typedef void * Any;
typedef struct BinNode BinNode;
typedef struct BinNode {
    uint64_t     key;
    uint64_t     height;
    Any         value;
    BinNode     *left;
    BinNode     *right;
} BinNode;

typedef struct BinTree{
    BinNode * root;
} BinTree;

BinNode * getBinTreeNode(BinNode * root, uint64_t key);
BinNode * addBinTreeNode(BinNode ** root, uint64_t key);

BinTree * newBinTree() {
    BinTree * tree = calloc(1, sizeof(BinTree));
    return tree;
}

inline uint64_t getBinTreeHeight(const BinTree * tree) {
    return tree->root ? tree->root->height + 1 : 0;
}

Any getBinTreeValue(const BinTree * const tree, uint64_t const key){
    if (!tree) { return nullptr; }
    const BinNode * const node = getBinTreeNode(tree->root, key);
    return node ? node->value : nullptr;
}

int32_t setBinTreeValue(BinTree * const tree, uint64_t const key, Any const value) {
    if (!tree) { return -1; }
    BinNode * node = addBinTreeNode(&tree->root, key);
    return node ? (node->value = value, 0) : -1;
}

int32_t delBinTreeNode(BinNode * root, int32_t (*del_content)(Any)) {
    if (!root) { return 0; }
    int32_t res = 0;
    if (root->left) { res += delBinTreeNode(root->left, del_content); }
    if (root->right) { res += delBinTreeNode(root->right, del_content); }
    if (del_content) { res += del_content(root->value); }
    free(root);
    return res < 0 ? -1 : 0;
}
int32_t destroyBinTree(BinTree * tree, int32_t (*del_value)(Any)) {
    if (!tree) { return -1; }
    int32_t const res = delBinTreeNode(tree->root, del_value);
    free(tree);
    return res;
}

BinNode * newBinNode(uint64_t const key) {
    BinNode * node = calloc(1, sizeof(BinNode));
    node->key = key;
    return node;
}

BinNode * getBinTreeNode(BinNode * root, uint64_t const key) {
    if (!root) { return nullptr; }
    if (key == root->key) { return root; }
    if(key < root->key) { return getBinTreeNode(root->left, key); }
    return getBinTreeNode(root->right, key);
}

#define max(_a, _b) ((_a) > (_b) ? (_a) : (_b))
#define LL_rotate(_pNode) do { \
    BinNode * temp = *_pNode; \
    *_pNode = temp->left; \
    temp->left = (*_pNode)->right; \
    (*_pNode)->right = temp;   \
    uint64_t tlh = temp->left ? temp->left->height + 1: 0; \
    uint64_t trh = temp->right? temp->right->height+ 1: 0; \
    temp->height = max(tlh, trh); \
    (*_pNode)->height = max((*_pNode)->left ? (*_pNode)->left->height + 1: 0, temp->height + 1); \
} while(0)
#define RR_rotate(_pNode) do { \
    BinNode * temp = *_pNode; \
    *_pNode = temp->right; \
    temp->right = (*_pNode)->left; \
    (*_pNode)->left = temp; \
    uint64_t tlh = temp->left ? temp->left->height + 1: 0; \
    uint64_t trh = temp->right? temp->right->height+ 1: 0; \
    temp->height = max(tlh, trh); \
    (*_pNode)->height =1 + max((*_pNode)->right? (*_pNode)->right->height + 1: 0, temp->height + 1); \
} while(0)

BinNode * addBinTreeNode(BinNode ** root, uint64_t const key) {
    if (!*root) { *root = newBinNode(key); return *root; }
    if (key == (*root)->key) { return *root; }
    BinNode ** pNode = (key < (*root)->key) ? &((*root)->left) : &((*root)->right);
    BinNode * node = addBinTreeNode(pNode, key);

    uint64_t left_height = (*root)->left ? (*root)->left->height + 1: 0;
    uint64_t right_height = (*root)->right ? (*root)->right->height + 1: 0;
    if (left_height - right_height >= 2) {
        uint64_t const llh = (*root)->left->left ? (*root)->left->left->height : 0;
        uint64_t const lrh = (*root)->left->right ? (*root)->left->right->height : 0;
        if (llh > lrh) { LL_rotate(root); }
        else { RR_rotate(&(*root)->left); LL_rotate(root); }
    } else if (right_height - left_height >= 2) {
        uint64_t const rlh = (*root)->right->left ? (*root)->right->left->height : 0;
        uint64_t const rrh = (*root)->right->right ? (*root)->right->right->height : 0;
        if (rlh > rrh) { LL_rotate(&(*root)->right); RR_rotate(root); }
        else { RR_rotate(root); }
    }
    left_height = (*root)->left ? (*root)->left->height + 1: 0;
    right_height = (*root)->right ? (*root)->right->height + 1: 0;
    (*root)->height = max(left_height, right_height);

    return node;
}
