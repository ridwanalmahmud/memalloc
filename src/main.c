#include "bump.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
    (void)argc; // Silence unused parameter warnings
    (void)argv;
    void *p1 = memalloc(3); // (1)
    Block *p1b = get_header(p1);
    assert(p1b->size == sizeof(word_t));
    void *p2 = memalloc(8); // (2)
    Block *p2b = get_header(p2);
    assert(p2b->size == 8);

    printf("\nAssertions passed!\n");
    return 0;
}
