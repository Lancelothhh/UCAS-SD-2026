#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdio.h>

typedef struct AVLNode {
    int key;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

typedef struct AVLTree {
    AVLNode *root;
    int size;
} AVLTree;

void avl_init(AVLTree *tree);
void avl_destroy(AVLTree *tree);

int avl_insert(AVLTree *tree, int key);
int avl_delete(AVLTree *tree, int key);
int avl_search(const AVLTree *tree, int key);

int avl_height(const AVLTree *tree);
int avl_size(const AVLTree *tree);
int avl_is_balanced_bst(const AVLTree *tree);

void avl_print_inorder(const AVLTree *tree);
void avl_print_tree(const AVLTree *tree);

void avl_merge(AVLTree *dest, const AVLTree *src);
void avl_split(const AVLTree *source, int x, AVLTree *less_equal, AVLTree *greater);

#endif

