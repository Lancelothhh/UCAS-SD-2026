#include "avl_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double seconds_between(clock_t start, clock_t end)
{
    return (double)(end - start) / CLOCKS_PER_SEC;
}

static int *make_values(int n)
{
    int *values = (int *)malloc((size_t)n * sizeof(int));
    int i;

    if (values == NULL) {
        fprintf(stderr, "memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < n; ++i) {
        values[i] = i + 1;
    }

    srand(20260601);
    for (i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = values[i];
        values[i] = values[j];
        values[j] = tmp;
    }

    return values;
}

int main(int argc, char **argv)
{
    AVLTree tree;
    int n = 100000;
    int *values;
    int found = 0;
    int i;
    clock_t start;
    clock_t end;
    double insert_time;
    double search_time;
    double delete_time;

    if (argc >= 2) {
        n = atoi(argv[1]);
        if (n <= 0) {
            n = 100000;
        }
    }

    values = make_values(n);
    avl_init(&tree);

    start = clock();
    for (i = 0; i < n; ++i) {
        avl_insert(&tree, values[i]);
    }
    end = clock();
    insert_time = seconds_between(start, end);

    start = clock();
    for (i = 0; i < n; ++i) {
        found += avl_search(&tree, values[i]);
    }
    end = clock();
    search_time = seconds_between(start, end);

    start = clock();
    for (i = 0; i < n; ++i) {
        avl_delete(&tree, values[i]);
    }
    end = clock();
    delete_time = seconds_between(start, end);

    printf("AVL benchmark with %d keys\n", n);
    printf("insert: %.6f s\n", insert_time);
    printf("search: %.6f s, found: %d\n", search_time, found);
    printf("delete: %.6f s\n", delete_time);
    printf("final size: %d\n", avl_size(&tree));

    avl_destroy(&tree);
    free(values);
    return 0;
}

