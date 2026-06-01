#include "avl_tree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void test_init_insert_search_delete(void)
{
    AVLTree tree;
    int values[] = {30, 20, 40, 10, 25, 35, 50, 5, 15, 27};
    int i;

    avl_init(&tree);
    assert(avl_size(&tree) == 0);
    assert(avl_height(&tree) == 0);

    for (i = 0; i < (int)(sizeof(values) / sizeof(values[0])); ++i) {
        assert(avl_insert(&tree, values[i]) == 1);
        assert(avl_search(&tree, values[i]) == 1);
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
    assert(avl_size(&tree) == 0);
    assert(avl_height(&tree) == 0);
}

static void test_four_rotation_shapes(void)
{
    AVLTree tree;

    avl_init(&tree);
    avl_insert(&tree, 30);
    avl_insert(&tree, 20);
    avl_insert(&tree, 10);
    assert(avl_is_balanced_bst(&tree));
    assert(avl_height(&tree) == 2);
    avl_destroy(&tree);

    avl_init(&tree);
    avl_insert(&tree, 10);
    avl_insert(&tree, 20);
    avl_insert(&tree, 30);
    assert(avl_is_balanced_bst(&tree));
    assert(avl_height(&tree) == 2);
    avl_destroy(&tree);

    avl_init(&tree);
    avl_insert(&tree, 30);
    avl_insert(&tree, 10);
    avl_insert(&tree, 20);
    assert(avl_is_balanced_bst(&tree));
    assert(avl_height(&tree) == 2);
    avl_destroy(&tree);

    avl_init(&tree);
    avl_insert(&tree, 10);
    avl_insert(&tree, 30);
    avl_insert(&tree, 20);
    assert(avl_is_balanced_bst(&tree));
    assert(avl_height(&tree) == 2);
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

static void test_random_core_operations(void)
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
    test_init_insert_search_delete();
    test_four_rotation_shapes();
    test_sorted_insert_height();
    test_random_core_operations();

    printf("Member A core AVL tests passed.\n");
    return 0;
}

