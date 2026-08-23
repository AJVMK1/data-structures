#include <stdio.h>
#include <stdlib.h>
#include "stats.h"
#include "record.h"
#include "node.h"

int main(void)
{
    int values[] = {4, 7, 1, 9, 3};
    size_t count = sizeof values / sizeof values[0];
    printf("sum=%d\n", sum_array(values, count));

    Record local = {1, 42.5};
    Record *local_ptr = &local;
    printf("record id=%d value=%.1f\n", local_ptr->id, local_ptr->value);

    Record *record = malloc(sizeof *record);
    if (record == nullptr) {
        fputs("record allocation failed\n", stderr);
        return EXIT_FAILURE;
    }
    record->id = 2;
    record->value = 99.67;

    printf("heap record id=%d value=%.1f\n", record->id, record->value);
    free(record);

    Node *node = malloc(sizeof *node);
    if (node == nullptr) {
        fputs("node allocation failed\n", stderr);
        return EXIT_FAILURE;
    }
    node->value = 100;
    node->next = nullptr;
    printf("node value=%d\n", node->value);
    free(node);

    return EXIT_SUCCESS;
}