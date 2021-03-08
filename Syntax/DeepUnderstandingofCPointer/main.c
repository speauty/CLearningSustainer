#include <stdio.h>

int main() {
    int num = 0;
    int *pi = &num;
    *pi = 100;
    printf("%d", *pi);
    return 0;
}
