#include "main.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

inline Block *get_header(word_t *data) {
    return (Block *)((char *)data + sizeof(((Block *)0)->data) - sizeof(Block));
}

inline size_t align(size_t n) {
    return (n + sizeof(word_t) - 1) & ~(sizeof(word_t) - 1);
}

inline size_t alloc_size(size_t size) {
    // ((Block*)0) -> gets the size of the Block struct without dereferencing to NULL pointer
    // treats 0 as Block
    // pretends to access data numbers of Block struct at address 0
    // needs to do this as in C you can't get the sizeof Block.data as block is a type not an instance
    return size + sizeof(Block) - sizeof(((Block *)0)->data);
}

// will only be used when there is no available free block in our linked list of blocks
Block *request_from_os(size_t size) {
    // curent heap break
    // beginning of newly allocated block
    void *block = sbrk(0);
    if (block == (void *)-1) {
        return NULL;
    }

    // request memory expansion
    if (sbrk(alloc_size(size)) == (void *)-1) {
        return NULL; // out of memory (OOM)
    }

    return (Block *)block;
}

word_t *memalloc(size_t size) {
    size = align(size);

    Block *block = request_from_os(size);
    block->size = size;
    block->is_used = true;

    // init heap
    if (head == NULL) {
        head = block;
    }

    // chain the blocks
    if (tail != NULL) {
        tail->next = block;
    }

    tail = block;

    // user payload
    return block->data;
}

int main(int argc, char const *argv[]) {
    (void)argc; // Silence unused parameter warnings
    (void)argv;

    // --------------------------------------
    // Test case 1: Alignment
    //
    // A request for 3 bytes is aligned to 8.
    //
    void *p1 = memalloc(3); // (1)
    Block *p1b = get_header(p1);
    assert(p1b->size == sizeof(word_t));

    // --------------------------------------
    // Test case 2: Exact amount of aligned bytes
    //
    void *p2 = memalloc(8); // (2)
    Block *p2b = get_header(p2);
    assert(p2b->size == 8);

    printf("\nAll assertions passed!\n");
    return 0;
}
