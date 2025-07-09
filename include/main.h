#ifndef __MEMALLOC_H__
#define __MEMALLOC_H__

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// Memory layout of a block
//
// +----------------------------+  <-------+
// | block size                 |          |
// +----------------------------+          | Block header
// | is used (1 byte)           |          |
// +----------------------------+          |
// | pointer to next block      |   <------+
// +----------------------------+
// |     Payload pointer        |   <------+
// |            ...             |          |
// |            ...             |          | Addressable content
// |            ...             |          |
// |            ...             |   <------+
// +----------------------------+

// Macine word size. Defending on the architecture
// Can be 4 or 8 bytes
typedef intptr_t word_t;

typedef struct Block {
    size_t size;        // size of the block
    bool is_used;       // block used status (malloc->true, free->false)
    struct Block *next; // pointer to the next block in the list
    word_t data[1];     // payload pointer, points to the first word of user data
} Block;

//  blocks in memory
//                    ↓----------------------↓
// +-----------------------------+ +----------------------------+
// | size | used | next * | data | | size | used | next * | data|
// +-----------------------------+ +----------------------------+

// since this is a linked list we will keep track of the head and tail of the heap
static Block *head = NULL, *tail = NULL;

// allocates a block of memory of at least size bytes
word_t *memalloc(size_t size);

// for faster access a memory block should be aligned
// usually by machine word size
// aligned block with an object header
// call -> memalloc(5) (bytes)
// +------------------------------------+-----------+
// | object header (+n) | user data (5) | align (3) |
// +------------------------------------+-----------+

// align function
static inline size_t align(size_t n);

#endif // ! __MEMALLOC_H__
