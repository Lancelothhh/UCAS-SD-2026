#include "avl_tree.h"

#include <limits.h>
#include <stdlib.h>

static int max_int(int a, int b)
{
    return a > b ? a : b;
}

static int node_height(const AVLNode *node)
{
    return node == NULL ? 0 : node->height;
}

static int balance_factor(const AVLNode *node)
{
    return node == NULL ? 0 : node_height(node->left) - node_height(node->right);
}

static void update_height(AVLNode *node)
{
    if (node != NULL) {
        node->height = 1 + max_int(node_height(node->left), node_height(node->right));
    }
}

static AVLNode *new_node(int key)
{
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    if (node == NULL) {
        fprintf(stderr, "memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->key = key;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static AVLNode *rotate_right(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *t2 = x->right;

    x->right = y;
    y->left = t2;

    update_height(y);
    update_height(x);
    return x;
}

static AVLNode *rotate_left(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *t2 = y->left;

    y->left = x;
    x->right = t2;

    update_height(x);
    update_height(y);
    return y;
}

static AVLNode *rebalance(AVLNode *node)
{
    int bf;

    if (node == NULL) {
        return NULL;
    }

    update_height(node);
    bf = balance_factor(node);

    if (bf > 1) {
        if (balance_factor(node->left) < 0) {
            node->left = rotate_left(node->left);
        }
        return rotate_right(node);
    }

    if (bf < -1) {
        if (balance_factor(node->right) > 0) {
            node->right = rotate_right(node->right);
        }
        return rotate_left(node);
    }

    return node;
}

static AVLNode *insert_rec(AVLNode *node, int key, int *inserted)
{
    if (node == NULL) {
        *inserted = 1;
        return new_node(key);
    }

    if (key < node->key) {
        node->left = insert_rec(node->left, key, inserted);
    } else if (key > node->key) {
        node->right = insert_rec(node->right, key, inserted);
    } else {
        *inserted = 0;
        return node;
    }

    return rebalance(node);
}

static AVLNode *min_value_node(AVLNode *node)
{
    AVLNode *current = node;
    while (current != NULL && current->left != NULL) {
        current = current->left;
    }
    return current;
}

static AVLNode *delete_rec(AVLNode *root, int key, int *deleted)
{
    if (root == NULL) {
        return NULL;
    }

    if (key < root->key) {
        root->left = delete_rec(root->left, key, deleted);
    } else if (key > root->key) {
        root->right = delete_rec(root->right, key, deleted);
    } else {
        AVLNode *replacement;
        *deleted = 1;

        if (root->left == NULL || root->right == NULL) {
            replacement = root->left != NULL ? root->left : root->right;
            free(root);
            return replacement;
        } else {
            AVLNode *successor = min_value_node(root->right);
            int ignored = 0;
            root->key = successor->key;
            root->right = delete_rec(root->right, successor->key, &ignored);
        }
    }

    return rebalance(root);
}

static void destroy_rec(AVLNode *node)
{
    if (node == NULL) {
        return;
    }
    destroy_rec(node->left);
    destroy_rec(node->right);
    free(node);
}

static int search_rec(const AVLNode *node, int key)
{
    if (node == NULL) {
        return 0;
    }
    if (key == node->key) {
        return 1;
    }
    if (key < node->key) {
        return search_rec(node->left, key);
    }
    return search_rec(node->right, key);
}

static void inorder_rec(const AVLNode *node)
{
    if (node == NULL) {
        return;
    }
    inorder_rec(node->left);
    printf("%d ", node->key);
    inorder_rec(node->right);
}

static void print_tree_rec(const AVLNode *node, int depth)
{
    int i;

    if (node == NULL) {
        return;
    }

    print_tree_rec(node->right, depth + 1);
    for (i = 0; i < depth; ++i) {
        printf("    ");
    }
    printf("%d(h=%d)\n", node->key, node->height);
    print_tree_rec(node->left, depth + 1);
}

static int validate_rec(const AVLNode *node, long long min_key, long long max_key, int *height, int *count)
{
    int left_height = 0;
    int right_height = 0;
    int left_count = 0;
    int right_count = 0;

    if (node == NULL) {
        *height = 0;
        *count = 0;
        return 1;
    }

    if ((long long)node->key <= min_key || (long long)node->key >= max_key) {
        return 0;
    }

    if (!validate_rec(node->left, min_key, node->key, &left_height, &left_count)) {
        return 0;
    }
    if (!validate_rec(node->right, node->key, max_key, &right_height, &right_count)) {
        return 0;
    }

    if (node->height != 1 + max_int(left_height, right_height)) {
        return 0;
    }
    if (abs(left_height - right_height) > 1) {
        return 0;
    }

    *height = node->height;
    *count = 1 + left_count + right_count;
    return 1;
}

static void merge_rec(AVLTree *dest, const AVLNode *node)
{
    if (node == NULL) {
        return;
    }
    merge_rec(dest, node->left);
    avl_insert(dest, node->key);
    merge_rec(dest, node->right);
}

static void split_rec(const AVLNode *node, int x, AVLTree *less_equal, AVLTree *greater)
{
    if (node == NULL) {
        return;
    }
    split_rec(node->left, x, less_equal, greater);
    if (node->key <= x) {
        avl_insert(less_equal, node->key);
    } else {
        avl_insert(greater, node->key);
    }
    split_rec(node->right, x, less_equal, greater);
}

void avl_init(AVLTree *tree)
{
    tree->root = NULL;
    tree->size = 0;
}

void avl_destroy(AVLTree *tree)
{
    destroy_rec(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

int avl_insert(AVLTree *tree, int key)
{
    int inserted = 0;
    tree->root = insert_rec(tree->root, key, &inserted);
    if (inserted) {
        tree->size++;
    }
    return inserted;
}

int avl_delete(AVLTree *tree, int key)
{
    int deleted = 0;
    tree->root = delete_rec(tree->root, key, &deleted);
    if (deleted) {
        tree->size--;
    }
    return deleted;
}

int avl_search(const AVLTree *tree, int key)
{
    return search_rec(tree->root, key);
}

int avl_height(const AVLTree *tree)
{
    return node_height(tree->root);
}

int avl_size(const AVLTree *tree)
{
    return tree->size;
}

int avl_is_balanced_bst(const AVLTree *tree)
{
    int height = 0;
    int count = 0;
    if (!validate_rec(tree->root, (long long)INT_MIN - 1LL, (long long)INT_MAX + 1LL, &height, &count)) {
        return 0;
    }
    return count == tree->size;
}

void avl_print_inorder(const AVLTree *tree)
{
    inorder_rec(tree->root);
    printf("\n");
}

void avl_print_tree(const AVLTree *tree)
{
    if (tree->root == NULL) {
        printf("(empty tree)\n");
        return;
    }
    print_tree_rec(tree->root, 0);
}

void avl_merge(AVLTree *dest, const AVLTree *src)
{
    merge_rec(dest, src->root);
}

void avl_split(const AVLTree *source, int x, AVLTree *less_equal, AVLTree *greater)
{
    split_rec(source->root, x, less_equal, greater);
}

