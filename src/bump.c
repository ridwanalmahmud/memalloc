#include "bump.h"
#include <unistd.h>

Block *get_header(word_t *data) {
    return (Block *)((char *)data + sizeof(((Block *)0)->data) - sizeof(Block));
}

size_t align(size_t n) {
    return (n + sizeof(word_t) - 1) & ~(sizeof(word_t) - 1);
}

size_t alloc_size(size_t size) {
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
