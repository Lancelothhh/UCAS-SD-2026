#include "avl_tree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void test_insert_search_delete(void)
{
    AVLTree tree;
    int values[] = {30, 20, 40, 10, 25, 35, 50, 5, 15, 27};
    int i;

    avl_init(&tree);
    for (i = 0; i < (int)(sizeof(values) / sizeof(values[0])); ++i) {
        assert(avl_insert(&tree, values[i]) == 1);
        assert(avl_is_balanced_bst(&tree));
    }

    assert(avl_size(&tree) == 10);
    assert(avl_insert(&tree, 25) == 0);
    assert(avl_size(&tree) == 10);
    assert(avl_search(&tree, 35) == 1);
    assert(avl_search(&tree, 99) == 0);

    assert(avl_delete(&tree, 20) == 1);
    assert(avl_is_balanced_bst(&tree));
    assert(avl_search(&tree, 20) == 0);

    assert(avl_delete(&tree, 5) == 1);
    assert(avl_is_balanced_bst(&tree));

    assert(avl_delete(&tree, 999) == 0);
    assert(avl_is_balanced_bst(&tree));

    avl_destroy(&tree);
}

static void test_sorted_insert_height(void)
{
    AVLTree tree;
    int i;

    avl_init(&tree);
    for (i = 1; i <= 1000; ++i) {
        assert(avl_insert(&tree, i) == 1);
        assert(avl_is_balanced_bst(&tree));
    }

    assert(avl_height(&tree) < 25);
    avl_destroy(&tree);
}

static void test_merge_split(void)
{
    AVLTree a;
    AVLTree b;
    AVLTree less_equal;
    AVLTree greater;
    int i;

    avl_init(&a);
    avl_init(&b);
    avl_init(&less_equal);
    avl_init(&greater);

    for (i = 1; i <= 10; ++i) {
        avl_insert(&a, i);
    }
    for (i = 8; i <= 15; ++i) {
        avl_insert(&b, i);
    }

    avl_merge(&a, &b);
    assert(avl_size(&a) == 15);
    assert(avl_is_balanced_bst(&a));

    avl_split(&a, 9, &less_equal, &greater);
    assert(avl_size(&less_equal) == 9);
    assert(avl_size(&greater) == 6);
    assert(avl_search(&less_equal, 9) == 1);
    assert(avl_search(&greater, 10) == 1);
    assert(avl_is_balanced_bst(&less_equal));
    assert(avl_is_balanced_bst(&greater));

    avl_destroy(&a);
    avl_destroy(&b);
    avl_destroy(&less_equal);
    avl_destroy(&greater);
}

static void test_random_operations(void)
{
    AVLTree tree;
    int present[2001] = {0};
    int expected_size = 0;
    int i;

    avl_init(&tree);
    srand(20260601);

    for (i = 0; i < 5000; ++i) {
        int key = rand() % 2001 - 1000;
        int index = key + 1000;
        int op = rand() % 3;

        if (op == 0) {
            int inserted = avl_insert(&tree, key);
            if (!present[index]) {
                assert(inserted == 1);
                present[index] = 1;
                expected_size++;
            } else {
                assert(inserted == 0);
            }
        } else if (op == 1) {
            int deleted = avl_delete(&tree, key);
            if (present[index]) {
                assert(deleted == 1);
                present[index] = 0;
                expected_size--;
            } else {
                assert(deleted == 0);
            }
        } else {
            assert(avl_search(&tree, key) == present[index]);
        }

        assert(avl_size(&tree) == expected_size);
        assert(avl_is_balanced_bst(&tree));
    }

    avl_destroy(&tree);
}

int main(void)
{
    test_insert_search_delete();
    test_sorted_insert_height();
    test_merge_split();
    test_random_operations();

    printf("All AVL correctness tests passed.\n");
    return 0;
}

