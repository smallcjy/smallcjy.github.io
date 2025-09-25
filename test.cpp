#include <stdio.h>

int main()
{
    // 指针的数据类型 是 type*
    // int* char* float* double*
    int a = 0; // 地址00000086E51FFC1C存放0
    printf("Address of a: %p\n", &a);

    int* ptr = &a;

    printf("Address of ptr: %p\n", ptr);
    printf("Address of ptr: %p\n", &ptr);

    printf("Value pointed to by ptr: %d\n", *ptr);

    // 00000086E51FFC10 -> 00000086E51FFC1C
    // 00000086E51FFC1C -> 5

    int* ptr2 = ptr;
    int* ptr3 = ptr;

    int b = 3;
    int c = b;

    return 0;
}