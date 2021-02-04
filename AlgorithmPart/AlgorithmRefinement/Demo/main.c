#include <stdio.h>
#include "list.h"

void destroy (void *data)
{
    free(data);
}

int main() {
    List test;
    listInit(&test, destroy);
    listInsNext(&test, NULL, (const void *) 'a');
    printf("Hello, World! %c", listData(listHead(&test)));
    return 0;
}
