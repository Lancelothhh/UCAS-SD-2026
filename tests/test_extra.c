#include "avl_tree.h"

#include <assert.h>
#include <stdio.h>

static void build_range(AVLTree *tree, int start, int end)
{
    int i;

    for (i = start; i <= end; ++i) {
        avl_insert(tree, i);
    }
}

static void test_display_functions(void)
{
    AVLTree tree;

    avl_init(&tree);
    build_range(&tree, 1, 7);

    printf("Inorder display smoke test: ");
    avl_print_inorder(&tree);
    printf("Tree display smoke test:\n");
    avl_print_tree(&tree);

    assert(avl_is_balanced_bst(&tree));
    avl_destroy(&tree);
}

static void test_merge_function(void)
{
    AVLTree a;
    AVLTree b;
    int i;

    avl_init(&a);
    avl_init(&b);

    build_range(&a, 1, 10);
    build_range(&b, 8, 15);

    avl_merge(&a, &b);

    assert(avl_size(&a) == 15);
    for (i = 1; i <= 15; ++i) {
        assert(avl_search(&a, i) == 1);
    }
    assert(avl_is_balanced_bst(&a));
    assert(avl_is_balanced_bst(&b));

    avl_destroy(&a);
    avl_destroy(&b);
}

static void test_split_function(void)
{
    AVLTree source;
    AVLTree less_equal;
    AVLTree greater;
    int i;

    avl_init(&source);
    avl_init(&less_equal);
    avl_init(&greater);

    build_range(&source, 1, 15);
    avl_split(&source, 9, &less_equal, &greater);

    assert(avl_size(&less_equal) == 9);
    assert(avl_size(&greater) == 6);

    for (i = 1; i <= 9; ++i) {
        assert(avl_search(&less_equal, i) == 1);
        assert(avl_search(&greater, i) == 0);
    }
    for (i = 10; i <= 15; ++i) {
        assert(avl_search(&less_equal, i) == 0);
        assert(avl_search(&greater, i) == 1);
    }

    assert(avl_is_balanced_bst(&source));
    assert(avl_is_balanced_bst(&less_equal));
    assert(avl_is_balanced_bst(&greater));

    avl_destroy(&source);
    avl_destroy(&less_equal);
    avl_destroy(&greater);
}

int main(void)
{
    test_display_functions();
    test_merge_function();
    test_split_function();

    printf("Member B extra AVL tests passed.\n");
    return 0;
}

