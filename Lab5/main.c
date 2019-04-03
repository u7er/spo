#include <malloc.h>
#include <stdio.h>

int main()
{
    void* arr[1000];
    for (int i = 0; i < 1000; ++i)
        arr[i] = malloc(100);

    for (int i = 0; i < 1000; ++i)
        free(arr[i]);
}