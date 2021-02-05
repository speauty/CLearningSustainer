#include <stdio.h>
#include "list.h"

void destroy (void *data)
{
    free(data);
}

int main() {
    List test;
    list_init(&test, destroy);
    list_ins_next(&test, NULL, (const void *) 'a');
    printf("Hello, World! %c", list_data(list_head(&test)));
    return 0;
}
