#include "avl_tree.h"

#include <stdio.h>

static void print_menu(void)
{
    printf("\n====== AVL 平衡二叉树动态查找表 ======\n");
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
    printf("当前节点数: %d, 高度: %d\n", avl_size(tree), avl_height(tree));
    avl_print_tree(tree);
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

    printf("待合并的树:\n");
    avl_print_tree(&other);
    avl_merge(tree, &other);
    avl_destroy(&other);

    printf("合并完成，当前树为:\n");
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

