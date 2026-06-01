#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

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

static int max_int(int a, int b)
{
    return a > b ? a : b;
}

static int height_of(const AVLNode *node)
{
    return node == NULL ? 0 : node->height;
}

static int balance_factor(const AVLNode *node)
{
    return node == NULL ? 0 : height_of(node->left) - height_of(node->right);
}

static void update_height(AVLNode *node)
{
    if (node != NULL) {
        node->height = 1 + max_int(height_of(node->left), height_of(node->right));
    }
}

static AVLNode *create_node(int key)
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
        return create_node(key);
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
        *deleted = 1;

        if (root->left == NULL || root->right == NULL) {
            AVLNode *replacement = root->left != NULL ? root->left : root->right;
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

static void avl_init(AVLTree *tree)
{
    tree->root = NULL;
    tree->size = 0;
}

static void avl_destroy(AVLTree *tree)
{
    destroy_rec(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

static int avl_insert(AVLTree *tree, int key)
{
    int inserted = 0;
    tree->root = insert_rec(tree->root, key, &inserted);
    if (inserted) {
        tree->size++;
    }
    return inserted;
}

static int avl_delete(AVLTree *tree, int key)
{
    int deleted = 0;
    tree->root = delete_rec(tree->root, key, &deleted);
    if (deleted) {
        tree->size--;
    }
    return deleted;
}

static int avl_search(const AVLTree *tree, int key)
{
    return search_rec(tree->root, key);
}

static int avl_height(const AVLTree *tree)
{
    return height_of(tree->root);
}

static int avl_is_balanced_bst(const AVLTree *tree)
{
    int height = 0;
    int count = 0;

    if (!validate_rec(tree->root, (long long)INT_MIN - 1LL, (long long)INT_MAX + 1LL, &height, &count)) {
        return 0;
    }
    return count == tree->size;
}

static void avl_print_inorder(const AVLTree *tree)
{
    inorder_rec(tree->root);
    printf("\n");
}

static void avl_print_tree(const AVLTree *tree)
{
    if (tree->root == NULL) {
        printf("(empty tree)\n");
        return;
    }
    print_tree_rec(tree->root, 0);
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

static void avl_merge(AVLTree *dest, const AVLTree *src)
{
    merge_rec(dest, src->root);
}

static void avl_split(const AVLTree *source, int x, AVLTree *less_equal, AVLTree *greater)
{
    split_rec(source->root, x, less_equal, greater);
}

static void print_menu(void)
{
    printf("\n====== AVL 平衡二叉树动态查找表：单文件参考版 ======\n");
    printf("1. 插入关键字\n");
    printf("2. 删除关键字\n");
    printf("3. 查找关键字\n");
    printf("4. 中序遍历显示\n");
    printf("5. 树形显示\n");
    printf("6. 合并另一棵 AVL 树\n");
    printf("7. 按 x 分裂 AVL 树\n");
    printf("0. 退出\n");
    printf("请选择操作: ");
}

static int read_int(const char *prompt, int *value)
{
    printf("%s", prompt);
    if (scanf("%d", value) != 1) {
        printf("输入无效，程序结束。\n");
        return 0;
    }
    return 1;
}

static void show_status(const AVLTree *tree)
{
    printf("当前节点数: %d, 高度: %d\n", tree->size, avl_height(tree));
    avl_print_tree(tree);
    printf("平衡性检查: %s\n", avl_is_balanced_bst(tree) ? "通过" : "失败");
}

static void merge_from_input(AVLTree *tree)
{
    AVLTree other;
    int n;
    int i;
    int key;

    if (!read_int("请输入另一棵树的节点个数: ", &n)) {
        return;
    }
    if (n < 0) {
        printf("节点个数不能为负数。\n");
        return;
    }

    avl_init(&other);
    for (i = 0; i < n; ++i) {
        if (!read_int("请输入关键字: ", &key)) {
            avl_destroy(&other);
            return;
        }
        avl_insert(&other, key);
    }

    avl_merge(tree, &other);
    avl_destroy(&other);
    printf("合并完成。\n");
    show_status(tree);
}

static void split_by_input(const AVLTree *tree)
{
    AVLTree less_equal;
    AVLTree greater;
    int x;

    if (!read_int("请输入分裂关键字 x: ", &x)) {
        return;
    }

    avl_init(&less_equal);
    avl_init(&greater);
    avl_split(tree, x, &less_equal, &greater);

    printf("关键字 <= %d 的 AVL 树:\n", x);
    avl_print_tree(&less_equal);
    printf("关键字 > %d 的 AVL 树:\n", x);
    avl_print_tree(&greater);

    avl_destroy(&less_equal);
    avl_destroy(&greater);
}

int main(void)
{
    AVLTree tree;
    int choice;
    int key;

    avl_init(&tree);

    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("输入无效，程序结束。\n");
            break;
        }
        if (choice == 0) {
            break;
        }

        switch (choice) {
        case 1:
            if (read_int("请输入要插入的关键字: ", &key)) {
                printf(avl_insert(&tree, key) ? "插入成功。\n" : "关键字已存在，未重复插入。\n");
                show_status(&tree);
            }
            break;
        case 2:
            if (read_int("请输入要删除的关键字: ", &key)) {
                printf(avl_delete(&tree, key) ? "删除成功。\n" : "关键字不存在。\n");
                show_status(&tree);
            }
            break;
        case 3:
            if (read_int("请输入要查找的关键字: ", &key)) {
                printf(avl_search(&tree, key) ? "查找成功。\n" : "未找到该关键字。\n");
            }
            break;
        case 4:
            printf("中序遍历: ");
            avl_print_inorder(&tree);
            break;
        case 5:
            show_status(&tree);
            break;
        case 6:
            merge_from_input(&tree);
            break;
        case 7:
            split_by_input(&tree);
            break;
        default:
            printf("无效选择，请重新输入。\n");
            break;
        }
    }

    avl_destroy(&tree);
    printf("程序结束。\n");
    return 0;
}

