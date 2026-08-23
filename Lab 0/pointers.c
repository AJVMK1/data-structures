#include <stdio.h>

int main(void)
{
    int score = 10;
    int *score_ptr = &score;

    printf("score is %d\n", score);
    printf("address is %p\n", (void *)score_ptr);
    printf("value at pointer is %d\n", *score_ptr);

    *score_ptr = 25;

    printf("score is now %d\n", score);

    // it changed because we accessed the value using the pointer and changed it    

    return 0;
}