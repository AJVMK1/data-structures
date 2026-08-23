#include <stdio.h>
#include <stddef.h>

int sum_array(const int *values, size_t count);

int main(void)
{
    int values[] = {4, 7, 1, 9, 3, 6, 2};
    size_t count = sizeof values / sizeof values[0];

    printf("count = %zu\n", count);

    for (size_t i = 0; i < count; ++i) {
        printf("values[%zu] = %d\n", i, values[i]);
    }

    printf("sum = %d\n", sum_array(values, count));

    return 0;
}

int sum_array(const int *values, size_t count)
{
    int total = 0;

    for (size_t i = 0; i < count; ++i) {
        total += values[i];
    }

    return total;
}